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

#ifndef ELEMENTS_ELEMENT_H
#define ELEMENTS_ELEMENT_H

#include <functional>
#include <set>
#include <string>
#include <thread>
#include <variant>
#include <vector>

#include "core/metadata.h"

namespace elements {

class Package;

class Element {
 public:
  using Value = std::variant<bool, int32_t, float>;
  enum class Type { eBool, eInt, eFloat };

  struct Input {
    Value *value{};
    Type type{};

    size_t id{};
    uint8_t slot{};
    std::string name{};
  };

  struct Output {
    Value value{};
    Type type{};

    std::string name{};
  };

  using Inputs = std::vector<Input>;
  using Outputs = std::vector<Output>;

  Element() = default;
  virtual ~Element() = default;

  virtual size_t type() const noexcept = 0;

  virtual bool calculate() { return false; }
  size_t id() const noexcept { return m_id; }

  void setName(std::string a_name) { m_name = a_name; }
  std::string_view name() const noexcept { return m_name; }

  Inputs const &inputs() const { return m_inputs; }
  Outputs const &outputs() const { return m_outputs; }

  bool addInput(Type a_type, std::string a_name);
  bool addOutput(Type a_type, std::string a_name);

  bool connect(size_t a_sourceId, uint8_t a_outputId, uint8_t a_inputId);

  using CallbackFunction = std::function<void(Element *const)>;
  void onChange(CallbackFunction &&a_callback) { m_callback = a_callback; }

 protected:
  void setMinInputs(uint8_t a_min);
  uint8_t minInputs() const { return m_minInputs; }
  void setMaxInputs(uint8_t a_max);
  uint8_t maxInputs() const { return m_maxInputs; }

  void setMinOutputs(uint8_t a_min);
  uint8_t minOutputs() const { return m_minOutputs; }
  void setMaxOutputs(uint8_t a_max);
  uint8_t maxOutputs() const { return m_maxOutputs; }

 protected:
  Inputs m_inputs{};
  Outputs m_outputs{};

  friend class Package;
  Package *m_package{};

 private:
  size_t m_id{};
  std::string m_name{};
  CallbackFunction m_callback{};
  uint8_t m_minInputs{};
  uint8_t m_maxInputs{ std::numeric_limits<uint8_t>::max() };
  uint8_t m_minOutputs{};
  uint8_t m_maxOutputs{ std::numeric_limits<uint8_t>::max() };
};

} // namespace elements

#endif // ELEMENTS_ELEMENT_H
