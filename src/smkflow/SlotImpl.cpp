// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <algorithm>
#include <smk/Shape.hpp>
#include <smk/Transformable.hpp>
#include <smkflow/BoardImpl.hpp>
#include <smkflow/ConnectorImpl.hpp>
#include <smkflow/Constants.hpp>
#include <smkflow/NodeImpl.hpp>
#include <smkflow/SlotImpl.hpp>

namespace smkflow {

smk::Transformable circle;
smk::Transformable circle_background;

SlotImpl::SlotImpl(NodeImpl* node,
                   std::string label,
                   bool is_right,
                   glm::vec4 color)
    : node_(node), is_right_(is_right), color_(color) {
  circle_background = smk::Shape::Circle(8);
  circle = smk::Shape::Circle(5);
  text_ = label + " ";
  SetText(label);
}

glm::vec2 SlotImpl::ComputeDimensions() {
  auto dimensions = label_.ComputeDimensions();
  dimensions.y = std::max(dimensions.y, size::widget_height);
  return dimensions;
}

glm::vec2 SlotImpl::GetPosition() {
  return node_->GetPosition() + position_;
}

void SlotImpl::Draw(smk::RenderTarget* target) {
  auto position = GetPosition();

  circle_background.SetPosition(position);
  circle_background.SetColor(color::connector_background);
  target->Draw(circle_background);

  circle.SetPosition(position);
  circle.SetColor(color_);
  target->Draw(circle);

  label_.SetPosition(position);
  target->Draw(label_);
}

void SlotImpl::Connect(ConnectorImpl* connector) {
  if (connector->output() == this) {
    while (!connectors_.empty())
      Disconnect(connectors_.back());
  }

  for (auto& it : connectors_) {
    if (it->input() == connector->input() &&
        it->output() == connector->output()) {
      return;
    }
  }
  connectors_.push_back(connector);
  // if (connector_)
  // Disconnect(connector_);
  // connector_ = connector;
}
void SlotImpl::Disconnect(ConnectorImpl* connector) {
  auto it = std::find(connectors_.begin(), connectors_.end(), connector);
  if (it == connectors_.end())
    return;
  connectors_.erase(it);
  connector->Disconnect();
}

bool SlotImpl::IsRight() {
  return is_right_;
}

glm::vec4 SlotImpl::GetColor() {
  return color_;
}

void SlotImpl::SetText(const std::string& text) {
  if (text_ == text)
    return;
  text_ = text;
  label_ = smk::Text(node_->boardImpl()->font(), text_);
  auto center = label_.ComputeDimensions();
  center.y *= 0.5;
  if (IsRight())
    center.x += 5;
  else
    center.x = -5;
  label_.SetCenter(center);

  node_->InvalidateLayout();
}

Node* SlotImpl::GetNode() {
  return node_;
}

Connector* SlotImpl::GetConnector() {
  return connectors_.size() >= 1 ? connectors_[0] : nullptr;
}

Slot* SlotImpl::OppositeSlot() {
  auto* connector = GetConnector();
  return !connector ? nullptr
                    : connector->GetInput() == this ? connector->GetOutput()
                                                    : connector->GetInput();
}

Node* SlotImpl::OppositeNode() {
  auto* opposite_slot = OppositeSlot();
  return opposite_slot ? opposite_slot->GetNode() : nullptr;
}

}  // namespace smkflow
