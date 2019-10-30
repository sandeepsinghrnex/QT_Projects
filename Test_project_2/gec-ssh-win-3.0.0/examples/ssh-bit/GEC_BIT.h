#ifndef GEC_BIT_H_
#define GEC_BIT_H_

#ifdef __cplusplus
extern "C" {
#endif

#define GEC_BIT_OK 0
#define GEC_BIT_NETWORK_ERROR (-1)
#define GEC_BIT_COMMAND_ERROR (-2)
#define GEC_BIT_FORMAT_ERROR  (-3)
#define GEC_BIT_OUT_OF_RANGE  (-4)
#define GEC_BIT_INVALID_PARAM (-5)

enum {
  NOT_STARTED,
  RUNNING,
  COMPLETED
};

#define  GEC_BIT_MAX_LABEL_SIZE  64
#define  GEC_BIT_MAX_MSG_SIZE    256

/**
  Returns a summary of the results from the most recent execution of a given BIT suite

  The summary consists of the status of the execution, the number of passed tests, 
  and the number of failed tests.

  The status of execution returns one of three constants. @ref NOT_STARTED is returned
  if the BIT log file contains no entries for the specfied BIT suite. @ref RUNNING is returned
  if the BIT suite is still executing. And @ref COMPLETED is returned if the most recent execution
  of the BIT suite is completed.

  @param ip
  Null terminated char string containing the IP address or host name of the XSR
  @param suite
  Null terminated char string containing the name of the BIT suite
  @param status
  pointer to integer variable which returns the the status of the most recent execution of the BIT suite
  @param passed
  pointer to unsigned integer variable which returns the number of passed test
  @param passed
  pointer to unsigned integer variable which returns the number of failed test

  @return GEC_BIT_OK
  Success
  @return GEC_BIT_NETWORK_ERROR
  Failed to get summary because XSR could not be accessed through SSH
  @return GEC_BIT_COMMAND_ERROR
  Failed to execute script for parsing BIT log file
  @return GEC_BIT_FORMAT_ERROR
  Failed to interpret info returned from script for parsing BIT log file

*/
int GEC_BIT_getSummary(const char* ip, const char* suite,
                       int* status, unsigned int* passed, unsigned int* failed);

/**
  Returns information about the first failed test in a given BIT suite

  This function fetches information about all failed tests in the last execution of the BIT suite.
  Information about failed tests beyond the first are stored, and used in subsequent calls 
  to @ref GEC_BIT_getNextFailedTest.

  The information returned include the label of the test and the message describing why
  the test failed. Both label and message are copied into char strings provided by the caller.
  
  The copy of a label/message is truncated, if it is too long to fit into
  the provided char string. To prevent this the caller can use the constants
  @ref GEC_BIT_MAX_LABEL_SIZE and @ref GEC_BIT_MAX_MSG_SIZE when allocating char strings
  for respectively labels and messages. Using these constants will guarantee that the
  allocated strings are big enough to hold copies of the full labels and messages returned
  by BITs provided by Galleon.

  The returned strings are allways null-terminated - also when they are truncated copies.

  If no tests failed in the last execution of the BIT suite, the returned strings are empty
  (only containing the terminating null character).

  @param ip
  Null terminated char string containing the IP address or host name of the XSR
  @param suite
  Null terminated char string containing the name of the BIT suite
  @param label
  Pointer to char string allocated by caller.
  Returns null-terminated string with copy of the label of the failed test.
  @param maxSizeLabel
  Unsigned integer specifying the size of the char string allocated for the test label.
  This value includes space for the terminating null.
  @param msg
  Pointer to char string allocated by caller.
  Returns null-terminated string with copy of the message from the failed test.
  @param maxSizeMsg
  Unsigned integer specifying the size of the char string allocated for the test message.
  This value includes space for the terminating null.

  @return GEC_BIT_OK
  Success
  @return GEC_BIT_NETWORK_ERROR
  Failed to get summary because XSR could not be accessed through SSH
  @return GEC_BIT_COMMAND_ERROR
  Failed to execute script for parsing BIT log file
  @return GEC_BIT_OUT_OF_RANGE
  Failed to find failed test with the specified index
*/
int GEC_BIT_getFirstFailedTest(const char* ip, const char* suite,
                               char* label, unsigned int maxSizeLabel,
                               char* msg, unsigned int maxSizeMsg);

/**
  Returns information about successive failed tests in a BIT suite.
  
  The information returned include the label of the test and the message describing why
  the test failed. Both label and message are copied into char strings provided by the caller.
  
  The copy of a label/message is truncated, if it is too long to fit into
  the provided char string. To prevent this the caller can use the constants
  @ref GEC_BIT_MAX_LABEL_SIZE and @ref GEC_BIT_MAX_MSG_SIZE when allocating char strings
  for respectively labels and messages. Using these constants will guarantee that the
  allocated strings are big enough to hold copies of the full labels and messages returned
  by BITs provided by Galleon.

  The returned strings are allways null-terminated - also when they are truncated copies.

  The first call to this function after a call to @ref GEC_BIT_getFirstFailedTest,
  returns information about the second failed test in a BIT suite. The second call 
  returns information about the third failed test, and so on.
  
  When there are no more failed tests in the BIT, this function returns no information
  - that is empty strings.

  This function does not fetch information from the XSR. Instead it uses the information
  fetched by the last call to @ref GEC_BIT_getNextFailedTest.

  @param label
  Pointer to char string allocated by caller.
  Returns null-terminated string with copy of the label of the failed test.
  @param maxSizeLabel
  Unsigned integer specifying the size of the char string allocated for the test label.
  This value includes space for the terminating null.
  @param msg
  Pointer to char string allocated by caller.
  Returns null-terminated string with copy of the message from the failed test.
  @param maxSizeMsg
  Unsigned integer specifying the size of the char string allocated for the test message.
  This value includes space for the terminating null.

  @return GEC_BIT_OK
  Success
*/
int GEC_BIT_getNextFailedTest(char* label, unsigned int maxSizeLabel,
                              char* msg, unsigned int maxSizeMsg);

#ifdef __cplusplus
}
#endif

#endif /* GEC_BIT_H_ */