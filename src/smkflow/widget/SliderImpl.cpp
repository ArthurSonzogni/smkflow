// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <fmt/core.h>

#include <algorithm>
#include <smk/Input.hpp>
#include <smk/RenderTarget.hpp>
#include <smk/Shape.hpp>
#include <smk/Text.hpp>
#include <smkflow/BoardImpl.hpp>
#include <smkflow/Constants.hpp>
#include <smkflow/widget/Slider.hpp>
#include <smkflow/widget/Widget.hpp>

namespace smkflow {

namespace {
const float handle_width = 8.f;
}  // namespace

class SliderImpl : public Widget, public SliderInterface {
 public:
  SliderImpl(Delegate* delegate,
             float min,
             float max,
             float value,
             std::string format)
      : Widget(delegate), min_(min), max_(max), value_(value), format_(format) {
    value_ = std::min(max_, std::max(min_, value_));

    track_ = smk::Shape::Square();
    handle_ = smk::Shape::Square();

    computed_dimensions_ = {200.f, size::widget_height};
  }

  // Widget implementation:
  glm::vec2 ComputeDimensions() override { return computed_dimensions_; }

  bool Step(smk::Input* input, const glm::vec2& cursor) override {
    auto position = Position();
    auto dimension = dimensions();

    if (cursor_capture_ && input->IsCursorReleased())
      cursor_capture_.Invalidate();

    hover_ = cursor.x > position.x &&                //
             cursor.x < position.x + dimension.x &&  //
             cursor.y > position.y &&                //
             cursor.y < position.y + dimension.y;    //

    focus_ = bool(cursor_capture_);

    if (hover_ && input->IsCursorPressed())
      cursor_capture_ = delegate()->CaptureCursor();

    if (cursor_capture_) {
      hover_ = true;
      value_ = min_ + (cursor.x - position.x) * (max_ - min_) / dimension.x;
      value_ = std::max(min_, std::min(max_, value_));
    }
    return Widget::Step(input, cursor);
  }

  void Draw(smk::RenderTarget* target) override {
    auto position = Position();
    auto dimension = dimensions();

    text_ = smk::Text(delegate()->Font(), fmt::format(format_, value_));

    track_.SetPosition(position);
    track_.SetScale(dimension);
    handle_.SetScale(handle_width, dimension.y);

    handle_.SetPosition(position.x + (dimension.x - handle_width) *
                                         (value_ - min_) / (max_ - min_),
                        position.y);

    auto& track_color_target = focus_ ? color::widget_background_focus
                                      : hover_ ? color::widget_background_hover
                                               : color::widget_background;
    auto& handle_color_target = focus_ ? color::widget_foreground_focus
                                       : hover_ ? color::widget_foreground_hover
                                                : color::widget_foreground;

    track_color_ += (track_color_target - track_color_) * color::transition;
    handle_color_ += (handle_color_target - handle_color_) * color::transition;

    track_.SetColor(track_color_);
    handle_.SetColor(handle_color_);

    target->Draw(track_);
    target->Draw(handle_);

    text_.SetPosition(position + dimension * 0.5f -
                      text_.ComputeDimensions() * 0.5f);
    target->Draw(text_);
  }

  void SetValue(float value) override { value_ = value; }
  float GetValue() override { return value_; }
  JSON Serialize() override {
    JSON json;
    json["value"] = value_;
    return json;
  }
  bool Deserialize(JSON& json) override {
    value_ = json["value"];
    return true;
  }

 private:
  float min_;
  float max_;
  float value_;
  glm::vec2 computed_dimensions_;
  bool focus_ = false;
  bool hover_ = false;
  smk::Transformable handle_;
  smk::Transformable track_;
  CursorCapture cursor_capture_;
  smk::Text text_;
  std::string format_;

  glm::vec4 track_color_ = glm::vec4(0.f);
  glm::vec4 handle_color_ = glm::vec4(0.f);
};

// static
WidgetFactory Slider(float min, float max, float value, std::string format) {
  return [=](Widget::Delegate* delegate) {
    return std::make_unique<SliderImpl>(delegate, min, max, value, format);
  };
}

// static
SliderInterface* Slider(Widget* w) {
  return dynamic_cast<SliderInterface*>(w);
}

}  // namespace smkflow
