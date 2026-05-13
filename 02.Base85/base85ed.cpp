#include "base85ed.h"
#include <cstdint>
#include <vector>
#include <stdexcept>
#include <array>

namespace base85
{

static constexpr char ALPHABET[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "!#$%&()*+-;<=>?@^_`{|}~";

static constexpr size_t ALPH_LEN = sizeof(ALPHABET) - 1; // 85

static const std::array<int8_t, 128> build_decode_table()
{
    std::array<int8_t, 128> table;
    table.fill(-1);
    for (int i = 0; i < static_cast<int>(ALPH_LEN); ++i)
    {
        unsigned char c = static_cast<unsigned char>(ALPHABET[i]);
        if (c < 128)
        {
            table[c] = static_cast<int8_t>(i);
        }
    }
    return table;
}

static const std::array<int8_t, 128> DECODE_TABLE = build_decode_table();

static uint32_t pack4(const uint8_t* p)
{
    return (static_cast<uint32_t>(p[0]) << 24) |
           (static_cast<uint32_t>(p[1]) << 16) |
           (static_cast<uint32_t>(p[2]) << 8)  |
           static_cast<uint32_t>(p[3]);
}

static void unpack4(uint32_t v, uint8_t* out)
{
    out[0] = static_cast<uint8_t>((v >> 24) & 0xFF);
    out[1] = static_cast<uint8_t>((v >> 16) & 0xFF);
    out[2] = static_cast<uint8_t>((v >> 8)  & 0xFF);
    out[3] = static_cast<uint8_t>(v         & 0xFF);
}

std::vector<uint8_t> encode(const std::vector<uint8_t>& bytes)
{
    std::vector<uint8_t> result;
    const size_t len = bytes.size();
    size_t pos = 0;

    // Полные блоки по 4 байта
    while (pos + 4 <= len)
    {
        uint32_t v = pack4(&bytes[pos]);
        pos += 4;

        char out[5];
        out[0] = ALPHABET[(v / 52200625UL) % 85];
        out[1] = ALPHABET[(v / 614125UL)   % 85];
        out[2] = ALPHABET[(v / 7225UL)     % 85];
        out[3] = ALPHABET[(v / 85UL)       % 85];
        out[4] = ALPHABET[v % 85];
        result.insert(result.end(), reinterpret_cast<uint8_t*>(out),
                      reinterpret_cast<uint8_t*>(out + 5));
    }

    // Остаток
    size_t rem = len - pos;
    if (rem > 0)
    {
        uint8_t padded[4] = {0, 0, 0, 0};
        for (size_t i = 0; i < rem; ++i)
        {
            padded[i] = bytes[pos + i];
        }
        uint32_t v = pack4(padded);

        char out[5];
        out[0] = ALPHABET[(v / 52200625UL) % 85];
        out[1] = ALPHABET[(v / 614125UL)   % 85];
        out[2] = ALPHABET[(v / 7225UL)     % 85];
        out[3] = ALPHABET[(v / 85UL)       % 85];
        out[4] = ALPHABET[v % 85];

        // Выдаём rem+1 символов
        result.insert(result.end(), reinterpret_cast<uint8_t*>(out),
                      reinterpret_cast<uint8_t*>(out + rem + 1));
    }

    return result;
}

std::vector<uint8_t> decode(const std::vector<uint8_t>& b85str)
{
    std::vector<uint8_t> result;
    size_t len = b85str.size();
    if (len == 0)
    {
        return result;
    }

    size_t pos = 0;
    // Обрабатываем полные группы по 5 символов
    while (pos + 5 <= len)
    {
        uint32_t v = 0;
        for (int i = 0; i < 5; ++i)
        {
            uint8_t c = b85str[pos++];
            if (c >= 128 || DECODE_TABLE[c] < 0)
            {
                throw std::runtime_error("Invalid character in base85 string");
            }
            v = v * 85 + static_cast<uint32_t>(DECODE_TABLE[c]);
        }
        uint8_t block[4];
        unpack4(v, block);
        result.insert(result.end(), block, block + 4);
    }

    // Оставшаяся неполная группа (2, 3 или 4 символа)
    size_t rem = len - pos;
    if (rem == 0)
    {
        return result;
    }

    if (rem == 1)
    {
        throw std::runtime_error("Invalid base85 final group length (1 char)");
    }

    // Дополняем '~' до 5 символов
    uint32_t v = 0;
    for (size_t i = 0; i < rem; ++i)
    {
        uint8_t c = b85str[pos++];
        if (c >= 128 || DECODE_TABLE[c] < 0)
        {
            throw std::runtime_error("Invalid character in base85 string");
        }
        v = v * 85 + static_cast<uint32_t>(DECODE_TABLE[c]);
    }
    size_t pad_needed = 5 - rem;
    for (size_t i = 0; i < pad_needed; ++i)
    {
        v = v * 85 + 84;   // 84 соответствует '~'
    }

    uint8_t block[4];
    unpack4(v, block);
    // Возвращаем первые (rem - 1) байт
    for (size_t i = 0; i < rem - 1; ++i)
    {
        result.push_back(block[i]);
    }

    return result;
}

} // namespace base85