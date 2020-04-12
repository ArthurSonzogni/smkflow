// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <iostream>
#include <smk/Shape.hpp>
#include <smkflow/BoardImpl.hpp>
#include <smkflow/ConnectorImpl.hpp>
#include <smkflow/Constants.hpp>
#include <smkflow/NodeImpl.hpp>
#include <smkflow/SlotImpl.hpp>

namespace smkflow {

// static
std::unique_ptr<Board> Board::Create(const model::Board& model) {
  return std::make_unique<BoardImpl>(model);
}

BoardImpl::~BoardImpl() = default;

BoardImpl::BoardImpl(const model::Board& model) {
  font_ = smk::Font(model.font, 30);
}

NodeImpl* BoardImpl::Create(const model::Node& model) {
  nodes_.push_back(std::make_unique<NodeImpl>(this, model));
  return nodes_.back().get();
}

void BoardImpl::Step(smk::RenderTarget* target, smk::Input* input) {
  input_ = input;
  (void)target;

  // For some reasons the scrolling offset in web browsers is 100x less than the
  // one on desktop?
  // TODO(arthursonzogni): Figure out why???
#if __EMSCRIPTEN__
  float zoom_increment = input->ScrollOffset().y * 0.001f;
#else
  float zoom_increment = input->ScrollOffset().y * 0.1f;
#endif

  view_shifting_ += (input->cursor() - target->dimension() * 0.5f) *
                    std::exp(view_zoom_) * (1.f - std::exp(zoom_increment));
  view_zoom_ += zoom_increment;

  cursor_ =
      (input->cursor() - target->dimension() * 0.5f) * std::exp(view_zoom_) +
      view_shifting_;

  ReleaseConnector();
  ReleaseView();
  AquireConnector();

  for (const auto& node : nodes_)
    node->Step(input_, cursor_);

  MoveConnector();
  AcquireView();
  MoveView();
}

void BoardImpl::AquireConnector() {
  if (!input_->IsCursorPressed())
    return;

  start_slot_ = FindSlot(cursor_);
  if (!start_slot_)
    return;

  cursor_captured_for_connector_ = CaptureCursor();
  if (!cursor_captured_for_connector_)
    return;

  connector_in_ = start_slot_->GetPosition();
  connector_in_pushed_ = start_slot_->GetPosition();
  connector_out_pushed_ = start_slot_->GetPosition();
  connector_out_ = start_slot_->GetPosition();
}

void BoardImpl::MoveConnector() {
  if (!cursor_captured_for_connector_)
    return;

  float d = glm::distance(connector_in_, cursor_);
  glm::vec2 strength(d * 0.4, 0);

  glm::vec2 connector_in_pushed =
      connector_in_ + (start_slot_->IsRight() ? +strength : -strength);
  glm::vec2 connector_out_pushed = cursor_;

  SlotImpl* end_slot = FindSlot(cursor_);
  if (end_slot && end_slot != start_slot_ &&
      end_slot->GetColor() == start_slot_->GetColor()) {
    connector_out_ += (end_slot->GetPosition() - connector_out_) * 0.5f;
    connector_out_pushed =
        connector_out_ + (end_slot->IsRight() ? +strength : -strength);
    connector_out_ += (connector_out_ - connector_out_) * 0.2f;
  } else {
    connector_out_ = cursor_;
  }
  connector_out_pushed_ +=
      (connector_out_pushed - connector_out_pushed_) * 0.2f;
  connector_in_pushed_ += (connector_in_pushed - connector_in_pushed_) * 0.2f;
}

void BoardImpl::ReleaseConnector() {
  if (!cursor_captured_for_connector_)
    return;

  if (!input_->IsCursorReleased())
    return;

  cursor_captured_for_connector_.reset();

  SlotImpl* end_slot = FindSlot(cursor_);
  if (!end_slot)
    return;

  if (end_slot->GetColor() != start_slot_->GetColor())
    return;

  auto connector = std::make_unique<ConnectorImpl>(start_slot_, end_slot);
  connectors_.push_back(std::move(connector));
}

// void BoardImpl::OnCursorPressed(smk::Input* input_, glm::vec2 cursor_) {

//// Move node.
// for (auto& node : nodes_) {
// if (node->OnCursorPressed(cursor_)) {
// selected_node_ = node.get();
//// Reorder to make selected_node_ to be displayed in front.
// int i = 0;
// while (nodes_[i].get() != selected_node_)
//++i;
// while (++i != (int)nodes_.size())
// std::swap(nodes_[i - 1], nodes_[i]);
// return;
//}
//}

//// Translation.
//}

void BoardImpl::AcquireView() {
  if (!input_->IsCursorPressed())
    return;

  cursor_captured_for_dragging_view_ = CaptureCursor();
  if (!cursor_captured_for_dragging_view_)
    return;

  grab_point_ = view_shifting_ + input_->cursor() * std::exp(view_zoom_);
}

void BoardImpl::MoveView() {
  if (!cursor_captured_for_dragging_view_)
    return;
  view_shifting_ = grab_point_ - input_->cursor() * std::exp(view_zoom_);
}

void BoardImpl::ReleaseView() {
  if (!input_->IsCursorReleased())
    return;

  cursor_captured_for_dragging_view_.reset();
}

void BoardImpl::Draw(smk::RenderTarget* target) {
  auto view = smk::View();
  view.SetCenter(view_shifting_);
  view.SetSize(target->dimension() * std::exp(view_zoom_));
  target->SetView(view);

  for (const auto& node : nodes_)
    node->Draw(target);
  for (const auto& connector : connectors_)
    connector->DrawBackground(target);
  for (const auto& connector : connectors_)
    connector->DrawForeground(target);

  if (cursor_captured_for_connector_) {
    auto bezier = smk::Shape::Bezier(
        {
            connector_in_,
            connector_in_pushed_,
            connector_out_pushed_,
            connector_out_,
        },
        32);

    auto background_ = smk::Shape::Path(bezier, 16);
    auto foreground_ = smk::Shape::Path(bezier, 10);

    background_.SetColor(color::connector_background);
    foreground_.SetColor(start_slot_->GetColor());
    target->Draw(background_);
    target->Draw(foreground_);
  }
}

SlotImpl* BoardImpl::FindSlot(const glm::vec2& position) {
  for (auto& node : nodes_) {
    SlotImpl* slot = node->FindSlot(position);
    if (slot)
      return slot;
  }
  return nullptr;
}

class CursorCaptureImpl : public CursorCaptureInterface {
 public:
  CursorCaptureImpl(bool* value) : value_(value) {}
  ~CursorCaptureImpl() override { *value_ = false; }

 private:
  bool* value_;
};

CursorCapture BoardImpl::CaptureCursor() {
  if (cursor_captured_)
    return nullptr;

  cursor_captured_ = true;
  return std::make_unique<CursorCaptureImpl>(&cursor_captured_);
}

}  // namespace smkflow
