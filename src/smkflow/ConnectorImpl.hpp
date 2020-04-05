// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef SMKFLOW_CONNECTOR_HPP
#define SMKFLOW_CONNECTOR_HPP

#include <smk/RenderTarget.hpp>
#include <smk/Transformable.hpp>
#include <smkflow/Elements.hpp>
#include <smkflow/Model.hpp>

namespace smkflow {

class SlotImpl;

class ConnectorImpl : public Connector {
 public:
  ConnectorImpl(SlotImpl* A, SlotImpl* B);
  ~ConnectorImpl();
  bool IsConnected();

  void Disconnect();
  void DrawForeground(smk::RenderTarget* target);
  void DrawBackground(smk::RenderTarget* target);

  SlotImpl* input() { return input_; }
  SlotImpl* output() { return output_; }

  // Connector implementations:
  Slot* GetInput() override;
  Slot* GetOutput() override;

 private:
  void RebuildVertex();

  SlotImpl* input_ = nullptr;
  SlotImpl* output_ = nullptr;

  smk::Transformable background_;
  smk::Transformable foreground_;
  glm::vec2 position_a_ = glm::vec2(0.f, 0.f);
  glm::vec2 position_b_ = glm::vec2(0.f, 0.f);
};

}  // namespace smkflow

#endif /* end of include guard: SMKFLOW_CONNECTOR_HPP */
