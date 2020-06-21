// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef SMKFLOW_NODE_HPP
#define SMKFLOW_NODE_HPP

#include <glm/glm.hpp>
#include <memory>
#include <smk/RenderTarget.hpp>
#include <smk/Text.hpp>
#include <smk/Transformable.hpp>
#include <smkflow/Elements.hpp>
#include <smkflow/Model.hpp>
#include <smkflow/SlotImpl.hpp>
#include <vector>

namespace smkflow {

class SlotImpl;
class BoardImpl;

class NodeImpl : public Node, public Widget::Delegate {
 public:
  NodeImpl(BoardImpl* board, const model::Node& model);
  ~NodeImpl() override;

  static NodeImpl* From(Node* node) { return static_cast<NodeImpl*>(node); }

  void Draw(smk::RenderTarget*);

  void Step(smk::Input* input, glm::vec2 cursor);

  void Layout();

  const glm::vec2& GetPosition();

  SlotImpl* FindSlot(const glm::vec2& position);

  BoardImpl* boardImpl() { return board_; }

  void Push(glm::vec2 direction);
  glm::vec2 position() { return position_; }
  glm::vec2 dimension() { return dimension_; }

  JSON Serialize();
  bool Deserialize(JSON& in);

  // Node:
  Board* GetBoard() override;
  int Identifier() override { return identifier_; }
  void SetPosition(const glm::vec2& position) override;
  int InputCount() override;
  Slot* InputAt(int i) override;
  int OutputCount() override;
  Slot* OutputAt(int i) override;
  Widget* widget() override;

  // WidgetDelegate:
  glm::vec2 Position() override { return position_; }
  void InvalidateLayout() override;
  smk::Font& Font() override;
  CursorCapture CaptureCursor() override;
  Board* board() override;

 private:
  BoardImpl* board_;
  int identifier_;
  std::vector<std::unique_ptr<SlotImpl>> inputs_;
  std::vector<std::unique_ptr<SlotImpl>> outputs_;
  std::unique_ptr<Widget> widget_;
  glm::vec2 position_ = glm::vec2(0, 0);
  glm::vec2 dimension_ = glm::vec2(0, 0);
  smk::Text title_;
  smk::Transformable title_base_;
  smk::Transformable base_;
  glm::vec4 color_;

  float width_;
  float height_;

  bool layout_invalidated_ = true;
  glm::vec2 speed_ = glm::vec2(0.f, 0.f);

  glm::vec2 cursor_drag_point;
  CursorCapture cursor_captured_;
  CursorCapture cursor_captured_for_selection_;

  bool Selected();

 public:
  const decltype(inputs_)& inputs() { return inputs_; }
  const decltype(outputs_)& outputs() { return outputs_; }
};

}  // namespace smkflow

#endif /* end of include guard: SMKFLOW_NODE_HPP */
