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

.. versionadded:: 1.12
  Spawn module added.

Support launching and interacting with other programs.

Using functions in this module, one can create a subprocess
and possibly capture its output. It is also possible to provide
input to a subprocess.

To start a subprocess, use :proc:`spawn` or :proc:`spawnshell`.  To wait for
the subprocess process to finish, use the :proc:`subprocess.wait` or
:proc:`subprocess.communicate` functions.

This example program produces a listing of files in the current directory with
names that begin with ``test.`` by using the ``ls`` command. The
output will be mixed in with the Chapel program's output.

.. code-block:: chapel

  use Spawn;

  var sub = spawn(["ls", "test.*"]);
  sub.wait();

This version also runs the ``ls`` command but uses a pipe
to read the output from the ``ls`` command.

.. code-block:: chapel

  use Spawn;

  var sub = spawn(["ls", "test.*"], stdout=PIPE);

  var line:string;
  while sub.stdout.readline(line) {
    write("ls returned: ", line);
  }

  sub.wait();

Finally, this version uses pipes to provide input to
a subprocess in addition to capturing its output. This
version uses the ``cat`` command, which just prints
back its input.

.. code-block:: chapel

  use Spawn;

  var sub = spawn(["cat"], stdin=PIPE, stdout=PIPE);

  sub.stdin.writeln("Hello");
  sub.stdin.writeln("World");

  sub.communicate();

  var line:string;
  while sub.stdout.readline(line) {
    write("Got line: ", line);
  }
  // prints out:
  // Got line: Hello
  // Got line: World


