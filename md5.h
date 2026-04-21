#ifndef MD5_H
#define MD5_H

#include <string>
#include <cstdint>

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

#endif // MD5_H
