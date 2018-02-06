// MIT License
//
// Copyright (c) 2017-2018 Artur Wyszyński, aljen at hitomi dot pl
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
#ifndef SPAGHETTI_ELEMENT_H
#define SPAGHETTI_ELEMENT_H

// clang-format off
#ifdef _MSC_VER
# pragma warning(disable:4251)
# pragma warning(disable:4307)
#endif
// clang-format on

#include <chrono>
#include <functional>
#include <set>
#include <string>
#include <thread>
#include <variant>
#include <vector>

#include <spaghetti/vendor/json.hpp>

#include <spaghetti/api.h>
#include <spaghetti/strings.h>

namespace spaghetti {

class Package;

class SPAGHETTI_API Element {
 public:
  using duration_t = std::chrono::duration<double, std::milli>;

  using Json = nlohmann::json;
  using Value = std::variant<bool, int32_t, float>;
  enum class ValueType { eBool, eInt, eFloat };
  struct Vec2 {
    double x{}, y{};
  };

  struct IOSocket {
    enum Flags {
      eCanHoldBool = 1 << 0,
      eCanHoldInt = 1 << 1,
      eCanHoldFloat = 1 << 2,
      eCanChangeName = 1 << 3,
      eCanHoldAllValues = eCanHoldBool | eCanHoldInt | eCanHoldFloat,
      eDefaultFlags = eCanHoldAllValues | eCanChangeName
    };
    Value value{};
    ValueType type{};

    size_t id{};
    uint8_t slot{};
    uint8_t flags{};
    std::string name{};
  };

  using IOSockets = std::vector<IOSocket>;

  Element() = default;
  virtual ~Element() = default;

  virtual char const *type() const noexcept = 0;
  virtual string::hash_t hash() const noexcept = 0;

  virtual void serialize(Json &a_json);
  virtual void deserialize(Json const &a_json);

  virtual void calculate() {}
  virtual void reset() {}

  virtual void update(duration_t const &a_delta) { (void)a_delta; }

  size_t id() const noexcept { return m_id; }

  void setName(std::string const a_name);

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

  IOSockets &inputs() { return m_inputs; }
  IOSockets const &inputs() const { return m_inputs; }
  IOSockets &outputs() { return m_outputs; }
  IOSockets const &outputs() const { return m_outputs; }

  bool addInput(ValueType const a_type, std::string const a_name, uint8_t const a_flags);
  void setInputName(uint8_t const a_input, std::string const a_name);
  void removeInput();
  void clearInputs();

  bool addOutput(ValueType const a_type, std::string const a_name, uint8_t const a_flags);
  void setOutputName(uint8_t const a_output, std::string const a_name);
  void removeOutput();
  void clearOutputs();

  void setIOValueType(bool const a_input, uint8_t const a_id, ValueType const a_type);

  bool connect(size_t const a_sourceId, uint8_t const a_outputId, uint8_t const a_inputId);

  uint8_t minInputs() const { return m_minInputs; }
  uint8_t maxInputs() const { return m_maxInputs; }
  uint8_t defaultNewInputFlags() const { return m_defaultNewInputFlags; }
  uint8_t minOutputs() const { return m_minOutputs; }
  uint8_t maxOutputs() const { return m_maxOutputs; }
  uint8_t defaultNewOutputFlags() const { return m_defaultNewOutputFlags; }

  Package *package() const { return m_package; }

  void resetIOSocketValue(IOSocket &a_io);

 protected:
  virtual void nameChanged(std::string const a_from, std::string const a_to);
  virtual void inputNameChanged(uint8_t const a_id, std::string const a_from, std::string const a_to);
  virtual void outputNameChanged(uint8_t const a_id, std::string const a_from, std::string const a_to);

  void setMinInputs(uint8_t const a_min);
  void setMaxInputs(uint8_t const a_max);
  void setDefaultNewInputFlags(uint8_t const a_flags) { m_defaultNewInputFlags = a_flags; }

  void setMinOutputs(uint8_t const a_min);
  void setMaxOutputs(uint8_t const a_max);
  void setDefaultNewOutputFlags(uint8_t const a_flags) { m_defaultNewOutputFlags = a_flags; }

 protected:
  IOSockets m_inputs{};
  IOSockets m_outputs{};

  friend class Package;
  Package *m_package{};

 private:
  size_t m_id{};
  std::string m_name{};
  Vec2 m_position{};
  bool m_isIconified{};
  uint8_t m_minInputs{};
  uint8_t m_maxInputs{ std::numeric_limits<uint8_t>::max() };
  uint8_t m_minOutputs{};
  uint8_t m_maxOutputs{ std::numeric_limits<uint8_t>::max() };
  uint8_t m_defaultNewInputFlags{};
  uint8_t m_defaultNewOutputFlags{};
};

inline void Element::nameChanged(std::string const a_from, std::string const a_to)
{
  (void)a_from;
  (void)a_to;
}

inline void Element::inputNameChanged(uint8_t const a_id, std::string const a_from, std::string const a_to)
{
  (void)a_id;
  (void)a_from;
  (void)a_to;
}

inline void Element::outputNameChanged(uint8_t const a_id, std::string const a_from, std::string const a_to)
{
  (void)a_id;
  (void)a_from;
  (void)a_to;
}

} // namespace spaghetti

#endif // SPAGHETTI_ELEMENT_H
