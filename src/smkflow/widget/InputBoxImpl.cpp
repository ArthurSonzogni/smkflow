#include <smk/Input.hpp>
#include <smk/RenderTarget.hpp>
#include <smk/Shape.hpp>
#include <smk/Text.hpp>
#include <smk/Input.hpp>
#include <smkflow/BoardImpl.hpp>
#include <smkflow/NodeImpl.hpp>
#include <smkflow/Widget.hpp>
#include <smkflow/Constants.hpp>

namespace smkflow {

class InputBoxImpl : public Widget, public InputBox {
 public:
  InputBoxImpl(Node* node, const std::string& value)
      : Widget(node), input_(value) {
    square_ = smk::Shape::Square();
    square_.SetScale({128.f, 32.f});
    SetText();
  }

  void SetText() {
    text_ = smk::Text(Font(), input_.empty() ? "..." : input_);
    computed_dimensions_ = text_.ComputeDimensions();
    computed_dimensions_.y =
        std::max(computed_dimensions_.y, size::widget_height);
    InvalidateDimensions();
  }

  smk::Font& Font() { return NodeImpl::From(node())->board()->font(); }

  // Widget implementation:
  glm::vec2 ComputeDimensions() override { return computed_dimensions_; }

  void Step(smk::Input* input, const glm::vec2& cursor) override {
    auto position = cursor - Position();
    hover_ = (position.x >= 0.f && position.y >= 0.f &&
             position.x <= dimensions().x && position.y <= dimensions().y);

    if (input->IsCursorPressed())
      focus_ = false;

    if (input->IsCursorReleased() && hover_) {
      character_listener_ = input->MakeCharacterListener();
      focus_ = true;
    }

    if (focus_) {
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
  }

  void SetValue(const std::string& value) override {
    input_ = value;
    SetText();
  }

  const std::string& GetValue() override { return input_; }

 private:
  glm::vec2 computed_dimensions_;
  bool focus_ = false;
  bool hover_ = false;
  smk::Text text_;
  std::string input_;
  smk::Transformable square_;
  smk::Input::CharacterListener character_listener_;
};

// static
WidgetFactory InputBox::Create(const std::string& initial_value) {
  return [initial_value](Node* node) {
    return std::make_unique<InputBoxImpl>(node, initial_value);
  };
}

// static
InputBox* InputBox::From(Widget* w) {
  return dynamic_cast<InputBox*>(w);
}

}  // namespace smkflow
