#include "mesi.hpp"

void MESI::fetch() {
    // Implementation for fetch
    this->state = State::SHARED;

}
void MESI::fetch_and_invalidate() {
    // Implementation for fetch and invalidate
    this->state = State::INVALID;
}

void MESI::invalidate() {
    // Implementation for invalidate
    this->state = State::INVALID;
}


void MESI::update_state(Directory &directory, u32 memory_address, CacheOperation operation, u32 core_idx) {
    // Implementation for updating state based on MESI protocol
    switch (this->state) {
        case State::INVALID:
            if (operation == CacheOperation::READ) {
                directory.handleReadMiss(core_idx, memory_address, *this);

                if (directory.checkIsExclusive(memory_address)) {
                    this->state = State::EXCLUSIVE;
                } else {
                    this->state = State::SHARED;
                }
                
            } else if (operation == CacheOperation::WRITE) {
                directory.handleWriteMiss(core_idx, memory_address, *this);
                this->state = State::MODIFIED;
            }
            break;

        case State::SHARED:
            if (operation == CacheOperation::WRITE) {
                directory.handleWriteMiss(core_idx, memory_address, *this);
                this->state = State::MODIFIED;
            }
            break;

        case State::EXCLUSIVE:
            if (operation == CacheOperation::WRITE) {
                directory.handleWriteMiss(core_idx, memory_address, *this);
                this->state = State::MODIFIED;
            }
            break;

        case State::MODIFIED:
            // No state change on read or write
            break;
    }
}