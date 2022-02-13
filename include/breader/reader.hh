#ifndef BREADER_READER_HH
#define BREADER_READER_HH

#include <array>
#include <bit>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "endian.hh"

namespace breader {
class reader {
public:
    explicit reader(std::string_view data) : data_(data) {}
    reader(std::string_view data, std::endian endian) : data_(data), endian_(endian) {}

    template <typename T>
    requires(std::is_arithmetic_v<T>) std::optional<T> read() {
        auto constexpr size = sizeof(T);
        if ((pos_ + size) > data_.size()) return std::nullopt;
        auto *ptr = reinterpret_cast<const T *>(&data_[pos_]);
        pos_ += size;
        if (endian_ == std::endian::native) [[likely]]
            return *ptr;
        else
            return endian::swap(*ptr);
    }

    template <typename T, uint64_t size>
    requires(std::is_arithmetic_v<T>) std::optional<std::array<T, size>> read() {
        std::array<T, size> result = {};
        for (auto i = 0ull; i < size; i++) {
            auto v = read<T>();
            if (!v) [[unlikely]]
                return std::nullopt;
            result[i] = *v;
        }
        return result;
    }

    template <typename T>
    requires(std::is_arithmetic_v<T>) std::optional<std::vector<T>> read(uint64_t size) {
        std::vector<T> result(size);
        for (auto i = 0ull; i < size; i++) {
            auto v = read<T>();
            if (!v) [[unlikely]]
                return std::nullopt;
            result[i] = *v;
        }

        return result;
    }

    template <typename T>
    requires(std::is_same_v<T, std::string>) std::optional<T> read(uint64_t size) {
        std::string result;
        result.resize(size);
        for (auto i = 0ull; i < size; i++) {
            auto v = read<char>();
            if (!v) [[unlikely]]
                return std::nullopt;
            result[i] = *v;
        }

        return result;
    }

    std::optional<std::string_view> read(uint64_t size) {
        if ((pos_ + size) > data_.size()) [[unlikely]]
            return std::nullopt;
        std::string_view result(data_.substr(pos_, size));
        pos_ += size;
        return result;
    }

    [[nodiscard]] uint64_t tellp() const { return pos_; }
    void seekp(uint64_t pos) {
        if (pos <= data_.size()) pos_ = pos;
    }

private:
    std::string_view data_;
    std::endian endian_ = std::endian::little;

    uint64_t pos_ = 0;
};
}  // namespace breader

#endif  // BREADER_READER_HH
