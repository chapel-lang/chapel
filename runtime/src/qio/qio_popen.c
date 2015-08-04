/*
 * Copyright 2004-2015 Cray Inc.
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

#include "qio.h"
#include "sys.h"

#include "qio_popen.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <spawn.h>

// current environment variables
extern char** environ;

struct subprocess {
};

// The "fd" for popen
struct popen_file {
  pid_t pid; // the child process ID
  int fd; // the input or output pipe
  char* command; // what to report in getpath
};

static inline
struct popen_file* to_popen_file(void* ptr) {
  return (struct popen_file*) ptr;
}

static
qioerr popen_writev(void* fl, const struct iovec* iov, int iovcnt, ssize_t* num_written_out, void* fs)
{
  return qio_int_to_err(
      sys_writev(to_popen_file(fl)->fd, iov, iovcnt, num_written_out));
}


static
qioerr popen_readv (void* fl, const struct iovec *iov, int iovcnt, ssize_t* num_read_out, void* fs)
{
  return qio_int_to_err(
      sys_readv(to_popen_file(fl)->fd, iov, iovcnt, num_read_out));
}



static
qioerr popen_close(void* fl, void* fs)
{
  qioerr err = 0;
  struct popen_file* pfl = to_popen_file(fl);
  pid_t pid = pfl->pid;
  int status = 127;
  pid_t got_pid = 0;

  // Close the file descriptor.
  sys_close(pfl->fd);
  pfl->fd = -1;

  STARTING_SLOW_SYSCALL;

  // Wait for the process to exit.
  if( pid != -1 ) {
    do {
      got_pid = waitpid( pid, &status, 0 );
    } while( got_pid == -1 && errno == EINTR );

    pfl->pid = -1;

    if( got_pid == -1 ) {
      err = qio_int_to_err(errno);
    } else {
      if( WIFEXITED( status ) ) status = WEXITSTATUS(status);
      err = 0;
    }
  }

  DONE_SLOW_SYSCALL;

  qio_free(pfl->command);
  qio_free(pfl);

  // TODO *status_out = status;
  return err; 
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
  } else if( *std__fd == QIO_FD_PIPE ) {
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
    assert(0);
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
    >0 -> use this file discriptor
   When this function returns, any file descriptors with -1
   will have their value replaced with the new pipe file descriptor.

   executable == NULL or "" -> search the path for argv[0]

 */
