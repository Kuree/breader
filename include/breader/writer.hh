#ifndef BREADER_WRITER_HH
#define BREADER_WRITER_HH

#include <bit>
#include <iostream>
#include <vector>

#include "endian.hh"

namespace breader {
class writer {
public:
    explicit writer(std::ostream &stream) : stream_(stream) {}
    writer(std::ostream &stream, std::endian endian) : stream_(stream), endian_(endian) {}

    template <typename T>
    requires(std::is_arithmetic_v<T>) void write(T value) {
        if (endian_ != std::endian::native) [[unlikely]] {
            value = endian::swap(value);
        }
        char *data = reinterpret_cast<char *>(&value);
        stream_.write(data, sizeof(T));
    }

    void write(const std::string &value) {
        // if it's a unicode string, the endian might be an issue
        stream_.write(value.data(), static_cast<int64_t>(value.size()));
    }

    template <typename T>
    void write(const std::vector<T> &value) {
        for (auto const v : value) {
            this->template write<T>(v);
        }
    }

    [[nodiscard]] uint64_t tellp() const { return stream_.tellp(); }
    void seekp(uint64_t pos) { stream_.seekp(static_cast<long>(pos), std::ios_base::beg); }

    writer operator=(const breader::writer &w) { return {w.stream_, w.endian_}; }

private:
    std::ostream &stream_;
    std::endian endian_ = std::endian::little;
};
}  // namespace breader

#endif  // BREADER_WRITER_HH
