#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string.h>

#include "ssh-common.h"

#include "GEC_BIT.h"


char const* PATH = "/home/nils/repo/gec-bit/scripts";
char const* CMD = "gec-bit-list.py";

const std::string STATUS_LABEL("status = ");
const std::string PASSED_LABEL("passed = ");
const std::string FAILED_LABEL("failed = ");

static size_t g_iNextFailedTest = 0;
static std::vector<std::string> g_failedTestLabel;
static std::vector<std::string> g_failedTestMsg;

static int issueBITListCommand(const char* ip, const char* suite, const char* mode, 
                               std::vector<std::string>& output)
{
  std::string command = std::string(PATH) + ("/") + CMD + " " + suite + " --mode=" + mode;
  std::vector<std::string> error;

  if (issue_command(ip, "root", "", command, output, error) != 0) {
    return GEC_BIT_NETWORK_ERROR;
  } else {
    if (output.empty()) {
      std::cout << "Command probably failed - return from 'stderr': " << std::endl;
      for (size_t i = 0; i < error.size(); ++i) {
        std::cout << std::setw(3) << i << ": " << error[i] << std::endl;
      }
      return GEC_BIT_COMMAND_ERROR;
    }
  }

  return GEC_BIT_OK;
}

bool hasSummaryLabel(std::string line, std::string label)
{
  size_t iLabel = line.find(label);
  return (iLabel != std::string::npos);
}

static int decodeSummaryStatus(std::string line, int* status)
{
  int rc = GEC_BIT_OK;

  std::map<std::string, int> statusDecodeTable;
  statusDecodeTable["NOT STARTED"] = NOT_STARTED;
  statusDecodeTable["RUNNING"] = RUNNING;
  statusDecodeTable["COMPLETED"] = COMPLETED;

  const std::string SEPERATOR(" = ");
  size_t iValue = line.find(SEPERATOR) + SEPERATOR.length();
  std::string statusAsString = line.substr(iValue);

  if (statusDecodeTable.find(statusAsString) != statusDecodeTable.end()) {
    *status = statusDecodeTable.find(statusAsString)->second;
  } else {
    std::cout << "ERROR: '" << statusAsString << "' is an unknown status string" << std::endl;
    rc = GEC_BIT_FORMAT_ERROR;
  }

  return rc;
}


static int decodeSummaryValue(std::string line, unsigned int* value)
{
  int rc = GEC_BIT_OK;

  const std::string SEPERATOR(" = ");
  size_t iValue = line.find(SEPERATOR) + SEPERATOR.length();
  std::string valueAsString = line.substr(iValue);

  std::istringstream(valueAsString) >> *value;

  return rc;
}

static bool isValidTestLabel(std::string line)
{
  size_t iStart = line.find_first_of('[');
  size_t iEnd = line.find_first_of(']');

  return (iStart == 0 && iEnd == line.length() - 1);
}

static int decodeTestLabel(std::string line, std::string& label)
{
  int rc = GEC_BIT_FORMAT_ERROR;

  if ( isValidTestLabel(line) ) {
    rc = GEC_BIT_OK;
    label = line.substr(1, line.length() - 2);
  }

  return rc;
}

static int decodeFailedTestInfo(std::vector<std::string> output)
{
  g_iNextFailedTest = 0;

  int rc = GEC_BIT_OK;
  std::vector<std::string>::iterator it = output.begin();
  while (rc == GEC_BIT_OK && it != output.end()) {
    std::string label;
    std::string msg;

    rc = decodeTestLabel(*it, label);
    ++it;

    if (rc == GEC_BIT_OK && it != output.end()) {
      msg = *it;
      ++it;

      g_failedTestLabel.push_back(label);
      g_failedTestMsg.push_back(msg);
    }
  }

  return rc;
}
extern "C" {

int GEC_BIT_getSummary(const char* ip, const char* suite, 
                       int* status, unsigned int* passed, unsigned int* failed)
{
  std::vector<std::string> output;

  int rc = issueBITListCommand(ip, suite, "summary", output);
  if (rc == GEC_BIT_OK) {
    for (size_t i = 0; i < output.size(); ++i) {

      if ( hasSummaryLabel(output[i], STATUS_LABEL) ) {
        rc = decodeSummaryStatus(output[i], status);
      } else if (hasSummaryLabel(output[i], PASSED_LABEL) ) {
        rc = decodeSummaryValue(output[i], passed);
      } else if (hasSummaryLabel(output[i], FAILED_LABEL) ) {
        rc = decodeSummaryValue(output[i], failed);
      }

      if (rc != GEC_BIT_OK) {
        break;
      }
    }
  }

  return rc;
}

int GEC_BIT_getFirstFailedTest(const char* ip, const char* suite,
                               char* label, unsigned int maxSizeLabel,
                               char* msg, unsigned int maxSizeMsg)
{
  std::vector<std::string> output;

  int rc = issueBITListCommand(ip, suite, "failed", output);
  if (rc == GEC_BIT_OK) {
    rc = decodeFailedTestInfo(output);
  }

  if (rc == GEC_BIT_OK) {
    rc = GEC_BIT_getNextFailedTest(label, maxSizeLabel, msg, maxSizeMsg);
  }

  return rc;
}

static void copyAndTerminate(char* dest, const char* src, unsigned int maxSize)
{
  strncpy(dest, src, maxSize - 1);
  dest[maxSize - 1] = '\0';
}

int GEC_BIT_getNextFailedTest(char* label, unsigned int maxSizeLabel,
                              char* msg, unsigned int maxSizeMsg)
{
  int rc = GEC_BIT_OK;

  if ( maxSizeLabel < 1 || maxSizeMsg < 1) {
    rc = GEC_BIT_INVALID_PARAM;
  }

  if ( g_iNextFailedTest < g_failedTestLabel.size() && g_iNextFailedTest < g_failedTestMsg.size() ) {
    copyAndTerminate(label, g_failedTestLabel[g_iNextFailedTest].c_str(), maxSizeLabel);
    copyAndTerminate(msg, g_failedTestMsg[g_iNextFailedTest].c_str(), maxSizeMsg);

    ++g_iNextFailedTest;
  } else {
    label[0] = '\0';
    msg[0] = '\0';
  }

  return GEC_BIT_OK;
}

}
