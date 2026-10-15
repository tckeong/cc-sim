#ifndef MESI_HPP
#define MESI_HPP

#include "directory.hpp"
#include "cache_coherence_model.hpp"

class MESI : public CacheCoherenceModel {
public:
    enum class State { MODIFIED, EXCLUSIVE, SHARED, INVALID };

    MESI() : state(State::INVALID) {}

    bool         is_valid() const { return state != State::INVALID; }
    virtual void change_state() override;
    virtual void get_state() override;
    virtual void set_state() override;
    virtual void cache_read() override;
    virtual void cache_write() override;

    // Additional methods for MESI protocol
    void fetch();
    void fetchAndInvalidate();
    void invalidate();

private:
    State     state;
    Directory directory;
};

#endif // MESI_HPP