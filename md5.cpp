#include "md5.h"
#include <cstring>
#include <cerrno>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <sstream>

const uint8_t MD5_PADDING[64] = {
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const uint32_t MD5_S[64] = {
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
    5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

const uint32_t MD5_K[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

MD5::MD5() : finalized(false) {
    a = 0x67452301;
    b = 0xefcdab89;
    c = 0x98badcfe;
    d = 0x10325476;
    count[0] = count[1] = 0;
    memset(buffer, 0, sizeof(buffer));
}

void MD5::update(const std::string& input) {
    update(reinterpret_cast<const uint8_t*>(input.c_str()), input.length());
}

void MD5::update(const uint8_t* data, size_t length) {
    if (finalized || length == 0) {
        return;
    }

    uint32_t inputLen = static_cast<uint32_t>(length);
    uint32_t index = (count[0] >> 3) & 0x3F;

    if ((count[0] += (inputLen << 3)) < (inputLen << 3)) {
        count[1]++;
    }
    count[1] += (inputLen >> 29);

    uint32_t partLen = 64 - index;
    uint32_t i = 0;

    if (inputLen >= partLen) {
        memcpy(&buffer[index], data, partLen);
        transform(buffer);

        for (i = partLen; i + 63 < inputLen; i += 64) {
            transform(data + i);
        }

        index = 0;
    }

    memcpy(&buffer[index], data + i, inputLen - i);
}

std::string MD5::finalize() {
    if (finalized) {
        return "";
    }

    uint8_t bits[8];
    uint32_t index, padLen;

    encode(count, bits, 8);

    index = (count[0] >> 3) & 0x3F;
    padLen = (index < 56) ? (56 - index) : (120 - index);
    update(MD5_PADDING, static_cast<size_t>(padLen));

    update(bits, 8);

    uint32_t digest[4] = {a, b, c, d};
    uint8_t result[16];
    encode(digest, result, 16);

    finalized = true;

    char hex[33];
    for (int i = 0; i < 16; i++) {
        snprintf(hex + i * 2, 3, "%02x", result[i]);
    }
    hex[32] = '\0';

    return std::string(hex);
}

void MD5::transform(const uint8_t block[64]) {
    uint32_t a_save = a, b_save = b, c_save = c, d_save = d;
    uint32_t m[16];

    decode(block, m, 64);

    for (uint32_t i = 0; i < 64; i++) {
        uint32_t f, g;

        if (i < 16) {
            f = (b & c) | ((~b) & d);
            g = i;
        } else if (i < 32) {
            f = (d & b) | ((~d) & c);
            g = (5 * i + 1) % 16;
        } else if (i < 48) {
            f = b ^ c ^ d;
            g = (3 * i + 5) % 16;
        } else {
            f = c ^ (b | (~d));
            g = (7 * i) % 16;
        }

        f = f + a + MD5_K[i] + m[g];
        a = d;
        d = c;
        c = b;
        b = b + ((f << MD5_S[i]) | (f >> (32 - MD5_S[i])));
    }

    a += a_save;
    b += b_save;
    c += c_save;
    d += d_save;
}

void MD5::encode(const uint32_t* input, uint8_t* output, size_t length) {
    for (size_t i = 0, j = 0; j < length; i++, j += 4) {
        output[j] = static_cast<uint8_t>(input[i] & 0xff);
        output[j + 1] = static_cast<uint8_t>((input[i] >> 8) & 0xff);
        output[j + 2] = static_cast<uint8_t>((input[i] >> 16) & 0xff);
        output[j + 3] = static_cast<uint8_t>((input[i] >> 24) & 0xff);
    }
}

void MD5::decode(const uint8_t* input, uint32_t* output, size_t length) {
    for (size_t i = 0, j = 0; j < length; i++, j += 4) {
        output[i] = static_cast<uint32_t>(input[j]) |
                   (static_cast<uint32_t>(input[j + 1]) << 8) |
                   (static_cast<uint32_t>(input[j + 2]) << 16) |
                   (static_cast<uint32_t>(input[j + 3]) << 24);
    }
}

std::string md5(const std::string& input) {
    MD5 md5;
    md5.update(input);
    return md5.finalize();
}

const char* md5_error_string(MD5Error error) {
    switch (error) {
        case MD5_SUCCESS:
            return "Success";
        case MD5_ERROR_FILE_NOT_FOUND:
            return "File not found";
        case MD5_ERROR_PERMISSION_DENIED:
            return "Permission denied";
        case MD5_ERROR_READ_FAILED:
            return "Read operation failed";
        case MD5_ERROR_INVALID_ARGUMENT:
            return "Invalid argument";
        case MD5_ERROR_UNKNOWN:
        default:
            return "Unknown error";
    }
}

MD5Error md5_file(const std::string& filepath, std::string& out_hash) {
    out_hash.clear();

    if (filepath.empty()) {
        return MD5_ERROR_INVALID_ARGUMENT;
    }

    FILE* file = nullptr;
    errno = 0;

#ifdef _WIN32
    if (fopen_s(&file, filepath.c_str(), "rb") != 0 || file == nullptr) {
        int errorCode = errno;
#else
    file = fopen(filepath.c_str(), "rb");
    if (file == nullptr) {
        int errorCode = errno;
#endif
        switch (errorCode) {
            case ENOENT:
                return MD5_ERROR_FILE_NOT_FOUND;
            case EACCES:
            case EPERM:
                return MD5_ERROR_PERMISSION_DENIED;
            default:
                return MD5_ERROR_FILE_NOT_FOUND;
        }
    }

    MD5 md5;
    const size_t bufferSize = 8192;
    uint8_t buffer[bufferSize];
    size_t bytesRead;
    bool readError = false;

    while ((bytesRead = fread(buffer, 1, bufferSize, file)) > 0) {
        md5.update(buffer, bytesRead);
    }

    if (ferror(file)) {
        readError = true;
    }

    fclose(file);

    if (readError) {
        return MD5_ERROR_READ_FAILED;
    }

    out_hash = md5.finalize();
    return MD5_SUCCESS;
}

std::string md5_file(const std::string& filepath) {
    std::string hash;
    MD5Error error = md5_file(filepath, hash);
    if (error == MD5_SUCCESS) {
        return hash;
    }
    return "";
}
