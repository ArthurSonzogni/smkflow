// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef SMKFLOW_WIDGET_WIDGET_HPP
#define SMKFLOW_WIDGET_WIDGET_HPP

#include <functional>
#include <glm/glm.hpp>
#include <memory>

namespace smk {
class RenderTarget;
class Input;
}  // namespace smk

namespace smkflow {
class Node;
class Widget;
using WidgetFactory = std::function<std::unique_ptr<Widget>(Node*)>;

class Widget {
 public:
  Widget(Node* node) : node_(node) {}
  virtual ~Widget() = default;

  // Layout:
  glm::vec2 Position();
  glm::vec2 PositionInWidget() { return position_; }
  glm::vec2 dimensions() { return dimensions_; }
  virtual glm::vec2 ComputeDimensions() = 0;
  virtual void SetDimensions(glm::vec2 dimensions);
  virtual void SetPosition(glm::vec2 position) { position_ = position; }
  void InvalidateDimensions();

  // Interactivity:
  virtual void Step(smk::Input*, const glm::vec2& /* cursor */) {}

  // Render:
  virtual void Draw(smk::RenderTarget*) {}

 protected:
  Node* node() { return node_; }

 private:
  Node* node_;

  glm::vec2 position_;
  glm::vec2 dimensions_;
};

}  // namespace smkflow

#endif /* end of include guard: SMKFLOW_WIDGET_WIDGET_HPP */
