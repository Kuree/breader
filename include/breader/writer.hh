#ifndef BREADER_WRITER_HH
#define BREADER_WRITER_HH

#include <bit>
#include <iostream>

#include "endian.hh"

namespace breader {
class writer {
public:
    explicit writer(std::ostream &stream) : stream_(stream) {}
    writer(std::ostream &stream, std::endian endian) : stream_(stream), endian_(endian) {}

    template <typename T>
    void write(T value) {
        if (endian_ != std::endian::native) [[unlikely]] {
            value = endian::swap(value);
        }
        char *data = reinterpret_cast<char *>(&value);
        stream_.write(data, sizeof(T));
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
