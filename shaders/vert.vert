#version 420
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;

out vec4 normalO;
out vec4 baseColor;

layout(std140, set=0, binding=0) uniform Matrices {
    mat4 m;
    mat4 rot;
    vec4 color;
} uni;

layout(std140, set=1, binding=0) uniform PV {
    mat4 pv;
} pv;

void main() {
    mat4 pvm = pv.pv * uni.m;
    gl_Position = pvm * vec4(vertex, 1);
    normalO = uni.rot * vec4(normal, 1);
    baseColor = uni.color;
}