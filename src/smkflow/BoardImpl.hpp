// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef EDITOR_BOARD_HPP
#define EDITOR_BOARD_HPP

#include <smk/Font.hpp>
#include <smkflow/CursorCapture.hpp>
#include <smkflow/Elements.hpp>
#include <smkflow/Model.hpp>
#include <smkflow/NodeImpl.hpp>

namespace smkflow {

class ConnectorImpl;
class SlotImpl;
class NodeImpl;

class BoardImpl : public Board {
 public:
  BoardImpl(const model::Board& model);
  ~BoardImpl();

  SlotImpl* FindSlot(const glm::vec2& position);
  smk::Font& font() { return font_; }

  // Board implementations:
  NodeImpl* Create(const model::Node&) override;
  void Step(smk::RenderTarget* target, smk::Input* input) override;
  void Draw(smk::RenderTarget* target) override;
  int NodeCount() override { return nodes_.size(); }
  NodeImpl* NodeAt(int i) override { return nodes_.at(i).get(); }
  CursorCapture CaptureCursor() override;
  CursorCapture CaptureSelection();
  void Connect(Slot* A, Slot* B) override;
  JSON Serialize() override;
  bool Deserialize(JSON&) override;

 private:
  void PushNodeAppart();

  // Move / Zoom the view ------------------------------------------------------
  void AcquireView();
  void MoveView();
  void ReleaseView();

  CursorCapture cursor_captured_for_dragging_view_;
  glm::vec2 grab_point_ = {0.f, 0.f};
  glm::vec2 view_shifting_ = {0.f, 0.f};
  float view_zoom_ = 0.f;

  // Build connectors in between nodes -----------------------------------------
  void AquireConnector();
  void MoveConnector();
  void ReleaseConnector();

  // Right click ---------------------------------------------------------------
  void Menu();
  std::unique_ptr<Widget> menu_widget_;
  CursorCapture cursor_captured_for_menu_;
  smk::Transformable square_;

  glm::vec2 connector_in_;
  glm::vec2 connector_in_pushed_;
  glm::vec2 connector_out_pushed_;
  glm::vec2 connector_out_;

  SlotImpl* start_slot_ = nullptr;
  CursorCapture cursor_captured_for_connector_;

  // ---------------------------------------------------------------------------

  std::vector<std::unique_ptr<NodeImpl>> nodes_;
  std::vector<std::unique_ptr<ConnectorImpl>> connectors_;

  smk::Font font_;

  CursorCapturable cursor_capturable_ = {1};
  CursorCapturable cursor_capturable_for_selection_ = {9999};

  smk::Input* input_;
  glm::vec2 cursor_;

  float push_ = 0.f;

  class MenuDelegate;
  std::unique_ptr<MenuDelegate> menu_delegate_;
  friend class MenuDelegate;

  model::Board model_;

  int selection_id_ = 0;
};

}  // namespace smkflow

#endif /* end of include guard: EDITOR_BOARD_HPP */
