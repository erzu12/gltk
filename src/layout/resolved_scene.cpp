#include "resolved_scene.h"

namespace gltk {

void ResolvedScene::render() const {
    if (!root.has_value()) {
        return;
    }
    for (const auto &layout : layouts) {
        if (layout->renderable.has_value()) {
            Mat3 modelMatrix = Mat3::translationMatrix(layout->Position);
            modelMatrix = modelMatrix * Mat3::scalingMatrix(layout->Size);
            layout->renderable.value()->render(root.value()->Size, modelMatrix, layout->Size, layout->clipRegion);
        }
    }
}

} // namespace gltk
