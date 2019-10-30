#include <iomanip>
#include <iostream>

#include "ssh-common.h"

void printUsage()
{
  std::cout << "Usage: ssh-rm <server> <path>" << std::endl
	    << std::endl
	    << "where: <server> is the host name or IP address to the server" << std::endl
	    << "       <path> is the full path of the file to be removed" << std::endl;
}

int main(int argc, char* argv[]) 
{
  if (argc != 3) {
    std::cout << "ERROR: Incorrect number of arguments" << std::endl
	      << std::endl;
    printUsage();
  }

  std::string command = std::string("rm ") + std::string(argv[2]);
  std::cout << "command = " << command << std::endl;
  std::vector<std::string> output;
  std::vector<std::string> error;

  if (issue_command(argv[1], "root", "", command, output, error) == 0) {
    if (!output.empty()) {
      for (size_t i = 0; i < output.size(); ++i) {
        std::cout << std::setw(3) << i << ": " << output[i] << std::endl;
      }
    } else if (!error.empty()) {
      std::cout << "Command probably failed - return from 'stderr': " << std::endl;
      for (size_t i = 0; i < error.size(); ++i) {
        std::cout << std::setw(3) << i << ": " << error[i] << std::endl;
      }
    }
  }
}
