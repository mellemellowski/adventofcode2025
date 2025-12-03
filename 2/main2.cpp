#include <iostream>
#include <string>
#include <filesystem>
#include <windows.h>
#include <sstream>
#include <vector>
#include <chrono>

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

long long extractInvalidIds(const std::vector<Range>& ranges) {

    long long cksum = 0;

    for (const Range& r : ranges) {

        int startDigits = std::to_string(r.start).length();
        int endDigits   = std::to_string(r.end).length();

        // Skip whole range if start & end have same odd-digit length
        if (startDigits == endDigits && (startDigits % 2 == 1)) {
            // std::cout << "Skipping entire range (odd-digit length): "
            //           << r.start << " -> " << r.end << "\n";
            continue;
        }

        //std::cout << "Processing range: " << r.start << " -> " << r.end << "\n";

        for (long long id = r.start; id <= r.end; ++id) {

            std::string s = std::to_string(id);
            int digits = s.length();

            // Skip odd-digit numbers
            if (digits % 2 == 1)
                continue;

            // Split into halves
            int half = digits / 2;
            std::string left  = s.substr(0, half);
            std::string right = s.substr(half, half);

            // Compare halves
            if (left == right) {
                cksum += id;
                //std::cout << "  Match: " << id << " (cksum now " << cksum << ")\n";
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
