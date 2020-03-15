#ifndef SMKFLOW_CONNECTOR_HPP
#define SMKFLOW_CONNECTOR_HPP

#include <smk/RenderTarget.hpp>
#include <smk/Transformable.hpp>

namespace smkflow {

class Slot;

class Connector {
 public:
  Connector(Slot* A, Slot* B);
  ~Connector();
  bool IsConnected();

  void Disconnect();
  void DrawForeground(smk::RenderTarget* target);
  void DrawBackground(smk::RenderTarget* target);

  Slot* input() { return input_; }
  Slot* output() { return output_; }

 private:
  void RebuildVertex();

  Slot* input_ = nullptr;
  Slot* output_ = nullptr;

  smk::Transformable background_;
  smk::Transformable foreground_;
  glm::vec2 position_a_ = glm::vec2(0.f, 0.f);
  glm::vec2 position_b_ = glm::vec2(0.f, 0.f);
};

}  // namespace smkflow

#endif /* end of include guard: SMKFLOW_CONNECTOR_HPP */
