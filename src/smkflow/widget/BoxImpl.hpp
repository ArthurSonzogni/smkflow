// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef SMKFLOW_WIDGET_BOXIMPL_HPP
#define SMKFLOW_WIDGET_BOXIMPL_HPP

#include <memory>
#include <smkflow/widget/Box.hpp>
#include <vector>

namespace smkflow {

class BoxImpl : public Widget, public BoxInterface {
 public:
  BoxImpl(Delegate* delegate, std::vector<WidgetFactory> children);
  bool Step(smk::Input* input, const glm::vec2& cursor) override;
  void Draw(smk::RenderTarget* target) override;
  Widget* ChildAt(int i) override;
  int ChildCount() override;
  JSON Serialize() override;
  bool Deserialize(JSON&) override;

 protected:
  std::vector<std::unique_ptr<Widget>> children_;
  std::vector<float> children_size_;
  glm::vec2 requested_dimensions_;
};

class BoxImplVertical : public BoxImpl {
 public:
  BoxImplVertical(Delegate* delegate, std::vector<WidgetFactory> children);
  glm::vec2 ComputeDimensions() override;
  void SetDimensions(glm::vec2 dimensions) override;
  void SetPosition(glm::vec2 position) override;
};

class BoxImplHorizontal : public BoxImpl {
 public:
  BoxImplHorizontal(Delegate* delegate, std::vector<WidgetFactory> children);
  glm::vec2 ComputeDimensions() override;
  void SetDimensions(glm::vec2 dimensions) override;
  void SetPosition(glm::vec2 position) override;
};

}  // namespace smkflow
#endif /* end of include guard: SMKFLOW_WIDGET_BOXIMPL_HPP */
