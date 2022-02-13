#include "io.hh"

#include <sstream>

#include "mio/mmap.hpp"

namespace breader {

ofile::ofile(const std::string &filename) {
    fstream_.open(filename, std::ios_base::binary | std::ios_base::trunc);
    if (!fstream_.bad()) {
        stream_ = &fstream_;
    }
}

writer ofile::get_writer(std::endian endian) {
    return {*stream_, endian};
}

ifile::ifile(const std::string &filename) {
    std::error_code code;
    source_.map(filename, code);
    if (code) bad_ = true;
}

ifile::ifile(std::istream &stream) {
    bad_ = stream.bad();
    if (!bad_) {
        // readout into memory
        std::ostringstream os;
        os << stream.rdbuf();
        stream_data_ = os.str();
    }
}

reader ifile::get_reader(std::endian endian) {
    std::string_view data;
    if (stream_data_.empty()) {
        data = std::string_view(source_.data(), source_.size());
    } else {
        data = stream_data_;
    }

    return {data, endian};
}

}  // namespace breader
