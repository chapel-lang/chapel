/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

Here is an example program that provides input to a subprocess in addition to
capturing its output.  This version uses the ``cat`` command, which just prints
back its input.

.. code-block:: chapel

  use Spawn;

  var sub = spawn(["cat"], stdin=BUFFERED_PIPE, stdout=PIPE);

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


Here is a final example in which the Chapel program uses 2 tasks
to work with a subprocess. One task is producing data and the
other task is consuming it.

.. code-block:: chapel

  use Spawn;

  var input = ["a", "b", "c"];

  var sub = spawn(["cat"], stdin=PIPE, stdout=PIPE);
  cobegin {
    {
      // one task writes data to the subprocess
      for x in input {
        sub.stdin.writeln(x);
      }
      // this close is important; otherwise the other task blocks forever
      sub.stdin.close();
    }

    {
      var line:string;
      while sub.stdout.readln(line) {
        writeln("Got line ", line);
      }
    }
  }
  sub.wait();

  // prints out:
  // Got line: a
  // Got line: b
  // Got line: c


.. note::

  Creating a subprocess that uses :const:`PIPE` to provide
  input or capture output does not work when using the ugni communications layer
  with hugepages enabled and when using more than one locale. In this
  circumstance, the program will halt with an error message. These scenarios do
  work when using GASNet instead of the ugni layer.

 */
module Spawn {
  public use IO;
  use SysError;
  use SysCTypes;
  use SysBasic;

  private extern proc qio_openproc(argv:c_ptr(c_string),
                                   env:c_ptr(c_string),
                                   executable:c_string,
                                   ref stdin_fd:c_int,
                                   ref stdout_fd:c_int,
                                   ref stderr_fd:c_int,
                                   ref pid:int(64)):syserr;
  private extern proc qio_waitpid(pid:int(64),
    blocking:c_int, ref done:c_int, ref exitcode:c_int):syserr;
  private extern proc qio_proc_communicate(threadsafe:c_int,
                                           input:qio_channel_ptr_t,
                                           output:qio_channel_ptr_t,
                                           error:qio_channel_ptr_t):syserr;

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

     Note that the subprocess will not be waited for if this record
     goes out of scope. Channels opened to communicate with the subprocess
     will be closed if the record goes out of scope, however.

     Generally, it is important to call :proc:`subprocess.wait` to wait for the
     process to complete. If the parent process is using pipes to communicate
     with the subprocess, the parent process may call :proc:`subprocess.close`
     in order to close the pipes and free any buffers. Such calls are
     generally not needed since the channels will be closed when the
     subprocess record is automatically destroyed.
   */
  record subprocess {
    /* The kind of a subprocess is used to create the types
       for any channels that are necessary. */
    param kind:iokind;
    /* As with kind, this value is used to create the types
       for any channels that are necessary. */
    param locking:bool;

    pragma "no doc"
    var home:locale = here;

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
    // true if we are currently buffering up stdin, meaning that
    // we need to 'commit' in order to actually send the data.
    pragma "no doc"
    var stdin_buffering:bool;
    pragma "no doc"
    var stdin_channel:channel(writing=true, kind=kind, locking=locking);
    pragma "no doc"
    var stdout_pipe:bool;
    pragma "no doc"
    var stdout_file:file;
    pragma "no doc"
    var stdout_channel:channel(writing=false, kind=kind, locking=locking);
    pragma "no doc"
    var stderr_pipe:bool;
    pragma "no doc"
    var stderr_file:file;
    pragma "no doc"
    var stderr_channel:channel(writing=false, kind=kind, locking=locking);

    // Ideally we don't have the _file versions, but they
    // are there now because of issues with when the reference counts
    // for the file are updated.

    pragma "no doc"
    var spawn_error:syserr;

    pragma "no doc"
    proc _stop_stdin_buffering() {
      if this.stdin_buffering && this.stdin_pipe {
        this.stdin_channel._commit();
        this.stdin_buffering = false; // Don't commit again on close again
      }
    }


