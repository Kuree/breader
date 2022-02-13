#ifndef BREADER_TEST_UTIL_HH
#define BREADER_TEST_UTIL_HH

#include <filesystem>

// this will introduce a race condition, but for normal purpose it's good enough
class temp_directory {
public:
    temp_directory() {
        namespace fs = std::filesystem;
        auto tmp = fs::temp_directory_path();
        uint64_t count = 0;
        bool success = false;
        do {
            filename_ = tmp / ("breader" + std::to_string(count));
            if (!fs::exists(filename_)) {
                success = true;
            }
        } while (!success);
    }

    [[nodiscard]] std::string str() const { return filename_; }

    ~temp_directory() {
        namespace fs = std::filesystem;
        if (fs::exists(filename_)) {
            fs::remove(filename_);
        }
    }

private:
    std::string filename_;
};

#endif  // BREADER_TEST_UTIL_HH
