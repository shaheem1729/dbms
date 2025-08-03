#include "Buffer/StaticBuffer.h"
#include "Disk_Class/Disk.h"
#include <iostream>
#include <string.h>

int main()
{
  Disk disk_run;

  unsigned char buffer[BLOCK_SIZE];

  Disk::readBlock(buffer, 0);

  for (int i = 0; i < BLOCK_SIZE; i++)
  {
    std::cout << (int)buffer[i] << " , ";
  }

  return 0;
}