    pragma "no doc"
    proc _throw_on_launch_error() throws {
      if !running {
        try ioerror(spawn_error,
                    "encountered when launching subprocess");
      }
    }

    /*
       Access the stdin pipe for the subprocess. The parent process
       can write to the subprocess through this pipe if the subprocess
       was created with stdin=PIPE.

       :throws SystemError: If the subprocess does not have a stdin pipe open.
     */
    proc stdin throws {
      try _throw_on_launch_error();
      if stdin_pipe == false {
        throw SystemError.fromSyserr(
            EINVAL, "subprocess was not configured with a stdin pipe");
      }
      return stdin_channel;
    }

    /*
       Access the stdout pipe for the subprocess. The parent process
       can read from the subprocess through this pipe if the subprocess
       was created with stdout=PIPE.

       :throws SystemError: If the subprocess does not have a stdout pipe open.
     */
    proc stdout throws {
      try _throw_on_launch_error();
      if stdout_pipe == false {
        throw SystemError.fromSyserr(
            EINVAL, "subprocess was not configured with a stdout pipe");
      }
      return stdout_channel;
    }

    /*
       Access the stderr pipe for the subprocess. The parent process
       can read from the subprocess through this pipe if the subprocess
       was created with stderr=PIPE.

       :throws SystemError: If the subprocess does not have a stderr pipe open.
     */
    proc stderr throws {
      try _throw_on_launch_error();
      if stderr_pipe == false {
        throw SystemError.fromSyserr(
            EINVAL, "subprocess was not configured with a stderr pipe");
      }
      return stderr_channel;
    }
  }

  private extern const QIO_FD_FORWARD:c_int;
  private extern const QIO_FD_CLOSE:c_int;
  private extern const QIO_FD_PIPE:c_int;
  private extern const QIO_FD_TO_STDOUT:c_int;
  private extern const QIO_FD_BUFFERED_PIPE:c_int;

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
  /*
     BUFFERED_PIPE is the same as PIPE, but when used for stdin causes all data
     to be buffered and sent on the communicate() call. This avoids certain
     deadlock scenarios where stdout or stderr are PIPE. In particular, without
     BUFFERED_PIPE, the sub-process might block on writing output which will not
     be consumed until the communicate() call.
   */
  const BUFFERED_PIPE = QIO_FD_BUFFERED_PIPE;

  private const empty_env:[1..0] string;

  private proc get_empty_env() {
    const ret:[1..0] string;
    return ret;
  }

