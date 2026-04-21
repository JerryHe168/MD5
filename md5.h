#ifndef MD5_H
#define MD5_H

#include <string>
#include <cstdint>

enum MD5Error {
    MD5_SUCCESS = 0,
    MD5_ERROR_FILE_NOT_FOUND = 1,
    MD5_ERROR_PERMISSION_DENIED = 2,
    MD5_ERROR_READ_FAILED = 3,
    MD5_ERROR_INVALID_ARGUMENT = 4,
    MD5_ERROR_UNKNOWN = 99
};

const char* md5_error_string(MD5Error error);

class MD5 {
public:
    MD5();
    void update(const std::string& input);
    void update(const uint8_t* data, size_t length);
    std::string finalize();

private:
    uint32_t a, b, c, d;
    uint32_t count[2];
    uint8_t buffer[64];
    bool finalized;

    void transform(const uint8_t block[64]);
    static void encode(const uint32_t* input, uint8_t* output, size_t length);
    static void decode(const uint8_t* input, uint32_t* output, size_t length);
};

std::string md5(const std::string& input);
std::string md5_file(const std::string& filepath);
MD5Error md5_file(const std::string& filepath, std::string& out_hash);

#endif // MD5_H
