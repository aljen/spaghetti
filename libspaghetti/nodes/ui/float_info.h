#pragma once
#ifndef NODES_UI_FLOAT_INFO_H
#define NODES_UI_FLOAT_INFO_H

#include "nodes/node.h"

namespace nodes::ui {

class FloatInfo : public Node {
 public:
  FloatInfo();

 private:
  void refreshCentralWidget() override;
  void showProperties() override;

 private:
  QGraphicsSimpleTextItem *m_info{};
};

} // namespace nodes::ui

#endif // NODES_UI_FLOAT_INFO_H