  /* TODO:
     stdin stdout and stderr can be PIPE, existing file descriptor,
     existing file object, or None. and stderr can be STDOUT which
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
                Defaults to :type:`IO.iokind` ``iokind.dynamic``.

     :arg locking: Should channels created use locking?
                   This argument is used to set :attr:`subprocess.locking`
                   in the resulting subprocess. Defaults to `true`.

     :returns: a :record:`subprocess` with kind and locking set according
               to the arguments.

     :throws IllegalArgumentError: Thrown when ``args`` is an empty array.
     */
  proc spawn(args:[] string, env:[] string=Spawn.empty_env, executable="",
             stdin:?t = FORWARD, stdout:?u = FORWARD, stderr:?v = FORWARD,
             param kind=iokind.dynamic, param locking=true) throws
  {
    var stdin_fd:c_int = QIO_FD_FORWARD;
    var stdout_fd:c_int = QIO_FD_FORWARD;
    var stderr_fd:c_int = QIO_FD_FORWARD;
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

    if args.size == 0 then
      throw new owned IllegalArgumentError('args cannot be an empty array');

    // When memory is registered with the NIC under ugni, a fork will currently
    // segfault. Here we halt before such a call is made to provide an
    // informative error message instead of a segfault. Note that we don't
    // register with the NIC for numLocales == 1, and vfork is used instead of
    // fork when stdin, stdout, stderr=FORWARD so we won't run into this issue
    // under those circumstances. See issue #7550 for more details.
    if CHPL_COMM == "ugni" {
      use Sys;
      if stdin != FORWARD || stdout != FORWARD || stderr != FORWARD then
        if numLocales > 1 {
          var env_c_str:c_string;
          var env_str:string;
          if sys_getenv(c"PE_PRODUCT_LIST", env_c_str)==1 {
            env_str = env_c_str;
            if env_str.count("HUGETLB") > 0 then
              throw SystemError.fromSyserr(
                  EINVAL,
                  "spawn with more than 1 locale for CHPL_COMM=ugni with hugepages currently requires stdin, stdout, stderr=FORWARD");
          }
        }
    }

    if stdin == QIO_FD_PIPE || stdin == QIO_FD_BUFFERED_PIPE then
      stdin_pipe = true;
    if stdout == QIO_FD_PIPE || stdout == QIO_FD_BUFFERED_PIPE then
      stdout_pipe = true;
    if stderr == QIO_FD_PIPE || stderr == QIO_FD_BUFFERED_PIPE then
      stderr_pipe = true;

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
      // stdin_file will decrement file reference count when it
      // goes out of scope, but the channel will still keep
      // the file alive by referring to it.
      try {
        var stdin_file = openfd(stdin_fd, hints=QIO_HINT_OWNED);
        ret.stdin_channel = stdin_file.writer();
      } catch e: SystemError {
        ret.spawn_error = e.err;
        return ret;
      } catch {
        ret.spawn_error = EINVAL;
        return ret;
      }

      if stdin == QIO_FD_BUFFERED_PIPE {
        // mark stdin so that we don't actually send any data
        // until communicate() is called.

        err = ret.stdin_channel._mark();
        if err {
          ret.spawn_error = err; return ret;
        }
        ret.stdin_buffering = true;
      }
    }

    if stdout_pipe {
      ret.stdout_pipe = true;
      try {
        var stdout_file = openfd(stdout_fd, hints=QIO_HINT_OWNED);
        ret.stdout_channel = stdout_file.reader();
      } catch e: SystemError {
        ret.spawn_error = e.err;
        return ret;
      } catch {
        ret.spawn_error = EINVAL;
        return ret;
      }
    }

