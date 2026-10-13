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
    virtual void invalidate() override;
    virtual void cache_read() override;
    virtual void cache_write() override;

    virtual ~MESI() override = default;

private:
    State     state;
    Directory directory;
};

#endif // MESI_HPP