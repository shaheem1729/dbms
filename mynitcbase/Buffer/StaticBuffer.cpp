#include "StaticBuffer.h"
#include <cstring>
#include <iostream>

unsigned char StaticBuffer::blocks[BUFFER_CAPACITY][BLOCK_SIZE];
struct BufferMetaInfo StaticBuffer::metainfo[BUFFER_CAPACITY];

StaticBuffer::StaticBuffer()
{

    // initialise all blocks as free
    for (int bufferBlockIdx = 0; bufferBlockIdx < BUFFER_CAPACITY; bufferBlockIdx++)
    {
        metainfo[bufferBlockIdx].free = true;
    }
}
/*
At this stage, we are not writing back from the buffer to the disk since we are
not modifying the buffer. So, we will define an empty destructor for now. In
subsequent stages, we will implement the write-back functionality here.
*/
StaticBuffer::~StaticBuffer() {}

int StaticBuffer::getFreeBuffer(int blockNum)
{
    if (blockNum < 0 || blockNum > DISK_BLOCKS)
    {
        return E_OUTOFBOUND;
    }
    int allocatedBuffer;

    // iterate through all the blocks in the StaticBuffer
    for (int bufferBlockIdx = 0; bufferBlockIdx < BUFFER_CAPACITY; bufferBlockIdx++)
    {

        if (metainfo[bufferBlockIdx].free && allocatedBuffer == -1)
        {
            allocatedBuffer = bufferBlockIdx;
        }
        else if (!metainfo[bufferBlockIdx].free)
        {
            metainfo[bufferBlockIdx].timeStamp++;
        }
    }
    // find the first free block in the buffer (check metainfo)
    // assign allocatedBuffer = index of the free block

    metainfo[allocatedBuffer].free = false;
    metainfo[allocatedBuffer].blockNum = blockNum;

    return allocatedBuffer;
}

int StaticBuffer::getBufferNum(int blockNum)
{

    if (blockNum < 0 || blockNum > DISK_BLOCKS)
    {
        return E_OUTOFBOUND;
    }

    // this to find where we have stored the block in the cache
    for (int bufferBlockIdx = 0; bufferBlockIdx < BUFFER_CAPACITY; bufferBlockIdx++)
    {
        if (!metainfo[bufferBlockIdx].free && metainfo[bufferBlockIdx].blockNum == blockNum)
        {
            return bufferBlockIdx;
        }
    }

    return E_BLOCKNOTINBUFFER;
}