// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef SMKFLOW_WIDGET_MENUIMPL_HPP
#define SMKFLOW_WIDGET_MENUIMPL_HPP

#include <glm/glm.hpp>

namespace smk {
class RenderTarget;
}

namespace smkflow {

void DrawBoxBackground(smk::RenderTarget* target,
                       glm::vec2 position,
                       glm::vec2 dimensions);

} /* namespace smkflow */

#endif /* end of include guard: SMKFLOW_WIDGET_MENUIMPL_HPP */
