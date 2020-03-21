#include <smk/Color.hpp>
#include <smk/Font.hpp>
#include <smk/Shape.hpp>
#include <smkflow/BoardImpl.hpp>
#include <smkflow/NodeImpl.hpp>
#include <smkflow/SlotImpl.hpp>

namespace smkflow {

float width = 150;
float height = 150;
float title_height = 48;
float padding = 10;

NodeImpl::NodeImpl(BoardImpl* board, const model::Node& model) : board_(board) {
  identifier_ = model.identifier;
  width_ = 0.f;   // 32.f;
  height_ = 0.f;  // 32.f;

  title_ = smk::Text(board->font(), model.label);
  title_.SetColor(smk::Color::Black);

  auto title_dimension = title_.ComputeDimensions();
  width_ = title_dimension.x + 2 * padding;
  height_ = title_dimension.y + 2 * padding;

  float input_width = 0.f;
  for (const auto& slot : model.input) {
    auto label = smk::Text(board->font(), slot.label);
    input_width = std::max(input_width, label.ComputeDimensions().x);
  }

  float output_width = 0.f;
  for (const auto& slot : model.output) {
    auto label = smk::Text(board->font(), slot.label);
    output_width = std::max(output_width, label.ComputeDimensions().x);
  }

  float input_output_y_start = title_height + padding;
  {
    float x = 0.f;
    float y = input_output_y_start;
    for (const auto& slot : model.input) {
      auto label = smk::Text(board->font(), slot.label);
      auto dimension = label.ComputeDimensions();
      x = 0.f;
      y += dimension.y * 0.5f;
      label.SetCenter(-padding, dimension.y * 0.5f);
      inputs_.push_back(std::make_unique<SlotImpl>(
          this, glm::vec2(x, y), std::move(label), false, slot.color));
      input_width = std::max(input_width, padding + dimension.x + padding);
      y += dimension.y * 0.5f + padding;
    }
    height_ = std::max(height_, y);
  }

  width_ = std::max(width_, input_width + padding + output_width);
  {
    float x = width_;
    float y = input_output_y_start;
    for (const auto& slot : model.output) {
      auto label = smk::Text(board->font(), slot.label);
      auto dimension = label.ComputeDimensions();
      y += dimension.y * 0.5f;
      label.SetCenter(padding + dimension.x, dimension.y * 0.5f);
      outputs_.push_back(std::make_unique<SlotImpl>(
          this, glm::vec2(x, y), std::move(label), true, slot.color));
      y += dimension.y * 0.5f + padding;
    }
    height_ = std::max(height_, y);
  }

  base_ = smk::Shape::RoundedRectangle(width_, height_, 10);
  base_.SetCenter(-width_ * 0.5, -height_ * 0.5);
  base_.SetColor({0.5, 0.5, 0.5, 0.5});

  title_base_ = smk::Shape::RoundedRectangle(width_, title_height, 10);
  title_base_.SetCenter(-width_ * 0.5, -title_height * 0.5);
  title_base_.SetColor(model.color * 0.8f);
}

NodeImpl::~NodeImpl() = default;
void NodeImpl::Draw(smk::RenderTarget* target) {
  base_.SetPosition(position_);
  title_base_.SetPosition(position_);
  title_.SetPosition(position_ + glm::vec2(padding, padding));

  target->Draw(base_);
  target->Draw(title_base_);
  target->Draw(title_);

  for (auto& slot : inputs_)
    slot->Draw(target);
  for (auto& slot : outputs_)
    slot->Draw(target);
}

bool NodeImpl::OnCursorPressed(glm::vec2 cursor) {
  bool hover = cursor.x > position_.x && cursor.x < position_.x + width_ &&
               cursor.y > position_.y && cursor.y < position_.y + height_;

  if (!hover)
    return false;

  cursor_drag_point = position_ - cursor;
  return true;
}

void NodeImpl::OnCursorMoved(glm::vec2 cursor) {
  position_ = cursor_drag_point + cursor;
}

void NodeImpl::OnCursorReleased(glm::vec2) {}

void NodeImpl::SetPosition(const glm::vec2& position) {
  position_ = position;
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

}  // namespace smkflow