    if stderr_pipe {
      ret.stderr_pipe = true;
      try {
        ret.stderr_file = openfd(stderr_fd, hints=QIO_HINT_OWNED);
        ret.stderr_channel = ret.stderr_file.reader();
      } catch e: SystemError {
        ret.spawn_error = e.err;
        return ret;
      } catch {
        ret.spawn_error = EINVAL;
        return ret;
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
                Defaults to :type:`IO.iokind` ``iokind.dynamic``.

     :arg locking: Should channels created use locking?
                   This argument is used to set :attr:`subprocess.locking`
                   in the resulting subprocess. Defaults to `true`.

     :returns: a :record:`subprocess` with kind and locking set according
               to the arguments.

     :throws IllegalArgumentError: Thrown when ``command`` is an empty string.
  */
  proc spawnshell(command:string, env:[] string=Spawn.empty_env,
                  stdin:?t = FORWARD, stdout:?u = FORWARD, stderr:?v = FORWARD,
                  executable="/bin/sh", shellarg="-c",
                  param kind=iokind.dynamic, param locking=true) throws
  {
    if command.isEmpty() then
      throw new owned IllegalArgumentError('command cannot be an empty string');
    
    var args = if shellarg == "" then [executable, command]
        else [executable, shellarg, command];

    return spawn(args, env, executable,
                 stdin=stdin, stdout=stdout, stderr=stderr,
                 kind=kind, locking=locking);
  }

  /*
     Check to see if a child process has terminated.
     If the child process has terminated, after this
     call, :attr:`~subprocess.running` will be `false`.
   */
  proc subprocess.poll() throws {
    try _throw_on_launch_error();

    var err:syserr = ENOERR;
    on home {
      // check if child process has terminated.
      var done:c_int = 0;
      var exitcode:c_int = 0;
      err = qio_waitpid(pid, 0, done, exitcode);
      if done {
        this.running = false;
        this.exit_status = exitcode;
      }
    }
    if err then try ioerror(err, "in subprocess.poll");
  }

  // documented in the throws version
  pragma "no doc"
  proc subprocess.poll(out error:syserr) {
    compilerWarning("'out error: syserr' pattern has been deprecated, use 'throws' function instead");
    try {
      this.poll();
    } catch e: SystemError {
      error = e.err;
    } catch {
      error = EINVAL;
    }
  }

  /*
    Wait for a child process to complete. After this function
    returns, :attr:`~subprocess.running` is `false` and
    :attr:`~subprocess.exit_status` stores the exit code returned
    by the subprocess.

    If `buffer` is `true`, then this call will handle cases in which
    stdin, stdout, or stderr for the child process is :const:`PIPE` by writing
    any input to the child process and buffering up the output of the child
    process as necessary while waiting for it to terminate. It will do
    so in the same manner as :proc:`subprocess.communicate`.


    .. note::

        Do not use `buffer` `false` when using :const:`PIPE` for stdin,
        stdout, or stderr.  If `buffer` is `false`, this function does not
        try to send any buffered input to the child process and so could result
        in a hang if the child process is waiting for input to finish.
        Similarly, this function does not consume the output from the child
        process and so the child process could hang while waiting to write data
        to its output while the parent process is waiting for it to complete
        (and not consuming its output).

    :arg buffer: if `true`, buffer input and output pipes (see above).
   */
  proc subprocess.wait(buffer=true) throws {
    try _throw_on_launch_error();

    if buffer {
      try this.communicate();
      return;
    }

    if !running {
      if this.spawn_error then
        try ioerror(this.spawn_error, "in subprocess.wait");

      // Otherwise, do nothing, since the child process already ended.
      return;
    }

    var stdin_err:syserr  = ENOERR;
    var wait_err:syserr   = ENOERR;
    var stdout_err:syserr = ENOERR;
    var stderr_err:syserr = ENOERR;

    on home {
      // Close stdin.
      if this.stdin_pipe {
        // send data to stdin
        _stop_stdin_buffering();
        try {
          this.stdin_channel.close();
        } catch e: SystemError {
          stdin_err = e.err;
        } catch {
          stdin_err = EINVAL;
        }
      }

      // wait for child process to terminate
      var done:c_int = 0;
      var exitcode:c_int = 0;
      wait_err = qio_waitpid(pid, 1, done, exitcode);
      if done {
        this.running = false;
        this.exit_status = exitcode;
      }

      // If these channels are to stay open, use buffer=true or communicate.
      // Close stdout channel.
      if this.stdout_pipe {
        try {
          this.stdout_channel.close();
        } catch e: SystemError {
          stdout_err = e.err;
        } catch {
          stdout_err = EINVAL;
        }
      }

      // Close stderr channel.
      if this.stderr_pipe {
        try {
          this.stderr_channel.close();
        } catch e: SystemError {
          stderr_err = e.err;
        } catch {
          stderr_err = EINVAL;
        }
      }
    }

    if wait_err {
      try ioerror(wait_err, "in subprocess.wait");
    } else if stdin_err {
      try ioerror(stdin_err, "in subprocess.wait");
    } else if stdout_err {
      try ioerror(stdout_err, "in subprocess.wait");
    } else if stderr_err {
      try ioerror(stderr_err, "in subprocess.wait");
    }
  }

  // documented in the throws version
  pragma "no doc"
  proc subprocess.wait(out error:syserr, buffer=true) {
    compilerWarning("'out error: syserr' pattern has been deprecated, use 'throws' function instead");
    try {
      this.wait(buffer);
    } catch e: SystemError {
      error = e.err;
    } catch {
      error = EINVAL;
    }
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
   */
  proc subprocess.communicate() throws {
    try _throw_on_launch_error();

    if !running {
      if this.spawn_error then
        try ioerror(this.spawn_error, "in subprocess.communicate");

      // Otherwise, do nothing, since the child process already ended.
      return;
    }

    var err:syserr = ENOERR;
    on home {
      if this.stdin_pipe {
        // send data to stdin
        _stop_stdin_buffering();
      }

      err = qio_proc_communicate(
        locking,
        stdin_channel._channel_internal,
        stdout_channel._channel_internal,
        stderr_channel._channel_internal);
    }
    if err then try ioerror(err, "in subprocess.communicate");

    // wait for child process to terminate
    var done:c_int = 0;
    var exitcode:c_int = 0;
    err = qio_waitpid(pid, 1, done, exitcode);
    if done {
      this.running = false;
      this.exit_status = exitcode;
    }
    if err then try ioerror(err, "in subprocess.communicate");
  }

  // documented in the throws version
  pragma "no doc"
  proc subprocess.communicate(out error:syserr) {
    compilerWarning("'out error: syserr' pattern has been deprecated, use 'throws' function instead");
    try {
      this.communicate();
    } catch e: SystemError {
      error = e.err;
    } catch {
      error = EINVAL;
    }
  }

  /*
    Close any open channels and pipes for interacting with a subprocess.  This
    function does not wait for the subprocess to complete.  Note that it is
    generally not necessary to call this function since these channels will be
    closed when the subprocess record goes out of scope.
   */
  proc subprocess.close() throws {
    // TODO: see subprocess.wait() for more on this error handling approach
    var err: syserr = ENOERR;

    // Close stdin.
    if this.stdin_pipe {
      // send data to stdin
      _stop_stdin_buffering();
      try {
        this.stdin_channel.close();
      } catch e: SystemError {
        err = e.err;
      } catch {
        err = EINVAL;
      }
    }
    // Close stdout.
    if this.stdout_pipe {
      try {
        this.stdout_channel.close();
      } catch e: SystemError {
        err = e.err;
      } catch {
        err = EINVAL;
      }
    }
    // Close stderr.
    if this.stderr_pipe {
      try {
        this.stderr_channel.close();
      } catch e: SystemError {
        err = e.err;
      } catch {
        err = EINVAL;
      }
    }
    if err then try ioerror(err, "in subprocess.close");
  }

  // documented in the throws version
  pragma "no doc"
  proc subprocess.close(out error:syserr) {
    compilerWarning("'out error: syserr' pattern has been deprecated, use 'throws' function instead");
    try {
      this.close();
    } catch e: SystemError {
      error = e.err;
    } catch {
      error = EINVAL;
    }
  }

  // Signals as required by POSIX.1-2008, 2013 edition
  // See note below about signals intentionally not included.
  pragma "no doc"
  extern const SIGABRT: c_int;
  pragma "no doc"
  extern const SIGALRM: c_int;
  pragma "no doc"
  extern const SIGBUS: c_int;
  pragma "no doc"
  extern const SIGCHLD: c_int;
  pragma "no doc"
  extern const SIGCONT: c_int;
  pragma "no doc"
  extern const SIGFPE: c_int;
  pragma "no doc"
  extern const SIGHUP: c_int;
  pragma "no doc"
  extern const SIGILL: c_int;
  pragma "no doc"
  extern const SIGINT: c_int;
  pragma "no doc"
  extern const SIGKILL: c_int;
  pragma "no doc"
  extern const SIGPIPE: c_int;
  pragma "no doc"
  extern const SIGQUIT: c_int;
  pragma "no doc"
  extern const SIGSEGV: c_int;
  pragma "no doc"
  extern const SIGSTOP: c_int;
  pragma "no doc"
  extern const SIGTERM: c_int;
  pragma "no doc"
  extern const SIGTRAP: c_int;
  pragma "no doc"
  extern const SIGTSTP: c_int;
  pragma "no doc"
  extern const SIGTTIN: c_int;
  pragma "no doc"
  extern const SIGTTOU: c_int;
  pragma "no doc"
  extern const SIGURG: c_int;
  pragma "no doc"
  extern const SIGUSR1: c_int;
  pragma "no doc"
  extern const SIGUSR2: c_int;
  pragma "no doc"
  extern const SIGXCPU: c_int;
  pragma "no doc"
  extern const SIGXFSZ: c_int;

  // These signals are not strictly required by POSIX.1.2008 2013 edition
  // and so should not be included here:

  // SIGPOLL is Obsolescent and optional as part of XSI STREAMS
  // SIGPROF is Obsolescent and optional as part of XSI STREAMS
  // SIGSYS is optional as part of X/Open Systems Interface
  // SIGVTALRM is optional as part of X/Open Systems Interface

  private extern proc qio_send_signal(pid: int(64), sig: c_int): syserr;

  /*
    Send a signal to a child process.

    Declarations for POSIX.1.2008 signals are provided in this module.
    These include `SIGABRT`, `SIGALRM`, `SIGBUS`, `SIGCHLD`, `SIGCONT`,
    `SIGFPE`, `SIGHUP`, `SIGILL`, `SIGINT`, `SIGKILL`, `SIGPIPE`, `SIGQUIT`,
    `SIGSEGV`, `SIGSTOP`, `SIGTERM`, `SIGTRAP`, `SIGTSTP`, `SIGTTIN`,
    `SIGTTOU`, `SIGURG`, `SIGUSR1`, `SIGUSR2`, `SIGXCPU`, `SIGXFSZ`.

    See your system's documentation for their meaning:

    ::

      man signal

    Other values for `signal` are system-specific and can be declared in this
    way, for example:

    .. code-block:: chapel

       extern const SIGPOLL: c_int;


    :arg signal: the signal to send
   */
  proc subprocess.send_signal(signal:int) throws {
    try _throw_on_launch_error();

    var err: syserr = ENOERR;
    on home {
      err = qio_send_signal(pid, signal:c_int);
    }
    if err then try ioerror(err, "in subprocess.send_signal, with signal " + signal:string);
  }

  // documented in the throws version
  pragma "no doc"
  proc subprocess.send_signal(out error:syserr, signal: int) {
    compilerWarning("'out error: syserr' pattern has been deprecated, use 'throws' function instead");
    try {
      this.send_signal(signal);
    } catch e: SystemError {
      error = e.err;
    } catch {
      error = EINVAL;
    }
  }

  /*
    Unconditionally kill the child process.  The associated signal,
    `SIGKILL`, cannot be caught by the child process. See
    :proc:`subprocess.send_signal`.
   */
  proc subprocess.kill() throws {
    try _throw_on_launch_error();
    try this.send_signal(SIGKILL);
  }

  // documented in the out error version
  pragma "no doc"
  proc subprocess.kill(out error:syserr) {
    compilerWarning("'out error: syserr' pattern has been deprecated, use 'throws' function instead");
    try {
      this.kill();
    } catch e: SystemError {
      error = e.err;
    } catch {
      error = EINVAL;
    }
  }

  /*
    Request termination of the child process.  The associated signal,
    `SIGTERM`, may be caught and handled by the child process. See
    :proc:`subprocess.send_signal`.
   */
  proc subprocess.terminate() throws {
    try _throw_on_launch_error();
    try this.send_signal(SIGTERM);
  }

  // documented in the out error version
  pragma "no doc"
  proc subprocess.terminate(out error:syserr) {
    compilerWarning("'out error: syserr' pattern has been deprecated, use 'throws' function instead");
    try {
      this.terminate();
    } catch e: SystemError {
      error = e.err;
    } catch {
      error = EINVAL;
    }
  }
}
