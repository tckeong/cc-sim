#ifndef MESI_HPP
#define MESI_HPP

#include "directory.hpp"
#include "cache_coherence_model.hpp"

enum class State
{
    MODIFIED,
    EXCLUSIVE,
    SHARED,
    INVALID
};

class MESI : public CacheCoherenceModel
{
public:
    MESI() : state(State::INVALID) {}

    bool is_valid() const { return state != State::INVALID; }
    virtual void change_state() override;
    virtual State get_state() override;
    virtual void set_state() override;
    virtual void cache_read() override;
    virtual void cache_write() override;

    // Additional methods for MESI protocol
    void fetch();
    void fetch_and_invalidate();
    void invalidate();
    void update_state(u32 memory_address, CacheOperation operation, u32 core_idx);

private:
    State state;
    Directory directory;
};

#endif // MESI_HPP