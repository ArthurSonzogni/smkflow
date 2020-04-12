// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef SMKFLOW_WIDGET_BOX_HPP
#define SMKFLOW_WIDGET_BOX_HPP

#include <smkflow/widget/Widget.hpp>

namespace smkflow {

// Constructor
WidgetFactory VBox(std::vector<WidgetFactory> children);
WidgetFactory HBox(std::vector<WidgetFactory> children);

// Interface
class BoxInterface {
 public:
  virtual Widget* ChildAt(int i) = 0;
  virtual int ChildCount() = 0;
};

// Cast:
BoxInterface* Box(Widget*);

}  // namespace smkflow

#endif /* end of include guard: SMKFLOW_WIDGET_BOX_HPP  */
