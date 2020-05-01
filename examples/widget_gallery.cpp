// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <iostream>
#include <smk/Color.hpp>
#include <smk/Window.hpp>
#include <smkflow/Constants.hpp>
#include <smkflow/Elements.hpp>
#include <smkflow/Model.hpp>
#include <smkflow/widget/Box.hpp>
#include <smkflow/widget/Input.hpp>
#include <smkflow/widget/Label.hpp>
#include <smkflow/widget/Menu.hpp>
#include <smkflow/widget/Slider.hpp>
#include <smkflow/widget/Widget.hpp>

#include "asset.hpp"

auto color_node = glm::vec4(0.5f, 0.2f, 0.2f, 1.f);

using namespace smkflow;

auto node_sliders = model::Node{
    0,
    "Sliders",
    color_node,
    {},
    {},
    VBox({
        Slider(),
        Slider(0, 10, 0, "{:.0f} children"),
        Slider(0, 1, 0.5, "{:.2f}"),
    }),
};

auto node_input = model::Node{
    0,
    "Input",
    color_node,
    {},
    {},
    VBox({
        Input("input box"),
        Input("input box"),
        Input("input box"),
    }),
};

auto node_label = model::Node{
    0,
    "Label",
    color_node,
    {},
    {},
    VBox({
        HBox({Label("R:"), Slider(0, 1, 0.5, "{:.2f}")}),
        HBox({Label("G:"), Slider(0, 1, 0.5, "{:.2f}")}),
        HBox({Label("B:"), Slider(0, 1, 0.5, "{:.2f}")}),
    }),
};

auto node_box = model::Node{
    0,
    "Box",
    color_node,
    {},
    {},
    HBox({
        VBox({
            Slider(),
            Slider(0, 10, 0, "{:.0f} children"),
            Slider(0, 1, 0.5, "{:.2f}"),
        }),
        VBox({
            Slider(),
            Slider(0, 1, 0.5, "{:.2f}"),
        }),
        VBox({
            Slider(),
            HBox({
                Slider(),
                Slider(0, 1, 0.5, "{:.2f}"),
            }),
            Slider(0, 1, 0.5, "{:.2f}"),
        }),
    }),
};

auto node_menu = model::Node{
    0,
    "Menu",
    color_node,
    {},
    {},
    Menu("menu",
         {
             Menu("File",
                  {
                      MenuEntry("1.html", ActionNone()),
                      MenuEntry("2.html", ActionNone()),
                      MenuEntry("3.html", ActionNone()),
                      MenuEntry("4.html", ActionNone()),
                      MenuEntry("5.html", ActionNone()),
                      MenuEntry("6.html", ActionNone()),
                      MenuEntry("7.html", ActionNone()),
                      MenuEntry("8.html", ActionNone()),
                      MenuEntry("9.html", ActionNone()),
                  }),
             Menu("Edition",
                  {
                      Slider(),
                      Slider(0, 1, 0.5, "{:.2f}"),
                  }),
             Menu("Display",
                  {
                      Menu("Sliders",
                           {
                               Slider(0, 1, 0.5, "{:.2f}"),
                               Slider(0, 1, 0.5, "{:.2f}"),
                           }),
                      Menu("Input",
                           {
                               Input("input 1"),
                               Input("input 2"),
                           }),
                  }),
         }),
};

auto my_board = model::Board{
    {
        node_sliders,
        node_box,
        node_input,
        node_label,
    },
    {
        MenuEntry("Sliders", CreateNode(node_sliders)),
        MenuEntry("Box", CreateNode(node_box)),
        MenuEntry("Input", CreateNode(node_input)),
        MenuEntry("Label", CreateNode(node_label)),
    },
    asset::arial_ttf,
};

int main() {
  auto window = smk::Window(512, 512, "test");
  auto board = Board::Create(my_board);

  board->Create(node_input)->SetPosition({-150, -150});
  board->Create(node_sliders)->SetPosition({+150, -150});
  board->Create(node_box)->SetPosition({-150, +100});
  board->Create(node_label)->SetPosition({-150, +300});
  board->Create(node_menu)->SetPosition({+150, +300});

  window.ExecuteMainLoop([&] {
    window.Clear(color::background);
    window.PoolEvents();
    board->Step(&window, &window.input());
    board->Draw(&window);
    window.Display();
  });
  return EXIT_SUCCESS;
}
