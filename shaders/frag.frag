#version 420
#extension GL_ARB_separate_shader_objects : enable

in vec4 normalO;
in vec4 baseColor;

layout(location=0) out vec4 color;

void main() {;
    color.xyz = (0.3 + (1.0 + normalO.x) * 0.25) * baseColor.xyz;
    color.w = baseColor.w;
}
