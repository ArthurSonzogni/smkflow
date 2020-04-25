// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <algorithm>
#include <smk/Input.hpp>
#include <smk/RenderTarget.hpp>
#include <smk/Shape.hpp>
#include <smk/Text.hpp>
#include <smk/VertexArray.hpp>
#include <smkflow/Constants.hpp>
#include <smkflow/widget/BoxImpl.hpp>
#include <smkflow/widget/Menu.hpp>
#include <smkflow/widget/MenuImpl.hpp>

namespace smkflow {

class MenuImpl : public Widget, public Widget::Delegate {
 public:
  MenuImpl(Widget::Delegate* delegate,
           const std::string& text,
           std::vector<WidgetFactory> children)
      : Widget(delegate) {
    text_ = smk::Text(delegate->Font(), text);
    computed_dimensions_ = text_.ComputeDimensions();
    computed_dimensions_.x += computed_dimensions_.y;
    square_ = smk::Shape::Square();
    children_ = std::make_unique<BoxImplVertical>(this, children);
    triangle_ = smk::Shape::FromVertexArray(smk::VertexArray({
        {{-0.5f, -0.5f}, {0.f, 0.0f}},
        {{+0.5f, +0.0f}, {1.f, 0.5f}},
        {{-0.5f, +0.5f}, {0.f, 1.0f}},
    }));
  }

  // Widget:
  glm::vec2 ComputeDimensions() override {
    child_dimension_ = children_->ComputeDimensions();
    children_->ComputeDimensions();
    // if (cursor_capture_) {
    // return glm::vec2(computed_dimensions_.x + size::widget_margin + .x,
    // std::max(computed_dimensions_.y, child.y));
    //}
    // else
    return computed_dimensions_;
  }

  void SetPosition(glm::vec2 position) override {
    Widget::SetPosition(position);
    position.x += dimensions().x + size::widget_margin;
    children_->SetPosition(position);
  }

  void SetDimensions(glm::vec2 dimension) override {
    Widget::SetDimensions(dimension);
    children_->SetDimensions(child_dimension_);
  }

  bool Step(smk::Input* input, const glm::vec2& cursor) override {
    auto position = Widget::Position();
    auto dimension = dimensions();

    hover_ = cursor.x > position.x &&                //
             cursor.x < position.x + dimension.x &&  //
             cursor.y > position.y &&                //
             cursor.y < position.y + dimension.y;    //
    // if (cursor_capture_ && hover_ && input->IsCursorPressed()) {
    ////cursor_capture_ = false;
    // InvalidateLayout();
    //}

    if (hover_) {
      if (cursor_capture_ && input->IsCursorPressed()) {
        cursor_capture_.Invalidate();
      } else if (!cursor_capture_ &&
                 (delegate()->IsInsideMenu() || input->IsCursorPressed())) {
        cursor_capturable_.Invalidate();
        cursor_capture_.Invalidate();
        cursor_capture_ = delegate()->CaptureCursor();
        Widget::InvalidateLayout();
      }
    }

    if (cursor_capture_) {
      bool clicked = false;
      clicked |= children_->Step(input, cursor);
      clicked |= Widget::Step(input, cursor);
      if (!clicked && input->IsCursorPressed())
        cursor_capture_.Invalidate();
      return clicked;
    }

    return Widget::Step(input, cursor);
  }

  void Draw(smk::RenderTarget* target) override {
    if (cursor_capture_ || hover_) {
      square_.SetPosition(Widget::Position());
      square_.SetScale(dimensions());
      square_.SetColor(cursor_capture_ ? color::widget_background_focus
                                       : color::widget_background_hover);
      target->Draw(square_);
    }

    text_.SetPosition(Widget::Position());
    target->Draw(text_);

    if (cursor_capture_) {
      DrawBoxBackground(
          target, children_->Position() - size::widget_margin * glm::vec2(1.f),
          children_->dimensions() + 2.f * size::widget_margin * glm::vec2(1.f));
      children_->Draw(target);
    }

    triangle_.SetScale(dimensions().y * 0.5f);
    triangle_.SetPosition(Widget::Position() +
                          glm::vec2(dimensions().x - 0.5f * dimensions().y,
                                    dimensions().y * 0.5f));
    target->Draw(triangle_);
  }

  glm::vec2 Position() override { return delegate()->Position(); }
  void InvalidateLayout() override { return delegate()->InvalidateLayout(); }
  smk::Font& Font() override { return delegate()->Font(); }
  CursorCapture CaptureCursor() override {
    if (!cursor_capture_) {
      cursor_capture_ = delegate()->CaptureCursor();
    }
    if (!cursor_capture_)
      return CursorCapture();
    cursor_capturable_.Invalidate();
    return cursor_capturable_.Capture();
  }
  bool IsInsideMenu() override { return true; }
  Board* board() override { return delegate()->board(); }

 private:
  std::unique_ptr<smkflow::BoxImplVertical> children_;
  smk::Transformable square_;
  smk::Transformable triangle_;
  glm::vec2 computed_dimensions_;
  glm::vec2 child_dimension_;
  smk::Text text_;
  bool hover_ = false;
  CursorCapture cursor_capture_;
  CursorCapturable cursor_capturable_;
};

WidgetFactory Menu(const std::string& label,
                   std::vector<WidgetFactory> children) {
  return [=](Widget::Delegate* delegate) {
    return std::make_unique<MenuImpl>(delegate, label, children);
  };
}

void DrawBoxBackground(smk::RenderTarget* target,
                       glm::vec2 position,
                       glm::vec2 dimension) {
  auto square = smk::Shape::Square();
  square.SetColor({0.f, 0.f, 0.f, 1.f});
  square.SetPosition(position);
  square.SetScale(dimension);
  target->Draw(square);
  auto border = smk::Shape::Path(
      {
          position,
          position + glm::vec2(dimension.x, 0.f),
          position + dimension,
          position + glm::vec2(0.f, dimension.y),
          position,
      },
      1);
  border.SetColor({1.f, 1.f, 1.f, 1.f});
  target->Draw(border);
}

}  // namespace smkflow
