// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <smk/Window.hpp>
#include <smkflow/Constants.hpp>
#include <smkflow/Elements.hpp>
#include <smkflow/Model.hpp>

#include "asset.hpp"

enum Node { A, B };

auto blue = glm::vec4(0.3, 0.3, 0.8, 1.0);
auto red = glm::vec4(0.8, 0.3, 0.3, 1.0);
auto green = glm::vec4(0.3, 0.8, 0.3, 1.0);

auto node_A = smkflow::model::Node{
    Node::A,
    "Transposition",
    blue,
    // Input:
    {
        {"a", blue},
        {"b", red},
    },
    // Output:
    {
        {"b", red},
        {"a", blue},
    },
};

auto my_board = smkflow::model::Board{
    // Node list:
    {
        node_A,
    },
    // Contextual menu:
    {},
    // Font to be used:
    asset::arial_ttf,
};

int main() {
  auto window = smk::Window(512, 512, "test");
  auto board = smkflow::Board::Create(my_board);

  // Instanciate some Node based on the model.
  board->Create(node_A)->SetPosition({-120, 0});
  board->Create(node_A)->SetPosition({+120, 0});
  board->Create(node_A)->SetPosition({+0, +120});

  window.ExecuteMainLoop([&] {
    window.Clear(smkflow::color::background);
    window.PoolEvents();
    board->Step(&window, &window.input());
    board->Draw(&window);
    window.Display();
  });
  return EXIT_SUCCESS;
}
