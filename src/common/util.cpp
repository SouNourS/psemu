#include <chrono>
#include <random>
#include <string>
#include <thread>
#include "bitstream.h"
#include "util.h"

uint32_t randomUnsignedInt() {
    std::random_device randomDevice;
    std::mt19937_64 generator(randomDevice());
    std::uniform_int_distribution<uint32_t> distribution;

    return distribution(generator);
}

uint8_t randomUnsignedChar() {
    std::random_device randomDevice;
    std::mt19937_64 generator(randomDevice());
    std::uniform_int_distribution<uint32_t> distribution(0, UCHAR_MAX);

    return distribution(generator);
}

std::size_t getTimeSeconds() {
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::chrono::system_clock::duration tp = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::seconds>(tp).count();
}

std::size_t getTimeMilliseconds() {
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::chrono::system_clock::duration tp = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(tp).count();
}

std::size_t getTimeNanoseconds() {
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::chrono::system_clock::duration tp = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(tp).count();
}

void utilSleep(size_t ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

int hexCharToInt(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'A' && c <= 'F') {
        return 10 + c - 'A';
    } else if (c >= 'a' && c <= 'f') {
        return 10 + c - 'a';
    }

    return -1;
}

std::vector<uint8_t> hexToBytes(std::string hexStr) {
    std::vector<uint8_t> bytes;

    uint8_t curValue = 0;
    bool firstDigitFilled = false;
    for (auto c : hexStr) {
        int charValue = hexCharToInt(c);

        if (charValue != -1) {
            if (!firstDigitFilled) {
                curValue = 16 * charValue;

                firstDigitFilled = true;
            } else {
                bytes.push_back(curValue + charValue);

                firstDigitFilled = false;
                curValue = 0;
            }
        }
    }

    return bytes;
}
