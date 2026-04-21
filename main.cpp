#include <iostream>
#include <string>
#include <cstring>
#include "md5.h"

void testMD5(const std::string& input, const std::string& expected) {
    std::string result = md5(input);
    std::cout << "Input: \"" << input << "\"" << std::endl;
    std::cout << "Result:  " << result << std::endl;
    std::cout << "Expected: " << expected << std::endl;
    std::cout << "Test: " << (result == expected ? "PASSED" : "FAILED") << std::endl;
    std::cout << "------------------------" << std::endl;
}

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [options]" << std::endl;
    std::cout << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -s, --string <text>  Calculate MD5 hash of a string" << std::endl;
    std::cout << "  -f, --file <path>    Calculate MD5 hash of a file" << std::endl;
    std::cout << "  -h, --help           Show this help message" << std::endl;
    std::cout << std::endl;
    std::cout << "Examples:" << std::endl;
    std::cout << "  " << programName << " -s \"Hello World\"" << std::endl;
    std::cout << "  " << programName << " -f \"C:\\test.txt\"" << std::endl;
    std::cout << std::endl;
    std::cout << "If no options provided, runs internal test suite." << std::endl;
}

void runTests() {
    std::cout << "MD5 Algorithm Test Suite" << std::endl;
    std::cout << "========================" << std::endl;
    std::cout << std::endl;

    testMD5("", "d41d8cd98f00b204e9800998ecf8427e");
    testMD5("a", "0cc175b9c0f1b6a831c399e269772661");
    testMD5("abc", "900150983cd24fb0d6963f7d28e17f72");
    testMD5("message digest", "f96b697d7cb7938d525a2f31aaf161d0");
    testMD5("abcdefghijklmnopqrstuvwxyz", "c3fcd3d76192e4007dfb496cca67e13b");
    testMD5("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", "d174ab98d277d9f5a5611c2c9f419d9f");
    testMD5("12345678901234567890123456789012345678901234567890123456789012345678901234567890", "57edf4a22be3c955ac49da2e2107b67a");

    std::cout << "All tests completed!" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        runTests();
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            printUsage(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--string") == 0) {
            if (i + 1 < argc) {
                std::string input(argv[i + 1]);
                std::cout << "MD5(\"" << input << "\") = " << md5(input) << std::endl;
                i++;
            } else {
                std::cerr << "Error: -s/--string requires an argument" << std::endl;
                return 1;
            }
        } else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--file") == 0) {
            if (i + 1 < argc) {
                std::string filepath(argv[i + 1]);
                std::string result;
                MD5Error error = md5_file(filepath, result);
                if (error != MD5_SUCCESS) {
                    std::cerr << "Error: " << md5_error_string(error) << ": " << filepath << std::endl;
                    return static_cast<int>(error);
                }
                std::cout << "MD5(\"" << filepath << "\") = " << result << std::endl;
                i++;
            } else {
                std::cerr << "Error: -f/--file requires a file path argument" << std::endl;
                return static_cast<int>(MD5_ERROR_INVALID_ARGUMENT);
            }
        } else {
            std::cerr << "Unknown option: " << argv[i] << std::endl;
            std::cerr << "Use -h or --help for usage information" << std::endl;
            return 1;
        }
    }

    return 0;
}
