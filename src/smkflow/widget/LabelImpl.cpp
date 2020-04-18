// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <smk/RenderTarget.hpp>
#include <smk/Text.hpp>
#include <smkflow/Constants.hpp>
#include <smkflow/widget/Label.hpp>
#include <smkflow/widget/Widget.hpp>

namespace smkflow {

class LabelImpl : public Widget {
 public:
  LabelImpl(Delegate* delegate, const std::string& text) : Widget(delegate) {
    text_ = smk::Text(delegate->Font(), text);
    computed_dimensions_ = text_.ComputeDimensions();
  }

  // Widget implementation:
  glm::vec2 ComputeDimensions() override { return computed_dimensions_; }

  void Draw(smk::RenderTarget* target) override {
    text_.SetPosition(Position() + dimensions() * 0.5f -
                      computed_dimensions_ * 0.5f);
    target->Draw(text_);
  }

 private:
  glm::vec2 computed_dimensions_;
  smk::Text text_;
};

WidgetFactory Label(const std::string& label) {
  return [label](Widget::Delegate* delegate) {
    return std::make_unique<LabelImpl>(delegate, label);
  };
}


}  // namespace smkflow
