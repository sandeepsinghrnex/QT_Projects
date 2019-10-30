/*
 * connect_ssh.c
 * This file contains an example of how to connect to a
 * SSH server using libssh
 */

/*
Copyright 2009 Aris Adamantiadis

This file is part of the SSH Library

You are free to copy this file, modify it in any way, consider it being public
domain. This does not apply to the rest of the library though, but it is
allowed to cut-and-paste working code from this file to any license of
program.
The goal is to show the API in action. It's not a reference on how terminal
clients must be made or how a client should react.
 */

#include <stdio.h>

#include "ssh-common.h"

ssh_session connect_ssh(const char *host, const char *user, const char* password, int verbosity)
{
  ssh_session session;
  int auth=0;

  session=ssh_new();
  if (session == NULL) {
    return NULL;
  }

  if (user && *user != '\0'){
    if (ssh_options_set(session, SSH_OPTIONS_USER, user) < 0) {
      ssh_free(session);
      return NULL;
    }
  }

  if (ssh_options_set(session, SSH_OPTIONS_HOST, host) < 0) {
    ssh_free(session);
    return NULL;
  }

  long timeoutInS = 5;
  if (ssh_options_set(session, SSH_OPTIONS_TIMEOUT, &timeoutInS) < 0) {
    ssh_free(session);
    return NULL;
  }

  ssh_options_set(session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
  if(ssh_connect(session)){
    fprintf(stderr,"Connection failed : %s\n",ssh_get_error(session));
    ssh_disconnect(session);
    ssh_free(session);
    return NULL;
  }
  if(verify_knownhost(session)<0){
    ssh_disconnect(session);
    ssh_free(session);
    return NULL;
  }
  auth=authenticate_console(session, password);
  if(auth==SSH_AUTH_SUCCESS){
    return session;
  } else if(auth==SSH_AUTH_DENIED){
    fprintf(stderr,"Authentication failed\n");
  } else {
    fprintf(stderr,"Error while authenticating : %s\n",ssh_get_error(session));
  }
  ssh_disconnect(session);
  ssh_free(session);
  return NULL;
}
