#include <iostream>
#include <smk/Input.hpp>
#include <smk/Shape.hpp>
#include <smkflow/BoardImpl.hpp>
#include <smkflow/Connector.hpp>
#include <smkflow/Node.hpp>
#include <smkflow/Slot.hpp>

namespace smkflow {

// static
std::unique_ptr<Board> Board::Create(const model::Board& model) {
  return std::make_unique<BoardImpl>(model);
}

BoardImpl::~BoardImpl() = default;

BoardImpl::BoardImpl(const model::Board& model) {
  font_ = smk::Font(model.font, 30);
  int x = 0;
  int y = 0;

  for (const model::Node& node : model.nodes) {
    nodes_.push_back(std::make_unique<Node>(this, node));
    nodes_.back()->SetPosition({150 * x, 150 * y});

    ++x;
    y += x / 4;
    x %= 4;
  }
}

void BoardImpl::Step(smk::RenderTarget* target, smk::Input* input) {
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

  auto cursor =
      (input->cursor() - target->dimension() * 0.5f) * std::exp(view_zoom_) +
      view_shifting_;

  if (input->IsCursorPressed()) {
    // Set cursor.
    start_slot_ = FindSlot(cursor);
    if (start_slot_) {
      connector_in_ = start_slot_->GetPosition();
      connector_in_pushed_ = start_slot_->GetPosition();
      connector_out_pushed_ = start_slot_->GetPosition();
      connector_out_ = start_slot_->GetPosition();
      return;
    }

    // Move node.
    for (auto& node : nodes_) {
      if (node->OnCursorPressed(cursor)) {
        selected_node_ = node.get();
        // Reorder moved node.
        std::swap(node, nodes_.back());
        return;
      }
    }

    // Translation.
    grab_point_ = view_shifting_ + input->cursor() * std::exp(view_zoom_);
  }

  if (input->IsCursorHold()) {
    if (start_slot_) {
      glm::vec2 connector_out = cursor;

      float d = glm::distance(connector_in_, connector_out);
      glm::vec2 strength(d * 0.4, 0);

      glm::vec2 connector_in_pushed =
          connector_in_ + (start_slot_->IsRight() ? +strength : -strength);
      glm::vec2 connector_out_pushed = connector_out;

      Slot* end_slot = FindSlot(cursor);
      if (end_slot && end_slot->GetColor() == start_slot_->GetColor()) {
        connector_out = end_slot->GetPosition();
        connector_out_pushed =
            connector_out_ + (end_slot->IsRight() ? +strength : -strength);
        connector_out_ += (connector_out - connector_out_) * 0.3f;
      }
      connector_out_ = connector_out;
      connector_out_pushed_ +=
          (connector_out_pushed - connector_out_pushed_) * 0.3f;
      connector_in_pushed_ +=
          (connector_in_pushed - connector_in_pushed_) * 0.3f;

      return;
    }

    if (selected_node_) {
      selected_node_->OnCursorMoved(cursor);
      return;
    }

    view_shifting_ = grab_point_ - input->cursor() * std::exp(view_zoom_);
    return;
  }

  if (input->IsCursorReleased()) {
    if (start_slot_) {
      if (Slot* end_slot = FindSlot(cursor)) {
        if (end_slot->GetColor() == start_slot_->GetColor()) {
          connectors_.push_back(
              std::make_unique<Connector>(start_slot_, end_slot));
        }
      }
      start_slot_ = nullptr;
    }
    selected_node_ = nullptr;
    return;
  }
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

  if (start_slot_) {
    auto bezier = smk::Shape::Bezier(
        {
            connector_in_,
            connector_in_pushed_,
            connector_out_pushed_,
            connector_out_,
        },
        16);

    auto background_ = smk::Shape::Path(bezier, 16);
    auto foreground_ = smk::Shape::Path(bezier, 10);

    background_.SetColor({0.0, 0.0, 0.0, 0.3});
    foreground_.SetColor(start_slot_->GetColor());
    target->Draw(background_);
    target->Draw(foreground_);
  }
}

Slot* BoardImpl::FindSlot(const glm::vec2& position) {
  for (auto& node : nodes_) {
    Slot* slot = node->FindSlot(position);
    if (slot)
      return slot;
  }
  return nullptr;
}

}  // namespace smkflow
