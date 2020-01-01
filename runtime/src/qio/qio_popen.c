/*
 * Copyright 2004-2020 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "sys_basic.h"

#ifndef CHPL_RT_UNIT_TEST
#include "chplrt.h"
#endif

#include "chpl-mem-sys.h" // need to call system allocator
#include "qio.h"
#include "sys.h"

#include "qio_popen.h"

#include <sys/select.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <spawn.h>

#include <pthread.h>

// current environment variables
extern char** environ;

/* Helper routines for allocating out of the C
   heap instead of the Chapel one. That is necessary
   since the Chapel heap might not be available to
   a forked process, and we need args for exec
   (command, arguments, environment) to be available
   after a fork.

   Using these routines resolves problems with
    * Gasnet/aries with segment=fast
    * Gasnet/udp with segment=fast
 */

const char* qio_spawn_strdup(const char* str)
{
  size_t len = strlen(str);
  char* ret = sys_malloc(len + 1);
  // note: also copies '\0' at end of string.
  memcpy(ret, str, len + 1);
  return ret;
}

const char** qio_spawn_allocate_ptrvec(size_t count)
{
  char** ret = sys_calloc(count, sizeof(char*));
  return (const char**) ret;
}

void qio_spawn_free_ptrvec(const char** args)
{
  sys_free((void*) args);
}

void qio_spawn_free_str(const char* str)
{
  sys_free((void*) str);
}


/* Set up file actions for posix_spawn.
   *std__fd is FD_FORWARD, FD_CLOSE, FD_PIPE etc or a file descriptor #
   pipe[2] is the pipe created for FD_PIPE
   childfd is 0 (for stdin) 1 (for stdout) 2 (for stderr)
   input_to_child is 1 for stdin, 0 otherwise
   *hasactions is set on output if we added any file actions
   */
static qioerr setup_actions(
    posix_spawn_file_actions_t * actions,
    int* std__fd, int pipe[2], int childfd,
    int input_to_child,
    bool * hasactions)
{
  int pipe_parent_end;
  int pipe_child_end;
  int rc;

  assert(childfd == 0 || childfd == 1 || childfd == 2);

  // for stdin, the parent end is pipe[1], child end is pipe[0];
  // for everything else, the parent end is pipe[0], child end is pipe[1].
  if( input_to_child ) {
    pipe_parent_end = pipe[1];
    pipe_child_end = pipe[0];
  } else {
    pipe_parent_end = pipe[0];
    pipe_child_end = pipe[1];
  }

  if( *std__fd == QIO_FD_FORWARD ) {
    // Do nothing. Assume file descriptor childfd does not have close-on-exec.
  } else if( *std__fd == QIO_FD_PIPE || *std__fd == QIO_FD_BUFFERED_PIPE ) {
    // child can't write to the parent end of the pipe.
    rc = posix_spawn_file_actions_addclose(actions, pipe_parent_end);
    if( rc ) return qio_int_to_err(errno);

    // child needs to use its end of the pipe as fd childfd.
    rc = posix_spawn_file_actions_adddup2(actions, pipe_child_end, childfd);
    if( rc ) return qio_int_to_err(errno);

    // then close the pipe we dup'd since it is now known by another
    // name (e.g. fd 0).
    posix_spawn_file_actions_addclose(actions, pipe_child_end);
    *hasactions = true;
  } else if( *std__fd == QIO_FD_CLOSE ) {
    // close stdin.
    rc = posix_spawn_file_actions_addclose(actions, childfd);
    if( rc ) return qio_int_to_err(errno);
    *hasactions = true;
  } else if( *std__fd == QIO_FD_TO_STDOUT ) {
    // Do nothing.

  } else {
    // Use a given file descriptor for childfd (e.g. stdin).
    rc = posix_spawn_file_actions_adddup2(actions, *std__fd, childfd);
    if( rc ) return qio_int_to_err(errno);
    // then close the pipe we dup'd
    rc = posix_spawn_file_actions_addclose(actions, *std__fd);
    if( rc ) return qio_int_to_err(errno);
    *hasactions = true;
  }

  return 0;
}

