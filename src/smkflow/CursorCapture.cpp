// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <algorithm>
#include <smkflow/CursorCapture.hpp>

namespace smkflow {

CursorCapture::CursorCapture() = default;
CursorCapture::CursorCapture(CursorCapturable* b) : b(b) {}
CursorCapture::operator bool() {
  return b;
}
CursorCapture::CursorCapture(CursorCapture&& o) {
  operator=(std::move(o));
}
void CursorCapture::operator=(CursorCapture&& o) {
  std::swap(b, o.b);
  if (b) {
    b->cursor_capture_ = this;
  }
  if (o.b) {
    o.b->cursor_capture_ = &o;
  }
}
void CursorCapture::Invalidate() {
  if (b) {
    b->cursor_capture_ = nullptr;
    b = nullptr;
  }
}
CursorCapture::~CursorCapture() {
  Invalidate();
}

CursorCapture CursorCapturable::Capture() {
  return cursor_capture_ ? CursorCapture() : CursorCapture(this);
}
void CursorCapturable::Invalidate() {
  if (cursor_capture_)
    cursor_capture_->Invalidate();
}

CursorCapturable::operator bool() {
  return cursor_capture_;
}

}  // namespace smkflow
