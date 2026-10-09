// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/public/platform/web_mouse_wheel_event.h"

namespace blink {

float WebMouseWheelEvent::DeltaXInRootFrame() const {
  return delta_x / frame_scalex_;
}

float WebMouseWheelEvent::DeltaYInRootFrame() const {
  return delta_y / frame_scaley_;
}

WebMouseWheelEvent WebMouseWheelEvent::FlattenTransform() const {
  WebMouseWheelEvent result = *this;
  result.delta_x /= result.frame_scalex_;
  result.delta_y /= result.frame_scaley_;
  result.FlattenTransformSelf();
  return result;
}

}  // namespace blink
