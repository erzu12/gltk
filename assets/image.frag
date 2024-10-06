#version 460 core
in vec2 UV;
out vec4 FragColor;

uniform sampler2D image;
uniform float radius;
uniform vec2 pixelSize;

bool udRoundBox( vec2 p, vec2 b, float r )
{
    return length(max(abs(p)-b+r,0.0)) > r;
}

void main() {
    vec2 halfRes = 0.5 * pixelSize;
    vec2 pixelPos = UV * pixelSize;

    if(udRoundBox(pixelPos - halfRes, halfRes, radius)) {
        discard;
    }

    FragColor = texture(image, UV);
}  
