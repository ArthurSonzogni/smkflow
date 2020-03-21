#include <iostream>
#include <smk/Color.hpp>
#include <smk/Window.hpp>
#include <smkflow/Elements.hpp>
#include <smkflow/Model.hpp>

#include "asset.hpp"

enum Node {
  Number,
  Add,
  Substract,
  Multiply,
  Divide,
};

auto type_number = glm::vec4(1.f, 0.5f, 0.5f, 1.f);
auto node_color = glm::vec4(1.0f, 1.f, 1.f, 1.f);
auto node_color_number = glm::vec4(1.0,0.5,0.5,1.f);

auto node_add = smkflow::model::Node{
    Node::Add,
    "Add",
    node_color,
    {
        {"a", type_number},
        {"b", type_number},
    },
    {
        {"  out", type_number},
    },
};

auto node_substract = smkflow::model::Node{
    Node::Substract,
    "Substract",
    node_color,
    {
        {"a", type_number},
        {"b", type_number},
    },
    {
        {"  out", type_number},
    },
};

auto node_multiply = smkflow::model::Node{
    Node::Multiply,
    "Multiply",
    node_color,
    {
        {"a", type_number},
        {"b", type_number},
    },
    {
        {"  out", type_number},
    },
};

auto node_divide = smkflow::model::Node{
    Node::Divide,
    "Divide",
    node_color,
    {
        {"a", type_number},
        {"b", type_number},
    },
    {
        {"  out", type_number},
    },
};

auto node_number = smkflow::model::Node{
    Node::Number,
    "Number",
    node_color_number,
    {},
    {
        {"  out", type_number},
    },
};

auto my_board = smkflow::model::Board{
    {
        node_number,
        node_add,
        node_multiply,
        node_substract,
        node_divide,
    },
    asset::arial_ttf,
};

std::map<smkflow::Node*, int> values;

void UpdateValues(smkflow::Board* board) {
  for (int i = 0; i < board->NodeCount(); ++i) {
    smkflow::Node* node = board->NodeAt(i);
    int value = 0;
    switch (node->Identifier()) {
      case Number:
        value = 1;
        break;
      case Add:
        value = values[node->InputAt(0)->OppositeNode()] +
                values[node->InputAt(1)->OppositeNode()];
        break;
      case Substract:
        value = values[node->InputAt(0)->OppositeNode()] -
                values[node->InputAt(1)->OppositeNode()];
        break;
      case Multiply:
        value = values[node->InputAt(0)->OppositeNode()] *
                values[node->InputAt(1)->OppositeNode()];
        break;
      case Divide:
        if (values[node->InputAt(1)->OppositeNode()] != 0) {
          value = values[node->InputAt(0)->OppositeNode()] /
                  values[node->InputAt(1)->OppositeNode()];
        }
        break;
    }
    values[node] = value;
    node->OutputAt(0)->SetText(std::to_string(value));
    for (int i = 0; i < node->InputCount(); ++i) {
      smkflow::Node* oppositeNode = node->InputAt(i)->OppositeNode();
      node->InputAt(i)->SetText(std::to_string(values[oppositeNode]));
    }
  }
}

int main() {
  auto window = smk::Window(512, 512, "test");
  auto board = smkflow::Board::Create(my_board);

  // Instanciate some Node based on the model.
  int x = -my_board.nodes.size() / 2;
  for (const auto& node_model : my_board.nodes) {
    for (int y = -2; y < 2; ++y) {
      smkflow::Node* node = board->Create(node_model);
      node->SetPosition({200 * x, 200 * y});
    }
    ++x;
  }

  window.ExecuteMainLoop([&] {
    window.Clear({0.2, 0.2, 0.2, 1.0});
    window.PoolEvents();
    UpdateValues(board.get());
    board->Step(&window, &window.input());
    board->Draw(&window);
    window.Display();
  });
  return EXIT_SUCCESS;
    }
