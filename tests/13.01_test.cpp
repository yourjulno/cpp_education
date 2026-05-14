#include <gtest/gtest.h>
#include "2nd_semester/13.01.hpp"
#include <vector>
#include <random>

TEST(BytesToHexTest, ConvertsRegularSequence)
{
    const std::vector<std::uint8_t> bytes{0xBA, 0xAD, 0xF0, 0x0D};
    EXPECT_EQ(bytes_to_hex(bytes), "baadf00d");
}

TEST(BytesToHexTest, PreservesLeadingZeros)
{
    const std::vector<std::uint8_t> bytes{0x00, 0x01, 0x0A, 0x10, 0xFF};
    EXPECT_EQ(bytes_to_hex(bytes), "00010a10ff");
}

TEST(BytesToHexTest, ConvertsEmptyVector)
{
    const std::vector<std::uint8_t> bytes;
    EXPECT_EQ(bytes_to_hex(bytes), "");
}

TEST(HexToBytesTest, ConvertsRegularSequence)
{
    const std::vector<std::uint8_t> expected{0xBA, 0xAD, 0xF0, 0x0D};
    EXPECT_EQ(hex_to_bytes("baadf00d"), expected);
}

TEST(HexToBytesTest, ConvertsSequenceWithLeadingZeros)
{
    const std::vector<std::uint8_t> expected{0x00, 0x01, 0x0A, 0x10, 0xFF};
    EXPECT_EQ(hex_to_bytes("00010a10ff"), expected);
}

TEST(HexToBytesTest, ConvertsEmptyString)
{
    const std::vector<std::uint8_t> expected;
    EXPECT_EQ(hex_to_bytes(""), expected);
}

TEST(HexToBytesTest, ThrowsOnOddLength)
{
    EXPECT_THROW(hex_to_bytes("abc"), std::invalid_argument);
}

TEST(HexToBytesTest, ThrowsOnInvalidCharacter)
{
    EXPECT_THROW(hex_to_bytes("0g"), std::invalid_argument);
}

TEST(HexToBytesTest, ThrowsOnUppercaseCharacter)
{
    EXPECT_THROW(hex_to_bytes("AF"), std::invalid_argument);
}

TEST(RoundTripTest, BytesToHexToBytes)
{
    const std::vector<std::uint8_t> original{0xDE, 0xAD, 0xBE, 0xEF, 0x00, 0x01};
    EXPECT_EQ(hex_to_bytes(bytes_to_hex(original)), original);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}