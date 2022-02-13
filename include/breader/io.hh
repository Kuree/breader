#ifndef BREADER_IO_HH
#define BREADER_IO_HH
#include <fstream>
#include <memory>

#include "mio/mmap.hpp"
#include "reader.hh"
#include "writer.hh"

namespace breader {

class ofile {
public:
    explicit ofile(const std::string &filename);
    explicit ofile(std::ostream &stream) : stream_(&stream) {}
    std::ostream &stream() { return *stream_; }
    writer get_writer(std::endian endian = std::endian::little);
    [[nodiscard]] bool bad() const { return stream_ == nullptr; }

private:
    std::ostream *stream_ = nullptr;
    std::ofstream fstream_;
};

class ifile {
public:
    explicit ifile(const std::string &filename);
    // this is for testing only
    explicit ifile(std::istream &stream);
    [[nodiscard]] bool bad() const { return bad_; }
    reader get_reader(std::endian endian = std::endian::little);

private:
    bool bad_ = false;
    mio::mmap_source source_;
    std::string stream_data_;
};

}  // namespace breader

#endif  // BREADER_IO_HH