/* Each of stdin/stdout/stderr take in a pointer to a file descriptor.
   *ptr should be:
    -1 -> use the existing stdin/stdout
    -2 -> close the file descriptor
    -3 -> create a pipe and return the parent's end
    >0 -> use this file descriptor
   When this function returns, any file descriptors with a negative
   value indicating a pipe should be created (-3)
   will have their value replaced with the new pipe file descriptor.

   executable == NULL or "" -> search the path for argv[0]

 */
static
qioerr qio_do_openproc(const char** argv,
                    const char** envp,
                    const char* executable,
                    int* stdin_fd,
                    int* stdout_fd,
                    int* stderr_fd,
                    int64_t *pid_out)
{
  qioerr err;
  int rc;
  pid_t pid = 0;
  //struct popen_file* pfl = NULL;
  int in_pipe[2];
  int out_pipe[2];
  int err_pipe[2];
  posix_spawn_file_actions_t actions;
  bool inited_actions = false;
  bool hasactions = false;
  const char* progname;

  progname = argv[0];
  if( executable && executable[0] ) progname = executable;

  in_pipe[0] = in_pipe[1] = -1;
  out_pipe[0] = out_pipe[1] = -1;
  err_pipe[0] = err_pipe[1] = -1;

  STARTING_SLOW_SYSCALL;

  // Create pipes

  if( *stdin_fd == QIO_FD_PIPE || *stdin_fd == QIO_FD_BUFFERED_PIPE ) {
    rc = pipe(in_pipe);
    if( rc != 0 ) {
      err = qio_int_to_err(errno);
      goto error;
    }
  }
  if( *stdout_fd == QIO_FD_PIPE || *stdout_fd == QIO_FD_BUFFERED_PIPE ) {
    rc = pipe(out_pipe);
    if( rc != 0 ) {
      err = qio_int_to_err(errno);
      goto error;
    }
  }
  if( *stderr_fd == QIO_FD_PIPE || *stderr_fd == QIO_FD_BUFFERED_PIPE ) {
    rc = pipe(err_pipe);
    if( rc != 0 ) {
      err = qio_int_to_err(errno);
      goto error;
    }
  }

  // pipe[0] is the read end of the pipe
  // pipe[1] is the write end

  // fd 0 is stdin, 1 is stdout, 2 is stderr.

  // In order for Cygwin support to work, because Windows
  // doesn't have a 'fork' call, we use posix_spawn.

  // Note that posix_spawn *might* be faster on Linux if
  // we don't specify any file actions, since it can use
  // vfork and so avoid duplicating page tables.

  // To request vfork for posix_spawn on linux/glibc,
  // set the spawn attr POSIX_SPAWN_USEVFORK.
  // It is unclear whether or not the linux implementation
  // will work correctly when combining POSIX_SPAWN_USEVFORK
  // with file actions (e.g. to make a pipe for stdin/stdout).

  // If we seek to improve performance on linux, here are some options:
  //  * POSIX_SPAWN_USEVFORK
  //  * madvise(MADV_DONTFORK) on the Chapel heap
  //  * implement fork/dup/exec with clone instead of fork on linux
  //    (clone can avoid copying page tables).

  // Note: posix_spawn can use file descriptors that have
  // close-on-exec set as long as they are dup'd in a file action.

  rc = posix_spawn_file_actions_init(&actions);
  if( rc ) {
    err = qio_int_to_err(errno);
    goto error;
  }

  inited_actions = true;
  hasactions = false;

  err = setup_actions(&actions, stdin_fd, in_pipe, 0, true, &hasactions);
  if( err ) goto error;
  err = setup_actions(&actions, stdout_fd, out_pipe, 1, false, &hasactions);
  if( err ) goto error;

  // handle QIO_FD_TO_STDOUT
  if( *stderr_fd == QIO_FD_TO_STDOUT &&
      (*stdout_fd == QIO_FD_PIPE || *stdout_fd == QIO_FD_BUFFERED_PIPE) ) {
    // forward stderr to stdout.
    err = setup_actions(&actions, stderr_fd, out_pipe, 2, false, &hasactions);
    if( err ) goto error;
  } else {
    if( *stderr_fd == QIO_FD_TO_STDOUT ) {
      if( *stdout_fd == QIO_FD_FORWARD ) {
        *stderr_fd = 1; // forward to parent's stdout.
      } else {
        // e.g. it's a file descriptor number.
        *stderr_fd = *stdout_fd;
      }
    }
    err = setup_actions(&actions, stderr_fd, err_pipe, 2, false, &hasactions);
    if( err ) goto error;
  }

  // spawn the subprogram, searching the path, returning the pid in pid
  rc = posix_spawnp(&pid, progname,
                   hasactions?(&actions):(NULL), /* file actions */
                   NULL, /* attributes */
                   (char*const*) argv,
                   // Use the current environment if none was specified.
                   (envp==NULL)?(environ):((char*const*) envp) );
  if( rc != 0 ) {
    err = qio_int_to_err(rc);
    goto error;
  }

  // destroy file actions, ignoring return code.
  posix_spawn_file_actions_destroy(&actions);
  inited_actions = false;

  // close the child-side of pipes. Return the parent side.
  if( in_pipe[0] != -1 ) {
    close(in_pipe[0]);
    *stdin_fd = in_pipe[1];
  }
  if( out_pipe[0] != -1 ) {
    close(out_pipe[1]);
    *stdout_fd = out_pipe[0];
  }
  if( err_pipe[0] != -1 ) {
    close(err_pipe[1]);
    *stderr_fd = err_pipe[0];
  }

  // return the pid.
  *pid_out = (int64_t) pid;

  DONE_SLOW_SYSCALL;

  return 0;

error:
  DONE_SLOW_SYSCALL;
  // intentionally ignoring error returns here...
  if( inited_actions ) posix_spawn_file_actions_destroy(&actions);

  if( in_pipe[0] ) close(in_pipe[0]);
  if( in_pipe[1] ) close(in_pipe[1]);
  if( out_pipe[0] ) close(out_pipe[0]);
  if( out_pipe[1] ) close(out_pipe[1]);
  if( err_pipe[0] ) close(err_pipe[0]);
  if( err_pipe[1] ) close(err_pipe[1]);

  return err;
}

