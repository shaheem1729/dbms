#include "RelCacheTable.h"

#include <cstring>

RelCacheEntry *RelCacheTable::relCache[MAX_OPEN];

int RelCacheTable::getRelCatEntry(int relId, RelCatEntry *relCatBuf)
{
    if (relId < 0 || relId >= MAX_OPEN)
    {
        return E_OUTOFBOUND;
    }

    if (relCache[relId] == nullptr)
    {
        return E_RELNOTOPEN;
    }

    // here we get the relcatEntry from the cache (simulation of cache memory)

    relCatBuf->firstBlk = relCache[relId]->relCatEntry.firstBlk;
    relCatBuf->lastBlk = relCache[relId]->relCatEntry.lastBlk;
    relCatBuf->numAttrs = relCache[relId]->relCatEntry.numAttrs;
    relCatBuf->numRecs = relCache[relId]->relCatEntry.numRecs;
    relCatBuf->numSlotsPerBlk = relCache[relId]->relCatEntry.numSlotsPerBlk;
    strcpy(relCatBuf->relName, (relCache[relId]->relCatEntry).relName);

    return SUCCESS;
}

/* Converts a relation catalog record to RelCatEntry struct
    We get the record as Attribute[] from the BlockBuffer.getRecord() function.
    This function will convert that to a struct RelCatEntry type.
NOTE: this function expects the caller to allocate memory for `*relCatEntry`
*/

void RelCacheTable::recordToRelCatEntry(union Attribute record[RELCAT_NO_ATTRS], RelCatEntry *relCatEntry)
{

    strcpy(relCatEntry->relName, record[RELCAT_REL_NAME_INDEX].sVal);
    relCatEntry->numAttrs = (int)record[RELCAT_NO_ATTRIBUTES_INDEX].nVal;
    relCatEntry->numRecs = (int)record[RELCAT_NO_RECORDS_INDEX].nVal;
    relCatEntry->firstBlk = (int)record[RELCAT_FIRST_BLOCK_INDEX].nVal;
    relCatEntry->lastBlk = (int)record[RELCAT_LAST_BLOCK_INDEX].nVal;
    relCatEntry->numSlotsPerBlk = (int)record[RELCAT_NO_SLOTS_PER_BLOCK_INDEX].nVal;
}