#ifndef GEC_DIRECTORYENTRY_H_
#define GEC_DIRECTORYENTRY_H_

#include <stdint.h>
#include <string>

class GEC_DirectoryEntry
{
public:
  GEC_DirectoryEntry()
    : m_isDirectory(false)
    , m_name("")
    , m_sizeInBytes(0)
  {}

  static GEC_DirectoryEntry* createFromLsOutputLine(std::string line);

  bool isDirectory() { return m_isDirectory; }
  std::string getName() { return m_name; }
  uint64_t getSizeInBytes() { return m_sizeInBytes; }

private:
  bool m_isDirectory;
  std::string m_name;
  uint64_t m_sizeInBytes;
};

#endif /* GEC_DIRECTORYENTRY_H_ */
