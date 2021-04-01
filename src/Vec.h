#pragma once


template<typename T>
struct tvec2 {
    T x, y;

    tvec2() = default;

    template<typename V>
    constexpr tvec2(V scalar): x(scalar), y(scalar) {}

    template<typename V>
    constexpr tvec2(V x, V y): x(x), y(y) {}

    template<typename V>
    constexpr tvec2(const tvec2<V>& other): x(other.x), y(other.y) {}

};
template<typename T>
struct tvec3 {
    T x, y, z;

    tvec3() = default;

    template<typename V>
    constexpr tvec3(V scalar): x(scalar), y(scalar), z(scalar) {}

    template<typename V>
    constexpr tvec3(V x, V y, V z): x(x), y(y), z(z) {}

    template<typename V>
    constexpr tvec3(const tvec3<V>& other): x(other.x), y(other.y), z(other.z) {}
};
using vec2 = tvec2<float>;
using vec3 = tvec3<float>;

using ivec2 = tvec2<int>;
using ivec3 = tvec3<int>;

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
