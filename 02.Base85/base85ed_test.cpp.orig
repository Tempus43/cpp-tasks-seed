#include <gtest/gtest.h>
#include <vector>
#include <cstdint>
#include <random>
#include <stdexcept>

#include "base85ed.h"

// Удобная обёртка
static std::vector<uint8_t> str2vec(const std::string& s)
{
    return std::vector<uint8_t>(s.begin(), s.end());
}

// Эталонные короткие пары из исходного задания
const std::vector<std::pair<const char*, const char*>> short_cases =
{
    {"",     ""},
    {"F#",   "1"},
    {"F){",  "12"},
    {"F)}j", "123"},
    {"F)}kW","1234"}
};

// ------------------- encode -------------------
TEST(Base85ShortsEncode, TrivialShortEncodes)
{
    for (const auto& p : short_cases)
    {
        EXPECT_EQ(base85::encode(str2vec(p.second)), str2vec(p.first));
    }
}

TEST(Base85Encode, Empty)
{
    EXPECT_EQ(base85::encode({}), std::vector<uint8_t>{});
}

// ------------------- decode -------------------
TEST(Base85ShortsDecode, TrivialShortDecodes)
{
    for (const auto& p : short_cases)
    {
        EXPECT_EQ(base85::decode(str2vec(p.first)), str2vec(p.second));
    }
}

TEST(Base85Decode, Empty)
{
    EXPECT_EQ(base85::decode({}), std::vector<uint8_t>{});
}

TEST(Base85Decode, InvalidChar)
{
    // Символ вне алфавита (например, пробел)
    EXPECT_THROW(base85::decode(str2vec("F# ")), std::runtime_error);
}

TEST(Base85Decode, InvalidFinalLen1)
{
    // Одиночный символ не может быть корректным концом
    EXPECT_THROW(base85::decode(str2vec("F")), std::runtime_error);
}

// ------------------- Round‑trip -------------------
TEST(Base85RoundTrip, RandomData)
{
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> byte_dist(0, 255);
    std::uniform_int_distribution<size_t> len_dist(0, 1000);

    for (int i = 0; i < 200; ++i)
    {
        size_t len = len_dist(rng);
        std::vector<uint8_t> original(len);
        for (auto& b : original)
        {
            b = static_cast<uint8_t>(byte_dist(rng));
        }

        auto encoded = base85::encode(original);
        auto decoded = base85::decode(encoded);
        EXPECT_EQ(decoded, original) << "Failed at length " << len;
    }
}

TEST(Base85RoundTrip, AllSingleBytes)
{
    for (int b = 0; b < 256; ++b)
    {
        std::vector<uint8_t> original = {static_cast<uint8_t>(b)};
        auto encoded = base85::encode(original);
        ASSERT_EQ(encoded.size(), 2);   // всегда 2 символа
        auto decoded = base85::decode(encoded);
        EXPECT_EQ(decoded, original);
    }
}

TEST(Base85RoundTrip, BoundaryLengths)
{
    for (size_t len : {0, 1, 2, 3, 4, 5, 7, 8, 9, 15, 16, 17, 127, 128, 129})
    {
        std::vector<uint8_t> original(len);
        for (size_t i = 0; i < len; ++i)
        {
            original[i] = static_cast<uint8_t>(i % 251);
        }
        auto decoded = base85::decode(base85::encode(original));
        EXPECT_EQ(decoded, original);
    }
}

TEST(Base85Decode, FinalGroup)
{
    auto decoded2 = base85::decode(str2vec("F#"));
    EXPECT_EQ(decoded2, str2vec("1"));

    auto decoded3 = base85::decode(str2vec("F){"));
    EXPECT_EQ(decoded3, str2vec("12"));

    auto decoded4 = base85::decode(str2vec("F)}j"));
    EXPECT_EQ(decoded4, str2vec("123"));
}