#version 330 core

layout(location = 0) out vec4 color;
// out vec4 color;

// layout(location = 1) out int gDrawID;

in vec2 vTexCoords;

uniform sampler2D uTexture;
// uniform vec4 uTint;
// uniform int uDrawID;
uniform vec4 uTexCrop; // (x1, y1, x2, y2)

float lerp(float from, float to, float t) {
    return from + ((to - from) * t);
}

void main() {
    color = texture(uTexture, vTexCoords);
    vec2 texCoord = vTexCoords;
    texCoord.x = lerp(uTexCrop.x, uTexCrop.z, vTexCoords.x);
    texCoord.y = lerp(uTexCrop.y, uTexCrop.w, vTexCoords.y);
    color = texture(uTexture, texCoord) /* uTint*/;
    // gDrawID = uDrawID;

    if(color.a < 0.1)
        discard;
}