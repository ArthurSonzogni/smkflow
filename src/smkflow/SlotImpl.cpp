#include <algorithm>
#include <smk/Color.hpp>
#include <smk/Shape.hpp>
#include <smk/Transformable.hpp>
#include <smkflow/ConnectorImpl.hpp>
#include <smkflow/Constants.hpp>
#include <smkflow/NodeImpl.hpp>
#include <smkflow/SlotImpl.hpp>
#include <smkflow/BoardImpl.hpp>

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
  SetText(label);
}

glm::vec2 SlotImpl::ComputeDimensions() {
  return label_.ComputeDimensions();
}

glm::vec2 SlotImpl::GetPosition() {
  return node_->GetPosition() + position_;
}

void SlotImpl::Draw(smk::RenderTarget* target) {
  auto position = GetPosition();

  circle_background.SetPosition(position);
  circle_background.SetColor(connector_background_color);
  target->Draw(circle_background);

  circle.SetPosition(position);
  circle.SetColor(color_);
  target->Draw(circle);

  label_.SetPosition(position);
  target->Draw(label_);
}

bool SlotImpl::ValidateDimensions() {
  bool ret = dimensions_modified_;
  dimensions_modified_ = false;
  return ret;
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
  label_ = smk::Text(node_->board()->font(), text);
  auto center = label_.ComputeDimensions();
  center.y *= 0.7;
  if (IsRight())
    center.x += 5;
  else
    center.x = -5;
  label_.SetCenter(center);
  dimensions_modified_ = true;
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
