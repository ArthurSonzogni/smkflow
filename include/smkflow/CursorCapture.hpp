// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#ifndef SMKFLOW_CURSOR_CAPTURE_HPP
#define SMKFLOW_CURSOR_CAPTURE_HPP

namespace smkflow {

class CursorCapture;
class CursorCapturable;

class CursorCapture {
 public:
  CursorCapture();
  CursorCapture(CursorCapturable* b);
  explicit operator bool();

  // Move-only
  CursorCapture(const CursorCapture&&) = delete;
  CursorCapture(CursorCapture&& o);
  void operator=(const CursorCapture&) = delete;
  void operator=(CursorCapture&& o);

  void Invalidate();
  virtual ~CursorCapture();

 private:
  CursorCapturable* b = nullptr;
};

class CursorCapturable {
 public:
  CursorCapture Capture();
  void Invalidate();
  explicit operator bool();

 private:
  friend CursorCapture;
  CursorCapture* cursor_capture_ = nullptr;
};

}  // namespace smkflow

#endif /* end of include guard: SMKFLOW_CURSOR_CAPTURE_HPP */
