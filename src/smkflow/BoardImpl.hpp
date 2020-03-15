#ifndef EDITOR_BOARD_HPP
#define EDITOR_BOARD_HPP

#include <smk/Font.hpp>
#include <smkflow/Board.hpp>

namespace smkflow {

class Connector;
class Node;
class Slot;

class BoardImpl : public Board {
 public:
  BoardImpl(const model::Board& model);
  ~BoardImpl();
  void Step(smk::RenderTarget* target, smk::Input* input) override;
  void Draw(smk::RenderTarget* target) override;

  Slot* FindSlot(const glm::vec2& position);
  smk::Font& font() { return font_; }

 private:
  std::vector<std::unique_ptr<Node>> nodes_;
  std::vector<std::unique_ptr<Connector>> connectors_;
  Node* selected_node_ = nullptr;
  Slot* start_slot_ = nullptr;
  glm::vec2 grab_point_ = {0.f,0.f};
  glm::vec2 view_shifting_ = {0.f,0.f};
  float view_zoom_ = 0.f;

  // Connector creation animations.
  glm::vec2 connector_in_;
  glm::vec2 connector_in_pushed_;
  glm::vec2 connector_out_pushed_;
  glm::vec2 connector_out_;

  smk::Font font_;
};

}  // namespace smkflow

#endif /* end of include guard: EDITOR_BOARD_HPP */
