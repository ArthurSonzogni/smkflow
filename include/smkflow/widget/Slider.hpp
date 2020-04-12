// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef SMKFLOW_WIDGET_SLIDER_HPP
#define SMKFLOW_WIDGET_SLIDER_HPP

#include <smkflow/widget/Widget.hpp>

namespace smkflow {

// Constructor
WidgetFactory Slider(float min = 0.f,
                     float max = 100.f,
                     float value = 50.f,
                     std::string format = "{:.0f}%");  // Use fmt lib.

// Interface
class SliderInterface {
 public:
  virtual void SetValue(float) = 0;
  virtual float GetValue() = 0;
};

// Cast
SliderInterface* Slider(Widget*);

}  // namespace smkflow

#endif /* end of include guard: SMKFLOW_WIDGET_SLIDER_HPP \
        */
