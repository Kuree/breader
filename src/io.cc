#include "io.hh"

#include "mio/mmap.hpp"

namespace breader {

ifile::ifile(const std::string &filename) {
    std::error_code code;
    source_.map(filename, code);
    if (code) bad_ = true;
}

reader ifile::get_reader(std::endian endian) const {
    std::string_view data(source_.data(), source_.size());
    return {data, endian};
}

}  // namespace breader
