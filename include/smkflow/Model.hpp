// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef SMKFLOW_MODEL_HPP
#define SMKFLOW_MODEL_HPP

#include <glm/glm.hpp>
#include <smkflow/widget/Widget.hpp>
#include <string>
#include <vector>

namespace smkflow {
namespace model {

struct Slot {
  std::string label;
  glm::vec4 color;
};

struct Node {
  int identifier;

  std::string label;
  glm::vec4 color;
  std::vector<Slot> input;
  std::vector<Slot> output;
  WidgetFactory widget;
};

struct Board {
  std::vector<Node> nodes;
  std::vector<WidgetFactory> context_widgets_;
  std::string font;
};

}  // namespace model
}  // namespace smkflow

#endif /* end of include guard: SMKFLOW_MODEL_HPP */
