#include <sstream>

#include "gtest/gtest.h"
#include "io.hh"
#include "util.hh"

class IO_LE : public ::testing::Test {
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

class IO_BE : public ::testing::Test {
protected:
    void SetUp() override {
        o_ = std::make_unique<breader::ofile>(ss_);
        writer = std::make_unique<breader::writer>(o_->get_writer(std::endian::big));
    }

    std::unique_ptr<breader::reader> reader() {
        i_ = std::make_unique<breader::ifile>(ss_);
        return std::make_unique<breader::reader>(i_->get_reader(std::endian::big));
    }

    std::unique_ptr<breader::writer> writer;

private:
    std::stringstream ss_;
    std::unique_ptr<breader::ofile> o_;
    std::unique_ptr<breader::ifile> i_;
};

TEST_F(IO_LE, uint8_t) {  // NOLINT
    writer->write<uint8_t>(42);
    auto value = reader()->read<uint8_t>();
    EXPECT_TRUE(value);
    EXPECT_EQ(*value, 42);
}

TEST_F(IO_LE, uint16_t) {  // NOLINT
    writer->write<uint16_t>(4242);
    auto value = reader()->read<uint16_t>();
    EXPECT_TRUE(value);
    EXPECT_EQ(*value, 4242);
}

TEST_F(IO_LE, uint32_t) {  // NOLINT
    writer->write<uint32_t>(0x424242);
    auto value = reader()->read<uint32_t>();
    EXPECT_TRUE(value);
    EXPECT_EQ(*value, 0x424242);
}

TEST_F(IO_LE, uint64_t) {  // NOLINT
    writer->write<uint64_t>(0x42424445464748);
    auto value = reader()->read<uint64_t>();
    EXPECT_TRUE(value);
    EXPECT_EQ(*value, 0x42424445464748);
}

TEST_F(IO_LE, float) {  // NOLINT
    writer->write<float>(42.0);
    auto value = reader()->read<float>();
    EXPECT_TRUE(value);
    EXPECT_EQ(*value, 42.0);
}

TEST_F(IO_LE, double) {  // NOLINT
    writer->write<double>(42.0);
    auto value = reader()->read<double>();
    EXPECT_TRUE(value);
    EXPECT_EQ(*value, 42.0);
}

TEST(IO, big_endian) {  // NOLINT
    // test it by hand to make sure it's actually swapped
    std::stringstream ss;
    auto writer = breader::writer(ss, std::endian::big);
    writer.write<uint64_t>(0x4243444546474849);
    auto data = ss.str();
    auto reader = breader::reader(data, std::endian::little);
    auto value = reader.read<uint64_t>();
    EXPECT_TRUE(value);
    EXPECT_EQ(*value, 0x4948474645444342);
}

TEST_F(IO_BE, uint8_t) {  // NOLINT
    writer->write<uint8_t>(42);
    auto value = reader()->read<uint8_t>();
    EXPECT_TRUE(value);
    EXPECT_EQ(*value, 42);
}

TEST_F(IO_BE, uint16_t) {  // NOLINT
    writer->write<uint16_t>(4242);
    auto value = reader()->read<uint16_t>();
    EXPECT_TRUE(value);
    EXPECT_EQ(*value, 4242);
}

TEST_F(IO_BE, uint32_t) {  // NOLINT
    writer->write<uint32_t>(0x424242);
    auto value = reader()->read<uint32_t>();
    EXPECT_TRUE(value);
    EXPECT_EQ(*value, 0x424242);
}

TEST_F(IO_BE, uint64_t) {  // NOLINT
    writer->write<uint64_t>(0x42424445464748);
    auto value = reader()->read<uint64_t>();
    EXPECT_TRUE(value);
    EXPECT_EQ(*value, 0x42424445464748);
}

TEST_F(IO_BE, float) {  // NOLINT
    writer->write<float>(42.0);
    auto value = reader()->read<float>();
    EXPECT_TRUE(value);
    EXPECT_EQ(*value, 42.0);
}

TEST_F(IO_BE, double) {  // NOLINT
    writer->write<double>(42.0);
    auto value = reader()->read<double>();
    EXPECT_TRUE(value);
    EXPECT_EQ(*value, 42.0);
}

TEST_F(IO_LE, uint8_t_array) {  // NOLINT
    std::vector<uint8_t> array = {0x41, 0x42, 0x43, 0x44};
    writer->write(array);
    auto value = reader()->read<uint8_t>(array.size());
    EXPECT_TRUE(value);
    for (auto i = 0u; i < array.size(); i++) {
        EXPECT_EQ(array[i], (*value)[i]);
    }
}

TEST_F(IO_LE, string) {  // NOLINT
    std::string v = "42";
    writer->write(v);
    auto value = reader()->read<std::string>(v.size());
    EXPECT_TRUE(value);
    EXPECT_EQ(*value, v);
}

TEST(IO, file) {  // NOLINT
    auto temp = temp_directory();
    {
        breader::ofile o(temp.str());
        auto w = o.get_writer();
        w.write<uint64_t>(42);
    }
    {
        breader::ifile i(temp.str());
        auto r = i.get_reader();
        auto value = r.read<uint64_t>();
        EXPECT_TRUE(value);
        EXPECT_EQ(*value, 42);
    }
}