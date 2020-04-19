// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef SMKFLOW_COLOR_HPP
#define SMKFLOW_COLOR_HPP

#include <glm/glm.hpp>

namespace smkflow {
namespace size {

const float widget_margin = 4.f;
const float widget_height = 48.f - 2.f * widget_margin;

}  // namespace size

namespace color {

const float transition = 0.1f;

const auto background = glm::vec4(0.1f, 0.1f, 0.1f, 1.f);
const auto node_background = glm::vec4(0.4f, 0.4f, 0.4f, 0.5f);

const auto text = glm::vec4(1.f, 1.f, 1.f, 1.f);
const auto connector_background = glm::vec4(0.15f, 0.15f, 0.15f, 0.5);

const auto widget_background = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
const auto widget_background_hover = glm::vec4(0.2f, 0.2f, 0.2f, 0.8f);
const auto widget_background_focus = glm::vec4(0.1f, 0.1f, 0.1f, 1.f);

const auto widget_foreground = glm::vec4(0.5f, 0.5f, 0.9f, 1.0f);
const auto widget_foreground_hover = glm::vec4(0.3f, 0.3f, 1.0f, 0.9f);
const auto widget_foreground_focus = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

}  // namespace color
}  // namespace smkflow

#endif /* end of include guard: SMKFLOW_COLOR_HPP */
