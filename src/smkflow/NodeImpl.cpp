// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <algorithm>
#include <smk/Font.hpp>
#include <smk/Shape.hpp>
#include <smkflow/BoardImpl.hpp>
#include <smkflow/Constants.hpp>
#include <smkflow/NodeImpl.hpp>
#include <smkflow/SlotImpl.hpp>

namespace smkflow {

float width = 150;
float height = 150;
float title_height = 48;
float padding = 10;

NodeImpl::NodeImpl(BoardImpl* board, const model::Node& model) : board_(board) {
  color_ = model.color;
  identifier_ = model.identifier;
  width_ = 0.f;   // 32.f;
  height_ = 0.f;  // 32.f;

  title_ = smk::Text(board->font(), model.label);
  title_.SetColor(color::text);

  for (const auto& model : model.input) {
    auto slot =
        std::make_unique<SlotImpl>(this, model.label, false, model.color);
    inputs_.push_back(std::move(slot));
  }

  for (const auto& model : model.output) {
    auto slot =
        std::make_unique<SlotImpl>(this, model.label, true, model.color);
    outputs_.push_back(std::move(slot));
  }

  if (model.widget)
    widget_ = model.widget(this);

  Layout();
}

void NodeImpl::InvalidateLayout() {
  layout_invalidated_ = true;
}

void NodeImpl::Layout() {
  layout_invalidated_ = false;
  auto title_dimension =
      title_.ComputeDimensions() + 6.f * padding * glm::vec2(1.f);

  float widget_width = 0.f;
  float widget_height = 0.f;
  if (widget_) {
    auto dimensions = widget_->ComputeDimensions();
    widget_->SetDimensions(dimensions);
    widget_width = std::max(widget_width, dimensions.x);
    widget_height += dimensions.y + size::widget_margin;
  }

  float input_width = 0.f;
  float input_height = 0.f;
  for (const auto& slot : inputs_) {
    auto dimension = slot->ComputeDimensions();
    input_width = std::max(input_width, dimension.x);
    input_height += dimension.y + size::widget_margin;
  }

  float output_width = 0.f;
  float output_height = 0.f;
  for (const auto& slot : outputs_) {
    auto dimension = slot->ComputeDimensions();
    output_width = std::max(output_width, dimension.x);
    output_height += dimension.y + size::widget_margin;
  }

  input_width += padding;
  output_width += padding;
  widget_width = std::max(widget_width, title_dimension.x - input_width -
                                            output_height + 2 * padding);
  const float x_a = 0.f;
  const float x_b = x_a + input_width;
  const float x_c = x_b + widget_width;
  const float x_d = x_c + output_width;

  const float y_a = 0.f;
  const float y_b = y_a + title_height + padding;
  const float y_c =
      y_b + std::max(input_height, std::max(widget_height, output_height));

  width_ = x_d;
  height_ = y_c;

  float x, y;
  x = x_a;
  y = y_b + board_->font().line_height() * 0.5f;
  for (auto& slot : inputs_) {
    slot->SetPosition({x, y});
    auto dimension = slot->ComputeDimensions();
    y += dimension.y + size::widget_margin;
  }

  x = x_d;
  y = y_b + board_->font().line_height() * 0.5f;
  for (auto& slot : outputs_) {
    auto dimension = slot->ComputeDimensions();
    slot->SetPosition({x, y});
    y += dimension.y + size::widget_margin;
  }

  x = x_b;
  y = y_b;
  if (widget_) {
    auto dimensions = widget_->ComputeDimensions();
    widget_->SetDimensions({widget_width, dimensions.y});
    widget_->SetPosition({x, y});
    y += dimensions.y + size::widget_margin;
  }

  width_ = x_d;
  height_ = y_c;

  base_ = smk::Shape::RoundedRectangle(width_, height_, 10);
  base_.SetCenter(-width_ * 0.5, -height_ * 0.5);
  base_.SetColor(color::node_background);

  title_base_ = smk::Shape::RoundedRectangle(width_, title_height, 10);
  title_base_.SetCenter(-width_ * 0.5, -title_height * 0.5);
  title_base_.SetColor(color_);

  dimension_ = {width_, height_};
}

NodeImpl::~NodeImpl() = default;

void NodeImpl::Draw(smk::RenderTarget* target) {
  base_.SetPosition(position_);
  title_base_.SetPosition(position_);
  title_.SetPosition(position_ + glm::vec2(padding, padding));

  target->Draw(base_);

  if (!Selected())
    target->Draw(title_base_);
  target->Draw(title_);
  for (auto& slot : inputs_)
    slot->Draw(target);
  for (auto& slot : outputs_)
    slot->Draw(target);
  if (widget_) {
    widget_->Draw(target);
  }
}

void NodeImpl::Step(smk::Input* input, glm::vec2 cursor) {
  position_ += speed_;
  speed_ *= 0.2f;
  if (input->IsCursorReleased())
    cursor_captured_.Invalidate();

  if (layout_invalidated_)
    Layout();

  if (widget_)
    widget_->Step(input, cursor);

  bool hover = cursor.x > position_.x && cursor.x < position_.x + width_ &&
               cursor.y > position_.y && cursor.y < position_.y + height_;

  if (input->IsCursorPressed()) {
    if (hover) {
      if (input->IsKeyHold(GLFW_KEY_LEFT_CONTROL))
        cursor_captured_for_selection_ = board_->CaptureSelection();
      cursor_captured_ = board_->CaptureCursor();
    }
    cursor_drag_point = position_ - cursor;
  }

  if (!input->IsCursorHeld())
    return;

  if (!(cursor_captured_ || cursor_captured_for_selection_))
    return;

  position_ = cursor_drag_point + cursor;
  speed_ = glm::vec2(0.f, 0.f);
}

void NodeImpl::Push(glm::vec2 direction) {
  position_ += direction;
  speed_ += direction * 0.2f;
}
void NodeImpl::SetPosition(const glm::vec2& position) {
  position_ = position;
  speed_ = glm::vec2(0.f, 0.f);
}

const glm::vec2& NodeImpl::GetPosition() {
  return position_;
}

SlotImpl* NodeImpl::FindSlot(const glm::vec2& position) {
  for (auto& slot : inputs_) {
    glm::vec2 slot_position = slot->GetPosition();
    if (glm::distance(position, slot_position) < 16)
      return slot.get();
  }
  for (auto& slot : outputs_) {
    glm::vec2 slot_position = slot->GetPosition();
    if (glm::distance(position, slot_position) < 16)
      return slot.get();
  }
  return nullptr;
}

Board* NodeImpl::GetBoard() {
  return board_;
}

int NodeImpl::InputCount() {
  return inputs_.size();
}
Slot* NodeImpl::InputAt(int i) {
  return inputs_.at(i).get();
}
int NodeImpl::OutputCount() {
  return outputs_.size();
}
Slot* NodeImpl::OutputAt(int i) {
  return outputs_.at(i).get();
}
Widget* NodeImpl::widget() {
  return widget_.get();
}

smk::Font& NodeImpl::Font() {
  return board_->font();
}

CursorCapture NodeImpl::CaptureCursor() {
  return board_->CaptureCursor();
}

Board* NodeImpl::board() {
  return board_;
}

JSON NodeImpl::Serialize() {
  JSON json;
  json["position"] = {position_.x, position_.y};
  json["identifier"] = identifier_;
  if (widget_)
    json["widget"] = widget_->Serialize();
  return json;
}

bool NodeImpl::Deserialize(JSON& json) {
  position_.x = json["position"][0];
  position_.y = json["position"][1];
  if (widget_)
    widget_->Deserialize(json["widget"]);
  return true;
}

bool NodeImpl::Selected() {
  return bool(cursor_captured_for_selection_);
}

}  // namespace smkflow
