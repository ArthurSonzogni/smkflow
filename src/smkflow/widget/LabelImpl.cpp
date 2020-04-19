// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <smk/RenderTarget.hpp>
#include <smkflow/Constants.hpp>
#include <smkflow/widget/LabelImpl.hpp>

namespace smkflow {

LabelImpl::LabelImpl(Delegate* delegate, const std::string& text)
    : Widget(delegate) {
  text_ = smk::Text(delegate->Font(), text);
  computed_dimensions_ = text_.ComputeDimensions();
}

// Widget implementation:
glm::vec2 LabelImpl::ComputeDimensions() {
  return computed_dimensions_;
}

void LabelImpl::Draw(smk::RenderTarget* target) {
  auto position = Position();
  position.y += dimensions().y * 0.5f - computed_dimensions_.y * 0.5f;
  text_.SetPosition(position);
  target->Draw(text_);
}

WidgetFactory Label(const std::string& label) {
  return [label](Widget::Delegate* delegate) {
    return std::make_unique<LabelImpl>(delegate, label);
  };
}

}  // namespace smkflow
