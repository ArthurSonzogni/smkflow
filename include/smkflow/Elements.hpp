// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef SMKFLOW_ELEMENT_HPP
#define SMKFLOW_ELEMENT_HPP

#include <memory>
#include <smk/Input.hpp>
#include <smk/RenderTarget.hpp>
#include <smkflow/Model.hpp>
#include <smkflow/JSON.hpp>

namespace smkflow {

class Board;
class Node;
class Slot;
class Connector;
class Widget;

class Board {
 public:
  static std::unique_ptr<Board> Create(const model::Board&);
  virtual ~Board() = default;

  // Called 60/s for animations:
  virtual void Step(smk::RenderTarget* target, smk::Input* input) = 0;

  // Called for producing a new image:
  virtual void Draw(smk::RenderTarget* target) = 0;

  // Modifier;
  virtual Node* Create(const model::Node&) = 0;
  virtual void Connect(Slot* A, Slot* B) = 0;

  // Observers:
  virtual int NodeCount() = 0;
  virtual Node* NodeAt(int i) = 0;

  // Used by widget to keep the cursor for it.
  virtual CursorCapture CaptureCursor() = 0;

  // Serialization:
  virtual JSON Serialize() = 0;
  virtual bool Deserialize(JSON& in) = 0;
};

class Slot {
 public:
  virtual ~Slot() = default;

  virtual Connector* GetConnector() = 0;

  virtual Slot* OppositeSlot() = 0;

  virtual Node* GetNode() = 0;
  virtual Node* OppositeNode() = 0;
  virtual void SetText(const std::string& text) = 0;
};

class Connector {
 public:
  virtual ~Connector() = default;

  virtual Slot* GetInput() = 0;
  virtual Slot* GetOutput() = 0;
};

class Node {
 public:
  virtual ~Node() = default;
  virtual Board* GetBoard() = 0;

  virtual int Identifier() = 0;
  virtual void SetPosition(const glm::vec2&) = 0;

  virtual int InputCount() = 0;
  virtual Slot* InputAt(int i) = 0;

  virtual int OutputCount() = 0;
  virtual Slot* OutputAt(int i) = 0;

  virtual Widget* widget() = 0;
};

}  // namespace smkflow
#endif /* end of include guard: SMKFLOW_ELEMENT_HPP */
