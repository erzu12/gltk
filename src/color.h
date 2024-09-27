#pragma once

#include <ostream>

namespace gltk {

class Color {
public:
    float r, g, b, a;

    constexpr Color(float rgb = 0) : r(rgb), g(rgb), b(rgb), a(1) {}
    constexpr Color(float r, float g, float b, float a = 1) : r(r), g(g), b(b), a(a) {}

    static Color fromHSVA(float h, float s, float v, float a);
    static Color fromHSV(float h, float s, float v);
    static Color fromHex(int hex);

    Color operator+(const Color other) const;
    Color operator+=(const Color other);

    Color operator-(const Color other) const;
    Color operator-=(const Color other);

    Color operator*(const float other) const;
    Color operator*=(const float other);
    Color operator*(const Color other) const;

    Color operator/(const float other) const;


    bool operator==(const Color &other) const;

    friend std::ostream &operator<<(std::ostream &os, const Color &f2);


    static constexpr Color aliceblue()               { return Color(0.9411764, 0.9725490, 1        ); }
    static constexpr Color antiquewhite()            { return Color(0.9803921, 0.9215686, 0.8431372); }
    static constexpr Color aqua()                    { return Color(0        , 1        , 1        ); }
    static constexpr Color aquamarine()              { return Color(0.4980392, 1        , 0.8313725); }
    static constexpr Color azure()                   { return Color(0.9411764, 1        , 1        ); }
    static constexpr Color beige()                   { return Color(0.9607843, 0.9607843, 0.8627450); }
    static constexpr Color bisque()                  { return Color(1        , 0.8941176, 0.7686274); }
    static constexpr Color black()                   { return Color(0        , 0        , 0        ); }
    static constexpr Color blanchedalmond()          { return Color(1        , 0.9215686, 0.8039215); }
    static constexpr Color blue()                    { return Color(0        , 0        , 1        ); }
    static constexpr Color blueviolet()              { return Color(0.5411764, 0.1686274, 0.8862745); }
    static constexpr Color brown()                   { return Color(0.6470588, 0.1647058, 0.1647058); }
    static constexpr Color burlywood()               { return Color(0.8705882, 0.7215686, 0.5294117); }
    static constexpr Color cadetblue()               { return Color(0.3725490, 0.6196078, 0.6274509); }
    static constexpr Color chartreuse()              { return Color(0.4980392, 1        , 0        ); }
    static constexpr Color chocolate()               { return Color(0.8235294, 0.4117647, 0.1176470); }
    static constexpr Color coral()                   { return Color(1        , 0.4980392, 0.3137254); }
    static constexpr Color cornflowerblue()          { return Color(0.3921568, 0.5843137, 0.9294117); }
    static constexpr Color cornsilk()                { return Color(1        , 0.9725490, 0.8627450); }
    static constexpr Color crimson()                 { return Color(0.8627450, 0.0784313, 0.2352941); }
    static constexpr Color cyan()                    { return Color(0        , 1        , 1        ); }
    static constexpr Color darkblue()                { return Color(0        , 0        , 0.5450980); }
    static constexpr Color darkcyan()                { return Color(0        , 0.5450980, 0.5450980); }
    static constexpr Color darkgoldenrod()           { return Color(0.7215686, 0.5254901, 0.0431372); }
    static constexpr Color darkgray()                { return Color(0.6627450, 0.6627450, 0.6627450); }
    static constexpr Color darkgreen()               { return Color(0        , 0.3921568, 0        ); }
    static constexpr Color darkgrey()                { return Color(0.6627450, 0.6627450, 0.6627450); }
    static constexpr Color darkkhaki()               { return Color(0.7411764, 0.7176470, 0.4196078); }
    static constexpr Color darkmagenta()             { return Color(0.5450980, 0        , 0.5450980); }
    static constexpr Color darkolivegreen()          { return Color(0.3333333, 0.4196078, 0.1843137); }
    static constexpr Color darkorange()              { return Color(1        , 0.5490196, 0        ); }
    static constexpr Color darkorchid()              { return Color(0.6      , 0.1960784, 0.8      ); }
    static constexpr Color darkred()                 { return Color(0.5450980, 0        , 0        ); }
    static constexpr Color darksalmon()              { return Color(0.9137254, 0.5882352, 0.4784313); }
    static constexpr Color darkseagreen()            { return Color(0.5607843, 0.7372549, 0.5607843); }
    static constexpr Color darkslateblue()           { return Color(0.2823529, 0.2392156, 0.5450980); }
    static constexpr Color darkslategray()           { return Color(0.1843137, 0.3098039, 0.3098039); }
    static constexpr Color darkslategrey()           { return Color(0.1843137, 0.3098039, 0.3098039); }
    static constexpr Color darkturquoise()           { return Color(0        , 0.8078431, 0.8196078); }
    static constexpr Color darkviolet()              { return Color(0.5803921, 0        , 0.8274509); }
    static constexpr Color deeppink()                { return Color(1        , 0.0784313, 0.5764705); }
    static constexpr Color deepskyblue()             { return Color(0        , 0.7490196, 1        ); }
    static constexpr Color dimgray()                 { return Color(0.4117647, 0.4117647, 0.4117647); }
    static constexpr Color dimgrey()                 { return Color(0.4117647, 0.4117647, 0.4117647); }
    static constexpr Color dodgerblue()              { return Color(0.1176470, 0.5647058, 1        ); }
    static constexpr Color firebrick()               { return Color(0.6980392, 0.1333333, 0.1333333); }
    static constexpr Color floralwhite()             { return Color(1        , 0.9803921, 0.9411764); }
    static constexpr Color forestgreen()             { return Color(0.1333333, 0.5450980, 0.1333333); }
    static constexpr Color fuchsia()                 { return Color(1        , 0        , 1        ); }
    static constexpr Color gainsboro()               { return Color(0.8627450, 0.8627450, 0.8627450); }
    static constexpr Color ghostwhite()              { return Color(0.9725490, 0.9725490, 1        ); }
    static constexpr Color gold()                    { return Color(1        , 0.8431372, 0        ); }
    static constexpr Color goldenrod()               { return Color(0.8549019, 0.6470588, 0.1254901); }
    static constexpr Color gray()                    { return Color(0.5019607, 0.5019607, 0.5019607); }
    static constexpr Color green()                   { return Color(0        , 0.5019607, 0        ); }
    static constexpr Color greenyellow()             { return Color(0.6784313, 1        , 0.1843137); }
    static constexpr Color grey()                    { return Color(0.5019607, 0.5019607, 0.5019607); }
    static constexpr Color honeydew()                { return Color(0.9411764, 1        , 0.9411764); }
    static constexpr Color hotpink()                 { return Color(1        , 0.4117647, 0.7058823); }
    static constexpr Color indianred()               { return Color(0.8039215, 0.3607843, 0.3607843); }
    static constexpr Color indigo()                  { return Color(0.2941176, 0        , 0.5098039); }
    static constexpr Color ivory()                   { return Color(1        , 1        , 0.9411764); }
    static constexpr Color khaki()                   { return Color(0.9411764, 0.9019607, 0.5490196); }
    static constexpr Color lavender()                { return Color(0.9019607, 0.9019607, 0.9803921); }
    static constexpr Color lavenderblush()           { return Color(1        , 0.9411764, 0.9607843); }
    static constexpr Color lawngreen()               { return Color(0.4862745, 0.9882352, 0        ); }
    static constexpr Color lemonchiffon()            { return Color(1        , 0.9803921, 0.8039215); }
    static constexpr Color lightblue()               { return Color(0.6784313, 0.8470588, 0.9019607); }
    static constexpr Color lightcoral()              { return Color(0.9411764, 0.5019607, 0.5019607); }
    static constexpr Color lightcyan()               { return Color(0.8784313, 1        , 1        ); }
    static constexpr Color lightgoldenrodyellow()    { return Color(0.9803921, 0.9803921, 0.8235294); }
    static constexpr Color lightgray()               { return Color(0.8274509, 0.8274509, 0.8274509); }
    static constexpr Color lightgreen()              { return Color(0.5647058, 0.9333333, 0.5647058); }
    static constexpr Color lightgrey()               { return Color(0.8274509, 0.8274509, 0.8274509); }
    static constexpr Color lightpink()               { return Color(1        , 0.7137254, 0.7568627); }
    static constexpr Color lightsalmon()             { return Color(1        , 0.6274509, 0.4784313); }
    static constexpr Color lightseagreen()           { return Color(0.1254901, 0.6980392, 0.6666666); }
    static constexpr Color lightskyblue()            { return Color(0.5294117, 0.8078431, 0.9803921); }
    static constexpr Color lightslategray()          { return Color(0.4666666, 0.5333333, 0.6      ); }
    static constexpr Color lightslategrey()          { return Color(0.4666666, 0.5333333, 0.6      ); }
    static constexpr Color lightsteelblue()          { return Color(0.6901960, 0.7686274, 0.8705882); }
    static constexpr Color lightyellow()             { return Color(1        , 1        , 0.8784313); }
    static constexpr Color lime()                    { return Color(0        , 1        , 0        ); }
    static constexpr Color limegreen()               { return Color(0.1960784, 0.8039215, 0.1960784); }
    static constexpr Color linen()                   { return Color(0.9803921, 0.9411764, 0.9019607); }
    static constexpr Color magenta()                 { return Color(1        , 0        , 1        ); }
    static constexpr Color maroon()                  { return Color(0.5019607, 0        , 0        ); }
    static constexpr Color mediumaquamarine()        { return Color(0.4      , 0.803    , 0.6666666); }
    static constexpr Color mediumblue()              { return Color(0        , 0        , 0.8039215); }
    static constexpr Color mediumorchid()            { return Color(0.7294117, 0.3333333, 0.8274509); }
    static constexpr Color mediumpurple()            { return Color(0.5764705, 0.4392156, 0.8588235); }
    static constexpr Color mediumseagreen()          { return Color(0.2352941, 0.7019607, 0.4431372); }
    static constexpr Color mediumslateblue()         { return Color(0.4823529, 0.4078431, 0.9333333); }
    static constexpr Color mediumspringgreen()       { return Color(0        , 0.9803921, 0.6039215); }
    static constexpr Color mediumturquoise()         { return Color(0.2823529, 0.8196078, 0.8      ); }
    static constexpr Color mediumvioletred()         { return Color(0.7803921, 0.0823529, 0.5215686); }
    static constexpr Color midnightblue()            { return Color(0.0980392, 0.0980392, 0.4392156); }
    static constexpr Color mintcream()               { return Color(0.9607843, 1        , 0.9803921); }
    static constexpr Color mistyrose()               { return Color(1        , 0.8941176, 0.8823529); }
    static constexpr Color moccasin()                { return Color(1        , 0.8941176, 0.7098039); }
    static constexpr Color navajowhite()             { return Color(1        , 0.8705882, 0.6784313); }
    static constexpr Color navy()                    { return Color(0        , 0        , 0.5019607); }
    static constexpr Color oldlace()                 { return Color(0.9921568, 0.9607843, 0.9019607); }
    static constexpr Color olive()                   { return Color(0.5019607, 0.5019607, 0        ); }
    static constexpr Color olivedrab()               { return Color(0.4196078, 0.5568627, 0.1372549); }
    static constexpr Color orange()                  { return Color(1        , 0.6470588, 0        ); }
    static constexpr Color orangered()               { return Color(1        , 0.2705882, 0        ); }
    static constexpr Color orchid()                  { return Color(0.8549019, 0.4392156, 0.8392156); }
    static constexpr Color palegoldenrod()           { return Color(0.9333333, 0.9098039, 0.6666666); }
    static constexpr Color palegreen()               { return Color(0.5960784, 0.9843137, 0.5960784); }
    static constexpr Color paleturquoise()           { return Color(0.6862745, 0.9333333, 0.9333333); }
    static constexpr Color palevioletred()           { return Color(0.8588235, 0.4392156, 0.5764705); }
    static constexpr Color papayawhip()              { return Color(1        , 0.9372549, 0.8352941); }
    static constexpr Color peachpuff()               { return Color(1        , 0.8549019, 0.7254901); }
    static constexpr Color peru()                    { return Color(0.8039215, 0.5215686, 0.2470588); }
    static constexpr Color pink()                    { return Color(1        , 0.7529411, 0.7960784); }
    static constexpr Color plum()                    { return Color(0.8666666, 0.6274509, 0.8666666); }
    static constexpr Color powderblue()              { return Color(0.6901960, 0.8784313, 0.9019607); }
    static constexpr Color purple()                  { return Color(0.5019607, 0        , 0.5019607); }
    static constexpr Color rebeccapurple()           { return Color(0.4      , 0.2      , 0.6      ); }
    static constexpr Color red()                     { return Color(1        , 0        , 0        ); }
    static constexpr Color rosybrown()               { return Color(0.7372549, 0.5607843, 0.5607843); }
    static constexpr Color royalblue()               { return Color(0.2549019, 0.4117647, 0.8823529); }
    static constexpr Color saddlebrown()             { return Color(0.5450980, 0.2705882, 0.0745098); }
    static constexpr Color salmon()                  { return Color(0.9803921, 0.5019607, 0.4470588); }
    static constexpr Color sandybrown()              { return Color(0.9568627, 0.6431372, 0.3764705); }
    static constexpr Color seagreen()                { return Color(0.1803921, 0.5450980, 0.3411764); }
    static constexpr Color seashell()                { return Color(1        , 0.9607843, 0.9333333); }
    static constexpr Color sienna()                  { return Color(0.6274509, 0.3215686, 0.1764705); }
    static constexpr Color silver()                  { return Color(0.7529411, 0.7529411, 0.7529411); }
    static constexpr Color skyblue()                 { return Color(0.5294117, 0.8078431, 0.9215686); }
    static constexpr Color slateblue()               { return Color(0.4156862, 0.3529411, 0.8039215); }
    static constexpr Color slategray()               { return Color(0.4392156, 0.5019607, 0.5647058); }
    static constexpr Color slategrey()               { return Color(0.4392156, 0.5019607, 0.5647058); }
    static constexpr Color snow()                    { return Color(1        , 0.9803921, 0.9803921); }
    static constexpr Color springgreen()             { return Color(0        , 1        , 0.4980392); }
    static constexpr Color steelblue()               { return Color(0.2745098, 0.5098039, 0.7058823); }
    static constexpr Color tan()                     { return Color(0.8235294, 0.7058823, 0.5490196); }
    static constexpr Color teal()                    { return Color(0        , 0.5019607, 0.5019607); }
    static constexpr Color thistle()                 { return Color(0.8470588, 0.7490196, 0.8470588); }
    static constexpr Color tomato()                  { return Color(1        , 0.3882352, 0.2784313); }
    static constexpr Color turquoise()               { return Color(0.2509803, 0.8784313, 0.8156862); }
    static constexpr Color violet()                  { return Color(0.9333333, 0.5098039, 0.9333333); }
    static constexpr Color wheat()                   { return Color(0.9607843, 0.8705882, 0.7019607); }
    static constexpr Color white()                   { return Color(1        , 1        , 1        ); }
    static constexpr Color whitesmoke()              { return Color(0.9607843, 0.9607843, 0.9607843); }
    static constexpr Color yellow()                  { return Color(1        , 1        , 0        ); }
    static constexpr Color yellowgreen()             { return Color(0.6039215, 0.8039215, 0.1960784); }
    static constexpr Color transparent()             { return Color(0, 0, 0, 0); }

};

}  // namespace gltk
