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


/*
 */
module Spawn {

  private extern proc qio_openproc(argv:c_ptr(c_string),
                                   env:c_ptr(c_string),
                                   executable:c_string,
                                   ref stdin_fd:c_int,
                                   ref stdout_fd:c_int,
                                   ref stderr_fd:c_int,
                                   ref pid:int(64)):syserr;
  private extern proc qio_waitpid(pid:int(64),
    blocking:c_int, ref done:c_int, ref exitcode:c_int):syserr;

  // When spawning, we need to allocate the command line
  // and environment to spawn with the C allocator (instead
  // of the Chapel one) because the Chapel heap might not
  // be accessible to a forked process.
  // So, we have here some functions that work with
  // the C allocator instead of the Chapel one.

  private extern proc qio_spawn_strdup(str: c_string): c_string;
  private extern proc qio_spawn_allocate_args(count: size_t): c_ptr(c_string);
  private extern proc qio_spawn_free_args(args: c_ptr(c_string));
  private extern proc qio_spawn_free_str(str: c_string);


  /* A subprocess */
  record subprocess {
    param kind:iokind;
    param locking:bool;

    var home:locale;

    var pid:int(64);
    var inputfd:c_int;
    var outputfd:c_int;
    var errorfd:c_int;

   
    /* is the subprocess still running? */
    var running:bool;
    /* what is its exit status? */
    var exit_status:int;

    // the channels
    var stdin_pipe:bool;
    var stdin_buffering:bool;
    var stdin_file:file;
    var stdin:channel(writing=true, kind=kind, locking=locking);
    var stdout_pipe:bool;
    var stdout_file:file;
    var stdout:channel(writing=false, kind=kind, locking=locking);
    var stderr_pipe:bool;
    var stderr_file:file;
    var stderr:channel(writing=false, kind=kind, locking=locking);

    // Ideally we don't have the _file versions, but they
    // are there now because of issues with when the reference counts
    // for the file are updated.

    // an error
    var spawn_error:syserr;
  }

  private extern const QIO_FD_FORWARD:c_int;
  private extern const QIO_FD_CLOSE:c_int;
  private extern const QIO_FD_PIPE:c_int;
  private extern const QIO_FD_TO_STDOUT:c_int;

  const FORWARD = QIO_FD_FORWARD;
  const CLOSE = QIO_FD_CLOSE;
  const PIPE = QIO_FD_PIPE;
  const STDOUT = QIO_FD_TO_STDOUT;


  private const empty_env:[1..0] string;

  private proc get_empty_env() {
    const ret:[1..0] string;
    return ret;
  }

