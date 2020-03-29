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

// -----------------------------------------------------------------------------

class InputBox {
 public:
  static WidgetFactory Create(int initial_value);
  static InputBox* From(Widget*);
  virtual void SetValue(const std::string& value) = 0;
  virtual const std::string& GetValue() = 0;
};

// -----------------------------------------------------------------------------

class Widget {
 public:
  Widget(Node* node) : node_(node) {}
  virtual ~Widget() = default;
  void SetPosition(glm::vec2 position) { position_ = position; }
  glm::vec2 PositionInWidget() { return position_; }
  glm::vec2 Position();

  glm::vec2 dimensions() { return dimensions_; }
  void SetDimensions(glm::vec2 dimensions);
  void InvalidateDimensions() { dimensions_modified_ = true; }
  bool ValidateDimensions();

  virtual glm::vec2 ComputeDimensions() { return dimensions_; }
  virtual void Step(smk::Input*, const glm::vec2& /* cursor */) {}
  virtual void Draw(smk::RenderTarget*) {}

 protected:
  Node* node() { return node_; }

 private:
  Node* node_;
  glm::vec2 position_;
  glm::vec2 dimensions_;
  bool dimensions_modified_ = true;
};

// -----------------------------------------------------------------------------

}  // namespace smkflow

#endif /* end of include guard: SMKFLOW_WIDGET_WIDGET_HPP */
