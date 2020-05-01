// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include "smkflow/ConnectorImpl.hpp"

#include <smk/Shape.hpp>
#include <smkflow/Constants.hpp>

#include "smkflow/SlotImpl.hpp"

namespace smkflow {

ConnectorImpl::ConnectorImpl(SlotImpl* A, SlotImpl* B) : input_(A), output_(B) {
  if (!input_->IsRight() && output_->IsRight())
    std::swap(input_, output_);
  input_->Connect(this);
  output_->Connect(this);
}

ConnectorImpl::~ConnectorImpl() {
  Disconnect();
}

bool ConnectorImpl::IsConnected() {
  return input_;
}

void ConnectorImpl::Disconnect() {
  SlotImpl* input = input_;
  SlotImpl* output = output_;
  input_ = nullptr;
  output_ = nullptr;
  if (input)
    input->Disconnect(this);
  if (output)
    output->Disconnect(this);
}

void ConnectorImpl::DrawBackground(smk::RenderTarget* target) {
  if (!IsConnected())
    return;
  RebuildVertex();
  target->Draw(background_);
}

void ConnectorImpl::DrawForeground(smk::RenderTarget* target) {
  if (!IsConnected())
    return;
  RebuildVertex();
  target->Draw(foreground_);
}

void ConnectorImpl::RebuildVertex() {
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
      32);

  background_ = smk::Shape::Path(bezier, 16);
  foreground_ = smk::Shape::Path(bezier, 10);

  background_.SetColor(color::connector_background);
  foreground_.SetColor(input_->GetColor());
}

Slot* ConnectorImpl::GetInput() {
  return input_;
}

Slot* ConnectorImpl::GetOutput() {
  return output_;
}

}  // namespace smkflow