qioerr qio_openproc(const char** argv,
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
  struct popen_file* pfl = NULL;
  int in_pipe[2];
  int out_pipe[2];
  int err_pipe[2];
  posix_spawn_file_actions_t actions;
  bool inited_actions = false;
  bool hasactions = false;
  const char* progname;

  /*
  char** use_argv;
  char** use_envp;

  // Copy argv and envp into NULL-terminated versions.
  use_argv = qio_calloc(n_argv+1, sizeof(char*));

  if( ! use_argv ) {
    err = QIO_ENOMEM;
    goto error;
  }

  for( i = 0; i < n_argv; i++ ) {
    use_argv[i] = qio_strdup(argv[i]);
  }
  use_argv[n_argv] = NULL; // argv must be NULL-terminated.

  if( n_envp == 0 ) use_envp = NULL;
  else {
    use_envp = qio_calloc(n_envp+1, sizeof(char*));

    if( ! use_envp ) {
      err = QIO_ENOMEM;
      goto error;
    }

    for( i = 0; i < n_envp; i++ ) {
      use_envp[i] = qio_strdup(envp[i]);
    }
    use_envp[n_argv] = NULL; // argv must be NULL-terminated.
  } */

  progname = argv[0];
  if( executable && executable[0] ) progname = executable;

  in_pipe[0] = in_pipe[1] = -1;
  out_pipe[0] = out_pipe[1] = -1;
  err_pipe[0] = err_pipe[1] = -1;

  STARTING_SLOW_SYSCALL;

  // Create pipes

  if( *stdin_fd == QIO_FD_PIPE ) {
    rc = pipe(in_pipe);
    if( rc != 0 ) {
      err = qio_int_to_err(errno);
      goto error;
    }
  }
  if( *stdout_fd == QIO_FD_PIPE ) {
    rc = pipe(out_pipe);
    if( rc != 0 ) {
      err = qio_int_to_err(errno);
      goto error;
    }
  }
  if( *stderr_fd == QIO_FD_PIPE ) {
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
  if( *stderr_fd == QIO_FD_TO_STDOUT && *stdout_fd == QIO_FD_PIPE ) {
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

  /*
  // Free the argument and environment copies.
  if( use_argv ) {
    for( i = 0; i < n_argv; i++ ) {
      qio_free(use_argv[i]);
    }
    qio_free(use_argv);
    use_argv = NULL;
  }
  if( use_envp ) {
    for( i = 0; i < n_envp; i++ ) {
      qio_free(use_envp[i]);
    }
    qio_free(use_envp);
    use_envp = NULL;
  }*/

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

  if( pfl ) {
    qio_free(pfl->command);
    qio_free(pfl);
  }

  return err;
}

// waitpid
qioerr qio_waitpid(int64_t pid,
                   int blocking, int* done, int* exitcode)
{
  int status = 0;
  int flags = 0;
  pid_t got;

  if( ! blocking ) flags |= WNOHANG;

  got = waitpid((pid_t) pid, &status, flags);
  if( got == -1 ) {
    return qio_int_to_err(errno);
  }

  if( WIFEXITED(status) ) {
    *exitcode = WEXITSTATUS(status);
    if( WIFSIGNALED(status) ) {
      *exitcode = - WSTOPSIG(status);
    }
    *done = 1;
  }
  return 0;
}


// commit input, sending any data to the subprocess.
// While sending that data, read output and error channels.
qioerr qio_proc_communicate(
    const int threadsafe,
    qio_channel_t* restrict input,
    qio_channel_t* restrict output,
    qio_channel_t* restrict error) {

  qioerr err;
  int rc;
  bool do_input;
  bool do_output;
  bool do_error;
  fd_set rfds, wfds, efds;
  int nfds = 1;

  int input_fd = -1;
  int output_fd = -1;
  int error_fd = -1;


  if( threadsafe ) {
    // lock all three channels.
    if( input ) {
      err = qio_lock(&input->lock);
      if( err ) return err;
    }
    if( output ) {
      err = qio_lock(&output->lock);
      if( err ) {
        if( input ) qio_unlock(&input->lock);
        return err;
      }
    }
    if( error ) {
      err = qio_lock(&error->lock);
      if( err ) {
        if( input ) qio_unlock(&input->lock);
        if( output ) qio_unlock(&output->lock);
        return err;
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
    if( do_input ) {
      err = _qio_channel_flush_qio_unlocked(input);
      if( !err ) do_input = false;
      if( qio_err_to_int(err) == EAGAIN ) err = 0;
      if( err ) break;
    }

    if( do_output ) {
      // read some into our buffer.
      err = qio_channel_advance(false, output, qbytes_iobuf_size);
      if( qio_err_to_int(err) == EEOF ) do_output = false;
      if( qio_err_to_int(err) == EAGAIN ) err = 0;
      if( err ) break;
    }

    if( do_error ) {
      // read some into our buffer.
      err = qio_channel_advance(false, error, qbytes_iobuf_size);
      if( qio_err_to_int(err) == EEOF ) do_error = false;
      if( qio_err_to_int(err) == EAGAIN ) err = 0;
      if( err ) break;
    }

    // Now call select to wait for one of the descriptors to
    // become ready.

    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    FD_ZERO(&efds);

    if( do_input ) {
      FD_SET(input_fd, &wfds);
      FD_SET(input_fd, &efds);
    }
    if( do_output ) {
      FD_SET(output_fd, &rfds);
      FD_SET(output_fd, &efds);
    }
    if( do_error ) {
      FD_SET(error_fd, &rfds);
      FD_SET(error_fd, &efds);
    }
    rc = select(nfds, &rfds, &wfds, &efds, NULL);

    if( rc == -1 ) {
      err = qio_int_to_err(errno);
      break;
    }

    if( rc == 0 ) break;

    /* ... we don't actually care about which fds are ready,
           we'll just try to work with all of them each time through the
           loop.
    if(do_input && (FD_ISSET(input_fd, &wfds) || FD_ISSET(input_fd, &efds)))
    if(do_output &&(FD_ISSET(output_fd, &wfds) || FD_ISSET(output_fd, &efds)))
    if( do_error &&(FD_ISSET(error_fd, &wfds) || FD_ISSET(error_fd, &efds)))
    */
  }

  // revert the output and error channels
  if( output) qio_channel_revert_unlocked(output);
  if( error ) qio_channel_revert_unlocked(error);


  // we could close the file descriptors at this point,
  // but we don't because we don't want to modify
  // the file descriptor of the file since it's
  // constant (and not protected by file's lock).
  // The pipes will be closed when the channels are destroyed.

  // mark the output and error channels so that we
  // can just keep advancing to read while buffering
  // up all data. Before returning, we'll revert them
  // so that this buffered data can be read again and
  // update end_pos so that the channel knows not to
  // call read again. Then, we can close the file
  // descriptor.

  if( threadsafe ) {
    // unlock all three channels.
    if( error ) qio_unlock(&error->lock);
    if( output ) qio_unlock(&output->lock);
    if( input ) qio_unlock(&input->lock);
  }

  return err;
}

static
qioerr popen_getpath(void* fl, const char** string_out, void* fs)  {

  *string_out = qio_strdup(to_popen_file(fl)->command);
  if( ! *string_out ) return QIO_ENOMEM;
  return 0;
}

static
qio_file_functions_t popen_function_struct = {
  &popen_writev,
  &popen_readv,
  NULL, // No pwritev for popen, no seeking!
  NULL, // No preadv for popen, no seeking!
  &popen_close,
  NULL, // &popen_open, TODO
  NULL, // No seek
  NULL, // No length
  &popen_getpath,
  NULL, // No fsync - no stable storage
  NULL, // No getcwd
  NULL, // No get_fs_type
  NULL, // No file chunk size
  NULL, // No locality information
};

const qio_file_functions_ptr_t popen_function_struct_ptr = &popen_function_struct;

