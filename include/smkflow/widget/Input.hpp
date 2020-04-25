// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef SMKFLOW_WIDGET_INPUTBOX_HPP
#define SMKFLOW_WIDGET_INPUTBOX_HPP

#include <smkflow/widget/Widget.hpp>

namespace smkflow {

// Constructor:
WidgetFactory Input(const std::string& placeholder,
                    const std::string& input = "");

// Interface:
class InputInterface {
 public:
  virtual void SetValue(const std::string& value) = 0;
  virtual const std::string& GetValue() = 0;
};

// Cast:
InputInterface* Input(Widget*);

}  // namespace smkflow

#endif /* end of include guard: SMKFLOW_WIDGET_INPUTBOX_HPP*/
