// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <iostream>
#include <smk/Color.hpp>
#include <smk/Window.hpp>
#include <smkflow/Constants.hpp>
#include <smkflow/Elements.hpp>
#include <smkflow/Model.hpp>
#include <smkflow/Widget.hpp>

#include "asset.hpp"

auto color_node = glm::vec4(0.5f, 0.2f, 0.2f, 1.f);
auto color_connector = glm::vec4(1.f, 0.0, 0.0, 1.f);

auto node_sliders = smkflow::model::Node{
    0,
    "Sliders",
    color_node,
    {},
    {
        smkflow::Slider::Create(),
        smkflow::Slider::Create(0, 10, 0, "{:.0f} children"),
        smkflow::Slider::Create(0, 1, 0.5, "{:.2f}"),
    },
    {
        {"d", color_connector},
        {"e", color_connector},
    },
};

auto node_input = smkflow::model::Node{
    0,
    "InputBox",
    color_node,
    {
        {"a", color_connector},
        {"b", color_connector},
    },
    {
        smkflow::InputBox::Create("input box"),
        smkflow::InputBox::Create("input box"),
        smkflow::InputBox::Create("input box"),
        // smkflow::HBox::Create({
        // smkflow::Label::Create("coucou:"),
        // smkflow::Checkbox::Create(false),
        //}),
    },
    {
        {"", color_connector},
        {"", color_connector},
    },
};

auto my_board = smkflow::model::Board{
    {
        node_sliders,
        node_input,
    },
    asset::arial_ttf,
};

int main() {
  auto window = smk::Window(512, 512, "test");
  auto board = smkflow::Board::Create(my_board);

  board->Create(node_input)->SetPosition({-150, 0});
  board->Create(node_sliders)->SetPosition({+150, 0});

  window.ExecuteMainLoop([&] {
    window.Clear(smkflow::color::background);
    window.PoolEvents();
    board->Step(&window, &window.input());
    board->Draw(&window);
    window.Display();
  });
  return EXIT_SUCCESS;
}
