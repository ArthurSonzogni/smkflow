// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.

#include <algorithm>
#include <smkflow/Constants.hpp>
#include <smkflow/widget/BoxImpl.hpp>

namespace smkflow {

BoxImpl::BoxImpl(Delegate* delegate, std::vector<WidgetFactory> children)
    : Widget(delegate) {
  for (auto& it : children)
    children_.push_back(it(delegate));
  children_size_.resize(children_.size());
}

bool BoxImpl::Step(smk::Input* input, const glm::vec2& cursor) {
  bool ret = false;
  for (auto& it : children_)
    ret |= it->Step(input, cursor);
  if (ret)
    return true;
  return Widget::Step(input, cursor);
}

void BoxImpl::Draw(smk::RenderTarget* target) {
  for (auto& child : children_)
    child->Draw(target);
}

Widget* BoxImpl::ChildAt(int i) {
  return children_.at(i).get();
}
int BoxImpl::ChildCount() {
  return children_.size();
}

JSON BoxImpl::Serialize() {
  JSON json = JSON::array();
  for (auto& child : children_)
    json.push_back(child->Serialize());
  return json;
}

bool BoxImpl::Deserialize(JSON& json) {
  int i = 0;
  bool ret = true;
  for (auto& child : children_)
    ret &= child->Deserialize(json[i++]);
  return ret;
}

BoxImplVertical::BoxImplVertical(Delegate* delegate,
                                 std::vector<WidgetFactory> children)
    : BoxImpl(delegate, children) {}

glm::vec2 BoxImplVertical::ComputeDimensions() {
  requested_dimensions_ = glm::vec2(0.f, 0.f);
  int i = 0;
  for (auto& it : children_) {
    auto child_dimension = it->ComputeDimensions();
    requested_dimensions_.x =
        std::max(requested_dimensions_.x, child_dimension.x);
    requested_dimensions_.y += child_dimension.y;
    children_size_[i++] = child_dimension.y;
  }
  if (children_.size())
    requested_dimensions_ += (children_.size() - 1) * size::widget_margin;
  return requested_dimensions_;
}

void BoxImplVertical::SetDimensions(glm::vec2 dimensions) {
  Widget::SetDimensions(dimensions);
  float space_left = dimensions.y - requested_dimensions_.y;
  space_left /= children_.size();
  int i = 0;
  for (auto& it : children_)
    it->SetDimensions({dimensions.x, children_size_[i++] + space_left});
}

void BoxImplVertical::SetPosition(glm::vec2 position) {
  Widget::SetPosition(position);
  for (auto& it : children_) {
    it->SetPosition(position);
    position.y += it->dimensions().y + size::widget_margin;
  }
}

BoxImplHorizontal::BoxImplHorizontal(Delegate* delegate,
                                     std::vector<WidgetFactory> children)
    : BoxImpl(delegate, children) {}

glm::vec2 BoxImplHorizontal::ComputeDimensions() {
  requested_dimensions_ = glm::vec2(0.f, 0.f);
  int i = 0;
  for (auto& it : children_) {
    auto child_dimension = it->ComputeDimensions();
    requested_dimensions_.y =
        std::max(requested_dimensions_.y, child_dimension.y);
    requested_dimensions_.x += child_dimension.x;
    children_size_[i++] = child_dimension.x;
  }
  if (children_.size())
    requested_dimensions_ += (children_.size() - 1) * size::widget_margin;
  return requested_dimensions_;
}

void BoxImplHorizontal::SetDimensions(glm::vec2 dimensions) {
  Widget::SetDimensions(dimensions);
  float space_left = dimensions.x - requested_dimensions_.x;
  space_left /= children_.size();
  int i = 0;
  for (auto& it : children_)
    it->SetDimensions({children_size_[i++] + space_left, dimensions.y});
}

void BoxImplHorizontal::SetPosition(glm::vec2 position) {
  Widget::SetPosition(position);
  for (auto& it : children_) {
    it->SetPosition(position);
    position.x += it->dimensions().x + size::widget_margin;
  }
}

// static
WidgetFactory HBox(std::vector<WidgetFactory> children) {
  return [=](Widget::Delegate* delegate) {
    return std::make_unique<BoxImplHorizontal>(delegate, children);
  };
}

// static
WidgetFactory VBox(std::vector<WidgetFactory> children) {
  return [=](Widget::Delegate* delegate) {
    return std::make_unique<BoxImplVertical>(delegate, children);
  };
}

// static
BoxInterface* Box(Widget* w) {
  return dynamic_cast<BoxInterface*>(w);
}

}  // namespace smkflow
