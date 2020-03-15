#include <smk/Color.hpp>
#include <smk/Shape.hpp>
#include <smk/Transformable.hpp>
#include <smkflow/Slot.hpp>
#include <smkflow/Connector.hpp>
#include <algorithm>
#include <smkflow/Node.hpp>

namespace smkflow {

smk::Transformable circle;
smk::Transformable circle_background;

Slot::Slot(Node* node,
           glm::vec2 position,
           smk::Text label,
           bool is_right,
           glm::vec4 color)
    : node_(node),
      position_(position),
      label_(label),
      is_right_(is_right),
      color_(color) {
  circle_background = smk::Shape::Circle(8);
  circle = smk::Shape::Circle(5);
}

glm::vec2 Slot::GetPosition() {
  return node_->GetPosition() + position_;
}

void Slot::Draw(smk::RenderTarget* target) {
  auto position = GetPosition();

  circle_background.SetPosition(position);
  circle_background.SetColor({0, 0, 0, 0.2});
  target->Draw(circle_background);

  circle.SetPosition(position);
  circle.SetColor(color_);
  target->Draw(circle);

  label_.SetPosition(position);
  target->Draw(label_);
}

void Slot::Connect(Connector* connector) {
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
void Slot::Disconnect(Connector* connector) {
  auto it = std::find(connectors_.begin(), connectors_.end(), connector);
  if (it == connectors_.end())
    return;
  connectors_.erase(it);
  connector->Disconnect();
}

bool Slot::IsRight() {
  return is_right_;
}

glm::vec4 Slot::GetColor() {
  return color_;
}

}  // namespace smkflow
