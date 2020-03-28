#include <iostream>
#include <smk/Input.hpp>
#include <smk/RenderTarget.hpp>
#include <smk/Shape.hpp>
#include <smk/Text.hpp>
#include <smk/Input.hpp>
#include <smkflow/BoardImpl.hpp>
#include <smkflow/NodeImpl.hpp>
#include <smkflow/Widget.hpp>

namespace smkflow {

namespace {
const auto color = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
const auto color_hover = glm::vec4(0.4f, 0.4f, 0.4f, 0.6f);
const auto color_focus = glm::vec4(0.1f, 0.1f, 0.1f, 0.8f);
const float margin = 2.f;
}  // namespace

class InputBoxImpl : public Widget, public InputBox {
 public:
  InputBoxImpl(Node* node, int value)
      : Widget(node), input_(std::to_string(value)) {
    square_ = smk::Shape::Square();
    square_.SetScale({128.f, 32.f});
    SetText();
  }

  void SetText() {
    text_ = smk::Text(Font(), input_.empty() ? "..." : input_);
    auto text_dimensions = text_.ComputeDimensions();
    computed_dimensions_ = text_dimensions + glm::vec2(margin * 2, margin * 2);
    InvalidateDimensions();
  }

  smk::Font& Font() { return NodeImpl::From(node())->board()->font(); }

  // Widget implementation:
  glm::vec2 ComputeDimensions() override { return computed_dimensions_; }

  void Step(smk::Input* input, const glm::vec2& cursor) override {
    auto position = cursor - Position() - glm::vec2(margin, margin);
    hover = (position.x >= 0.f && position.y >= 0.f &&
             position.x <= dimensions().x && position.y <= dimensions().y);

    if (input->IsCursorPressed())
      focus = false;

    if (input->IsCursorReleased() && hover) {
      character_listener_ = input->MakeCharacterListener();
      focus = true;
    }

    if (focus) {
      bool modified = false;
      wchar_t character;
      while(character_listener_->Receive(&character)) {
        input_ += character;
        modified = true;
      }
      if (input->IsKeyReleased(GLFW_KEY_BACKSPACE) && !input_.empty()) {
        input_.erase(input_.size() - 1);
        modified = true;
      }
      if (modified)
        SetText();
    }
  }

  void Draw(smk::RenderTarget* target) override {
    auto position = Position();
    auto dimension = dimensions();

    square_.SetPosition(position + glm::vec2(margin, margin));
    square_.SetScale(dimension - glm::vec2(margin*2, margin*2));
    square_.SetColor(focus ? color_focus : hover ? color_hover : color);
    target->Draw(square_);

    text_.SetPosition(position +
                      glm::vec2(margin, margin - dimension.y * 0.1f));
    target->Draw(text_);
  }

  void SetValue(const std::string& value) override {
    input_ = value;
    SetText();
  }

  const std::string& GetValue() override { return input_; }

 private:
  glm::vec2 computed_dimensions_;
  bool focus = false;
  bool hover = false;
  smk::Text text_;
  std::string input_;
  smk::Transformable square_;
  smk::Input::CharacterListener character_listener_;
};

// static
WidgetFactory InputBox::Create(int initial_value) {
  return [initial_value](Node* node) {
    return std::make_unique<InputBoxImpl>(node, initial_value);
  };
}
  
// static
InputBox* InputBox::From(Widget* w) {
  return dynamic_cast<InputBox*>(w);
}

}  // namespace smkflow
