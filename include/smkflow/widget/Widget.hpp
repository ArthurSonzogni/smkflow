// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef SMKFLOW_WIDGET_WIDGET_HPP
#define SMKFLOW_WIDGET_WIDGET_HPP

#include <functional>
#include <glm/glm.hpp>
#include <memory>
#include <smkflow/CursorCapture.hpp>
#include <smkflow/JSON.hpp>

namespace smk {
class RenderTarget;
class Input;
class Font;
}  // namespace smk

namespace smkflow {
class Board;

class Widget {
 public:
  class Delegate {
   public:
    virtual ~Delegate() = default;
    virtual Board* board() = 0;
    virtual CursorCapture CaptureCursor() = 0;
    virtual bool IsInsideMenu() { return false; }
    virtual glm::vec2 Position() = 0;
    virtual smk::Font& Font() = 0;
    virtual void InvalidateLayout() = 0;
  };

  Widget(Delegate* delegate) : delegate_(delegate) {}
  virtual ~Widget() = default;

  // Layout:
  glm::vec2 Position();
  glm::vec2 PositionInWidget() { return position_; }
  glm::vec2 dimensions() { return dimensions_; }
  virtual glm::vec2 ComputeDimensions() = 0;
  virtual void SetDimensions(glm::vec2 dimensions);
  virtual void SetPosition(glm::vec2 position) { position_ = position; }
  void InvalidateLayout();

  // Interactivity:
  virtual bool Step(smk::Input*, const glm::vec2& /* cursor */);

  // Render:
  virtual void Draw(smk::RenderTarget*) {}

  // Serialization:
  virtual JSON Serialize() { return JSON(); }
  virtual bool Deserialize(JSON&) { return true; }

 protected:
  Delegate* delegate() { return delegate_; }

 private:
  Delegate* delegate_;

  glm::vec2 position_;
  glm::vec2 dimensions_;
};

using WidgetFactory = std::function<std::unique_ptr<Widget>(Widget::Delegate*)>;

}  // namespace smkflow

#endif /* end of include guard: SMKFLOW_WIDGET_WIDGET_HPP */
