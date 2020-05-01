// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <algorithm>
#include <smk/Input.hpp>
#include <smk/RenderTarget.hpp>
#include <smk/Shape.hpp>
#include <smk/Text.hpp>
#include <smkflow/BoardImpl.hpp>
#include <smkflow/Constants.hpp>
#include <smkflow/widget/Input.hpp>
#include <smkflow/widget/Widget.hpp>

namespace smkflow {

class InputImpl : public Widget, public InputInterface {
 public:
  InputImpl(Delegate* delegate,
            const std::string& placeholder,
            const std::string& value)
      : Widget(delegate), placeholder_(placeholder), input_(value) {
    square_ = smk::Shape::Square();
    square_.SetScale({128.f, 32.f});
    SetText();
  }

  void SetText() {
    text_ =
        smk::Text(delegate()->Font(), input_.empty() ? placeholder_ : input_);
    text_.SetColor(input_.empty() ? glm::vec4(0.5f, 0.5f, 0.5f, 1.f)
                                  : glm::vec4(1.f, 1.f, 1.f, 1.f));
    computed_dimensions_ = text_.ComputeDimensions();
    computed_dimensions_.y =
        std::max(computed_dimensions_.y, size::widget_height);
    InvalidateLayout();
  }

  // Widget implementation:
  glm::vec2 ComputeDimensions() override { return computed_dimensions_; }

  bool Step(smk::Input* input, const glm::vec2& cursor) override {
    auto position = cursor - Position();
    hover_ = (position.x >= 0.f && position.y >= 0.f &&
              position.x <= dimensions().x && position.y <= dimensions().y);

    if (input->IsCursorPressed())
      focus_ = false;

    if (input->IsCursorReleased() && hover_) {
      character_listener_ = input->MakeCharacterListener();
      focus_ = true;
      cursor_position_ = 0;
      for (cursor_position_ = 0; cursor_position_ < (int)input_.size();
           ++cursor_position_) {
        auto subtext =
            smk::Text(delegate()->Font(), input_.substr(0, cursor_position_));
        auto dim = subtext.ComputeDimensions();
        if (3.f + dim.x > position.x)
          break;
      }
    }

    if (focus_) {
      bool modified = false;
      wchar_t character;
      while (character_listener_->Receive(&character)) {
        input_.insert(cursor_position_++, 1, character);
        modified = true;
      }
      if (CheckKey(GLFW_KEY_BACKSPACE, &backspace_repeat, input)) {
        if (cursor_position_ >= 1) {
          input_.erase(--cursor_position_, 1);
          modified = true;
        }
      }
      if (modified)
        SetText();

      if (CheckKey(GLFW_KEY_LEFT, &left_repeat, input))
        cursor_position_ = std::max(0, cursor_position_ - 1);
      if (CheckKey(GLFW_KEY_RIGHT, &right_repeat, input))
        cursor_position_ = std::min((int)input_.size(), cursor_position_ + 1);
    }
    return hover_ && input->IsCursorPressed();
  }

  void Draw(smk::RenderTarget* target) override {
    auto position = Position();
    auto dimension = dimensions();

    square_.SetPosition(position);
    square_.SetScale(dimension);
    square_.SetColor(focus_ ? color::widget_background_focus
                            : hover_ ? color::widget_background_hover
                                     : color::widget_background);
    target->Draw(square_);

    text_.SetPosition(
        position +
        glm::vec2(3.f, dimension.y * 0.5f - computed_dimensions_.y * 0.5f));
    target->Draw(text_);

    if (focus_) {
      auto subtext =
          smk::Text(delegate()->Font(), input_.substr(0, cursor_position_));
      auto dim = subtext.ComputeDimensions();
      square_.SetScale({1.f, dim.y});
      square_.SetPosition({position.x + dim.x + 3.f, position.y});
      square_.SetColor({1.f, 1.f, 1.f, 1.f});
      target->Draw(square_);
    }
  }

  void SetValue(const std::string& value) override {
    if (input_ == value)
      return;
    input_ = value;
    cursor_position_ = input_.size();
    SetText();
  }

  bool CheckKey(int key, int* repeat, smk::Input* input) {
    if (input->IsKeyPressed(key))
      return true;
    if (!input->IsKeyHold(key)) {
      *repeat = 0;
      return false;
    }
    (*repeat)++;
    if (*repeat <= 20)
      return false;
    if (*repeat % 3)
      return false;
    return true;
  }

  const std::string& GetValue() override { return input_; }

  JSON Serialize() override {
    JSON json;
    json["input"] = input_;
    return json;
  }

  bool Deserialize(JSON& json) override {
    SetValue(json["input"]);
    return true;
  }

 private:
  glm::vec2 computed_dimensions_;
  int backspace_repeat = 0;
  int left_repeat = 0;
  int right_repeat = 0;
  int cursor_position_ = 0;
  bool focus_ = false;
  bool hover_ = false;
  smk::Text text_;
  std::string placeholder_;
  std::string input_;
  smk::Transformable square_;
  smk::Input::CharacterListener character_listener_;
};

// static
WidgetFactory Input(const std::string& placeholder,
                    const std::string& initial_value) {
  return [placeholder, initial_value](Widget::Delegate* delegate) {
    return std::make_unique<InputImpl>(delegate, placeholder, initial_value);
  };
}

InputInterface* Input(Widget* w) {
  return dynamic_cast<InputInterface*>(w);
}

}  // namespace smkflow
