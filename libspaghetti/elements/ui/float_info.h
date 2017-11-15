#pragma once
#ifndef ELEMENTS_UI_FLOAT_INFO_H
#define ELEMENTS_UI_FLOAT_INFO_H

#include "elements/element.h"

namespace elements::ui {

class FloatInfo final : public elements::Element {
 public:
  static constexpr char const *const TYPE{ "ui/float_info" };
  static constexpr string::hash_t const HASH{ string::hash(TYPE) };

  FloatInfo();
  ~FloatInfo() override;

  char const *type() const noexcept override { return TYPE; }
  string::hash_t hash() const noexcept override { return HASH; }

  bool calculate() override;
};

} // namespace elements::ui

#endif // ELEMENTS_UI_FLOAT_INFO_H
