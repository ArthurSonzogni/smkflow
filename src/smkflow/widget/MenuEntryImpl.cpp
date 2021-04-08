// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <smk/RenderTarget.hpp>
#include <smk/Shape.hpp>
#include <smkflow/Constants.hpp>
#include <smkflow/Elements.hpp>
#include <smkflow/Model.hpp>
#include <smkflow/widget/LabelImpl.hpp>
#include <smkflow/widget/Menu.hpp>

namespace smkflow {

class MenuEntryImpl : public LabelImpl, public ActionContext {
 public:
  MenuEntryImpl(Widget::Delegate* delegate,
                const std::string& label,
                Action action)
      : LabelImpl(delegate, label), action_(action) {
    square_ = smk::Shape::Square();
  }
  virtual ~MenuEntryImpl() = default;

  bool Step(smk::Input* input, const glm::vec2& cursor) override {
    cursor_ = cursor;
    auto position = cursor_ - Position();
    hover_ = (position.x >= 0.f && position.y >= 0.f &&
              position.x <= dimensions().x && position.y <= dimensions().y);
    focused_ = hover_ && input->IsCursorHeld();
    LabelImpl::Step(input, cursor_);

    if (hover_ && input->IsCursorPressed())
      action_(this);

    return Widget::Step(input, cursor_);
  }

  void Draw(smk::RenderTarget* target) override {
    if (hover_) {
      square_.SetColor(focused_ ? color::widget_background_focus
                                : hover_ ? color::widget_background_hover
                                         : color::widget_background);
      square_.SetPosition(Position());
      square_.SetScale(dimensions());
      target->Draw(square_);
    }

    LabelImpl::Draw(target);
  }

  Board* board() override { return delegate()->board(); }
  glm::vec2 cursor() override { return cursor_; }

 private:
  glm::vec2 cursor_;
  smk::Transformable square_;
  Action action_;
  bool focused_ = false;
  bool hover_ = false;
};

WidgetFactory MenuEntry(const std::string& label, Action action) {
  return [=](Widget::Delegate* delegate) {
    return std::make_unique<MenuEntryImpl>(delegate, label, action);
  };
}

Action CreateNode(model::Node node) {
  return [=](ActionContext* context) {
    context->board()->Create(node)->SetPosition(context->cursor());
  };
}

Action ActionNone() {
  return [=](ActionContext*) {};
}

}  // namespace smkflow
