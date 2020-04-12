// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
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
  SlotImpl(NodeImpl* node, std::string label, bool is_right, glm::vec4 color);
  void SetPosition(const glm::vec2 position) { position_ = position; }
  glm::vec2 ComputeDimensions();
  void Draw(smk::RenderTarget* target);
  glm::vec2 GetPosition();

  bool ValidateDimensions();

  void Connect(ConnectorImpl* connector_);
  void Disconnect(ConnectorImpl* connector_);

  bool IsRight();
  glm::vec4 GetColor();
  NodeImpl* node() { return node_; }

  // Slot implementation:
  void SetText(const std::string& text) override;
  Node* GetNode() override;
  Connector* GetConnector() override;
  Slot* OppositeSlot() override;
  Node* OppositeNode() override;

 private:
  bool dimensions_modified_ = false;
  NodeImpl* const node_;  // Owner;
  glm::vec2 position_;
  smk::Text label_;
  std::string text_;
  bool is_right_ = true;
  glm::vec4 color_;

  std::vector<ConnectorImpl*> connectors_;
};

}  // namespace smkflow

#endif /* end of include guard: SMKFLOW_SLOT_HPP */
