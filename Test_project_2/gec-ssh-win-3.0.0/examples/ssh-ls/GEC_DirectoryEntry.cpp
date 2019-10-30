#include <iostream>
#include <sstream>
#include <string>

#include "GEC_DirectoryEntry.h"

GEC_DirectoryEntry* GEC_DirectoryEntry::createFromLsOutputLine(std::string line)
{
  GEC_DirectoryEntry* entry = 0;

  std::string dummy;
  std::string mask;
  uint64_t sizeInBytes;
  std::string name;

  std::stringstream ss(line);
  ss >> mask
     >> dummy >> dummy >> dummy
     >> sizeInBytes
     >> dummy >> dummy >> dummy
     >> name;

  if (!mask.empty() && !name.empty()) {
    if (mask[0] == 'd' || mask[0] == '-') {
      entry = new GEC_DirectoryEntry();
      entry->m_isDirectory = mask[0] == 'd';
      entry->m_sizeInBytes = sizeInBytes;
      entry->m_name = name;
    }
    
  }
  return entry;
}
