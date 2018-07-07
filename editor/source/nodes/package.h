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
#ifndef NODES_PACKAGE_H
#define NODES_PACKAGE_H

#include "spaghetti/editor/node.h"

namespace spaghetti::nodes {

class Package : public Node {
 public:
  static constexpr char const *const TYPE{ "logic/package" };
  static constexpr string::hash_t const HASH{ string::hash(TYPE) };

  Package();

  void setInputsNode(Node *const a_node) { m_inputsNode = a_node; }
  Node *inputsNode() { return m_inputsNode; }
  Node const *inputsNode() const { return m_inputsNode; }

  void setOutputsNode(Node *const a_node) { m_outputsNode = a_node; }
  Node *outputsNode() { return m_outputsNode; }
  Node const *outputsNode() const { return m_outputsNode; }

 private:
  void showProperties() override;
  void handleEvent(Event const &a_event) override;
  bool open() override;

 private:
  Node *m_inputsNode{};
  Node *m_outputsNode{};
};

} // namespace spaghetti::nodes

#endif // NODES_PACKAGE_H
