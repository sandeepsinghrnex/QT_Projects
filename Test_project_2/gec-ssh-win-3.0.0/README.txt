PREREQUISITES
-------------

Windows:

Microsoft Visual Studio 2010 or later is required to build the examples for
Windows. It should be possible use the Express versions of Visual Studio, but
this is not thoroughly tested.


Linux:

The GNU C++ compiler is required to build the examples.

On Debian 6.0.x this compiler is installed by:

  su
  apt-get install g++
  
The examples also require libssh version 0.4.5 or later.

On Debian 6.0.x this version on libssh must be downloaded from the backports:

  su
  vim /etc/apt/sources.list
  # Add the following line:
  #
  #   deb http://backports.debian.org/debian-backports squeeze-backports main
  apt-get update
  apt-get -t squeeze-backports install libssh-dev


EXAMPLES
========

examples/ssh-common
  Libary providing utility functions that are used by the other
  examples.

examples/ssh-ls
  Example showing how to list the contents of a directory located on
  the remote server.

examples/ssh-rm
  Example showing how a file is deleted on the remote server.
  
examples/ssh-mkdir
  Example showing how a new directory is created on the remote server.

examples/ssh-stat
  Example showing how to determine the amount of free space available
  on a file system on remote server.

examples/ssh-raid0-setup
  Example showing how a new RAID0 device is created on the remote
  server, how a file system is created on the RAID0 device, and how
  the file system is mounted.

examples/ssh-exec
  Example showing how an arbitrary command can be executed on the
  remote server.

examples/ssh-bit
  Example showing how built-in test results are retrieved from a
  Galleon XSR. This example expects Galleon BIT functionality to be
  installed and running on the remote server.
