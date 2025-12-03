#include <iostream>
#include <string>
#include <filesystem>
#include <windows.h>
#include <sstream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <regex>

#include "DataFileReader.h"

std::string getExecutableDir()
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);

    std::filesystem::path exePath(buffer);
    return exePath.parent_path().string();
}

struct Range {
    long long start;
    long long end;
};

std::vector<Range> parseRanges(const std::string& input) {
    std::vector<Range> results;
    std::stringstream ss(input);
    std::string part;

    while (std::getline(ss, part, ',')) {
        size_t dashPos = part.find('-');
        if (dashPos != std::string::npos) {
            long long start = std::stoll(part.substr(0, dashPos));
            long long end   = std::stoll(part.substr(dashPos + 1));

            results.push_back({start, end});
        }
    }

    return results;
}

bool blocksMatch(const std::string& s, int blockSize) {
    int len = s.length();
    std::string pattern = s.substr(0, blockSize);

    for (int i = blockSize; i + blockSize <= len; i += blockSize) {
        if (s.substr(i, blockSize) != pattern) {
            return false;
        }
    }
    return true;
}

// Regex-based check for repeating blocks of size n
bool matchesPatternRegex(const std::string& s, int blockSize) {
    // Construct regex: ^(.{n})\1*$
    std::string pattern = "^(.{" + std::to_string(blockSize) + "})\\1*$";
    std::regex re(pattern);
    return std::regex_match(s, re);
}

long long extractInvalidIds(const std::vector<Range>& ranges) {
    long long cksum = 0;

    for (const Range& r : ranges) {
        for (long long num = r.start; num <= r.end; ++num) {
            std::string s = std::to_string(num);
            int len = s.length();
            int maxBlock = len / 2;
            bool matched = false;

            // Loop over block sizes according to even/odd rule
            for (int blockSize = 1; ; ) {
                int currentBlock = std::min(blockSize, maxBlock);

                if (matchesPatternRegex(s, currentBlock)) {
                    cksum += num;
                    std::cout << "  Match: " << s << " (cksum now " << cksum << ")\n";
                    matched = true;
                    break;
                }

                if (currentBlock == maxBlock) break;

                // Increment block size
                if (len % 2 == 0) {
                    blockSize *= 2;      // even
                } else {
                    blockSize = blockSize * 2 + 1; // odd
                }
            }
        }
    }

    return cksum;
}


int main() {
    auto start = std::chrono::high_resolution_clock::now();  // Start

    try {
        std::string filePath = getExecutableDir() + "\\data.txt";

        DataFileReader reader(filePath);
        std::string content = reader.readAll();

        //std::cout << "Loaded from: " << filePath << "\n\n";
        //std::cout << "File contents:\n" << content << std::endl;

        std::vector<Range> ranges = parseRanges(content);

        long long cksum = extractInvalidIds(ranges);
        std::cout << "\nFinal checksum: " << cksum << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();    // End
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "\nExecution time: " << elapsed.count() << " seconds\n";

    return 0;
}
