#ifndef SMKFLOW_NODE_HPP
#define SMKFLOW_NODE_HPP

#include <glm/glm.hpp>
#include <memory>
#include <smk/RenderTarget.hpp>
#include <smk/Text.hpp>
#include <smk/Transformable.hpp>
#include <smkflow/Model.hpp>
#include <vector>

namespace smkflow {

class Slot;
class Board;
class BoardImpl;

class Node {
 public:
  Node(BoardImpl* board, const model::Node& model);
  ~Node();
  void Draw(smk::RenderTarget*);

  bool OnCursorPressed(glm::vec2);
  void OnCursorMoved(glm::vec2);
  void OnCursorReleased(glm::vec2);

  void SetPosition(const glm::vec2& position);
  const glm::vec2& GetPosition();

  Slot* FindSlot(const glm::vec2& position);

 private:
  std::vector<std::unique_ptr<Slot>> slots_;
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
