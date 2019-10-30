#include <iomanip>
#include <iostream>
#include <stdio.h>

#include "ssh-common.h"

#include "GEC_DirectoryEntry.h"

void printUsage()
{
  std::cout << "Usage: ssh-ls <server> <path>" << std::endl
            << std::endl
            << "where: <server> is the host name or IP address to the server" << std::endl
            << "       <path> is the full path of the server directory that will be listed" << std::endl;
}

int main(int argc, char* argv[]) 
{
  if (argc != 3) {
    std::cout << "ERROR: Incorrect number of arguments" << std::endl
              << std::endl;
    printUsage();
  }

  std::string command = std::string("ls -l ") + std::string(argv[2]);
  std::cout << "command = " << command << std::endl;
  std::vector<std::string> output;
  std::vector<std::string> error;

  if (issue_command(argv[1], "root", "", command, output, error) == 0) {
    if (output.empty()) {
      std::cout << "Command probably failed - return from 'stderr': " << std::endl;
      for (size_t i = 0; i < error.size(); ++i) {
        std::cout << std::setw(3) << i << ": " << error[i] << std::endl;
      }
    } else {
      std::vector<GEC_DirectoryEntry*> entries;
      
      for (unsigned int i = 1; i < output.size(); ++i) {
        GEC_DirectoryEntry* entry = GEC_DirectoryEntry::createFromLsOutputLine(output[i]);
        if (entry) {
          entries.push_back(entry);
        }
      }

      std::vector<GEC_DirectoryEntry*>::iterator it;
      std::cout << "Directories:" << std::endl;
      for (it = entries.begin(); it != entries.end(); ++it) {
        if ((*it)->isDirectory()) {
          std::cout << std::setw(40) << std::left << (*it)->getName() << " " 
            << std::setw(12) << std::right << (*it)->getSizeInBytes() << " bytes" << std::endl;
        }
      }
      std::cout << std::endl << "Files:" << std::endl;
      for (it = entries.begin(); it != entries.end(); ++it) {
        if (!(*it)->isDirectory()) {
          std::cout << std::setw(40) << std::left << (*it)->getName() << " " 
            << std::setw(12) << std::right << (*it)->getSizeInBytes() << " bytes" << std::endl;
        }
      }

      for (it = entries.begin(); it != entries.end(); ++it) {
        delete (*it);
      }
      entries.clear();
    }
  }
}
