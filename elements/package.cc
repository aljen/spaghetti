// MIT License
//
// Copyright (c) 2017 Artur Wyszyński, aljen at hitomi dot pl
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <iostream>
#include <string_view>

#include "core/registry.h"
#include "elements/logic/clock.h"
#include "elements/package.h"

namespace elements {

Package::Package()
  : Element{}
{
  m_data.push_back(this);

  addInput(ValueType::eBool, "#1");
  addInput(ValueType::eBool, "#2");
  addInput(ValueType::eBool, "#3");
  addOutput(ValueType::eBool, "#1");
}

Package::~Package() {}

Element *Package::add(string::hash_t a_hash)
{
  core::Registry &registry{ core::Registry::get() };

  Element *const element{ registry.createElement(a_hash) };
  assert(element);

  size_t index{};
  if (m_free.empty()) {
    index = m_data.size();
    m_data.emplace_back(element);
  } else {
    index = m_free.back();
    assert(m_data[index] == nullptr);
    m_data[index] = element;
    m_free.pop_back();
  }

  element->m_package = this;
  element->m_id = index;

  if (element->hash() == logic::Clock::HASH) {
    using logic::Clock;
    Clock *const clock{ reinterpret_cast<Clock *const>(element) };
    clock->reset(Clock::clock_t::now());
    m_clocks.push_back(clock);
  }

  return element;
}

void Package::remove(size_t a_id)
{
  assert(a_id > 0);
  assert(a_id < m_data.size());
  assert(std::find(std::begin(m_free), std::end(m_free), a_id) == std::end(m_free));

  if (m_data[a_id]->hash() == logic::Clock::HASH) std::remove(std::begin(m_clocks), std::end(m_clocks), m_data[a_id]);

  delete m_data[a_id];
  m_data[a_id] = nullptr;
  m_free.emplace_back(a_id);
}

Element *Package::get(size_t a_id) const
{
  assert(a_id < m_data.size());
  assert(std::find(std::begin(m_free), std::end(m_free), a_id) == std::end(m_free));
  return m_data[a_id];
}

bool Package::connect(size_t a_sourceId, uint8_t a_outputId, size_t a_targetId, uint8_t a_inputId)
{
  Element *const source{ get(a_sourceId) };
  Element *const target{ get(a_targetId) };

  std::cerr << "source_id: " << a_sourceId << "@" << static_cast<int>(a_outputId) << " "
            << "target_id: " << a_targetId << "@" << static_cast<int>(a_inputId) << std::endl;
  if (a_sourceId != 0 && a_targetId != 0) {
    std::cerr << "normal connect, element to element" << std::endl;
    assert(target->m_inputs[a_inputId].type == source->m_outputs[a_outputId].type);
    target->m_inputs[a_inputId].id = a_sourceId;
    target->m_inputs[a_inputId].slot = a_outputId;
    target->m_inputs[a_inputId].value = &source->m_outputs[a_outputId].value;
  } else if (a_sourceId == 0) {
    std::cerr << "package connect, package input to element input" << std::endl;
    target->m_inputs[a_inputId].id = a_sourceId;
    target->m_inputs[a_inputId].slot = a_outputId;
    target->m_inputs[a_inputId].value = source->m_inputs[a_outputId].value;
  } else if (a_targetId == 0) {
    std::cerr << "package connect, element output to package output" << std::endl;
  }

//  std::cerr << "Notifying " << a_targetId << " (" << target->name() << ")";
//  std::cerr << "@" << static_cast<int32_t>(a_inputId) << " when " << a_sourceId << " (" << source->name() << ")";
//  std::cerr << "@" << static_cast<int32_t>(a_outputId) << " changes.." << std::endl;
  m_callbacks[a_sourceId].insert(a_targetId);

  if (target->calculate()) elementChanged(a_targetId);

  return true;
}

void Package::threadFunction()
{
  while (!m_quit) {
    auto const now = logic::Clock::clock_t::now();
    for (auto &&clock : m_clocks) {
      clock->update(now);
    }

    if (!tryDispatch()) std::this_thread::yield();
  }
}

void Package::startDispatchThread()
{
  m_thread = std::thread(&Package::threadFunction, this);
}

void Package::quitDispatchThread()
{
  m_quit = true;
  m_thread.join();
}

bool Package::tryDispatch()
{
  size_t id{};
  bool const dequeued{ m_queue.try_dequeue(id) };
  if (dequeued) {
//    std::cerr << "Dequeued id: " << id << std::endl;
    dispatch(id);
  }

  return dequeued;
}

void Package::dispatch(size_t a_id)
{
  Element *const source{ get(a_id) };
  if (source->m_callback) source->m_callback(source);

  if (m_callbacks.find(a_id) == std::end(m_callbacks)) {
//    std::cerr << "Callbacks list for id: " << a_id << " (" << source->name() << ")"
//              << " don't exist." << std::endl;
    return;
  }

  if (m_callbacks[a_id].empty()) {
//    std::cerr << "Callbacks list for id: " << a_id << " (" << source->name() << ")"
//              << " is empty." << std::endl;
    return;
  }

//  std::cerr << "Dispatching dependencies for id: " << a_id << " (" << source->name() << ")" << std::endl;
  for (auto id : m_callbacks[a_id]) {
    Element *const element{ get(id) };
//    std::cerr << "Recalculating id: " << id << " (" << element->name() << ")"
//              << " because id: " << a_id << " (" << source->name() << ")"
//              << " changed." << std::endl;
    if (element->calculate()) elementChanged(id);
  }
}

void Package::elementChanged(size_t a_id)
{
  m_queue.enqueue(a_id);
}

} // namespace elements
