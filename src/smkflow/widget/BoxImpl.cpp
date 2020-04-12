// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.

#include <smkflow/widget/Box.hpp>
#include <smkflow/Constants.hpp>

namespace smkflow {

class BoxImpl : public Widget, public BoxInterface {
 public:
  BoxImpl(Node* node, std::vector<WidgetFactory> children): Widget(node) {
    for (auto& it : children)
      children_.push_back(it(node));
    children_size_.resize(children_.size());
  }

  void Step(smk::Input* input, const glm::vec2& cursor) override {
    for(auto& it : children_)
      it->Step(input, cursor);
  }

  void Draw(smk::RenderTarget* target) override {
    for (auto& child : children_)
      child->Draw(target);
  }

  virtual Widget* ChildAt(int i) override { return children_.at(i).get();}
  virtual int ChildCount() override { return children_.size();}

 protected:
  std::vector<std::unique_ptr<Widget>> children_;
  std::vector<float> children_size_;
  glm::vec2 requested_dimensions_;
};

class BoxImplVertical : public BoxImpl {
 public:
  BoxImplVertical(Node* node, std::vector<WidgetFactory> children)
      : BoxImpl(node, children) {}

  glm::vec2 ComputeDimensions() override {
    requested_dimensions_ = glm::vec2(0.f, 0.f);
    int i = 0;
    for (auto& it : children_) {
      auto child_dimension = it->ComputeDimensions();
      requested_dimensions_.x =
          std::max(requested_dimensions_.x, child_dimension.x);
      requested_dimensions_.y += child_dimension.y;
      children_size_[i++] = child_dimension.y;
    }
    requested_dimensions_ += (children_.size() - 1) * size::widget_margin;
    return requested_dimensions_;
  }

  void SetDimensions(glm::vec2 dimensions) override {
    Widget::SetDimensions(dimensions);
    float space_left = dimensions.y - requested_dimensions_.y;
    space_left /= children_.size();
    int i = 0;
    for (auto& it : children_)
      it->SetDimensions({dimensions.x, children_size_[i++] + space_left});
  };

  void SetPosition(glm::vec2 position) override {
    Widget::SetPosition(position);
    for (auto& it : children_) {
      it->SetPosition(position);
      position.y += it->dimensions().y + size::widget_margin;
    }
  };
};

class BoxImplHorizontal: public BoxImpl {
 public:
  BoxImplHorizontal(Node* node, std::vector<WidgetFactory> children)
      : BoxImpl(node, children) {}

  glm::vec2 ComputeDimensions() override {
    requested_dimensions_ = glm::vec2(0.f, 0.f);
    int i = 0;
    for (auto& it : children_) {
      auto child_dimension = it->ComputeDimensions();
      requested_dimensions_.y =
          std::max(requested_dimensions_.y, child_dimension.y);
      requested_dimensions_.x += child_dimension.x;
      children_size_[i++] = child_dimension.x;
    }
    requested_dimensions_ += (children_.size() - 1) * size::widget_margin;
    return requested_dimensions_;
  }

  void SetDimensions(glm::vec2 dimensions) override {
    Widget::SetDimensions(dimensions);
    float space_left = dimensions.x - requested_dimensions_.x;
    space_left /= children_.size();
    int i = 0;
    for (auto& it : children_)
      it->SetDimensions({children_size_[i++] + space_left, dimensions.y});
  };

  void SetPosition(glm::vec2 position) override {
    Widget::SetPosition(position);
    for (auto& it : children_) {
      it->SetPosition(position);
      position.x += it->dimensions().x + size::widget_margin;
    }
  };
};

// static
WidgetFactory HBox(std::vector<WidgetFactory> children) {
  return [=](Node* node) {
    return std::make_unique<BoxImplHorizontal>(node, children);
  };
}

// static
WidgetFactory VBox(std::vector<WidgetFactory> children) {
  return [=](Node* node) {
    return std::make_unique<BoxImplVertical>(node, children);
  };
}

// static
BoxInterface* Box(Widget* w) {
  return dynamic_cast<BoxInterface*>(w);
}

}  // namespace smkflow
