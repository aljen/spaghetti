#pragma once
#ifndef NODES_UI_TOGGLE_BUTTON_H
#define NODES_UI_TOGGLE_BUTTON_H

#include "nodes/node.h"

namespace nodes::ui {

class ToggleButton : public Node {
 public:
  ToggleButton();

 private:
  void paint(QPainter *a_painter, QStyleOptionGraphicsItem const *a_option, QWidget *a_widget) override;

  void showProperties() override;
  void elementSet() override;
};

} // namespace nodes::ui

#endif // NODES_UI_TOGGLE_BUTTON_H
