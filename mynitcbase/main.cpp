#include "Buffer/StaticBuffer.h"
#include "Cache/OpenRelTable.h"
#include "Disk_Class/Disk.h"
#include "FrontendInterface/FrontendInterface.h"
#include <iostream>
#include <string.h>
using namespace std;

int main()
{
  Disk disk_run;
  StaticBuffer buffer;
  OpenRelTable cache;

  // recBlockIdx = 0 -> RELCAT_RELID
  // recBlockIdx = 1 -> ATTRCAT_RELID
  for (int relId = 0; relId < 3; relId++)
  {
    RelCatEntry relCatBuf;
    RelCacheTable::getRelCatEntry(relId, &relCatBuf);
    cout << relCatBuf.relName << endl;

    for (int attrs = 0; attrs < relCatBuf.numAttrs; attrs++)
    {
      AttrCatEntry attrCatBuf;
      AttrCacheTable::getAttrCatEntry(relId, attrs, &attrCatBuf);
      cout << " " << attrCatBuf.attrName << ": " << attrCatBuf.attrType << endl;
    }
    cout << endl;
  }

  return 0;
}
