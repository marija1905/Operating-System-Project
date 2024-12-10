#include "../lib/hw.h"
#include "../h/MemoryAllocator.hpp"

MemoryAllocator* MemoryAllocator :: iPtr= nullptr;
MemoryAllocator::BlockHeader *MemoryAllocator::freeMemHead = (BlockHeader*)((char*) HEAP_START_ADDR);


void *MemoryAllocator::mem_alloc(size_t size){

    if (size == 0 || freeMemHead == nullptr) {
        return nullptr;
    }

    BlockHeader* blk = freeMemHead, *prev = nullptr;
    if(size % MEM_BLOCK_SIZE){
        size += MEM_BLOCK_SIZE - size % MEM_BLOCK_SIZE;
    }
    size_t size_in_bytes = size;

    for(;blk != nullptr; prev = blk, blk = blk->next){
        if (blk->size >= size_in_bytes) break;
    }

    if(blk == nullptr){
        // nema slobodnog regiona gde se moze smestiti size blokova
        return nullptr;
    }
    size_t rSize = blk->size - size_in_bytes;
    if(rSize >= sizeof(BlockHeader)) {
        blk->size = size_in_bytes;
        size_t off = sizeof(BlockHeader) + size_in_bytes; // offset --> velicina nase memorije koju treba zauzeti
        BlockHeader *newBlk = (BlockHeader*)((char*) blk + off);

        if(prev != nullptr){
            blk->prev->next = newBlk;
        }
        else{
            freeMemHead = newBlk;
        }
        newBlk->prev = prev;
        newBlk->next = blk->next;
        if(blk->next) blk->next->prev = newBlk;
        newBlk->size = rSize - sizeof(BlockHeader); // cuvanje koliko jos blokova ima slobodnih
    }


    else{
        if(prev != nullptr){
            blk->prev->next = blk->next;
            blk->next->prev = blk->prev;
        }
        else{
            freeMemHead = blk->next;
        }
    }

    blk->next = nullptr;
    blk->prev = nullptr;

    return (char*)blk + sizeof(BlockHeader);
}

int try_join(MemoryAllocator::BlockHeader* cur){
    if(!cur) return 0;
    char* cmp = (char*)cur + cur->size + sizeof(MemoryAllocator::BlockHeader);
    if(cur->next && cmp == (char*) (cur->next)){
        cur->size += cur->next->size + sizeof(MemoryAllocator::BlockHeader);
        cur->next = cur->next->next;
        if(cur->next) cur->next->prev = cur;
        return 1;
    }
    else{
        return 0;
    }
}

int MemoryAllocator::mem_free(void * ptr){ //TODO

    if(ptr == nullptr) return -1; //greska
    BlockHeader* addr = (BlockHeader*)((char*)ptr - sizeof(BlockHeader));
    BlockHeader *blk = freeMemHead, *prev = nullptr;
    if(!addr) return -1;

    for (; blk!=nullptr && blk >= addr; prev = blk, blk=blk->next);

    if(prev != nullptr){
        addr->next = blk;
        prev->next = addr;
        addr->prev = prev;
        if(blk != nullptr) blk->prev = addr;
        try_join(addr);
        try_join(prev);
        return 0;
    }
    else{
        freeMemHead = addr;
        addr->next = blk;
        if(blk != nullptr) blk->prev = addr;
        return 0;

    }
}

void MemoryAllocator::print() {
    for(BlockHeader *blk = freeMemHead; blk!=nullptr; blk=blk->next){
        printInt(blk->size);
        printString("\n");
    }
    printString("----------------------------------\n");

}
