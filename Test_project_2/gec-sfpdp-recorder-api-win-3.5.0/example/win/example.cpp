/**
   @example example.cpp

   This is an example of how the GEC_ISfpdpRecorder interface is used.
*/

#include "stdafx.h"

#pragma comment (lib, "Ws2_32.lib")

#ifdef _UNICODE
std::string convertToString(_TCHAR* textW)
{
  std::string result;

  int lenA = WideCharToMultiByte(CP_ACP, 0, textW, -1, 0, 0, NULL, NULL);
  printf("lenA = %d\n", lenA);
  if (lenA > 0)
  {
    char* textA  = new char[lenA + 1]; // allocate a final null terminator as well
    printf("textA = %p\n", textA);
    WideCharToMultiByte(CP_ACP, 0, textW, -1, textA, lenA, NULL, NULL);
    printf("%d\n", __LINE__);
    textA[lenA] = 0; // Set the null terminator yourself
    result = textA;
    delete [] textA;
  }

  return result;
}
#endif

int _tmain(int argc, _TCHAR* argv[])
{
  if (argc < 2) {
    printf("ERROR: Missing argument with the IP address of the SFPDP Record Manager Server\n");
    return -1;
  }

  // Initialize Winsock
  WSADATA wsaData;
  int result = WSAStartup(MAKEWORD(2,2), &wsaData);
  if (result != 0) {
    printf("WSAStartup failed with error: %d\n", result);
    return -1;
  }

  int status = 0;

  std::string ipAddress;
#ifdef  _UNICODE
  ipAddress = convertToString(argv[1]);
#else
  ipAddress = argv[1];
#endif
  GEC_ISfpdpRecorder* instance = GEC_SfpdpRecorderFactory::createTcpBasedInstance(ipAddress);
  if (instance) {
    int status = instance->reset();
    printf("reset() returned %d as return code\n", status);

    uint32_t id = 0;
    status = instance->create("/data", "test", 0, 0, id);
    printf("create() returned %d as return code\n", status);
    printf("         and 0x%x as id\n", id);

    status = instance->startAll();
    printf("startAll() returned %d as return code\n", status);

    uint64_t numBytesRecorded = 0;
    std::string state;
    status = instance->getStatus(id, numBytesRecorded, state);
    printf("getStatus() returned %d as return code,\n", status);
    printf("            %ld as number of recorded bytes,\n", numBytesRecorded);
    printf("            and %s as state\n", state.c_str());

    for (int i = 0; i < 12; ++i) {
      Sleep(5000);

      status = instance->getStatus(id, numBytesRecorded, state);
      printf("getStatus() returned %d as return code,\n", status);
      printf("            %ld as number of recorded bytes,\n", numBytesRecorded);
      printf("            and %s as state\n", state.c_str());
    }

    status = instance->stopAll();
    printf("stopAll() returned %d as return code\n", status);

    status = instance->destroy(id);
    printf("destroy() returned %d as return code\n", status);

    GEC_SfpdpRecorderFactory::destroyTcpBasedInstance(instance);
  }

  WSACleanup();

  return status;
}

