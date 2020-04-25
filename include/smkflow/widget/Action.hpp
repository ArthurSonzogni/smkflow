// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef SMKFLOW_WIDGET_ACTION_HPP
#define SMKFLOW_WIDGET_ACTION_HPP

namespace smkflow {
namespace model {
struct Node;
}

class ActionContext {
 public:
  virtual Board* board() = 0;
  virtual glm::vec2 cursor() = 0;
};
using Action = std::function<void(ActionContext*)>;

// Predefined Actions.
Action CreateNode(model::Node);
Action ActionNone();

} /* namespace smkflow */

#endif /* end of include guard: SMKFLOW_WIDGET_ACTION_HPP */
