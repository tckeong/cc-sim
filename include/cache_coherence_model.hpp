#ifndef CACHE_COHERENCE_MODEL_HPP
#define CACHE_COHERENCE_MODEL_HPP

class CacheCoherenceModel {
public:
    virtual void change_state() = 0;
    virtual void get_state()    = 0;
    virtual void set_state()    = 0;
    virtual void invalidate()   = 0;
    virtual void cache_read()   = 0;
    virtual void cache_write()  = 0;

    virtual ~CacheCoherenceModel() = default;
};

#endif // CACHE_COHERENCE_HPP