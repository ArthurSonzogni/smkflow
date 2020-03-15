#ifndef SMKFLOW_SLOT_HPP
#define SMKFLOW_SLOT_HPP

#include <glm/glm.hpp>
#include <smk/RenderTarget.hpp>
#include <smk/Text.hpp>

namespace smkflow {

class Node;
class Connector;

class Slot {
 public:
  Slot(Node* node,
       glm::vec2 position,
       smk::Text label,
       bool is_right,
       glm::vec4 color);
  void Draw(smk::RenderTarget* target);
  glm::vec2 GetPosition();

  void Connect(Connector* connector_);
  void Disconnect(Connector* connector_);

  bool IsRight();
  glm::vec4 GetColor();

 private:
  Node* node_;  // Owner;
  glm::vec2 position_;
  smk::Text label_;
  bool is_right_ = true;
  glm::vec4 color_;

  std::vector<Connector*> connectors_;
};

}  // namespace smkflow

#endif /* end of include guard: SMKFLOW_SLOT_HPP */
