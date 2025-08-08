#include <gltk.h>
#include <iostream>

using namespace gltk;

int main() {
    gltk::Window window;
    window.add_key_down_callback([](auto e) { std::cout << "Key down: " << static_cast<int>(e.key) << std::endl; });
    auto rootLayout = window.getScene()->getRoot();

    // auto bg = LayoutBuilder(rootLayout)
    //               .setRenderable(std::make_unique<Box>(Style{Color(0.0f, 0.0f, 0.5f)}))
    //               .setSize(MessureVec2(100_pct, 100_pct))
    //               .build();

    // auto box = LayoutBuilder(rootLayout)
    //                .setRenderable(std::make_unique<Box>(Style{.color = Color::white(), .radius = 20}))
    //                .setAnchor(Anchors::TopLeft)
    //                .setPivot(Anchors::TopLeft)
    //                .setSize(MessureVec2(50_pct, 50_pct))
    //                .setOffset(MessureVec2(100_px, 100_px))
    //                .build();

    auto svg = LayoutBuilder(rootLayout)
                   .setRenderable(std::make_unique<SVGImage>("assets/fleet.svg", Style{.radius = 20}))
                   .setAnchor(Anchors::TopLeft)
                   .setPivot(Anchors::TopLeft)
                   .setOffset(MessureVec2(100_px, 100_px))
                   .setSizing(Sizing{SizingMode::Content, SizingMode::Content})
                   .build();

    auto image = LayoutBuilder(rootLayout)
                     .setRenderable(std::make_unique<Image>("assets/image.png", Style{.radius = 20}))
                     .setOffset(MessureVec2(100_px, 800_px))
                     .setAnchor(Anchors::TopLeft)
                     .setPivot(Anchors::TopLeft)
                     .setSizing(Sizing{SizingMode::Content, SizingMode::Content})
                     .build();
    // window.getScene()->addRelativeLayout(std::move(bg));
    window.getScene()->addRelativeLayout(std::move(svg), rootLayout);
    window.getScene()->addRelativeLayout(std::move(image), rootLayout);

    window.run([&](Vec2 viewport) {});
    return 0;
}
