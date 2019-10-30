#include <iostream>
#include <iomanip>
#include <stdio.h>

#include "ssh-common.h"


void printUsage()
{
  std::cout << "Usage: ssh-command <server> <user> <password> <command>" << std::endl
            << std::endl
            << "where: <server> is the host name or IP address to the server" << std::endl
            << "       <user> and <password> are the name and the password" << std::endl
            << "       of the user account to use on the server" << std::endl
            << "       <command> is the command to be issued to the host" << std::endl;
}

int main(int argc, char* argv[]) 
{
  if (argc != 5) {
    std::cout << "ERROR: Incorrect number of arguments" << std::endl
              << std::endl;
    printUsage();
    return (-1);
  }

  std::string command = std::string(argv[4]);
  std::cout << "command = " << command << std::endl;
  std::vector<std::string> output;
  std::vector<std::string> error;

  if (issue_command(argv[1], argv[2], argv[3], command, output, error) == 0) {
    for (size_t i = 0; i < output.size(); ++i) {
      std::cout << output[i] << std::endl;
    }
    if (!error.empty()) {
      std::cout << "stderr:" << std::endl;
      for (size_t i = 0; i < error.size(); ++i) {
        std::cout << error[i] << std::endl;
      }
    }
  } else {
    std::cout << "issue_command failed" << std::endl;
  }
  
}
