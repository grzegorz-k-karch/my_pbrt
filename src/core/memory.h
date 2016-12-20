#ifndef CORE_MEMORY_H
#define CORE_MEMORY_H

#include <cstdlib>
#include <cstdint>
#include <list>
#include <utility>

namespace pbrt {

void *AllocAligned(size_t size);

template <typename T> T* AllocAligned(size_t count) {
    return (T*)AllocAligned(count*sizeof(T));
}

void FreeAligned(void*);

class MemoryArena {

public:
    MemoryArena(size_t blockSize = 262144) : blockSize(blockSize) {}

    void* Alloc(size_t nBytes) {
        // <round up to a minimum machine alignment>
        nBytes = ((nBytes + 15) & (~15));
        if (currentBlockPos + nBytes > currentAllocSize) {
            // <add current block to usedBlocks list>
            if (currentBlock) {
                usedBlocks.push_back(std::make_pair(currentAllocSize, currentBlock));
                currentBlock = nullptr;
            }
            // <get new block of memory for MemoryArena>
            // <try to get memory block from availableBlocks>
            for (auto iter = availableBlocks.begin(); iter != availableBlocks.end(); ++iter) {
                if (iter->first >= nBytes) {
                    currentAllocSize = iter->first;
                    currentBlock = iter->second;
                    availableBlocks.erase(iter);
                    break;
                }
            }
            if (!currentBlock) {
                currentAllocSize = std::max(nBytes, blockSize);
                currentBlock = AllocAligned<uint8_t>(currentAllocSize);
            }
            currentBlockPos = 0;
        }
        void *ret = currentBlock + currentBlockPos;
        currentBlockPos += nBytes;

        return ret;
    }

    void Reset() {
        currentBlockPos = 0;
        availableBlocks.splice(availableBlocks.begin(), usedBlocks);
    }

    template <typename T> T* Alloc(size_t n = 1, bool runConstructor = true) {
        T *ret = (T*)Alloc(n*sizeof(T));
        if (runConstructor) {
            for (size_t i = 0; i < n; ++i) {
                new (&ret[i]) T();
            }
        }
        return ret;
    }
private:
    const size_t blockSize;
    size_t currentBlockPos = 0;
    size_t currentAllocSize = 0;
    uint8_t *currentBlock = nullptr;
    std::list<std::pair<size_t, uint8_t*>> usedBlocks, availableBlocks;
};

template <typename T, int logBlockSize> class BlockedArray {

public:
    BlockedArray(int uRes, int vRes, const T* d = nullptr)
    : uRes(uRes), vRes(vRes), uBlocks(RoundUp(uRes) >> logBlockSize) {
        int nAlloc = RoundUp(uRes)*RoundUp(vRes);
        data = AllocAligned<T>(nAlloc);
        for (int i = 0; i < nAlloc; ++i) {
            new (&data[i]) T();
        }
        if (d) {
            for (int v = 0; v < vRes; ++v) {
                for (int u = 0; u < uRes; ++u) {
                    (*this)(u,v) = d[v*uRes + u];
                }
            }
        }
    }
    constexpr int BlockSize() const { return 1 << logBlockSize; }
    int RoundUp(int x) const {
        return (x + BlockSize() - 1) & ~(BlockSize() - 1);
    }
    int uSize() const { return uRes; }
    int vSize() const { return vRes; }
    int Block(int a) const { return a >> logBlockSize; }
    int Offset(int a) const { return (a & (BlockSize() - 1)); }
    T& operator()(int u, int v) {
        int bu = Block(u), bv = Block(v);
        int ou = Offset(u), ov = Offset(v);
        int offset = BlockSize()*BlockSize()*(uBlocks*bv + bu);
        offset += BlockSize()*ov + ou;

        return data[offset];
    }
private:
    T *data;
    const int uRes, vRes, uBlocks;
};

} //namespace pbrt

#endif//CORE_MEMORY_H
