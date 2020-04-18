#include <smkflow/CursorCapture.hpp>
#include <algorithm>

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
  if (b)
    b->Move(this);
}
void CursorCapture::Invalidate() {
  this->~CursorCapture();
}
CursorCapture::~CursorCapture() {
  if (b)
    b->cursor_capture_ = nullptr;
  b = nullptr;
}

CursorCapture CursorCapturable::Capture() {
  return cursor_capture_ ? CursorCapture() : CursorCapture(this);
}
void CursorCapturable::Invalidate() {
  if (cursor_capture_)
    cursor_capture_->Invalidate();
}

void CursorCapturable::Move(CursorCapture* c) {
  cursor_capture_ = c;
}

CursorCapturable::operator bool() {
  return cursor_capture_;
}

}  // namespace smkflow
