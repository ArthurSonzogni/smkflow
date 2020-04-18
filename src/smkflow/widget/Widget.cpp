// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <smkflow/NodeImpl.hpp>
#include <smkflow/widget/Widget.hpp>

namespace smkflow {

void Widget::SetDimensions(glm::vec2 dimensions) {
  dimensions_ = dimensions;
}

glm::vec2 Widget::Position() {
  return delegate_->Position() + position_;
}

void Widget::InvalidateLayout() {
  delegate_->InvalidateLayout();
}

bool Widget::Step(smk::Input* input, const glm::vec2& cursor) {
  if (!input->IsCursorPressed())
    return false;

  auto position = Position();
  auto dimension = dimensions();
  bool hover = cursor.x > position.x &&                //
               cursor.x < position.x + dimension.x &&  //
               cursor.y > position.y &&                //
               cursor.y < position.y + dimension.y;    //
  return hover;
}

}  // namespace smkflow
