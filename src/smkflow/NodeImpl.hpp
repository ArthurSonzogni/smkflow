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

class NodeImpl : public Node {
 public:
  NodeImpl(BoardImpl* board, const model::Node& model);
  ~NodeImpl() override;

  static NodeImpl* From(Node* node) { return static_cast<NodeImpl*>(node); }

  void Draw(smk::RenderTarget*);

  void Step(smk::Input* input, glm::vec2 cursor);
  bool OnCursorPressed(glm::vec2);
  void OnCursorMoved(glm::vec2);
  void OnCursorReleased(glm::vec2);
  
  void Layout();

  const glm::vec2& GetPosition();

  SlotImpl* FindSlot(const glm::vec2& position);

  BoardImpl* board() { return board_; }

  // Board implementation:
  int Identifier() override { return identifier_; }
  void SetPosition(const glm::vec2& position) override;
  int InputCount() override;
  Slot* InputAt(int i) override;
  int OutputCount() override;
  Slot* OutputAt(int i) override;
  int WidgetCount() override;
  Widget* WidgetAt(int i) override;

 private:
  BoardImpl* board_;
  int identifier_;
  std::vector<std::unique_ptr<SlotImpl>> inputs_;
  std::vector<std::unique_ptr<SlotImpl>> outputs_;
  std::vector<std::unique_ptr<Widget>> widgets_;
  glm::vec2 position_ = glm::vec2(0, 0);
  smk::Text title_;
  smk::Transformable title_base_;
  smk::Transformable base_;
  glm::vec4 color_;

  float width_;
  float height_;

  glm::vec2 cursor_drag_point;
};

}  // namespace smkflow

#endif /* end of include guard: SMKFLOW_NODE_HPP */
