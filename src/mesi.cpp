#include "mesi.hpp"

State MESI::get_state() {
    return this->state;
}

void MESI::change_state() {
    // Implementation for changing state
}

void MESI::cache_read() {
    // Implementation for cache read
}

void MESI::cache_write() {
    // Implementation for cache write
}

void MESI::fetch() {
    // Implementation for fetch

}
void MESI::fetch_and_invalidate() {
    // Implementation for fetch and invalidate
}
void MESI::invalidate() {
    // Implementation for invalidate
}

void MESI::update_state(u32 memory_address, CacheOperation operation, u32 core_idx) {
    
}