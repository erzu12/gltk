#include <gltk.h>
#include <iostream>

using namespace gltk;

int main() {
    gltk::Window window;
    window.add_key_down_callback([](auto e) { std::cout << "Key down: " << static_cast<int>(e.key) << std::endl; });
    auto rootLayout = window.getScene()->getRoot();

    auto bg = LayoutBuilder(rootLayout)
                  .setRenderable(std::make_unique<Box>(Style{Color(0.0f, 0.0f, 0.5f)}))
                  .setSize(MessureVec2(80_pct, 80_pct))
                  .setChildPlacement(ChildPlacement::ListStretch)
                  .build();

    // auto box = LayoutBuilder(rootLayout)
    //                .setRenderable(std::make_unique<Box>(Style{.color = Color::white(), .radius = 20}))
    //                .setAnchor(Anchors::TopLeft)
    //                .setPivot(Anchors::TopLeft)
    //                .setSize(MessureVec2(50_pct, 50_pct))
    //                .setOffset(MessureVec2(100_px, 100_px))
    //                .build();

    auto svg = LayoutBuilder(bg.get())
                   .setRenderable(std::make_unique<SVGImage>("assets/fleet.svg", Style{.radius = 20}))
                   .setSize(MessureVec2(50_pct, 100_px))
                   .setSizing(Sizing{SizingMode::Layout, SizingMode::Content})
                   .build();

    auto image = LayoutBuilder(bg.get())
                     .setRenderable(std::make_unique<Image>("assets/image.png", Style{.radius = 20}))
                     .setSize(MessureVec2(50_pct, 100_px))
                     .setSizing(Sizing{SizingMode::Layout, SizingMode::Content})
                     .build();

    auto box = LayoutBuilder(bg.get())
                   .setRenderable(std::make_unique<Box>(Style{.color = Color(0.0f, 1.0f, 1.0f), .radius = 20}))
                   .setSize(MessureVec2(50_pct, 20_pct))
                   .build();

    auto box2 = LayoutBuilder(bg.get())
                    .setRenderable(std::make_unique<Box>(Style{.color = Color(1.0f, 0.0f, 1.0f), .radius = 20}))
                    .setSize(MessureVec2(50_pct, 20_pct))
                    .build();

    window.getScene()->addEventCallback<MouseMoveEvent>(
        [&](MouseMoveEvent &event) { std::cout << "Mouse moved at: " << event.pos << std::endl; }, svg.get()
    );

    // window.getScene()->addRelativeLayout(std::move(bg));
    auto bgRef = bg.get();
    window.getScene()->addRelativeLayout(std::move(bg), rootLayout);
    window.getScene()->addRelativeLayout(std::move(image), bgRef);
    window.getScene()->addRelativeLayout(std::move(box), bgRef);
    window.getScene()->addRelativeLayout(std::move(svg), bgRef);
    window.getScene()->addRelativeLayout(std::move(box2), bgRef);

    window.run([&](Vec2 viewport) {});
    return 0;
}
