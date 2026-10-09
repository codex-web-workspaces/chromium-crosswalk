// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <utility>

#include "third_party/blink/renderer/core/css/cssom/paint_worklet_input.h"

namespace blink {

PaintWorkletInput::PaintWorkletInput(
    const String& name,
    const FloatSize& container_size,
    float effective_zoom,
    int worklet_id,
    PaintWorkletStylePropertyMap::CrossThreadData data,
    Vector<std::unique_ptr<CrossThreadStyleValue>> parsed_input_arguments)
    : name_(name.IsolatedCopy()),
      container_size_(container_size),
      effective_zoomx_(effective_zoom),
      effective_zoomy_(1.0f),
      worklet_id_(worklet_id),
      style_map_data_(std::move(data)),
      parsed_input_arguments_(std::move(parsed_input_arguments)) {}

PaintWorkletInput::PaintWorkletInput(
    const String& name,
    const FloatSize& container_size,
    float effective_zoomx,
    float effective_zoomy,
    int worklet_id,
    PaintWorkletStylePropertyMap::CrossThreadData data,
    Vector<std::unique_ptr<CrossThreadStyleValue>> parsed_input_arguments)
    : name_(name.IsolatedCopy()),
      container_size_(container_size),
      effective_zoomx_(effective_zoomx),
      effective_zoomy_(effective_zoomy),
      worklet_id_(worklet_id),
      style_map_data_(std::move(data)),
      parsed_input_arguments_(std::move(parsed_input_arguments)) {}

}  // namespace blink
