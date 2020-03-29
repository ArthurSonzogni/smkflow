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
  color_ = model.color;
  identifier_ = model.identifier;
  width_ = 0.f;   // 32.f;
  height_ = 0.f;  // 32.f;

  title_ = smk::Text(board->font(), model.label);
  title_.SetColor(smk::Color::Black);

  for(const auto& model: model.input) {
    auto slot = std::make_unique<SlotImpl>(this, model.label, false, model.color);
    inputs_.push_back(std::move(slot));
  }

  for (const auto& model : model.output) {
    auto slot = std::make_unique<SlotImpl>(this, model.label, true,
                                           model.color);
    outputs_.push_back(std::move(slot));
  }

  for (const auto& builder : model.widgets)
    widgets_.push_back(builder(this));

  Layout();
}

void NodeImpl::Layout() {
  // Compute the dimensions ----------------------------------------------------
  auto title_dimension = title_.ComputeDimensions();

  float widget_width = 0.f;
  float widget_height = 0.f;
  for (const auto& widget : widgets_) {
    auto dimensions = widget->ComputeDimensions();
    widget->SetDimensions(dimensions);
    widget_width = std::max(widget_width, dimensions.x);
    widget_height += dimensions.y;
  }

  float input_width = 0.f;
  float input_height = 0.f;
  for (const auto& slot : inputs_) {
    auto dimension = slot->ComputeDimensions();
    input_width = std::max(input_width, dimension.x);
    input_height += dimension.y;
  }

  float output_width = 0.f;
  float output_height = 0.f;
  for (const auto& slot : outputs_) {
    auto dimension = slot->ComputeDimensions();
    output_width = std::max(output_width, dimension.x);
    output_height += dimension.y;
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
  y = y_b + board_->font().size() * 0.5f;
  for (auto& slot : inputs_) {
    slot->SetPosition({x, y});
    auto dimension = slot->ComputeDimensions();
    y += dimension.y;
  }

  x = x_d;
  y = y_b + board_->font().size() * 0.5f;
  for (auto& slot : outputs_) {
    auto dimension = slot->ComputeDimensions();
    slot->SetPosition({x, y});
    y += dimension.y;
  }

  x = x_b;
  y = y_b;
  for(auto& widget : widgets_) {
    auto dimensions = widget->ComputeDimensions();
    widget->SetPosition({x, y});
    widget->SetDimensions({widget_width, dimensions.y});
    y += dimensions.y;
  }

  width_ = x_d;
  height_ = y_c;

  base_ = smk::Shape::RoundedRectangle(width_, height_, 10);
  base_.SetCenter(-width_ * 0.5, -height_ * 0.5);
  base_.SetColor({0.5, 0.5, 0.5, 0.5});

  title_base_ = smk::Shape::RoundedRectangle(width_, title_height, 10);
  title_base_.SetCenter(-width_ * 0.5, -title_height * 0.5);
  title_base_.SetColor(color_ * 0.8f);
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
  for(auto& widget : widgets_)
    widget->Draw(target);
}

void NodeImpl::Step(smk::Input* input, glm::vec2 cursor) {
  for(auto& widget : widgets_)
    widget->Step(input, cursor);

  bool relayout = false;
  for (auto& slot : inputs_)
    relayout |= slot->ValidateDimensions();
  for (auto& slot : outputs_)
    relayout |= slot->ValidateDimensions();
  for (auto& widget : widgets_)
    relayout |= widget->ValidateDimensions();

  if (relayout)
    Layout();
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
int NodeImpl::WidgetCount() {
  return widgets_.size();
}
Widget* NodeImpl::WidgetAt(int i) {
  return widgets_.at(i).get();
}

}  // namespace smkflow
