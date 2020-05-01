// the LICENSE file.

#include <gtest/gtest.h>

#include <smkflow/Elements.hpp>
#include <smkflow/Model.hpp>

#include "test.hpp"

using Serialization = Test;
using JSON = smkflow::JSON;

TEST_F(Serialization, BoardEmpty) {
  auto model = smkflow::model::Board{{}, {}, font()};
  auto board = smkflow::Board::Create(model);
  JSON expected = {
      {"nodes", JSON::array()},
      {"connectors", JSON::array()},
  };

  JSON out = board->Serialize();
  EXPECT_EQ(out, expected);
}

TEST_F(Serialization, NodeBasic) {
  auto node_model = smkflow::model::Node{
      113,                         //
      "label",                   //
      {1.0f, 0.8f, 0.6f, 0.4f},  //
      {},                        // Input.
      {},                        // Output.
      {},                        // Widget.
  };
  auto board_model = smkflow::model::Board{{node_model}, {}, font()};
  auto board = smkflow::Board::Create(board_model);
  auto node = board->Create(node_model);
  node->SetPosition({42, 13});
  JSON expected = {
      {"nodes",
       {
           {
               {"identifier", 113},
               {"position", {42.f, 13.f}},
           },
       }},
      {"connectors", JSON::array()},
  };

  JSON out = board->Serialize();
  EXPECT_EQ(out, expected);
  
  // We should be able to deserialize / serialize and get the same output.
  {
    auto board = smkflow::Board::Create(board_model);
    board->Deserialize(out);
    EXPECT_EQ(board->Serialize(), out);
  }
}

TEST_F(Serialization, Connectors) {
  auto white = glm::vec4(1.f, 1.f, 1.f, 1.f);
  auto node_model = smkflow::model::Node{
      0,        //
      "label",  //
      white,
      // Input:
      {
          {"A", white},
          {"B", white},
          {"C", white},
      },
      // Output:
      {
          {"A", white},
          {"B", white},
          {"C", white},
      },
      {},  // Widget.
  };
  auto board_model = smkflow::model::Board{{node_model}, {}, font()};
  auto board = smkflow::Board::Create(board_model);
  auto node_1 = board->Create(node_model);
  auto node_2 = board->Create(node_model);
  auto node_3 = board->Create(node_model);

  board->Connect(node_1->OutputAt(0), node_2->InputAt(1));
  board->Connect(node_1->OutputAt(2), node_3->InputAt(0));
  board->Connect(node_2->OutputAt(1), node_1->InputAt(2));
  board->Connect(node_2->OutputAt(0), node_3->InputAt(1));
  board->Connect(node_3->OutputAt(2), node_1->InputAt(0));
  board->Connect(node_3->OutputAt(1), node_2->InputAt(1));

  JSON expected = {
      {"nodes",
       {
           {
               {"identifier", 0},
               {"position", {0.f, 0.f}},
           },
           {
               {"identifier", 0},
               {"position", {0.f, 0.f}},
           },
           {
               {"identifier", 0},
               {"position", {0.f, 0.f}},
           },
       }},
      {"connectors", {5, 12, 10, 2, 9, 13, 17, 0, 16, 7}},
  };

  JSON out = board->Serialize();
  EXPECT_EQ(out, expected);
  
  // We should be able to deserialize / serialize and get the same output.
  {
    auto board = smkflow::Board::Create(board_model);
    board->Deserialize(out);
    EXPECT_EQ(board->Serialize(), out);
  }
}
