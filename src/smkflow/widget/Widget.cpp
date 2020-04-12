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
  return NodeImpl::From(node_)->GetPosition() + position_;
}

void Widget::InvalidateDimensions() {
  NodeImpl::From(node_)->InvalidateLayout();
}

}  // namespace smkflow
