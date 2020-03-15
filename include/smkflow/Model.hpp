#ifndef SMKFLOW_MODEL_HPP
#define SMKFLOW_MODEL_HPP

#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace smkflow {
namespace model {

struct Slot {
  std::string label;
  glm::vec4 color;
};

struct Node {
  std::string label;
  glm::vec4 color;
  std::vector<Slot> input;
  std::vector<Slot> output;
};

struct Board {
  std::vector<Node> nodes;
  std::string font;
};

}  // namespace model
}  // namespace smkflow

#endif /* end of include guard: SMKFLOW_MODEL_HPP */
