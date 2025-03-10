#include <intrin.h>
#include <iostream>
#include <map>

auto checkCPUForFunctionality() {

    std::map<std::string, bool> capabilities{
        //  Misc.
        {"HW_MMX",false},
        {"HW_x64",false},
        {"HW_ABM",false},      // Advanced Bit Manipulation
        {"HW_RDRAND",false},
        {"HW_BMI1",false},
        {"HW_BMI2",false},
        {"HW_ADX",false},
        {"HW_PREFETCHWT1",false},
        //  SIMD: 128-bit
        {"HW_SSE",false},
        {"HW_SSE2",false},
        {"HW_SSE3",false},
        {"HW_SSSE3",false},
        {"HW_SSE41",false},
        {"HW_SSE42",false},
        {"HW_SSE4a",false},
        {"HW_AES",false},
        {"HW_SHA",false},
        //  SIMD: 256-bit
        {"HW_AVX",false},
        {"HW_XOP",false},
        {"HW_FMA3",false},
        {"HW_FMA4",false},
        {"HW_AVX2",false},
        //  SIMD: 512-bit
        {"HW_AVX512F",false},    //  AVX512 Foundation
        {"HW_AVX512CD",false},   //  AVX512 Conflict Detection
        {"HW_AVX512PF",false},   //  AVX512 Prefetch
        {"HW_AVX512ER",false},   //  AVX512 Exponential + Reciprocal
        {"HW_AVX512VL",false},   //  AVX512 Vector Length Extensions
        {"HW_AVX512BW",false},   //  AVX512 Byte + Word
        {"HW_AVX512DQ",false},   //  AVX512 Doubleword + Quadword
        {"HW_AVX512IFMA",false}, //  AVX512 Integer 52-bit Fused Multiply-Add
        {"HW_AVX512VBMI",false}, //  AVX512 Vector Byte Manipulation Instructions
        // AVX10 support
        {"HW_AVX10", false},
    };

    int info[4];
    __cpuid(info, 0);
    int nIds = info[0];
    __cpuid(info, 0x80000000);
    unsigned nExIds = info[0];

    //  Detect Features
    if (nIds >= 0x00000001) {
        __cpuid(info, 0x00000001);
        capabilities["HW_MMX"] = (info[3] & ((int)1 << 23)) != 0;
        capabilities["HW_SSE"] = (info[3] & ((int)1 << 25)) != 0;
        capabilities["HW_SSE2"] = (info[3] & ((int)1 << 26)) != 0;
        capabilities["HW_SSE3"] = (info[2] & ((int)1 << 0)) != 0;
        capabilities["HW_SSSE3"] = (info[2] & ((int)1 << 9)) != 0;
        capabilities["HW_SSE41"] = (info[2] & ((int)1 << 19)) != 0;
        capabilities["HW_SSE42"] = (info[2] & ((int)1 << 20)) != 0;
        capabilities["HW_AES"] = (info[2] & ((int)1 << 25)) != 0;
        capabilities["HW_AVX"] = (info[2] & ((int)1 << 28)) != 0;
        capabilities["HW_FMA3"] = (info[2] & ((int)1 << 12)) != 0;
        capabilities["HW_RDRAND"] = (info[2] & ((int)1 << 30)) != 0;
    }
    if (nIds >= 0x00000007) {
        __cpuid(info, 0x00000007);
        capabilities["HW_AVX2"] = (info[1] & ((int)1 << 5)) != 0;
        capabilities["HW_BMI1"] = (info[1] & ((int)1 << 3)) != 0;
        capabilities["HW_BMI2"] = (info[1] & ((int)1 << 8)) != 0;
        capabilities["HW_ADX"] = (info[1] & ((int)1 << 19)) != 0;
        capabilities["HW_SHA"] = (info[1] & ((int)1 << 29)) != 0;
        capabilities["HW_PREFETCHWT1"] = (info[2] & ((int)1 << 0)) != 0;
        capabilities["HW_AVX512F"] = (info[1] & ((int)1 << 16)) != 0;
        capabilities["HW_AVX512CD"] = (info[1] & ((int)1 << 28)) != 0;
        capabilities["HW_AVX512PF"] = (info[1] & ((int)1 << 26)) != 0;
        capabilities["HW_AVX512ER"] = (info[1] & ((int)1 << 27)) != 0;
        capabilities["HW_AVX512VL"] = (info[1] & ((int)1 << 31)) != 0;
        capabilities["HW_AVX512BW"] = (info[1] & ((int)1 << 30)) != 0;
        capabilities["HW_AVX512DQ"] = (info[1] & ((int)1 << 17)) != 0;
        capabilities["HW_AVX512IFMA"] = (info[1] & ((int)1 << 21)) != 0;
        capabilities["HW_AVX512VBMI"] = (info[2] & ((int)1 << 1)) != 0;
    }
    if (nExIds >= 0x80000001) {
        __cpuid(info, 0x80000001);
        capabilities["HW_x64"] = (info[3] & ((int)1 << 29)) != 0;
        capabilities["HW_ABM"] = (info[2] & ((int)1 << 5)) != 0;
        capabilities["HW_SSE4a"] = (info[2] & ((int)1 << 6)) != 0;
        capabilities["HW_FMA4"] = (info[2] & ((int)1 << 16)) != 0;
        capabilities["HW_XOP"] = (info[2] & ((int)1 << 11)) != 0;
    }

    memset(info, 0, sizeof(int) * 4);
    __cpuidex(info, 7, 1);
    capabilities["HW_AVX10"] = (info[3] & (1 << 19)) != 0;

    return capabilities;
}

