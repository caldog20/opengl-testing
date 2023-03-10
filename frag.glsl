#version 330 core
in vec4 vertexColor;
in vec2 texCoords;
flat in ivec2 clutBase;
flat in ivec2 texpageBase;
flat in int texMode;

// We use dual-source blending in order to emulate the fact that the GPU can enable blending per-pixel
// FragColor: The colour of the pixel before alpha blending comes into play
// BlendColor: Contains blending coefficients
layout(location = 0, index = 0) out vec4 FragColor;
layout(location = 0, index = 1) out vec4 BlendColor;

// Tex window uniform format
// x, y components: masks to & coords with
// z, w components: masks to | coords with
uniform ivec4 u_texWindow;
uniform sampler2D u_vramTex;
uniform vec4 u_blendFactors;
uniform vec4 u_blendFactorsIfOpaque = vec4(0.0, 0.0, 0.0, 1.0);

int floatToU5(float f) {
    return int(floor(f * 31.0 + 0.5));
}

vec4 sampleVRAM(ivec2 coords) {
    coords &= ivec2(1023, 511); // Out-of-bounds VRAM accesses wrap
    return texelFetch(u_vramTex, coords, 0);
}

int sample16(ivec2 coords) {
    vec4 colour = sampleVRAM(coords);
    int r = floatToU5(colour.r);
    int g = floatToU5(colour.g);
    int b = floatToU5(colour.b);
    int msb = int(ceil(colour.a)) << 15;
    return r | (g << 5) | (b << 10) | msb;
}

// Apply texture blending
// Formula for RGB8 colours: col1 * col2 / 128
vec4 texBlend(vec4 colour1, vec4 colour2) {
    vec4 ret = (colour1 * colour2) / (128.0 / 255.0);
    ret.a = 1.0;
    return ret;
}

void main() {
    if (texMode == 4) { // Untextured primitive
        FragColor = vertexColor;
        BlendColor = u_blendFactors;
        return;
    }

    // Fix up UVs and apply texture window
    ivec2 UV = ivec2(floor(texCoords + vec2(0.0001, 0.0001))) & ivec2(0xff);
    UV = (UV & u_texWindow.xy) | u_texWindow.zw;

    if (texMode == 0) { // 4bpp texture
        ivec2 texelCoord = ivec2(UV.x >> 2, UV.y) + texpageBase;

        int sample1 = sample16(texelCoord);
        int shift = (UV.x & 3) << 2;
        int clutIndex = (sample1 >> shift) & 0xf;

        ivec2 sampleCoords = ivec2(clutBase.x + clutIndex, clutBase.y);
        FragColor = texelFetch(u_vramTex, sampleCoords, 0);

        if (FragColor.rgb == vec3(0.0, 0.0, 0.0)) discard;
        BlendColor = FragColor.a >= 0.5 ? u_blendFactors : u_blendFactorsIfOpaque;
        FragColor = texBlend(FragColor, vertexColor);
    } else if (texMode == 1) { // 8bpp texture
        ivec2 texelCoord = ivec2(UV.x >> 1, UV.y) + texpageBase;

        int sample1 = sample16(texelCoord);
        int shift = (UV.x & 1) << 3;
        int clutIndex = (sample1 >> shift) & 0xff;

        ivec2 sampleCoords = ivec2(clutBase.x + clutIndex, clutBase.y);
        FragColor = texelFetch(u_vramTex, sampleCoords, 0);

        if (FragColor.rgb == vec3(0.0, 0.0, 0.0)) discard;
        BlendColor = FragColor.a >= 0.5 ? u_blendFactors : u_blendFactorsIfOpaque;
        FragColor = texBlend(FragColor, vertexColor);
    } else { // Texture depth 2 and 3 both indicate 16bpp textures
        ivec2 texelCoord = UV + texpageBase;
        FragColor = sampleVRAM(texelCoord);

        if (FragColor.rgb == vec3(0.0, 0.0, 0.0)) discard;
        FragColor = texBlend(FragColor, vertexColor);
        BlendColor = u_blendFactors;
    }
}