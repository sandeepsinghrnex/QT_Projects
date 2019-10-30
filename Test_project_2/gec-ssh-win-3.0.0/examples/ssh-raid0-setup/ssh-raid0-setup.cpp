#include <iomanip>
#include <iostream>
#include <sstream>

#include "ssh-common.h"

void printUsage()
{
  std::cout << "Usage: ssh-raid0-setup <server> <md-name> <mount-path> <dev-name-0> <dev-name-1> ..." << std::endl
	    << std::endl
	    << "where: <server> is the host name or IP address to the server" << std::endl
	    << "       <md-name> is the device name to be used for the created RAID device" << std::endl
	    << "       <mount-path> is the full path where the resulting file system is mounted" << std::endl
	    << "       <dev-name-N> is the device name of a disk device to be included in the RAID" << std::endl
	    << "                    At least 2 disk device names must be specified" << std::endl;
}

int main(int argc, char* argv[]) 
{
  if (argc < 6) {
    std::cout << "ERROR: Incorrect number of arguments" << std::endl
    << std::endl;
    printUsage();
  }

  std::stringstream ss;
  ss << "echo y | /sbin/mdadm --create " << argv[2] << " --level=raid0 --raid-devices=" << argc - 4;
  for (int i = 4; i < argc; ++i) {
    ss << " " << argv[i];
  }
  ss << " && ";
  ss << "/sbin/mkfs.ext4 -b 4096 " << argv[2] << " && ";
  ss << "mount " << argv[2] << " " << argv[3];

  std::string command = ss.str();
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
