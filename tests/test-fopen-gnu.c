/* Test of opening a file stream.
   Copyright (C) 2020 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* Written by Bruno Haible <bruno@clisp.org>, 2020.  */

#include <config.h>

/* Specification.  */
#include <stdio.h>

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "macros.h"

#define BASE "test-fopen-gnu.t"

int
main (void)
{
  FILE *f;
  int fd;
  int flags;

  /* Remove anything from prior partial run.  */
  unlink (BASE "file");

  /* Create the file.  */
  f = fopen (BASE "file", "w");
  ASSERT (f);
  fd = fileno (f);
  ASSERT (fd >= 0);
  flags = fcntl (fd, F_GETFD);
  ASSERT (flags >= 0);
  ASSERT ((flags & FD_CLOEXEC) == 0);
  ASSERT (fclose (f) == 0);

  /* Create the file and check the 'e' mode.  */
  f = fopen (BASE "file", "we");
  ASSERT (f);
  fd = fileno (f);
  ASSERT (fd >= 0);
  flags = fcntl (fd, F_GETFD);
  ASSERT (flags >= 0);
  ASSERT ((flags & FD_CLOEXEC) != 0);
  ASSERT (fclose (f) == 0);

  /* Open the file and check the 'x' mode.  */
  f = fopen (BASE "file", "ax");
  ASSERT (f == NULL);
  ASSERT (errno == EEXIST);

  /* Cleanup.  */
  ASSERT (unlink (BASE "file") == 0);

  return 0;
}