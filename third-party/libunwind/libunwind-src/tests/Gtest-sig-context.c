/**
 * Verify that unwinding in a signal handler gets a sane result past the signal
 * trampoline.
 *
 * The sanity of unwinding past the signal trampoline is a matter of making sure
 * the symbol `main` is seen.
 *
 * Both unwinding through the context passed to the signal handler and (if
 * configured for local unwinding) the context retrieved through
 * unw_getcontext() are exercised.
 *
 * This test needs to be built with `-g` (and * unstripped) in order to pass.
 */
/*
 * Copyright 2024 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
 *
 * This file is part of libunwind.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "compiler.h"
#include "libunwind.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static const int max_steps = 10;

int verbose = 0;

void stepper (unw_cursor_t *cursor)
{
  for (int steps = 0; steps < max_steps; ++steps)
    {
      int ret = unw_step (cursor);
      if (ret <= 0)
        {
          fprintf (stderr, "unw_step returned %d after %d steps\n", ret, steps);
          break;
        }

      if (unw_is_signal_frame (cursor))
        {
          if (verbose)
            {
              fprintf (stdout, " stepping through signal trampoline\n");
            }
        }
      else
        {
          char sym[256];
          unw_word_t offset;
          ret = unw_get_proc_name (cursor, sym, sizeof(sym), &offset);
          if (ret == 0)
            {
              if (verbose)
                {
                  fprintf (stdout, " stepping through \"%s\"\n", sym);
                }
              if (strcmp(sym, "main") == 0)
                {
                  if (verbose)
                    {
                      fprintf (stdout, " symbol \"main\" found after %d steps\n", steps);
                    }
                  return;
                }
            }
        }
    }

    fprintf (stderr, "symbol \"main\" not found after %d steps\n", max_steps);
    exit (EXIT_FAILURE);
}

void
handler (int signo UNUSED, siginfo_t *info UNUSED, void *sigcontext)
{
  if (verbose)
    {
      fprintf (stdout, "using signal context:\n");
    }
  unw_cursor_t sig_cursor;
  int ret = unw_init_local2 (&sig_cursor, sigcontext, UNW_INIT_SIGNAL_FRAME);
  if (ret != 0)
    { 
      fprintf (stderr, "error %d in unw_init_local2()\n", ret);
      exit (EXIT_FAILURE);
    } 
  stepper (&sig_cursor);

#if defined(UNW_LOCAL_ONLY)
  if (verbose)
    {
      fprintf (stdout, "using unw_context:\n");
    }
  unw_context_t context;
  ret = unw_getcontext (&context);
  if (ret != 0)
    {
      fprintf (stderr, "error %d in unw_getcontext()\n", ret);
      exit (EXIT_FAILURE);
    }
  unw_cursor_t cursor;
  ret = unw_init_local (&cursor, &context);
  if (ret != 0)
    {
      fprintf (stderr, "error %d in unw_init_local()\n", ret);
      exit (EXIT_FAILURE);
    }
  stepper (&cursor);
#endif /* !defined(UNW_LOCAL_ONLY) */
}

int main (int argc, char *argv[] UNUSED)
{
  struct sigaction a;
  memset (&a, 0, sizeof (struct sigaction));
  a.sa_sigaction = &handler;
  a.sa_flags = SA_SIGINFO;
  sigaction (SIGHUP, &a, NULL);

  verbose = (argc > 1);

  kill (getpid (), SIGHUP);
  exit (EXIT_SUCCESS);
}