struct openproc_args_s {
  const char** argv;
  const char** envp;
  const char* executable;
  int* stdin_fd;
  int* stdout_fd;
  int* stderr_fd;
  int64_t* pid_out;
  qioerr err;
};

static
void* qio_openproc_wrapper(void* arg)
{
  struct openproc_args_s* s = (struct openproc_args_s*) arg;
  s->err = qio_do_openproc(s->argv, s->envp, s->executable,
                           s->stdin_fd, s->stdout_fd, s->stderr_fd,
                           s->pid_out);
  return NULL;
}

qioerr qio_openproc(const char** argv,
                    const char** envp,
                    const char* executable,
                    int* stdin_fd,
                    int* stdout_fd,
                    int* stderr_fd,
                    int64_t *pid_out)
{
  // runs qio_do_openproc in a pthread in order
  // to avoid issues where a Chapel task is allocated
  // from memory with MAP_SHARED.
  //
  // If such a thread is the thread running fork(),
  // after the fork() occurs, there will be 2 threads
  // sharing the same stack.
  //
  // If it mattered, we could do this extra step
  // only for configurations where the Chapel heap
  // has this problem (GASNet with SEGMENT=fast,large
  // and possibly others).

  int rc;
  pthread_t thread;
  struct openproc_args_s s;

  s.argv = argv;
  s.envp = envp;
  s.executable = executable;
  s.stdin_fd = stdin_fd;
  s.stdout_fd = stdout_fd;
  s.stderr_fd = stderr_fd;
  s.pid_out = pid_out;
  s.err = 0;

  rc = pthread_create(&thread, NULL, qio_openproc_wrapper, &s);
  if (rc)
    QIO_RETURN_CONSTANT_ERROR(EAGAIN, "failed pthread_create in qio_openproc");

  rc = pthread_join(thread, NULL);
  if (rc)
    QIO_RETURN_CONSTANT_ERROR(EAGAIN, "failed pthread_join in qio_openproc");

  return s.err;
}


