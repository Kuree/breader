#ifndef BREADER_IO_HH
#define BREADER_IO_HH
#include <memory>

#include "mio/mmap.hpp"
#include "reader.hh"
#include "writer.hh"

namespace breader {

class ofile {
public:
    explicit ofile(const std::string &filename);
    std::ostream &stream();
    [[nodiscard]] bool bad() const;

private:
    std::ostream stream_;
};

class ifile {
public:
    explicit ifile(const std::string &filename);
    [[nodiscard]] bool bad() const { return bad_; }
    [[nodiscard]] reader get_reader(std::endian endian = std::endian::little) const;

private:
    bool bad_ = false;
    mio::mmap_source source_;
};

}  // namespace breader

#endif  // BREADER_IO_HH
