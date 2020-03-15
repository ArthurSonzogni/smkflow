#include "smkflow/Connector.hpp"

#include <smk/Color.hpp>
#include <smk/Shape.hpp>

#include "smkflow/Slot.hpp"

namespace smkflow {

Connector::Connector(Slot* A, Slot* B) : input_(A), output_(B) {
  if (!input_->IsRight() && output_->IsRight())
    std::swap(input_, output_);
  input_->Connect(this);
  output_->Connect(this);
}

Connector::~Connector() {
  Disconnect();
}

bool Connector::IsConnected() {
  return input_;
}

void Connector::Disconnect() {
  Slot* input = input_;
  Slot* output = output_;
  input_ = nullptr;
  output_ = nullptr;
  if (input)
    input->Disconnect(this);
  if (output)
    output->Disconnect(this);
}

void Connector::DrawBackground(smk::RenderTarget* target) {
  if (!IsConnected())
    return;
  RebuildVertex();
  target->Draw(background_);
}

void Connector::DrawForeground(smk::RenderTarget* target) {
  if (!IsConnected())
    return;
  RebuildVertex();
  target->Draw(foreground_);
}

void Connector::RebuildVertex() {
  glm::vec2 position_a = input_->GetPosition();
  glm::vec2 position_b = output_->GetPosition();

  if (position_a == position_a_ && position_b == position_b_)
    return;

  position_a_ = position_a;
  position_b_ = position_b;

  float d = glm::distance(position_a, position_b);
  glm::vec2 strength(d * 0.4, 0);

  auto bezier = smk::Shape::Bezier(
      {
          position_a,
          position_a + (input_->IsRight() ? +strength : -strength),
          position_b + (output_->IsRight() ? +strength : -strength),
          position_b,
      },
      16);

  background_ = smk::Shape::Path(bezier, 16);
  foreground_ = smk::Shape::Path(bezier, 10);

  background_.SetColor({0.0, 0.0, 0.0, 0.3});
  foreground_.SetColor(input_->GetColor());
}

}  // namespace smkflow
