#include "Buffer/StaticBuffer.h"
#include "Cache/OpenRelTable.h"
#include "Disk_Class/Disk.h"
#include "FrontendInterface/FrontendInterface.h"
#include <iostream>
#include <vector>
#include <string.h>
using namespace std;
void printAttributeCatalog()
{
  // create objects for the relation catalog and attribute catalog
  RecBuffer relCatBuffer(RELCAT_BLOCK);
  RecBuffer attrCatBuffer(ATTRCAT_BLOCK);

  // creating headers for relation catalog and attribute catalog
  HeadInfo relCatHeader;
  HeadInfo attrCatHeader;

  // load the headers of both the blocks into relCatHeader and attrCatHeader.
  // (we will implement these functions later)
  relCatBuffer.getHeader(&relCatHeader);
  attrCatBuffer.getHeader(&attrCatHeader);

  // attrCatBaseSlot stores the index of current slot,
  // which is incremented everytime an attribute entry
  // is handled
  for (int i = 0, attrCatSlotIndex = 0; i < relCatHeader.numEntries; i++)
  {
    // will store the record from the relation catalog
    Attribute relCatRecord[RELCAT_NO_ATTRS];
    relCatBuffer.getRecord(relCatRecord, i);

    printf("Relation: %s\n", relCatRecord[RELCAT_REL_NAME_INDEX].sVal);

    int j = 0;
    for (; j < relCatRecord[RELCAT_NO_ATTRIBUTES_INDEX].nVal; j++, attrCatSlotIndex++)
    {
      // declare attrCatRecord and load the attribute catalog entry into it
      Attribute attrCatRecord[ATTRCAT_NO_ATTRS];
      attrCatBuffer.getRecord(attrCatRecord, attrCatSlotIndex);

      // if the current attribute belongs to the current relation
      // then we print it, which is checked by comparing names
      if (strcmp(attrCatRecord[ATTRCAT_REL_NAME_INDEX].sVal,
                 relCatRecord[RELCAT_REL_NAME_INDEX].sVal) == 0)
      {
        const char *attrType = attrCatRecord[ATTRCAT_ATTR_TYPE_INDEX].nVal == NUMBER
                                   ? "NUM"
                                   : "STR";
        printf("  %s: %s\n", attrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal, attrType);
      }

      // once all the slots are traversed, we update the block number
      // of the attrCatBuffer to the next block, and then attrCatHeader
      // is updated to the header of next block
      if (attrCatSlotIndex == attrCatHeader.numSlots - 1)
      {
        attrCatSlotIndex = -1; // implementational operation, since the loop will increment anyways
        attrCatBuffer = RecBuffer(attrCatHeader.rblock);
        attrCatBuffer.getHeader(&attrCatHeader);
      }
    }

    printf("\n");
  }
}

int main()
{
  Disk disk_run;

  // create objects for the relation catalog and attribute catalog
  RecBuffer relCatBuffer(RELCAT_BLOCK);
  RecBuffer attrCatBuffer(ATTRCAT_BLOCK);

  HeadInfo relCatHeader;
  HeadInfo attrCatHeader;

  // load the headers of both the blocks into relCatHeader and attrCatHeader.
  relCatBuffer.getHeader(&relCatHeader);
  attrCatBuffer.getHeader(&attrCatHeader);

  // Iterate through all relation records
  for (int i = 0; i < relCatHeader.numEntries; i++)
  {
    Attribute relCatRecord[RELCAT_NO_ATTRS];
    relCatBuffer.getRecord(relCatRecord, i);

    const char *relationName = relCatRecord[RELCAT_REL_NAME_INDEX].sVal;
    printf("Relation: %s\n", relationName);

    // Scan attribute catalog for this relation
    for (int j = 0; j < attrCatHeader.numEntries; j++)
    {
      Attribute attrCatRecord[ATTRCAT_NO_ATTRS];
      attrCatBuffer.getRecord(attrCatRecord, j);

      // Check if this attribute belongs to the current relation
      if (strcmp(attrCatRecord[ATTRCAT_REL_NAME_INDEX].sVal, relationName) == 0)
      {
        const char *attrName = attrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal;
        const char *attrType = attrCatRecord[ATTRCAT_ATTR_TYPE_INDEX].nVal == NUMBER ? "NUM" : "STR";
        printf("  %s: %s\n", attrName, attrType);
      }
    }

    printf("\n");
  }

  return 0;
}
