#pragma once
#ifndef NODES_CONST_VALUE_H
#define NODES_CONST_VALUE_H

#include "nodes/node.h"

namespace nodes {

class ConstBool: public Node {
  void showProperties();
};

class ConstInt: public Node {
  void showProperties();
};

class ConstFloat : public Node {
  void showProperties();
};

} // namespace nodes

#endif // NODES_CONST_VALUE_H
