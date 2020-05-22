// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef SMKFLOW_CURSOR_CAPTURE_HPP
#define SMKFLOW_CURSOR_CAPTURE_HPP

#include <set>

namespace smkflow {

class CursorCapture;
class CursorCapturable;

class CursorCapture {
 public:
  CursorCapture();
  CursorCapture(CursorCapturable* ref);
  explicit operator bool();

  // Move-only
  CursorCapture(const CursorCapture&&) = delete;
  CursorCapture(CursorCapture&& o);
  void operator=(const CursorCapture&) = delete;
  void operator=(CursorCapture&& o);

  void Invalidate();
  virtual ~CursorCapture();

  void Swap(CursorCapture& other);

 private:
  CursorCapturable* ref_ = nullptr;
};

class CursorCapturable {
 public:
  CursorCapturable(int capacity = 1);
  ~CursorCapturable();
  CursorCapture Capture();
  void Invalidate();
  explicit operator bool();

 private:
  friend CursorCapture;
  int capacity_ = 0;
  std::set<CursorCapture*> set_;
};

}  // namespace smkflow

#endif /* end of include guard: SMKFLOW_CURSOR_CAPTURE_HPP */