void checkForAVX() {
    bool HW_AVX = false, HW_AVX2 = false, HW_AVX512 = false;
    int info[4];

    __cpuid(info, 0);
    int nIds = info[0];
    //  Detect Features
    if (nIds >= 0x00000001) {
        __cpuid(info, 0x00000001);
        HW_AVX = (info[2] & ((int)1 << 28)) != 0;
    }
    if (nIds >= 0x00000007) {
        __cpuid(info, 0x00000007);
        HW_AVX2 = (info[1] & ((int)1 << 5)) != 0;
    }
    if (nIds >= 0x00000007) {
        __cpuid(info, 0x00000007);
        HW_AVX512 = (info[1] & ((int)1 << 16)) != 0;
    }

    std::cout << "AVX: " << (HW_AVX ? "supported" : "AV What...?") << std::endl;
    std::cout << "AVX2: " << (HW_AVX2 ? "supported" : "Audio Video Xtreme 2 - WHAT?") << std::endl;
    std::cout << "AVX512: " << (HW_AVX512 ? "supported" : "No - Linus said this is stupid anyway") << std::endl;
}

void checkForAVX10()
{
    int info[4] = { 0 };

    memset(info, 0, sizeof(int) * 4);
    __cpuidex(info, 7, 1);
    if ((info[3] & (1 << 19)) == 0) {
        std::cout << "AVX10: unsupported" << std::endl;
        return;
    }

    memset(info, 0, sizeof(int) * 4);
    __cpuidex(info, 0x24, 0x00);

    int avx10Ver = static_cast<uint8_t>(info[1] & 0xFF);
    std::cout << "AVX10: supported in version " << avx10Ver << std::endl;

    std::cout << "AVX10: 128bit vector support = " << ((info[1] & (1 << 16)) != 0) << std::endl;
    std::cout << "AVX10: 256bit vector support = " << ((info[1] & (1 << 17)) != 0) << std::endl;
    std::cout << "AVX10: 512bit vector support = " << ((info[1] & (1 << 18)) != 0) << std::endl;
}

int main() {

    // short check 
    checkForAVX();
    checkForAVX10();

    // check enabled
    std::cout << std::endl << "All CPU Capabilities " << std::endl;
    auto allcaps = checkCPUForFunctionality();
    std::cout << std::endl << "enabled caps:" << std::endl;
    for (auto c : allcaps) {
        if (c.second)
            std::cout << c.first << " ";
    }

    // check disabled
    std::cout << std::endl << std::endl << "disabled caps:" << std::endl;
    for (auto c : allcaps) {
        if (!c.second)
            std::cout << c.first << " ";
    }
    std::cout << std::endl;

}