// waitpid
qioerr qio_waitpid(int64_t pid,
                   int blocking, int* done, int* exitcode)
{
  int status = 0;
  int flags = 0;
  pid_t got;

  flags |= WNOHANG;

  do {
    got = waitpid((pid_t) pid, &status, flags);
    if ( got == -1 && errno == EINTR ) {
      // Try again is a non-blocking wait was interrupted
      got = 0;
    }
    if ( ! blocking ) {
      break;
    }
    chpl_task_yield();
  } while (got == 0);

  // Check for error
  if( got == -1 ) {
    return qio_int_to_err(errno);
  }
  // Only update (done, exitcode) if waitpid() returned for the desired pid
  else if ( got == pid ) {
    if( WIFEXITED(status) ) {
      *exitcode = WEXITSTATUS(status);
      *done = 1;
    }
    else if( WIFSIGNALED(status) ) {
      *exitcode = -WTERMSIG(status);
      *done = 1;
    }
  }

  return 0;
}

// commit input, sending any data to the subprocess.
// once input is sent, close input channel and file.
// While sending that data, read output and error channels,
// buffering up the read data.
// once output/error reads EOF, close output/error file (not channel
// since output channel has the buffered data).
qioerr qio_proc_communicate(
    const int threadsafe,
    qio_channel_t* input,
    qio_channel_t* output,
    qio_channel_t* error) {

  qioerr err = 0;
  int rc = 0;
  bool do_input;
  bool do_output;
  bool do_error;
  bool input_ready;
  bool output_ready;
  bool error_ready;
  fd_set rfds, wfds, efds;
  int nfds = 1;

  int input_fd = -1;
  int output_fd = -1;
  int error_fd = -1;

  if( threadsafe ) {
    // lock all three channels.
    // but unlock them immediately and set them to NULL
    // if they are already closed.
    if( input ) {
      err = qio_lock(&input->lock);
      if( err ) return err;
      if( qio_channel_isclosed(false, input) ) {
        qio_unlock(&input->lock);
        input = NULL;
      }
    }
    if( output ) {
      err = qio_lock(&output->lock);
      if( err ) {
        if( input ) qio_unlock(&input->lock);
        return err;
      }
      if( qio_channel_isclosed(false, output) ) {
        qio_unlock(&output->lock);
        output = NULL;
      }
    }
    if( error ) {
      err = qio_lock(&error->lock);
      if( err ) {
        if( input ) qio_unlock(&input->lock);
        if( output ) qio_unlock(&output->lock);
        return err;
      }
      if( qio_channel_isclosed(false, error) ) {
        qio_unlock(&error->lock);
        error = NULL;
      }
    }
  }

  if( input ) {
    input_fd = input->file->fd;
    if( nfds <= input_fd ) nfds = input_fd + 1;
  }
  if( output ) {
    output_fd = output->file->fd;
    if( nfds <= output_fd ) nfds = output_fd + 1;
  }
  if( error ) {
    error_fd = error->file->fd;
    if( nfds <= error_fd ) nfds = error_fd + 1;
  }

  // Adjust all three pipes to be non-blocking.
  if( input_fd != -1 ) {
    rc = fcntl(input_fd, F_SETFL, O_NONBLOCK);
    if( rc == -1 ) {
      err = qio_int_to_err(errno);
    }
  }
  if( output_fd != -1 ) {
    rc = fcntl(output_fd, F_SETFL, O_NONBLOCK);
    if( rc == -1 ) {
      err = qio_int_to_err(errno);
    }
  }
  if( error_fd != -1 ) {
    rc = fcntl(error_fd, F_SETFL, O_NONBLOCK);
    if( rc == -1 ) {
      err = qio_int_to_err(errno);
    }
  }

  // mark the output and error channels so that we
  // can just keep advancing to read while buffering
  // up all data. Before returning, we'll revert them
  // so that this buffered data can be read again and
  // update end_pos so that the channel knows not to
  // call read again. Then, we can close the file
  // descriptor.
  if( output ) {
    qio_channel_mark(false, output);
  }
  if( error ) {
    qio_channel_mark(false, error);
  }

  do_input = (input != NULL);
  do_output = (output != NULL);
  do_error = (error != NULL);

  while( do_input || do_output || do_error ) {

    // Now call select to wait for one of the descriptors to
    // become ready.

    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    FD_ZERO(&efds);

    if( do_input && input_fd != -1 ) {
      FD_SET(input_fd, &wfds);
      FD_SET(input_fd, &efds);
    }
    if( do_output && output_fd != -1 ) {
      FD_SET(output_fd, &rfds);
      FD_SET(output_fd, &efds);
    }
    if( do_error && error_fd != -1 ) {
      FD_SET(error_fd, &rfds);
      FD_SET(error_fd, &efds);
    }

    input_ready = false;
    output_ready = false;
    error_ready = false;

    // Run select to wait for something
    if( do_input || do_output || do_error ) {
      // TODO -- use sys_select so threading can interact
      struct timeval t;
      t.tv_sec = 0;
      t.tv_usec = 10;
      rc = select(nfds, &rfds, &wfds, &efds, &t);
      if (rc > 0) {
        // read ready file descriptors
        input_ready = input_fd != -1 && FD_ISSET(input_fd, &wfds);
        output_ready = output_fd != -1 && FD_ISSET(output_fd, &rfds);
        error_ready = error_fd != -1 && FD_ISSET(error_fd, &rfds);
      }
      // Ignore EAGAIN and EINTR
      if (rc == EAGAIN || rc == EINTR) rc = 0;
    }

    if( rc == -1 ) {
      err = qio_int_to_err(errno);
      break;
    }

    if( do_input && input_ready ) {
      err = _qio_channel_flush_qio_unlocked(input);
      if( !err ) {
        do_input = false;
        // Close input channel.
        err = qio_channel_close(false, input);
      }
      if( qio_err_to_int(err) == EAGAIN ) err = 0;
      if( err ) break;
    }

    if( do_output && output_ready ) {
      // read some into our buffer.
      err = qio_channel_advance(false, output, qbytes_iobuf_size);
      if( qio_err_to_int(err) == EEOF ) {
        qio_file_t* output_file = qio_channel_get_file(output);

        do_output = false;
        // close the output file (not channel), in case closing output
        // causes the program to output on stderr, e.g.
        if( output_file )
          err = qio_file_close(output_file);
        // Set the output channel maximum position
        // This prevents a read on output from trying to get
        // more data from the (now closed) file.
        output->end_pos = qio_channel_offset_unlocked(output);
      }
      if( qio_err_to_int(err) == EAGAIN ) err = 0;
      if( err ) break;
    }

    if( do_error && error_ready ) {
      // read some into our buffer.
      err = qio_channel_advance(false, error, qbytes_iobuf_size);
      if( qio_err_to_int(err) == EEOF ) {
        qio_file_t* error_file = qio_channel_get_file(error);

        do_error = false;
        // close the error file (not channel)
        if( error_file )
          err = qio_file_close(error_file);
        // Set the error channel maximum position
        error->end_pos = qio_channel_offset_unlocked(error);
      }
      if( qio_err_to_int(err) == EAGAIN ) err = 0;
      if( err ) break;
    }

    chpl_task_yield();
  }

  // we could close the file descriptors at this point,
  // but we don't because we don't want to modify
  // the file descriptor of the file since it's
  // constant (and not protected by file's lock).
  // The pipes will be closed when the channels are destroyed.

  // We marked the output and error channels so that we
  // can just keep advancing to read while buffering
  // up all data. Before returning, we'll revert them
  // so that this buffered data can be read again and
  // update end_pos so that the channel knows not to
  // call read again. Then, we can close the file
  // descriptor.

  // revert the output and error channels
  if( output) qio_channel_revert_unlocked(output);
  if( error ) qio_channel_revert_unlocked(error);


  if( threadsafe ) {
    // unlock all three channels.
    if( error ) qio_unlock(&error->lock);
    if( output ) qio_unlock(&output->lock);
    if( input ) qio_unlock(&input->lock);
  }

  return err;
}

// Send a signal to the specified pid
qioerr qio_send_signal(int64_t pid, int sig)
{
  qioerr err = 0;

  int rc = kill(pid, sig);
  if (rc == -1)
    err = qio_mkerror_errno();

  return err;
}
