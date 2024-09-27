# version 460 core
out vec4 FragColor;

uniform vec4 color;
uniform float radius;
uniform vec2 pixelSize;

in vec2 UV;

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

	FragColor = vec4(color);
}
