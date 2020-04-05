// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <smkflow/NodeImpl.hpp>
#include <smkflow/Widget.hpp>

namespace smkflow {

void Widget::SetDimensions(glm::vec2 dimensions) {
  if (dimensions_ == dimensions)
    return;
  dimensions_ = dimensions;
  dimensions_modified_ = true;
}

bool Widget::ValidateDimensions() {
  bool ret = dimensions_modified_;
  dimensions_modified_ = false;
  return ret;
}

glm::vec2 Widget::Position() {
  return NodeImpl::From(node_)->GetPosition() + position_;
}

}  // namespace smkflow
