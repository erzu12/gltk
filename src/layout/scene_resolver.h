#pragma once

#include "relative_scene.h"
#include "resolved_scene.h"
#include "vec_math.h"

#include <memory>

namespace gltk {

std::unique_ptr<ResolvedScene> resolveScene(const RelativeScene &scene, Vec2 viewportSize);



} // namespace gltk
