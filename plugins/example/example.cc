#include <cstdlib>
#include <iostream>

#include "core/registry.h"
#include "elements/element.h"
#include "elements/types.h"
#include "nodes/node.h"

class Example final : public elements::Element {
 public:
  static core::MetaData &metaData();

  Example()
    : elements::Element{}
  {
  }

  bool calculate() override
  {
    return false;
  }

  size_t type() const noexcept override { return Types::eUser + 100; }
};

core::MetaData &Example::metaData()
{
  static core::MetaData metaData{ "Example (Bool)", "plugins/example", ":/elements/logic/and.png" };

  return metaData;
}

extern "C" {

void register_plugin(core::Registry &a_registry)
{
  a_registry.registerElement<Example, nodes::Node>();
}

}
