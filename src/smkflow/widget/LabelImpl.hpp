// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <smk/Text.hpp>
#include <smkflow/widget/Label.hpp>
#include <smkflow/widget/Widget.hpp>

namespace smkflow {

class LabelImpl : public Widget {
 public:
  LabelImpl(Delegate* delegate, const std::string& text);
  // Widget implementation:
  glm::vec2 ComputeDimensions() override;
  void Draw(smk::RenderTarget* target) override;

 private:
  glm::vec2 computed_dimensions_;
  smk::Text text_;
};

}  // namespace smkflow
