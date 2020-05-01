// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.

#include <gtest/gtest.h>

#include <smkflow/Elements.hpp>
#include <smkflow/Model.hpp>
#include <smk/Window.hpp>

#include "asset.hpp"

class Test : public ::testing::Test {
 public:
  Test() { window_ = smk::Window(640, 480, ""); }
  smk::Window& window() { return window_; }
  const char* font() { return asset::arial_ttf; }

 private:
  smk::Window window_;
};
