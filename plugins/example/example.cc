#include <cstdlib>
#include <iostream>

#include "core/registry.h"
#include "elements/element.h"
#include "nodes/node.h"

class Example final : public elements::Element {
 public:
  static constexpr char const *const TYPE{ "plugins/example" };
  static constexpr string::hash_t const HASH{ string::hash(TYPE) };

  Example()
    : elements::Element{}
  {
  }

  char const *type() const noexcept override { return TYPE; }
  string::hash_t hash() const noexcept override { return HASH; }

  bool calculate() override { return false; }
};

extern "C" SPAGHETTI_API void register_plugin(core::Registry &a_registry)
{
  a_registry.registerElement<Example>("Example (Bool)", ":/elements/logic/and.png");
}
