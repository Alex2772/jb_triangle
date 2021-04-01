#pragma once


template<typename T>
struct tvec2 {
    T x, y;
};
template<typename T>
struct tvec3 {
    T x, y, z;
};
template<typename T>
struct tvec4 {
    T x, y, z, w;
};

using vec2 = tvec2<float>;
using vec3 = tvec3<float>;
using vec4 = tvec4<float>;

using ivec2 = tvec2<int>;
using ivec3 = tvec3<int>;
using ivec4 = tvec4<int>;

/**
 * @brief Compose color from hex integer
 * @param color Color represented in HEX
 * @param 0xff00ff_rgb returns {1, 0, 1}, which is purple
 * @return Color represented in vec3
 */
constexpr vec3 operator"" _rgb(unsigned long long color) {
    return vec3{
            float((color >> 16) & 0xff) / 255.f,
            float((color >> 8) & 0xff) / 255.f,
            float((color) & 0xff) / 255.f
    };

}
