#include "float_info.h"

#include <iostream>
#include "elements/package.h"

namespace elements::ui {

FloatInfo::FloatInfo()
{
  setMinInputs(1);
  setMaxInputs(1);
  setMinOutputs(0);
  setMaxOutputs(0);

  addInput(ValueType::eFloat, "Value");
}

FloatInfo::~FloatInfo() {}

bool FloatInfo::calculate()
{
  return true;
}

} // namespace elements::ui
