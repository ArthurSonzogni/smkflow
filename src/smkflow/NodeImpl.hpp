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
  ~NodeImpl();
  void Draw(smk::RenderTarget*);

  bool OnCursorPressed(glm::vec2);
  void OnCursorMoved(glm::vec2);
  void OnCursorReleased(glm::vec2);

  const glm::vec2& GetPosition();

  SlotImpl* FindSlot(const glm::vec2& position);

  BoardImpl* board() { return board_; }

  // Board implementation:
  int Identifier() override { return identifier_; }
  void SetPosition(const glm::vec2& position) override;
  virtual int InputCount();
  virtual Slot* InputAt(int i);
  virtual int OutputCount();
  virtual Slot* OutputAt(int i);

 private:
  BoardImpl* board_;
  int identifier_;
  std::vector<std::unique_ptr<SlotImpl>> inputs_;
  std::vector<std::unique_ptr<SlotImpl>> outputs_;
  glm::vec2 position_ = glm::vec2(0, 0);
  smk::Text title_;
  smk::Transformable title_base_;
  smk::Transformable base_;

  float width_;
  float height_;

  glm::vec2 cursor_drag_point;
};

}  // namespace smkflow

#endif /* end of include guard: SMKFLOW_NODE_HPP */
