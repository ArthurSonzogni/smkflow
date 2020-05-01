// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <fmt/core.h>

#include <iostream>
#include <smk/Color.hpp>
#include <smk/Window.hpp>
#include <smkflow/Constants.hpp>
#include <smkflow/Elements.hpp>
#include <smkflow/Model.hpp>
#include <smkflow/widget/Box.hpp>
#include <smkflow/widget/Input.hpp>
#include <smkflow/widget/Slider.hpp>

#include "asset.hpp"

enum Node {
  Number,
  Add,
  Substract,
  Multiply,
  Divide,
};

auto type_number = glm::vec4(1.f, 0.5f, 0.5f, 1.f);
auto node_color = glm::vec4(0.5f, 0.5f, 0.5f, 1.f);
auto node_color_number = glm::vec4(0.5, 0.25f, 0.25f, 1.f);

auto node_add = smkflow::model::Node{
    Node::Add,
    "Add",
    node_color,
    {
        {"", type_number},
        {"", type_number},
    },
    {
        {"out", type_number},
    },
    smkflow::VBox({
        smkflow::Input("0.0"),
        smkflow::Input("0.0"),
    }),
};

auto node_substract = smkflow::model::Node{
    Node::Substract,
    "Substract",
    node_color,
    {
        {"", type_number},
        {"", type_number},
    },
    {
        {"out", type_number},
    },
    smkflow::VBox({
        smkflow::Input("0.0"),
        smkflow::Input("0.0"),
    }),
};

auto node_multiply = smkflow::model::Node{
    Node::Multiply,
    "Multiply",
    node_color,
    {
        {"", type_number},
        {"", type_number},
    },
    {
        {"out", type_number},
    },
    smkflow::VBox({
        smkflow::Input("0.0"),
        smkflow::Input("0.0"),
    }),
};

auto node_divide = smkflow::model::Node{
    Node::Divide,
    "Divide",
    node_color,
    {
        {"", type_number},
        {"", type_number},
    },
    {
        {"", type_number},
    },
    smkflow::VBox({
        smkflow::Input("0.0"),
        smkflow::Input("0.0"),
    }),
};

auto node_number = smkflow::model::Node{
    Node::Number,
    "Number",
    node_color_number,
    {},
    {
        {"out", type_number},
    },
    smkflow::Slider(-20.f, 20.f, 0.f, "{:.2f}"),
};

auto my_board = smkflow::model::Board{
    // Node list
    {
        node_number,
        node_add,
        node_substract,
        node_divide,
        node_multiply,
    },
    // Contextual menu:
    {},
    // Font:
    asset::arial_ttf,
};

std::map<smkflow::Node*, float> values;

const char* float_format = "{:.2f}";

void UpdateValues(smkflow::Board* board) {
  for (int i = 0; i < board->NodeCount(); ++i) {
    smkflow::Node* node = board->NodeAt(i);
    float value = 0;
    if (node->Identifier() == Number) {
      auto slider = smkflow::Slider(node->widget());
      value = slider->GetValue();
      values[node] = value;
      continue;
    }

    auto box = smkflow::Box(node->widget());
    auto input_1 = smkflow::Input(box->ChildAt(0));
    auto input_2 = smkflow::Input(box->ChildAt(1));

    if (auto* a = node->InputAt(0)->OppositeNode())
      input_1->SetValue(fmt::format(float_format, values[a]));

    if (auto* b = node->InputAt(1)->OppositeNode())
      input_2->SetValue(fmt::format(float_format, values[b]));

    float a_value = std::atof(input_1->GetValue().c_str());
    float b_value = std::atof(input_2->GetValue().c_str());

    // clang-format off
    switch (node->Identifier()) {
      case Number: break;
      case Add: value = a_value + b_value; break;
      case Substract: value = a_value - b_value; break;
      case Multiply: value = a_value * b_value; break;
      case Divide: value = b_value ? a_value / b_value : 0; break;
    }
    // clang-format on
    values[node] = value;
    node->OutputAt(0)->SetText(fmt::format(float_format, value));
  }
}

int main() {
  auto window = smk::Window(512, 512, "test");
  auto board = smkflow::Board::Create(my_board);

  auto nodes = {
      node_number, node_add, node_multiply, node_substract, node_divide,
  };

  // Instanciate some Node based on the model.
  int x = nodes.size() / 2;
  for (const auto& node_model : nodes) {
    for (int y = -30; y < 30; ++y) {
      smkflow::Node* node = board->Create(node_model);
      node->SetPosition({30 * x, 30 * y});
    }
    ++x;
  }

  window.ExecuteMainLoop([&] {
    window.Clear(smkflow::color::background);
    window.PoolEvents();
    UpdateValues(board.get());
    board->Step(&window, &window.input());
    board->Draw(&window);
    window.Display();
  });
  return EXIT_SUCCESS;
}
