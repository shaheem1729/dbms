#include "BlockBuffer.h"

#include <cstdlib>
#include <cstring>

#include "BlockBuffer.h"
#include <cstring> // for memcpy

// Constructor: initialize blockNum
BlockBuffer::BlockBuffer(int blockNum)
{
    this->blockNum = blockNum;
}

// RecBuffer constructor: call BlockBuffer constructor
RecBuffer::RecBuffer(int blockNum) : BlockBuffer(blockNum) {}

// Load the block header into the argument pointer
int BlockBuffer::getHeader(struct HeadInfo *head)
{
    unsigned char buffer[BLOCK_SIZE];

    // Read the block at this->blockNum into the buffer
    Disk::readBlock(buffer, this->blockNum);

    // Populate the fields from fixed offsets in the block
    memcpy(&head->blockType, buffer, 4);
    memcpy(&head->pblock, buffer + 4, 4);
    memcpy(&head->lblock, buffer + 8, 4);
    memcpy(&head->rblock, buffer + 12, 4);
    memcpy(&head->numEntries, buffer + 16, 4);
    memcpy(&head->numAttrs, buffer + 20, 4);
    memcpy(&head->numSlots, buffer + 24, 4);

    return SUCCESS;
}

// Load the record at slotNum into the argument pointer
int RecBuffer::getRecord(union Attribute *rec, int slotNum)
{
    struct HeadInfo head;

    // Get the header info
    this->getHeader(&head);

    int attrCount = head.numAttrs;
    int slotCount = head.numSlots;

    unsigned char buffer[BLOCK_SIZE];

    // Read the block into buffer
    Disk::readBlock(buffer, this->blockNum);

    // Calculate record size and offset
    int recordSize = attrCount * ATTR_SIZE;
    int slotMapSize = slotCount; // 1 byte per slot
    int offset = HEADER_SIZE + slotMapSize + (recordSize * slotNum);

    // Pointer to the start of the desired record
    unsigned char *slotPointer = buffer + offset;

    // Load the record into the rec structure
    memcpy(rec, slotPointer, recordSize);

    return SUCCESS;
}
