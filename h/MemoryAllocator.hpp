#include "../lib/hw.h"
#include "../test/printing.hpp"

class MemoryAllocator{
private:
    static MemoryAllocator* iPtr;


    MemoryAllocator(){
        freeMemHead = (BlockHeader*) ((char*) HEAP_START_ADDR);
        freeMemHead->next = nullptr;
        freeMemHead->prev = nullptr;
        size_t diff = (size_t)((char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR - sizeof(BlockHeader));
        freeMemHead->size = diff;
    }

public:
    struct BlockHeader{
        BlockHeader* next;
        BlockHeader* prev;
        size_t size;
    };

    static BlockHeader* freeMemHead;

    static MemoryAllocator& getInstance(){

        if (iPtr == nullptr) {
            static MemoryAllocator ins;
            iPtr = &ins;

            return ins;
        }
        return *iPtr;
    }

    void* mem_alloc(size_t size);

    int mem_free(void*);

    static void print();
};


