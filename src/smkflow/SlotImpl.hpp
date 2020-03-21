#ifndef SMKFLOW_SLOT_HPP
#define SMKFLOW_SLOT_HPP

#include <glm/glm.hpp>
#include <smk/RenderTarget.hpp>
#include <smk/Text.hpp>
#include <smkflow/Elements.hpp>
#include <smkflow/Model.hpp>
#include <smkflow/NodeImpl.hpp>

namespace smkflow {

class ConnectorImpl;
class NodeImpl;

class SlotImpl : public Slot {
 public:
  SlotImpl(NodeImpl* node,
           glm::vec2 position,
           smk::Text label,
           bool is_right,
           glm::vec4 color);
  void Draw(smk::RenderTarget* target);
  glm::vec2 GetPosition();

  void Connect(ConnectorImpl* connector_);
  void Disconnect(ConnectorImpl* connector_);

  bool IsRight();
  glm::vec4 GetColor();
  NodeImpl* node() { return node_; }

  // Slot implementation:
  void SetText(const std::string text) override;
  Node* GetNode() override;
  Connector* GetConnector() override;
  Slot* OppositeSlot() override;
  Node* OppositeNode() override;

 private:
  NodeImpl* node_;  // Owner;
  glm::vec2 position_;
  smk::Text label_;
  bool is_right_ = true;
  glm::vec4 color_;

  std::vector<ConnectorImpl*> connectors_;
};

}  // namespace smkflow

#endif /* end of include guard: SMKFLOW_SLOT_HPP */
