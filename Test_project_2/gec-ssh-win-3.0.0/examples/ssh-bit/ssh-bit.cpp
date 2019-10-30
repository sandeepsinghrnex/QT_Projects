#include <iostream>
#include <stdlib.h>

#include "GEC_BIT.h"


void printUsage()
{
  std::cout << "Usage: ssh-bit <server> [<suite>]" << std::endl
    << std::ends
    << "where: <server> is the host name or IP address to the server" << std::endl
    << "       <suite> is the name of the BIT suite to list the result for" << std::endl
    << "               (default is 'boot')" << std::endl;
}

int main(int argc, char* argv[]) 
{
  if (argc != 2 && argc != 3) {
    std::cout << "ERROR: Incorrect number of arguments" << std::endl
      << std::endl;
    printUsage();
    exit(1);
  }

  std::string suite("");
  if (argc == 3) {
      suite = argv[2];
  }

  int status = 0;
  unsigned int numPassed = 0;
  unsigned int numFailed = 0;
  int rc = GEC_BIT_getSummary(argv[1], suite.c_str(), &status, &numPassed, &numFailed);

  if (rc == 0) {
    std::cout << "status = " << status << std::endl
              << "passed = " << numPassed << std::endl
              << "failed = " << numFailed << std::endl;

    std::cout << "Failed:" << std::endl;

    char label[GEC_BIT_MAX_LABEL_SIZE];
    char msg[GEC_BIT_MAX_MSG_SIZE];
    rc = GEC_BIT_getFirstFailedTest(argv[1], suite.c_str(), 
                                    label, GEC_BIT_MAX_LABEL_SIZE, 
                                    msg, GEC_BIT_MAX_MSG_SIZE);
    while (rc == GEC_BIT_OK && *label) {
      std::cout << label << " : " << msg << std::endl;
      rc = GEC_BIT_getNextFailedTest(label, GEC_BIT_MAX_LABEL_SIZE, 
                                     msg, GEC_BIT_MAX_MSG_SIZE);
    }
  }

  return 0;
}
