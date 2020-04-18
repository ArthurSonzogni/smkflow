// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef SMKFLOW_WIDGET_MENU_HPP
#define SMKFLOW_WIDGET_MENU_HPP

#include <smkflow/widget/Widget.hpp>

namespace smkflow {

/// Usage:
/// ~~~cpp
/// Menu("Main", {
///   Menu("File", {
///     MenuEntry("Open", Open),
///     MenuEntry("Save", Save),
///     MenuEntry("Quit", Quit),
///   });
///   Menu("Display", {
///     MenuEntry("Bar", Bar),
///   });
/// });
/// ~~~

WidgetFactory Menu(const std::string& label,
                   std::vector<WidgetFactory> children);
WidgetFactory MenuEntry(const std::string& label, std::function<void()>);

}  // namespace smkflow

#endif /* end of include guard: SMKFLOW_WIDGET_MENU_HPP \
        */