  /* TODO:
     stdin stdout and stderr can be PIPE, existing file descriptor,
     existing file opject, or None. and stderr can be STDOUT which
     indicates stderr -> stdout.

     What about a string for a file path? To support that, use
     arguments like this: stdin:?t = FORWARD 

     * forward it -> posix_spawn_file_actions_adddup2
     * close it -> posix_spawn_file_actions_addclose
     * string for file path -> posix_spawn_file_actions_addopen
     * open file descriptor ->  posix_spawn_file_actions_adddup2 
                 (dup it to remove close-on-exec flag)
     * in-memory file, HDFS file, etc that don't have
       a file descriptor.

     Note -- it's not possible to thread-safe change directory
     when spawning (unless we fork twice e.g.) since posix_spawn
     does not have a change-directory file action. We could support
     a cwd argument like Python does, but it wouldn't be thread-safe
     without heroic effort on our part.  Boo.
   */
  /*
     */
  proc spawn(args:[] string, env:[] string=Spawn.empty_env, executable="",
             stdin:?t = FORWARD, stdout:?u = FORWARD, stderr:?v = FORWARD,
             param kind=iokind.dynamic, param locking=true)
  {
    var stdin_fd:c_int;
    var stdout_fd:c_int;
    var stderr_fd:c_int;
    var stdin_pipe = false;
    var stdout_pipe = false;
    var stderr_pipe = false;
    var pid:int;
    var err:syserr;

    if isIntegralType(stdin.type) then stdin_fd = stdin;
    else compilerError("only FORWARD/CLOSE/PIPE/STDOUT supported"); 
    if isIntegralType(stdout.type) then stdout_fd = stdout;
    else compilerError("only FORWARD/CLOSE/PIPE/STDOUT supported"); 
    if isIntegralType(stderr.type) then stderr_fd = stderr;
    else compilerError("only FORWARD/CLOSE/PIPE/STDOUT supported"); 

    if stdin == QIO_FD_PIPE then stdin_pipe = true;
    if stdout == QIO_FD_PIPE then stdout_pipe = true;
    if stderr == QIO_FD_PIPE then stderr_pipe = true;

    // Create the C pointer structures appropriate for spawn/exec
    // that are NULL terminated and consist of C strings.

    var nargs = args.size + 1;
    var use_args = qio_spawn_allocate_args( nargs.safeCast(size_t) );
    for (a,i) in zip(args, 0..) {
      use_args[i] = qio_spawn_strdup(a.c_str());
    }
    var use_env:c_ptr(c_string) = nil;
    if env.size != 0 {
      var nenv = env.size + 1;
      use_env = qio_spawn_allocate_args( nenv.safeCast(size_t) );
      for (a,i) in zip(env, 0..) {
        use_env[i] = qio_spawn_strdup(a.c_str());
      }
    }

    pid = -1;

    err = qio_openproc(use_args, use_env, executable.c_str(),
                       stdin_fd, stdout_fd, stderr_fd, pid);

    // free the c structures we created.
    for i in 0..#args.size {
      qio_spawn_free_str(use_args[i]);
    }
    for i in 0..#env.size {
      qio_spawn_free_str(use_env[i]);
    }
    qio_spawn_free_args(use_args);
    qio_spawn_free_args(use_env);

    var ret = new subprocess(kind=kind, locking=locking,
                             home=here,
                             pid=pid,
                             inputfd=stdin_fd,
                             outputfd=stdout_fd,
                             errorfd=stderr_fd,
                             running=true, exit_status=256);
 
    if err {
      ret.running = false;
      ret.exit_status = 257;
      ret.spawn_error = err;
      return ret;
    }
    ret.spawn_error = ENOERR;

    // open the QIO files if a pipe was used.

    // (we could use open_usr to enable the getpath function
    //  to return the command name).

    if stdin_pipe {
      ret.stdin_pipe = true;
      ret.stdin_file = openfd(stdin_fd, error=err, hints=QIO_HINT_OWNED);
      if err {
        ret.spawn_error = err; return ret;
      }
      ret.stdin = ret.stdin_file.writer(error=err);
      if err {
        ret.spawn_error = err; return ret;
      }

      if stdout_pipe || stderr_pipe {
        // mark stdin so that we don't actually send any data
        // until communicate() is called.

        err = ret.stdin._mark();
        if err {
          ret.spawn_error = err; return ret;
        }
        ret.stdin_buffering = true;
      }
    }

    if stdout_pipe {
      ret.stdout_pipe = true;
      ret.stdout_file = openfd(stdout_fd, error=err, hints=QIO_HINT_OWNED);
      if err {
        ret.spawn_error = err; return ret;
      }

      ret.stdout = ret.stdout_file.reader(error=err);
      if err {
        ret.spawn_error = err; return ret;
      }
    }

    if stderr_pipe {
      ret.stderr_pipe = true;
      ret.stderr_file = openfd(stderr_fd, error=err, hints=QIO_HINT_OWNED);
      if err {
        ret.spawn_error = err; return ret;
      }

      ret.stderr = ret.stderr_file.reader(error=err);
      if err {
        ret.spawn_error = err; return ret;
      }
    }

    return ret;
  }

  proc spawnshell(command:string, env:[] string=Spawn.empty_env,
                  stdin:?t = FORWARD, stdout:?u = FORWARD, stderr:?v = FORWARD,
                  executable="/bin/sh", shellarg="-c",
                  param kind=iokind.dynamic, param locking=true)
  {
    var args = [command];
    if shellarg != "" then args.push_front(shellarg);
    args.push_front(executable);
    return spawn(args, env, executable, kind=kind, locking=locking);
  }

  proc subprocess.poll(out error:syserr) {
    on home {
      // check if child process has terminated.
      var done:c_int = 0;
      var exitcode:c_int = 0;
      error = qio_waitpid(pid, 0, done, exitcode);
      if done {
        this.running = false;
        this.exit_status = exitcode;
      }
    }
  }
  proc subprocess.wait(out error:syserr) {

    error = this.spawn_error;
    if !running then return;

    on home {
      
      // Close stdin.
      if this.stdin_pipe {
        // send data to stdin
        if this.stdin_buffering then this.stdin._commit();
        this.stdin.close(error=error);
        this.stdin_file.close(error=error);
      }

      // check if child process has terminated.
      var done:c_int = 0;
      var exitcode:c_int = 0;
      error = qio_waitpid(pid, 1, done, exitcode);
      if done {
        this.running = false;
        this.exit_status = exitcode;
      }
    }
  }
  proc subprocess.wait() {
    var err:syserr = ENOERR;

    this.wait(error=err);
    if err then halt("Error in subprocess.wait: " + errorToString(err));
  }

  // wait for process to terminate...
  // commit stdin, sending any data to the process,
  // while also reading any output from the process
  // into the stdout and stderr channel buffers.
  proc subprocess.communicate(out error:syserr) {
    
    on home {
      stdin._commit();

      error = qio_proc_communicate(
        locking, 
        stdin._channel_internal,
        stdout._channel_internal,
        stderr._channel_internal);
    }
  }
  proc subprocess.communicate() {
    var err:syserr = ENOERR;

    this.wait(error=err);
    if err then halt("Error in subprocess.communicate: " + errorToString(err));
  }


// Future work: support
// send_signal
// terminate
// kill (send SIGKILL)


}

