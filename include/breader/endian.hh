#ifndef BREADER_ENDIAN_HH
#define BREADER_ENDIAN_HH

#include <type_traits>

namespace breader::endian {
template <typename T>
requires(std::is_arithmetic_v<T>) T swap(T value) {
    auto constexpr size = sizeof(T);
    if constexpr (size == 1) {
        // no need to change bit-order
        return value;
    } else if constexpr (size == 2) {
        // notice that we will use compiler built-in intrinsics.
        // since we only support gcc and clang, this is good enough
        return __builtin_bswap16(value);
    } else if constexpr (size == 4) {
        return __builtin_bswap32(value);
    } else if constexpr (size == 8) {
        return __builtin_bswap64(value);
    } else {
        static_assert("unsupported data type");
    }
}
}  // namespace breader::endian

#endif  // BREADER_ENDIAN_HH
