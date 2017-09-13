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

#include "core/registry.h"
#include "elements/package.h"

namespace elements {

core::MetaData &Package::metaData()
{
  static core::MetaData metaData{ "Package", "package", "qrc://elements/package.png" };

  return metaData;
}

Element *Package::add(string::hash_t a_hash)
{
  core::Registry &registry{ core::Registry::get() };

  Element *const element{ registry.create(a_hash) };
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

  return element;
}

void Package::remove(size_t a_id)
{
  assert(a_id < m_data.size());
  assert(std::find(std::begin(m_free), std::end(m_free), a_id) == std::end(m_free));

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

  assert(target->m_inputs[a_inputId].type == source->m_outputs[a_outputId].type);
  target->m_inputs[a_inputId].id = a_sourceId;
  target->m_inputs[a_inputId].slot = a_outputId;
  target->m_inputs[a_inputId].value = &source->m_outputs[a_outputId].value;

  std::cout << "Notifying " << a_targetId << " (" << target->name() << ")";
  std::cout << "@" << static_cast<int32_t>(a_inputId) << " when " << a_sourceId << " (" << source->name() << ")";
  std::cout << "@" << static_cast<int32_t>(a_outputId) << " changes.." << std::endl;
  m_callbacks[a_sourceId].insert(a_targetId);

  if (target->calculate()) elementChanged(a_targetId);

  return true;
}

void Package::threadFunction()
{
  std::cout << __PRETTY_FUNCTION__ << ">" << std::endl;
  while (!m_quit) {
    if (!tryDispatch()) std::this_thread::yield();
  }
  std::cout << __PRETTY_FUNCTION__ << "<" << std::endl;
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
    std::cout << "Dequeued id: " << id << std::endl;
    dispatch(id);
  }

  return dequeued;
}
void Package::dispatch(size_t a_id)
{
  Element *const source{ get(a_id) };
  if (source->m_callback) source->m_callback(source);

  if (!m_callbacks.contains(a_id)) {
    std::cout << "Callbacks list for id: " << a_id << " (" << source->name() << ")"
              << " don't exist." << std::endl;
    return;
  }

  if (m_callbacks[a_id].empty()) {
    std::cout << "Callbacks list for id: " << a_id << " (" << source->name() << ")"
              << " is empty." << std::endl;
    return;
  }

  std::cout << "Dispatching dependencies for id: " << a_id << " (" << source->name() << ")" << std::endl;
  for (auto id : m_callbacks[a_id]) {
    Element *const element{ get(id) };
    std::cout << "Recalculating id: " << id << " (" << element->name() << ")"
              << " because id: " << a_id << " (" << source->name() << ")"
              << " changed." << std::endl;
    if (element->calculate()) elementChanged(id);
  }
}

void Package::elementChanged(size_t a_id)
{
  m_queue.enqueue(a_id);
}

} // namespace elements
