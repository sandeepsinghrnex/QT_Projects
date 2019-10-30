#include <iostream>

#include "ssh-common.h"

std::string getNextLine(std::string& stringBuffer)
{
  std::string line;

  size_t iNewLine = stringBuffer.find("\n");
  if (iNewLine != std::string::npos) {
    line = stringBuffer.substr(0, iNewLine);
    stringBuffer = stringBuffer.substr(iNewLine + 1);
  }

  return line;
}


int readResponseFromChannel(ssh_channel channel, std::vector<std::string>& response, int isStderr)
{
  char charBuffer[256 + 1];
  std::string stringBuffer;
  int nbytes = ssh_channel_read(channel, charBuffer, sizeof(charBuffer) - 1, isStderr);
  while (nbytes > 0) {
    charBuffer[nbytes] = 0;
    stringBuffer += charBuffer;

    std::string line = getNextLine(stringBuffer);
    while (!line.empty()) {
      response.push_back(line);
      line = getNextLine(stringBuffer);
    }

    nbytes = ssh_channel_read(channel, charBuffer, sizeof(charBuffer) - 1, 0);
  }

  return nbytes;
}

int issue_command(std::string host, std::string user, std::string password,
                  std::string command, 
                  std::vector<std::string>& output,
                  std::vector<std::string>& error)
{
  ssh_session session;
  ssh_channel channel;
  std::string stringBuffer;
  int nbytes;
  int rc;

  session = connect_ssh(host.c_str(), user.c_str(), password.c_str(), SSH_LOG_NOLOG);
  if (session == NULL) {
    ssh_finalize();
    return 1;
  }

  channel = ssh_channel_new(session);
  if (channel == NULL) {
    ssh_disconnect(session);
    ssh_free(session);
    ssh_finalize();
    return 1;
  }

  rc = ssh_channel_open_session(channel);
  if (rc < 0) {
    goto failed;
  }

  rc = ssh_channel_request_exec(channel, command.c_str());
  if (rc < 0) {
    goto failed;
  }

  nbytes = readResponseFromChannel(channel, output, 0);
  if (nbytes < 0) {
    goto failed;
  }
  nbytes = readResponseFromChannel(channel, error, 1);
  if (nbytes < 0) {
    goto failed;
  }

  ssh_channel_send_eof(channel);
  ssh_channel_close(channel);
  ssh_channel_free(channel);
  ssh_disconnect(session);
  ssh_free(session);
  ssh_finalize();

  return 0;
failed:
  ssh_channel_close(channel);
  ssh_channel_free(channel);
  ssh_disconnect(session);
  ssh_free(session);
  ssh_finalize();

  return 1;
}
