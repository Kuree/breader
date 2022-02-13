#include <sstream>

#include "gtest/gtest.h"
#include "io.hh"

class IO : public ::testing::Test {
protected:
    void SetUp() override {
        o_ = std::make_unique<breader::ofile>(ss_);
        writer = std::make_unique<breader::writer>(o_->get_writer());
    }

    std::unique_ptr<breader::reader> reader() {
        i_ = std::make_unique<breader::ifile>(ss_);
        return std::make_unique<breader::reader>(i_->get_reader());
    }

    std::unique_ptr<breader::writer> writer;

private:
    std::stringstream ss_;
    std::unique_ptr<breader::ofile> o_;
    std::unique_ptr<breader::ifile> i_;
};

TEST_F(IO, uint8_t) {  // NOLINT
    writer->write<uint8_t>(42);
    auto value = reader()->read<uint8_t>();
    EXPECT_TRUE(value);
    EXPECT_EQ(*value, 42);
}

TEST_F(IO, uint16_t) {  // NOLINT
    writer->write<uint16_t>(4242);
    auto value = reader()->read<uint16_t>();
    EXPECT_TRUE(value);
    EXPECT_EQ(*value, 4242);
}

TEST_F(IO, uint32_t) {  // NOLINT
    writer->write<uint32_t>(0x424242);
    auto value = reader()->read<uint32_t>();
    EXPECT_TRUE(value);
    EXPECT_EQ(*value, 0x424242);
}

TEST_F(IO, uint64_t) {  // NOLINT
    writer->write<uint64_t>(0x42424445464748);
    auto value = reader()->read<uint64_t>();
    EXPECT_TRUE(value);
    EXPECT_EQ(*value, 0x42424445464748);
}

TEST_F(IO, float) {  // NOLINT
    writer->write<float>(42.0);
    auto value = reader()->read<float>();
    EXPECT_TRUE(value);
    EXPECT_EQ(*value, 42.0);
}

TEST_F(IO, double) {  // NOLINT
    writer->write<double>(42.0);
    auto value = reader()->read<double>();
    EXPECT_TRUE(value);
    EXPECT_EQ(*value, 42.0);
}