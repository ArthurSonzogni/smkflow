// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <algorithm>
#include <smk/Shape.hpp>
#include <smkflow/BoardImpl.hpp>
#include <smkflow/ConnectorImpl.hpp>
#include <smkflow/Constants.hpp>
#include <smkflow/NodeImpl.hpp>
#include <smkflow/SlotImpl.hpp>
#include <smkflow/widget/Box.hpp>
#include <smkflow/widget/MenuImpl.hpp>

namespace smkflow {

class BoardImpl::MenuDelegate : public Widget::Delegate {
 public:
  MenuDelegate(BoardImpl* board) : board_(board) {}
  glm::vec2 Position() override { return {0.f, 0.f}; }
  void InvalidateLayout() override { invalidated_ = true; }
  smk::Font& Font() override { return board_->font(); }
  CursorCapture CaptureCursor() override {
    cursor_capturable_.Invalidate();
    return cursor_capturable_.Capture();
  }
  bool IsInsideMenu() override { return true; }
  bool Validate() {
    bool ret = invalidated_;
    invalidated_ = false;
    return ret;
  }
  BoardImpl* board() override { return board_; }

 private:
  bool invalidated_ = true;
  BoardImpl* board_;
  CursorCapturable cursor_capturable_;
};

// static
std::unique_ptr<Board> Board::Create(const model::Board& model) {
  return std::make_unique<BoardImpl>(model);
}

BoardImpl::~BoardImpl() = default;

BoardImpl::BoardImpl(const model::Board& model) : model_(model) {
  font_ = smk::Font(model_.font, 30);
  menu_delegate_ = std::make_unique<MenuDelegate>(this);
  menu_widget_ = VBox(model.context_widgets_)(menu_delegate_.get());
  square_ = smk::Shape::Square();
}

NodeImpl* BoardImpl::Create(const model::Node& model) {
  cursor_captured_for_menu_.Invalidate();
  nodes_.push_back(std::make_unique<NodeImpl>(this, model));
  return nodes_.back().get();
}

void BoardImpl::Step(smk::RenderTarget* target, smk::Input* input) {
  input_ = input;

  // For some reasons the scrolling offset in web browsers is 100x less than the
  // one on desktop?
  // TODO(arthursonzogni): Figure out why???
#if __EMSCRIPTEN__
  float zoom_increment = input->scroll_offset().y * 0.001f;
#else
  float zoom_increment = input->scroll_offset().y * 0.1f;
#endif

  view_shifting_ += (input->cursor() - target->dimensions() * 0.5f) *
                    std::exp(view_zoom_) * (1.f - std::exp(zoom_increment));
  view_zoom_ += zoom_increment;

  cursor_ =
      (input->cursor() - target->dimensions() * 0.5f) * std::exp(view_zoom_) +
      view_shifting_;

  if (input_->IsKeyReleased(GLFW_KEY_LEFT_CONTROL))
    cursor_capturable_for_selection_.Invalidate();

  ReleaseConnector();
  ReleaseView();
  AquireConnector();

  for (const auto& node : nodes_)
    node->Step(input_, cursor_);

  Menu();

  MoveConnector();
  AcquireView();
  MoveView();

  PushNodeAppart();
}

void BoardImpl::PushNodeAppart() {
  if (cursor_capturable_) {
    push_ = 0.f;
    return;
  }

  // Find 2 nodes, try to move them away from each other.
  std::vector<NodeImpl*> h_nodes;
  for (auto& it : nodes_)
    h_nodes.push_back(it.get());
  std::sort(h_nodes.begin(), h_nodes.end(), [](NodeImpl* a, NodeImpl* b) {
    return a->position().x < b->position().x;
  });

  const float margin = 20.f;
  for (size_t a = 0; a < h_nodes.size(); ++a) {
    NodeImpl* node_a = nodes_.at(a).get();
    glm::vec2 a1 = node_a->position() - glm::vec2(margin, margin);
    glm::vec2 a2 = a1 + node_a->dimension() + 2.f * glm::vec2(margin, margin);
    for (size_t b = a + 1; b < h_nodes.size(); ++b) {
      NodeImpl* node_b = nodes_.at(b).get();
      glm::vec2 b1 = node_b->position() - glm::vec2(margin, margin);
      if (a2.x < b1.x)
        continue;
      glm::vec2 b2 = b1 + node_b->dimension() + 2.f * glm::vec2(margin, margin);
      float dist = std::max(std::max(b1.x - a2.x, a1.x - b2.x),
                            std::max(b1.y - a2.y, a1.y - b2.y));
      if (dist > 0.f)
        continue;

      // clang-format off
      glm::vec2 dir;
      if (b1.x - a2.x == dist)
        dir = glm::vec2(dist, 0.f);
      if (a1.x - b2.x == dist)
        dir = glm::vec2(-dist, 0.f);
      if (b1.y - a2.y == dist)
        dir = glm::vec2(0.f, dist);
      if (a1.y - b2.y == dist)
        dir = glm::vec2(0.f, -dist);
      // clang-format on

      glm::vec2 ca = node_a->position() + node_a->dimension() * 0.5f;
      glm::vec2 cb = node_b->position() + node_b->dimension() * 0.5f;
      dir += glm::normalize(cb - ca) * dist * 0.1f;

      dir *= push_;
      node_a->Push(dir);
      node_b->Push(-dir);
    }
  }
  push_ += (0.2f - push_) * 0.01f;
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

  cursor_captured_for_connector_.Invalidate();

  SlotImpl* end_slot = FindSlot(cursor_);
  if (!end_slot)
    return;

  if (end_slot->GetColor() != start_slot_->GetColor())
    return;

  Connect(start_slot_, end_slot);
}

void BoardImpl::AcquireView() {
  if (!input_->IsCursorPressed())
    return;

  if (cursor_capturable_for_selection_)
    return;

  cursor_captured_for_dragging_view_ = CaptureCursor();
  if (!cursor_captured_for_dragging_view_)
    return;

  selection_id_ ++;
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

  cursor_captured_for_dragging_view_.Invalidate();
}

void BoardImpl::Draw(smk::RenderTarget* target) {
  auto view = smk::View();
  view.SetCenter(view_shifting_);
  view.SetSize(target->dimensions() * std::exp(view_zoom_));
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

  if (cursor_captured_for_menu_) {
    auto pos = menu_widget_->Position();
    auto dim = menu_widget_->dimensions();
    DrawBoxBackground(target, pos - size::widget_margin * glm::vec2(1.f),
                      dim + size::widget_margin * glm::vec2(2.f));

    menu_widget_->Draw(target);
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

CursorCapture BoardImpl::CaptureCursor() {
  return cursor_capturable_.Capture();
}

CursorCapture BoardImpl::CaptureSelection() {
  return cursor_capturable_for_selection_.Capture();
}

void BoardImpl::Menu() {
  if (!cursor_captured_for_menu_) {
    if (input_->IsMousePressed(GLFW_MOUSE_BUTTON_2))
      cursor_captured_for_menu_ = CaptureCursor();
    if (!cursor_captured_for_menu_)
      return;
    menu_widget_->SetDimensions(menu_widget_->ComputeDimensions());
    menu_widget_->SetPosition(cursor_);
  }

  if (menu_delegate_->Validate())
    menu_widget_->SetDimensions(menu_widget_->ComputeDimensions());

  bool clicked = menu_widget_->Step(input_, cursor_);
  if (input_->IsCursorPressed() && !clicked)
    cursor_captured_for_menu_.Invalidate();
}

JSON BoardImpl::Serialize() {
  auto non_connected =
      std::remove_if(connectors_.begin(), connectors_.end(),
                     [](const auto& x) { return !x->IsConnected(); });
  connectors_.erase(non_connected, connectors_.end());

  int slot_last = 0;
  std::map<SlotImpl*, int> slot_index;
  slot_index[nullptr] = -1;
  JSON json;

  json["nodes"] = JSON::array();
  for (auto& node : nodes_) {
    json["nodes"].push_back(node->Serialize());
    for (auto& it : node->inputs())
      slot_index[it.get()] = slot_last++;
    for (auto& it : node->outputs())
      slot_index[it.get()] = slot_last++;
  }

  json["connectors"] = JSON::array();
  for (auto& connector : connectors_) {
    json["connectors"].push_back(slot_index[connector->input()]);
    json["connectors"].push_back(slot_index[connector->output()]);
  }
  return json;
}

#define CHECK(what) if (!(what)) return false;
bool BoardImpl::Deserialize(JSON& json) {
  nodes_.clear();
  connectors_.clear();

  std::map<int, int> index;
  for (int i = 0; i < (int)model_.nodes.size(); ++i)
    index[model_.nodes[i].identifier] = i;

  int slot_last = 0;
  std::map<int, SlotImpl*> slot_ptr;

  for (auto& node_json : json["nodes"]) {
    auto& model_index = index[node_json["identifier"]];
    auto* node = Create(model_.nodes[model_index]);
    node->Deserialize(node_json);
    for (auto& it : node->inputs())
      slot_ptr[slot_last++] = it.get();
    for (auto& it : node->outputs())
      slot_ptr[slot_last++] = it.get();
  }

  auto it = json["connectors"].begin();
  while (it != json["connectors"].end()) {
    int A = *it; ++it;
    if (it == json["connectors"].end()) return false;
    int B = *it; ++it;
    Connect(slot_ptr[A], slot_ptr[B]);
  }
  return true;
}

void BoardImpl::Connect(Slot* A, Slot* B) {
  connectors_.push_back(std::make_unique<ConnectorImpl>(
      static_cast<SlotImpl*>(A), static_cast<SlotImpl*>(B)));
}

}  // namespace smkflow
