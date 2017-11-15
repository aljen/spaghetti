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

#pragma once
#ifndef ELEMENTS_ELEMENT_H
#define ELEMENTS_ELEMENT_H

#include <functional>
#include <set>
#include <string>
#include <thread>
#include <variant>
#include <vector>

#include <json.hpp>

#include "core/api.h"
#include "core/strings.h"

namespace elements {

class Package;

class SPAGHETTI_API Element {
 public:
  using Json = nlohmann::json;
  using Value = std::variant<bool, int32_t, float>;
  enum class ValueType { eBool, eInt, eFloat };
  struct Vec2 {
    double x{}, y{};
  };

  struct Input {
    Value *value{};
    ValueType type{};

    size_t id{};
    uint8_t slot{};
    std::string name{};
  };

  struct Output {
    Value value{};
    ValueType type{};

    std::string name{};
  };

  using Inputs = std::vector<Input>;
  using Outputs = std::vector<Output>;

  Element() = default;
  virtual ~Element() = default;

  virtual char const *type() const noexcept = 0;
  virtual string::hash_t hash() const noexcept = 0;

  virtual void serialize(Json &a_json);
  virtual void deserialize(Json const &a_json);

  virtual bool calculate() { return false; }

  size_t id() const noexcept { return m_id; }

  void setName(std::string const a_name) { m_name = a_name; }
  std::string name() const noexcept { return m_name; }

  void setPosition(double const a_x, double const a_y)
  {
    m_position.x = a_x;
    m_position.y = a_y;
  }
  void setPosition(Vec2 const a_position) { m_position = a_position; }
  Vec2 const &position() const { return m_position; }

  void iconify(bool const a_iconify) { m_isIconified = a_iconify; }
  bool isIconified() const { return m_isIconified; }

  Inputs const &inputs() const { return m_inputs; }
  Outputs const &outputs() const { return m_outputs; }

  bool addInput(ValueType const a_type, std::string const a_name);
  void clearInputs();
  bool addOutput(ValueType const a_type, std::string const a_name);
  void clearOutputs();

  bool connect(size_t const a_sourceId, uint8_t const a_outputId, uint8_t const a_inputId);

  using CallbackFunction = std::function<void(Element *const)>;
  void onChange(CallbackFunction &&a_callback) { m_callback = a_callback; }

  uint8_t minInputs() const { return m_minInputs; }
  uint8_t maxInputs() const { return m_maxInputs; }
  uint8_t minOutputs() const { return m_minOutputs; }
  uint8_t maxOutputs() const { return m_maxOutputs; }

 protected:
  void setMinInputs(uint8_t const a_min);
  void setMaxInputs(uint8_t const a_max);

  void setMinOutputs(uint8_t const a_min);
  void setMaxOutputs(uint8_t const a_max);

 protected:
  Inputs m_inputs{};
  Outputs m_outputs{};

  friend class Package;
  Package *m_package{};

 private:
  size_t m_id{};
  std::string m_name{};
  Vec2 m_position{};
  CallbackFunction m_callback{};
  bool m_isIconified{};
  uint8_t m_minInputs{};
  uint8_t m_maxInputs{ std::numeric_limits<uint8_t>::max() };
  uint8_t m_minOutputs{};
  uint8_t m_maxOutputs{ std::numeric_limits<uint8_t>::max() };
};

} // namespace elements

#endif // ELEMENTS_ELEMENT_H
