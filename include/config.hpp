#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "types.hpp"
#include "params.hpp"

#include <string>
#include <optional>

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
class Config
{
public:
    CCProtocol  cc_protocol;
    std::string input_file;
    u32         cache_size;
    u32         associativity;
    u32         block_size;

    Config()
        : cc_protocol(CCProtocol::MESI),
          input_file(""),
          cache_size(DEFAULT_CACHE_SIZE),
          associativity(DEFAULT_ASSOCIATIVITY),
          block_size(DEFAULT_BLOCK_SIZE)
    {
    }

    Config(std::string cc_protocol, std::string input_file, std::string cache_size,
           std::string associativity, std::string block_size)
    {
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

    std::optional<std::string> check()
    {
        if (cc_protocol == CCProtocol::DRAGON) {
            return std::optional<std::string>(
                "Dragon protocol is not supported in this version of the simulator");
        }

        // cache_size need to be power of two
        if (!is_pow_of_two(cache_size)) {
            return std::optional<std::string>("cache_size must be a power of two");
        }

        // associativity need >= 1
        if (associativity < 1) {
            return std::optional<std::string>("associativity must be >= 1");
        }

        // block_size need >= 4 and be power of two
        if (block_size < 4 || !is_pow_of_two(block_size)) {
            return std::optional<std::string>("block_size must be >= 4 and a power of two");
        }

        if (cache_size < associativity * block_size ||
            cache_size % (associativity * block_size) != 0) {
            return std::optional<std::string>(
                "cache_size must be as large as associativity * block_size and divisible by it");
        }

        return std::nullopt;
    }

private:
    CCProtocol parse_cc_protocol(std::string cc_protocol)
    {
        if (cc_protocol == "Dragon") {
            return CCProtocol::DRAGON;
        }

        return CCProtocol::MESI;
    }

    bool is_pow_of_two(u32 n) { return (n > 0) && (n & (n - 1)) == 0; }
};

#endif // CONFIG_HPP
