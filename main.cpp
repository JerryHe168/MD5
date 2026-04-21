#include <iostream>
#include <string>
#include "md5.h"

void testMD5(const std::string& input, const std::string& expected) {
    std::string result = md5(input);
    std::cout << "Input: \"" << input << "\"" << std::endl;
    std::cout << "Result:  " << result << std::endl;
    std::cout << "Expected: " << expected << std::endl;
    std::cout << "Test: " << (result == expected ? "PASSED" : "FAILED") << std::endl;
    std::cout << "------------------------" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        std::string input(argv[1]);
        std::cout << "MD5(\"" << input << "\") = " << md5(input) << std::endl;
        return 0;
    }

    std::cout << "MD5 Algorithm Test Suite" << std::endl;
    std::cout << "========================" << std::endl;
    std::cout << std::endl;

    testMD5("", "d41d8cd98f00b204e9800998ecf8427e");
    testMD5("a", "0cc175b9c0f1b6a831c399e269772661");
    testMD5("abc", "900150983cd24fb0d6963f7d28e17f72");
    testMD5("message digest", "f96b697d7cb7938d525a2f31aaf161d");
    testMD5("abcdefghijklmnopqrstuvwxyz", "c3fcd3d76192e4007dfb496cca67e13b");
    testMD5("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", "d174ab98d277d9f5a5611c2c9f419d9f");
    testMD5("12345678901234567890123456789012345678901234567890123456789012345678901234567890", "57edf4a22be3c955ac49da2e2107b67a");

    std::cout << "All tests completed!" << std::endl;
    std::cout << std::endl;
    std::cout << "Usage: md5.exe <string>" << std::endl;
    std::cout << "Example: md5.exe \"Hello World\"" << std::endl;

    return 0;
}
