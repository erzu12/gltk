//#include <iostream>
//#include <gltk.h>

//using namespace gltk;

//class Button {
    //std::unique_ptr<Layout> layout;
    //std::unique_ptr<Layout> textLayout;
    //Style buttonStyle;
    //bool pressed = false;
//public:
    //Button(Layout *parent, Window *window, std::string text, Style buttonStyle, Style textStyle) : buttonStyle(buttonStyle) {
        //layout = LayoutBuilder(parent)
            //.addBox(buttonStyle)
            //.setSize(MessureVec2(160, 60))
            //.setMargin({.right=20, .left=20})
            //.setAnchor(Anchors::Center)
            //.build();

        //textLayout = LayoutBuilder(layout.get())
            //.setRenderable(std::make_unique<Text>(text, textStyle))
            //.setAnchor(Anchors::Center)
            //.setSizing(Sizing::Fit, Sizing::Fit)
            //.build();
        
        //layout->addOnMouseKeyDownCallback([&](MouseButtonEvent e) {
            //if (e.button == MouseButton::MOUSE_BUTTON_LEFT) {
                //pressed = true;
                //this->buttonStyle.color = Color(0.3);
                //layout->getRenderable()->setStyle(this->buttonStyle);
            //}
        //});

        //window->add_mouse_up_callback([&](auto _) {
            //pressed = false;
            //this->buttonStyle.color = Color(0.2);
            //layout->getRenderable()->setStyle(this->buttonStyle);
            //layout->registerForRenderRecursive();
        //});

    //}

    //Layout* get() {
        //return layout.get();
    //}
//};

//int main () {
    //gltk::Window window;

    //auto backgroundColor = Color(0.15f);

    //auto background = LayoutBuilder(window.get_layout())
        //.setRenderable(std::make_unique<Box>(Style{.color=backgroundColor}))
        //.setSize(MessureVec2(1., 1.))
        //.setChildPlacement(ChildPlacement::ListStretch)
        //.build();

    //auto header = LayoutBuilder(background.get())
        //.setRenderable(std::make_unique<Box>(Style{Color(0.1)}))
        //.setSize(MessureVec2(1., 100))
        //.setChildPlacement(ChildPlacement::List)
        //.setListDirection(ListDirection::Right)
        //.build();

    //auto buttonStyle = Style{.color=Color(0.2), .radius=15.0};
    //auto buttonTextStyle = Style{.color=Color(0.8), .font="Arial", .fontSize=30};

    //Button boxButton(header.get(), &window, "Box", buttonStyle, buttonTextStyle);
    //boxButton.get()->addOnMouseKeyDownCallback([](MouseButtonEvent e) {
        //std::cout << "Box button clicked" << std::endl;
    //});

    //Button textButton(header.get(), &window, "Text", buttonStyle, buttonTextStyle);


    //auto workspace = LayoutBuilder(background.get())
        //.setSize(MessureVec2(1., 1.))
        //.setOverflow(Overflow::Scroll)
        //.build();

    //auto slideBG = LayoutBuilder(workspace.get())
        //.setRenderable(std::make_unique<Box>(Style{.color=Color::transparent()}))
        //.setSize(MessureVec2(3., 3.))
        //.setAnchor(Anchors::Center)
        //.build();

    //auto slide = LayoutBuilder(slideBG.get())
        //.setRenderable(std::make_unique<Box>(Style{.color=Color::white()}))
        //.setSize(MessureVec2(1920, 1080))
        //.setAnchor(Anchors::Center)
        //.build();

    //bool dragging = false;
    //Vec2 lastMousePos;
    //workspace->addOnMouseKeyDownCallback([&](MouseButtonEvent e) {
        //if (e.button == MouseButton::MOUSE_BUTTON_LEFT && e.mods.isModSet(KeyModifiers::MOD_CONTROL) || e.button == MouseButton::MOUSE_BUTTON_MIDDLE) {
            //dragging = true;
        //}
    //});
    //window.add_mouse_up_callback([&](auto _) {
        //dragging = false;
    //});
    //window.add_mouse_move_callback([&](MouseMoveEvent e) {
        //if (dragging) {
            //Vec2 offset = e.pos - lastMousePos;
            //workspace->scroll(offset);
        //}
        //lastMousePos = e.pos;
    //});


    //window.run([&](Vec2 viewport) {
    //});
    //return 0;
//}
