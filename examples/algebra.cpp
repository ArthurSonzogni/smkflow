#include <smk/Color.hpp>
#include <smk/Window.hpp>
#include <smkflow/Board.hpp>
#include <smkflow/Model.hpp>
#include "asset.hpp"


namespace smkflow {
namespace model {

glm::vec4 type_number = {1.f, 0.8f, 0.8f, 1.f};
glm::vec4 node_color = {1.0f, 1.f, 1.f, 1.f};

Board my_board = {
    //
    {
        {
            "Number",
            node_color,
            {
                // No input.
            },
            {
                {"number", type_number},
            },
        },
        {
            "Add",
            node_color,
            {
                {"a", type_number},
                {"b", type_number},
            },
            {
                {"a+b", type_number},
            },
        },
        {
            "Substract",
            node_color,
            {
                {"a", type_number},
                {"b", type_number},
            },
            {
                {"a-b", type_number},
            },
        },
        {
            "Multiply",
            node_color,
            {
                {"a", type_number},
                {"b", type_number},
            },
            {
                {"a*b", type_number},
            },
        },
        {
            "Divide",
            node_color,
            {
                {"a", type_number},
                {"b", type_number},
            },
            {
                {"a*b", type_number},
            },
        },
    },
    asset::arial_ttf};
}  // namespace model
}  // namespace smkflow

int main() {
  auto window = smk::Window(512, 512, "test");
  auto board = smkflow::Board::Create(smkflow::model::my_board);
  window.ExecuteMainLoop([&] {
    window.Clear({0.2, 0.2, 0.2, 1.0});
    window.PoolEvents();
    board->Step(&window, &window.input());
    board->Draw(&window);
    window.Display();
  });
  return EXIT_SUCCESS;
}
