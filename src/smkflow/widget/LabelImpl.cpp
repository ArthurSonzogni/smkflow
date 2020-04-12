// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <smk/RenderTarget.hpp>
#include <smk/Text.hpp>
#include <smkflow/Constants.hpp>
#include <smkflow/NodeImpl.hpp>
#include <smkflow/widget/Label.hpp>
#include <smkflow/widget/Widget.hpp>
#include <smkflow/BoardImpl.hpp>

namespace smkflow {

class LabelImpl : public Widget {
 public:
  LabelImpl(Node* node, const std::string& text) : Widget(node) {
    text_ = smk::Text(Font(), text);
    computed_dimensions_ = text_.ComputeDimensions();
  }

  smk::Font& Font() { return NodeImpl::From(node())->board()->font(); }

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
  return [label](Node* node) {
    return std::make_unique<LabelImpl>(node, label);
  };
}


}  // namespace smkflow