.. note::

  As of Chapel v1.12, creating a subprocess that uses :const:`PIPE` to provide
  input or capture output does not work when using the ugni communications
  layer and when using more than one locale. In this circumstance, the program
  will halt with an error message. These scenarios do work when using GASNet
  instead of the ugni layer.

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
  private extern proc qio_spawn_allocate_ptrvec(count: size_t): c_ptr(c_string);
  private extern proc qio_spawn_free_ptrvec(args: c_ptr(c_string));
  private extern proc qio_spawn_free_str(str: c_string);


  /* 
     This record represents a subprocess.
   */
  record subprocess {
    /* The kind of a subprocess is used to create the types
       for any channels that are necessary. */
    param kind:iokind;
    /* As with kind, this value is used to create the types
       for any channels that are necessary. */
    param locking:bool;

    pragma "no doc"
    var home:locale;

    /* The Process ID number of the spawned process */
    var pid:int(64);

    /* If the subprocess is configured to use pipes, inputfd
       is the file descriptor for the write end of a pipe
       connected to the child's standard input.
     */
    pragma "no doc"
    var inputfd:c_int;
    /* If the subprocess is configured to use pipes, outputfd
       is the file descriptor for the read end of a pipe
       connected to the child's standard output.
     */
    pragma "no doc"
    var outputfd:c_int;
    /* If the subprocess is configured to use pipes, errorfd
       is the file descriptor for the read end of a pipe
       connected to the child's standard error.
     */
    pragma "no doc"
    var errorfd:c_int;

   
    /* `false` if this library knows that the subprocess is not running */
    var running:bool;
    /* The exit status from the subprocess, or possibly a value >= 256
       if there was en error when creating the subprocess */
    var exit_status:int;

    // the channels
    // TODO -- these could be private to this module
    pragma "no doc"
    var stdin_pipe:bool;
    pragma "no doc"
    var stdin_buffering:bool;
    pragma "no doc"
    var stdin_file:file;
    pragma "no doc"
    var stdin:channel(writing=true, kind=kind, locking=locking);
    pragma "no doc"
    var stdout_pipe:bool;
    pragma "no doc"
    var stdout_file:file;
    pragma "no doc"
    var stdout:channel(writing=false, kind=kind, locking=locking);
    pragma "no doc"
    var stderr_pipe:bool;
    pragma "no doc"
    var stderr_file:file;
    pragma "no doc"
    var stderr:channel(writing=false, kind=kind, locking=locking);

    // Ideally we don't have the _file versions, but they
    // are there now because of issues with when the reference counts
    // for the file are updated.

    pragma "no doc" 
    var spawn_error:syserr;
  }

  private extern const QIO_FD_FORWARD:c_int;
  private extern const QIO_FD_CLOSE:c_int;
  private extern const QIO_FD_PIPE:c_int;
  private extern const QIO_FD_TO_STDOUT:c_int;

  /*
     FORWARD indicates that the child process should inherit
     the stdin/stdout/stderr of this process.
   */
  const FORWARD = QIO_FD_FORWARD;
  /*
     CLOSE indicates that the child process should close
     its stdin/stdout/stderr.
   */
  const CLOSE = QIO_FD_CLOSE;
  /*
     PIPE indicates that the spawn operation should set up
     a pipe between the parent process and the child process
     so that the parent process can provide input to the
     child process or capture its output.
   */
  const PIPE = QIO_FD_PIPE;
  /*
     STDOUT indicates that the stderr stream of the child process
     should be forwarded to its stdout stream.
   */
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
     Create a subprocess.
     
     :arg args: An array of strings storing the command to run and
                its arguments. The command to run is always the first argument.
                The command could be found in the current PATH or
                it could be a full path to a file to execute. If the
                executable argument is set, the first argument will
                be the name of the spawned program provided to that
                program and typically shown in process listings.

     :arg env: An array of strings storing the environment to use when
               spawning the child process instead of forwarding the
               current environment. By default, this argument
               is an empty array. In that case, 
               the current environment will be forwarded to the child
               process.

     :arg executable: By default, the executable argument is "". In that
                      case, the program to launch is the first element
                      of the args array. If the executable
                      argument is provided, it will be used instead of
                      the first element of the args array as the program
                      to launch. In either case, the program could be
                      found by searching the PATH.

     :arg stdin: indicates how the standard input of the child process
                 should be handled. It could be :const:`FORWARD`,
                 :const:`CLOSE`, :const:`PIPE`, or a file
                 descriptor number to use. Defaults to :const:`FORWARD`.

     :arg stdout: indicates how the standard output of the child process
                  should be handled. It could be :const:`FORWARD`,
                  :const:`CLOSE`, :const:`PIPE`, or a file
                  descriptor number to use. Defaults to :const:`FORWARD`.

     :arg stderr: indicates how the standard error of the child process
                  should be handled. It could be :const:`FORWARD`,
                  :const:`CLOSE`, :const:`PIPE`, :const:`STDOUT`, or
                  a file descriptor number to use. Defaults to
                  :const:`FORWARD`.

     :arg kind: What kind of channels should be created when
                :const:`PIPE` is used. This argument is used to set
                :attr:`subprocess.kind` in the resulting subprocess.
                Defaults to :enum:`IO.iokind.dynamic`.
                 
     :arg locking: Should channels created use locking?
                   This argument is used to set :attr:`subprocess.locking`
                   in the resulting subprocess. Defaults to `true`.

     :returns: a :record:`subprocess` with kind and locking set according
               to the arguments.

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

    // When memory is registered with the NIC under ugni, a fork will currently
    // segfault. Here we halt before such a call is made to provide an
    // informative error message instead of a segfault. Note that we don't
    // register with the NIC for numLocales == 1, and vfork is used instead of
    // fork when stdin, stdout, stderr=FORWARD so we won't run into this issue
    // under those circumstances. See JIRA issue 113 for more details.
    if CHPL_COMM == "ugni" then
      if stdin != FORWARD || stdout != FORWARD || stderr != FORWARD then
        if numLocales > 1 then
          halt("spawn with more than 1 locale for CHPL_COMM=ugni currently ",
               "requires stdin, stdout, stderr=FORWARD");

    if stdin == QIO_FD_PIPE then stdin_pipe = true;
    if stdout == QIO_FD_PIPE then stdout_pipe = true;
    if stderr == QIO_FD_PIPE then stderr_pipe = true;

    // Create the C pointer structures appropriate for spawn/exec
    // that are NULL terminated and consist of C strings.

    var nargs = args.size + 1;
    var use_args = qio_spawn_allocate_ptrvec( nargs.safeCast(size_t) );
    for (a,i) in zip(args, 0..) {
      use_args[i] = qio_spawn_strdup(a.c_str());
    }
    var use_env:c_ptr(c_string) = nil;
    if env.size != 0 {
      var nenv = env.size + 1;
      use_env = qio_spawn_allocate_ptrvec( nenv.safeCast(size_t) );
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
    qio_spawn_free_ptrvec(use_args);
    qio_spawn_free_ptrvec(use_env);

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

  /*
     Create a subprocess by invoking a shell.

     .. note::

       Since the command string is passed to a shell, it is
       very unsecure to pass user input to this command
       without proper quoting.
    

     :arg command: A string representing the command to run.
                   This string will be interpreted by the shell.
    
     :arg env: An array of strings storing the environment to use when
               spawning the child process instead of forwarding the
               current environment. By default, this argument
               is an empty array. In that case, 
               the current environment will be forwarded to the child
               process.

     :arg stdin: indicates how the standard input of the child process
                 should be handled. It could be :const:`FORWARD`,
                 :const:`CLOSE`, :const:`PIPE`, or a file
                 descriptor number to use. Defaults to :const:`FORWARD`.

     :arg stdout: indicates how the standard output of the child process
                  should be handled. It could be :const:`FORWARD`,
                  :const:`CLOSE`, :const:`PIPE`, or a file
                  descriptor number to use. Defaults to :const:`FORWARD`.

     :arg stderr: indicates how the standard error of the child process
                  should be handled. It could be :const:`FORWARD`,
                  :const:`CLOSE`, :const:`PIPE`, :const:`STDOUT`, or
                  a file descriptor number to use. Defaults to
                  :const:`FORWARD`.

     :arg executable: By default, the executable argument is "/bin/sh".
                      That directs the subprocess to run the /bin/sh shell
                      in order to interpret the command string.
                     
     :arg shellarg: An argument to pass to the shell before
                    the command string. By default this is "-c".

     :arg kind: What kind of channels should be created when
                :const:`PIPE` is used. This argument is used to set
                :attr:`subprocess.kind` in the resulting subprocess.
                Defaults to :enum:`IO.iokind.dynamic`.
                 
     :arg locking: Should channels created use locking?
                   This argument is used to set :attr:`subprocess.locking`
                   in the resulting subprocess. Defaults to `true`.

     :returns: a :record:`subprocess` with kind and locking set according
               to the arguments.

  */
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

  /*
     Check to see if a child process has terminated.
     If the child process has terminated, after this
     call, :attr:`~subprocess.running` will be `false`.
   */
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
  
  /*
    Wait for a child process to complete. After this function
    returns, :attr:`~subprocess.running` is `false` and
    :attr:`~subprocess.exit_status` stores the exit code returned
    by the subprocess.
     
    .. note::
     
        Use :proc:`subprocess.communicate` instead of this function when using
        :const:`PIPE` for stdin, stdout, or stderr.  This function does not try
        to send any buffered input to the child process and so could result in
        a hang if the child process is waiting for input to finish. Similarly,
        this function does not consume the output from the child process and so
        the child process could hang while waiting to write data to its output
        while the parent process is waiting for it to complete (and not
        consuming its output).

    :arg error: optional argument to capture any error encountered
                when waiting for the child process.
   */
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

  // documented in the out error version
  pragma "no doc"
  proc subprocess.wait() {
    var err:syserr = ENOERR;

    this.wait(error=err);
    if err then halt("Error in subprocess.wait: " + errorToString(err));
  }

  /*
     
    Wait for a child process to complete. After this function
    returns, :attr:`~subprocess.running` is `false` and
    :attr:`~subprocess.exit_status` stores the exit code returned
    by the subprocess.
 
    This function handles cases in which stdin, stdout, or stderr
    for the child process is :const:`PIPE` by writing any
    input to the child process and buffering up the output
    of the child process as necessary while waiting for
    it to terminate.

    :arg error: optional argument to capture any error encountered
                when waiting for the child process.
   */
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

  // documented in the out error version
  pragma "no doc"
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

