#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "types.h"

#include <string>

#define DEFAULT_CACHE_SIZE    4 * 1024 // 4 KiB
#define DEFAULT_ASSOCIATIVITY 2
#define DEFAULT_BLOCK_SIZE    32 // 32 bytes

using std::string, std::stoi;

// Cache coherence protocol
enum class CCProtocol { MESI, DRAGON };

/*
 * User provided Configuration
 *   - protocol
 *   - input_file_name
 *   - cache_size
 *   - associativity
 *   - block_size
 */
class Config {
public:
    CCProtocol cc_protocol;
    string     input_file_name;
    u32        cache_size;
    u32        associativity;
    u32        block_size;

    Config()
        : cc_protocol(CCProtocol::MESI), input_file_name(""), cache_size(DEFAULT_CACHE_SIZE),
          associativity(DEFAULT_ASSOCIATIVITY), block_size(DEFAULT_BLOCK_SIZE) {}

    Config(string cc_protocol, string input_file_name, string cache_size, string associativity,
           string block_size) {
        this->cc_protocol     = parse_cc_protocol(cc_protocol);
        this->input_file_name = input_file_name;

        try {
            this->cache_size = stoi(cache_size);
        } catch (...) {
            this->cache_size = DEFAULT_CACHE_SIZE;
        }

        try {
            this->associativity = stoi(associativity);
        } catch (...) {
            this->cache_size = DEFAULT_ASSOCIATIVITY;
        }

        try {
            this->block_size = stoi(block_size);
        } catch (...) {
            this->block_size = DEFAULT_BLOCK_SIZE;
        }
    }

private:
    fn parse_cc_protocol(string cc_protocol) -> CCProtocol {
        if (cc_protocol == "Dragon") {
            return CCProtocol::DRAGON;
        }

        return CCProtocol::MESI;
    }
};

#endif // CONFIG_HPP
