#ifndef SMKFLOW_BOARD_HPP
#define SMKFLOW_BOARD_HPP

#include <memory>
#include <smk/Input.hpp>
#include <smk/RenderTarget.hpp>
#include <smkflow/Model.hpp>

namespace smkflow {

class Board {
 public:
  static std::unique_ptr<Board> Create(const model::Board&);
  virtual ~Board() = default;

  // Called 60/s for animations.
  virtual void Step(smk::RenderTarget* target, smk::Input* input) = 0;

  // Called for each new images.
  virtual void Draw(smk::RenderTarget* target) = 0;
};

}  // namespace smkflow
#endif /* end of include guard: SMKFLOW_BOARD_HPP */
