#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "types.hpp"
#include "params.hpp"

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
    CCProtocol  cc_protocol;
    std::string input_file;
    u32         cache_size;
    u32         associativity;
    u32         block_size;

    Config()
        : cc_protocol(CCProtocol::MESI), input_file(""), cache_size(DEFAULT_CACHE_SIZE),
          associativity(DEFAULT_ASSOCIATIVITY), block_size(DEFAULT_BLOCK_SIZE) {}

    Config(std::string cc_protocol, std::string input_file, std::string cache_size,
           std::string associativity, std::string block_size) {
        this->cc_protocol = parse_cc_protocol(cc_protocol);
        this->input_file  = input_file;

        try {
            this->cache_size = stoi(cache_size);
        } catch (...) {
            this->cache_size = DEFAULT_CACHE_SIZE;
        }

        try {
            this->associativity = stoi(associativity);
        } catch (...) {
            this->associativity = DEFAULT_ASSOCIATIVITY;
        }

        try {
            this->block_size = stoi(block_size);
        } catch (...) {
            this->block_size = DEFAULT_BLOCK_SIZE;
        }
    }

private:
    CCProtocol parse_cc_protocol(std::string cc_protocol) {
        if (cc_protocol == "Dragon") {
            return CCProtocol::DRAGON;
        }

        return CCProtocol::MESI;
    }
};

#endif // CONFIG_HPP
