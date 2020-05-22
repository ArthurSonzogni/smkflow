// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <algorithm>
#include <smkflow/CursorCapture.hpp>

namespace smkflow {

CursorCapture::CursorCapture() = default;

CursorCapture::CursorCapture(CursorCapturable* ref) {
  if (ref->set_.size() >= ref->capacity_)
    return;

  ref_ = ref;
  ref_->set_.insert(this);
}

CursorCapture::~CursorCapture() {
  Invalidate();
}

CursorCapture::operator bool() {
  return ref_;
}

CursorCapture::CursorCapture(CursorCapture&& other) {
  Swap(other);
}

void CursorCapture::operator=(CursorCapture&& other) {
  Swap(other);
}

void CursorCapture::Swap(CursorCapture& other) {
  if (this == &other)
    return;

  std::swap(ref_, other.ref_);
  if (other.ref_) {
    other.ref_->set_.erase(this);
    other.ref_->set_.insert(&other);
  }
  if (ref_) {
    ref_->set_.erase(&other);
    ref_->set_.insert(this);
  }
}

void CursorCapture::Invalidate() {
  if (!ref_)
    return;

  ref_->set_.erase(this);
  ref_ = nullptr;
}

CursorCapturable::CursorCapturable(int capacity) : capacity_(capacity) {}

CursorCapturable::~CursorCapturable() {
  Invalidate();
}

CursorCapture CursorCapturable::Capture() {
  return CursorCapture(this);
}

void CursorCapturable::Invalidate() {
  while (set_.size())
    (**set_.begin()).Invalidate();
}

CursorCapturable::operator bool() {
  return set_.size();
}

}  // namespace smkflow
