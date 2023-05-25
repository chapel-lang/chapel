/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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

  use Subprocess;

  var sub = spawn(["ls", "test.*"]);
  sub.wait();

This version also runs the ``ls`` command but uses a pipe
to read the output from the ``ls`` command.

.. code-block:: chapel

  use Subprocess;

  var sub = spawn(["ls", "test.*"], stdout=pipeStyle.pipe);

  var line:string;
  while sub.stdout.readLine(line) {
    write("ls returned: ", line);
  }

  sub.wait();

Here is an example program that provides input to a subprocess in addition to
capturing its output.  This version uses the ``cat`` command, which just prints
back its input.

.. code-block:: chapel

  use Subprocess;

  var sub = spawn(["cat"], stdin=pipeStyle.bufferAll, stdout=pipeStyle.pipe);

  sub.stdin.writeln("Hello");
  sub.stdin.writeln("World");

  sub.communicate();

  var line:string;
  while sub.stdout.readLine(line) {
    write("Got line: ", line);
  }

  // prints out:
  // Got line: Hello
  // Got line: World


Here is a final example in which the Chapel program uses 2 tasks
to work with a subprocess. One task is producing data and the
other task is consuming it.

.. code-block:: chapel

  use Subprocess;

  var input = ["a", "b", "c"];

  var sub = spawn(["cat"], stdin=pipeStyle.pipe, stdout=pipeStyle.pipe);
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

  Creating a subprocess that uses :type:`pipeStyle` ``pipeStyle.pipe`` to
  provide input or capture output does not work when using the ugni
  communications layer with hugepages enabled and when using more than one
  locale. In this circumstance, the program will halt with an error message.
  These scenarios do work when using GASNet instead of the ugni layer.

 */
module Subprocess {
  public use IO;
  use OS;
  use CTypes;
  use OS.POSIX;

  private extern proc qio_openproc(argv:c_ptr(c_string),
                                   env:c_ptr(c_string),
                                   executable:c_string,
                                   ref stdin_fd:c_int,
                                   ref stdout_fd:c_int,
                                   ref stderr_fd:c_int,
                                   ref pid:int(64)):errorCode;
  private extern proc qio_waitpid(pid:int(64),
    blocking:c_int, ref done:c_int, ref exitcode:c_int):errorCode;
  private extern proc qio_proc_communicate(threadsafe:c_int,
                                           input:qio_channel_ptr_t,
                                           output:qio_channel_ptr_t,
                                           error:qio_channel_ptr_t):errorCode;

  // When spawning, we need to allocate the command line
  // and environment to spawn with the C allocator (instead
  // of the Chapel one) because the Chapel heap might not
  // be accessible to a forked process.
  // So, we have here some functions that work with
  // the C allocator instead of the Chapel one.

  private extern proc qio_spawn_strdup(str: c_string): c_string;
  private extern proc qio_spawn_allocate_ptrvec(count: c_size_t): c_ptr(c_string);
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

    @chpldoc.nodoc
    var home:locale = here;

    /* The Process ID number of the spawned process */
    var pid:int(64);

    /* If the subprocess is configured to use pipes, inputfd
       is the file descriptor for the write end of a pipe
       connected to the child's standard input.
     */
    @chpldoc.nodoc
    var inputfd:c_int;
    /* If the subprocess is configured to use pipes, outputfd
       is the file descriptor for the read end of a pipe
       connected to the child's standard output.
     */
    @chpldoc.nodoc
    var outputfd:c_int;
    /* If the subprocess is configured to use pipes, errorfd
       is the file descriptor for the read end of a pipe
       connected to the child's standard error.
     */
    @chpldoc.nodoc
    var errorfd:c_int;


    /* `false` if this library knows that the subprocess is not running */
    var running:bool;
    /* The exit status from the subprocess, or possibly a value >= 256
       if there was en error when creating the subprocess */
    var exitCode:int;

    // the channels
    // TODO -- these could be private to this module
    @chpldoc.nodoc
    var stdin_pipe:bool;
    // true if we are currently buffering up stdin, meaning that
    // we need to 'commit' in order to actually send the data.
    @chpldoc.nodoc
    var stdin_buffering:bool;
    @chpldoc.nodoc
    var stdin_channel:fileWriter(kind=kind, locking=locking);
    @chpldoc.nodoc
    var stdout_pipe:bool;
    @chpldoc.nodoc
    var stdout_file:file;
    @chpldoc.nodoc
    var stdout_channel:fileReader(kind=kind, locking=locking);
    @chpldoc.nodoc
    var stderr_pipe:bool;
    @chpldoc.nodoc
    var stderr_file:file;
    @chpldoc.nodoc
    var stderr_channel:fileReader(kind=kind, locking=locking);

    // Ideally we don't have the _file versions, but they
    // are there now because of issues with when the reference counts
    // for the file are updated.

    @chpldoc.nodoc
    var spawn_error:errorCode;

    @chpldoc.nodoc
    proc _stop_stdin_buffering() {
      if this.stdin_buffering && this.stdin_pipe {
        this.stdin_channel.commit();
        this.stdin_buffering = false; // Don't commit again on close again
      }
    }

    @chpldoc.nodoc
    proc _throw_on_launch_error() throws {
      if !running {
        try ioerror(spawn_error,
                    "encountered when launching subprocess");
      }
    }

    /*
       Access the stdin pipe for the subprocess. The parent process
       can write to the subprocess through this pipe if the subprocess
       was created with stdin=pipeStyle.pipe.

       :throws SystemError: If the subprocess does not have a stdin pipe open.
     */
    proc stdin throws {
      try _throw_on_launch_error();
      if stdin_pipe == false {
        throw createSystemError(
            EINVAL, "subprocess was not configured with a stdin pipe");
      }
      return stdin_channel;
    }

    /*
       Access the stdout pipe for the subprocess. The parent process
       can read from the subprocess through this pipe if the subprocess
       was created with stdout=pipeStyle.pipe.

       :throws SystemError: If the subprocess does not have a stdout pipe open.
     */
    proc stdout throws {
      try _throw_on_launch_error();
      if stdout_pipe == false {
        throw createSystemError(
            EINVAL, "subprocess was not configured with a stdout pipe");
      }
      return stdout_channel;
    }

    /*
       Access the stderr pipe for the subprocess. The parent process
       can read from the subprocess through this pipe if the subprocess
       was created with stderr=pipeStyle.pipe.

       :throws SystemError: If the subprocess does not have a stderr pipe open.
     */
    proc stderr throws {
      try _throw_on_launch_error();
      if stderr_pipe == false {
        throw createSystemError(
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
     Styles of piping to use in a subprocess.

     ``forward`` indicates that the child process should inherit
     the stdin/stdout/stderr of this process.

     ``close`` indicates that the child process should close
     its stdin/stdout/stderr.

     ``pipe`` indicates that the spawn operation should set up
     a pipe between the parent process and the child process
     so that the parent process can provide input to the
     child process or capture its output.

     ``stdout`` indicates that the stderr stream of the child process
     should be forwarded to its stdout stream.

     ``bufferAll`` is the same as pipe, but when used for stdin causes all data
     to be buffered and sent on the communicate() call. This avoids certain
     deadlock scenarios where stdout or stderr are ``pipe``. In particular,
     without ``bufferAll``, the sub-process might block on writing output
     which will not be consumed until the communicate() call.

   */
  enum pipeStyle {
    forward,
    close,
    pipe,
    stdout,
    bufferAll
  }

  private proc pipeStyleToInt(style: ?t)
    where isIntegralType(t) || t == pipeStyle {
    if isIntegralType(t) then return style;
    else if style == pipeStyle.forward then return QIO_FD_FORWARD;
    else if style == pipeStyle.close then return QIO_FD_CLOSE;
    else if style == pipeStyle.pipe then return QIO_FD_PIPE;
    else if style == pipeStyle.stdout then return QIO_FD_TO_STDOUT;
    else if style == pipeStyle.bufferAll then return QIO_FD_BUFFERED_PIPE;
    else return -1;
  }

  private const empty_env:[1..0] string;

  private proc get_empty_env() {
    const ret:[1..0] string;
    return ret;
  }

  /* TODO:
     stdin stdout and stderr can be pipeStyle.pipe, existing file descriptor,
     existing file object, or None. and stderr can be pipeStyle.stdout which
     indicates stderr -> stdout.

     What about a string for a file path? To support that, use
     arguments like this: stdin:?t = pipeStyle.forward

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
                 should be handled. It could be :type:`pipeStyle`
                 ``pipeStyle.forward``, ``pipeStyle.close``,
                 ``pipeStyle.pipe``, or a file descriptor number to use.
                 Defaults to ``pipeStyle.forward``.

     :arg stdout: indicates how the standard output of the child process
                  should be handled. It could be :type:`pipeStyle`
                  ``pipeStyle.forward``, ``pipeStyle.close``,
                  ``pipeStyle.pipe``, or a file descriptor number to use.
                  Defaults to ``pipeStyle.forward``.

     :arg stderr: indicates how the standard error of the child process
                  should be handled. It could be :type:`pipeStyle`
                  ``pipeStyle.forward``, ``pipeStyle.close``,
                  ``pipeStyle.pipe``, ``pipeStyle.stdout``, or a file
                  descriptor number to use. Defaults to ``pipeStyle.forward``.

     :arg kind: What kind of channels should be created when
                ``pipeStyle.pipe`` is used. This argument is used to set
                :attr:`subprocess.kind` in the resulting subprocess.
                Defaults to :type:`IO.iokind` ``iokind.dynamic``.

     :arg locking: Should channels created use locking?
                   This argument is used to set :attr:`subprocess.locking`
                   in the resulting subprocess. Defaults to `true`.

     :returns: a :record:`subprocess` with kind and locking set according
               to the arguments.

     :throws IllegalArgumentError: Thrown when ``args`` is an empty array.
     */
  proc spawn(args:[] string, env:[] string=Subprocess.empty_env, executable="",
             stdin:?t = pipeStyle.forward, stdout:?u = pipeStyle.forward,
             stderr:?v = pipeStyle.forward,
             param kind=iokind.dynamic, param locking=true) throws
  {
    use ChplConfig;
    extern proc sys_getenv(name:c_string, ref string_out:c_string):c_int;

    var stdin_fd:c_int = QIO_FD_FORWARD;
    var stdout_fd:c_int = QIO_FD_FORWARD;
    var stderr_fd:c_int = QIO_FD_FORWARD;
    var stdin_pipe = false;
    var stdout_pipe = false;
    var stderr_pipe = false;
    var pid:int;
    var err:errorCode;

    if stdin.type == pipeStyle || isIntegralType(stdin.type) then
      stdin_fd = pipeStyleToInt(stdin);
    else compilerError("only pipeStyle.forward/close/pipe/stdout supported");

    if stdout.type == pipeStyle || isIntegralType(stdout.type) then
      stdout_fd = pipeStyleToInt(stdout);
    else compilerError("only pipeStyle.forward/close/pipe/stdout supported");

    if stderr.type == pipeStyle || isIntegralType(stderr.type) then
      stderr_fd = pipeStyleToInt(stderr);
    else compilerError("only pipeStyle.forward/close/pipe/stdout supported");

    if args.size == 0 then
      throw new owned IllegalArgumentError('args cannot be an empty array');

    // When memory is registered with the NIC under ugni, a fork will currently
    // segfault. Here we halt before such a call is made to provide an
    // informative error message instead of a segfault. Note that we don't
    // register with the NIC for numLocales == 1, and vfork is used instead of
    // fork when stdin, stdout, stderr=``pipeStyle.forward`` so we won't run
    // into this issue under those circumstances. See issue #7550 for
    // more details.
    if CHPL_COMM == "ugni" {
      if stdin != pipeStyle.forward || stdout != pipeStyle.forward || stderr != pipeStyle.forward then
        if numLocales > 1 {
          var env_c_str:c_string;
          var env_str:string;
          if sys_getenv(c"PE_PRODUCT_LIST", env_c_str)==1 {
            env_str = string.createCopyingBuffer(env_c_str);
            if env_str.count("HUGETLB") > 0 then
              throw createSystemError(
                  EINVAL,
                  "spawn with more than 1 locale for CHPL_COMM=ugni with hugepages currently requires stdin, stdout, stderr=pipeStyle.forward");
          }
        }
    }

    if stdin == pipeStyle.pipe || stdin == pipeStyle.bufferAll then
      stdin_pipe = true;
    if stdout == pipeStyle.pipe || stdout == pipeStyle.bufferAll then
      stdout_pipe = true;
    if stderr == pipeStyle.pipe || stderr == pipeStyle.bufferAll then
      stderr_pipe = true;

    // Create the C pointer structures appropriate for spawn/exec
    // that are NULL terminated and consist of C strings.

    var nargs = args.size + 1;
    var use_args = qio_spawn_allocate_ptrvec( nargs.safeCast(c_size_t) );
    for (a,i) in zip(args, 0..) {
      use_args[i] = qio_spawn_strdup(a.c_str());
    }
    var use_env:c_ptr(c_string) = nil;
    if env.size != 0 {
      var nenv = env.size + 1;
      use_env = qio_spawn_allocate_ptrvec( nenv.safeCast(c_size_t) );
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
                             running=true, exitCode=256);

    if err {
      ret.running = false;
      ret.exitCode = 257;
      ret.spawn_error = err;
      return ret;
    }
    ret.spawn_error = 0;

    // open the QIO files if a pipe was used.

    // (we could use open_usr to enable the getpath function
    //  to return the command name).

    if stdin_pipe {
      ret.stdin_pipe = true;
      // stdin_file will decrement file reference count when it
      // goes out of scope, but the channel will still keep
      // the file alive by referring to it.
      try {
        var stdin_file = new file(stdin_fd, own=true);
        ret.stdin_channel = stdin_file.writer();
      } catch e: SystemError {
        ret.spawn_error = e.err;
        return ret;
      } catch {
        ret.spawn_error = EINVAL;
        return ret;
      }

      if stdin == pipeStyle.bufferAll {
        // mark stdin so that we don't actually send any data
        // until communicate() is called.

        err = ret.stdin_channel.mark();
        if err {
          ret.spawn_error = err; return ret;
        }
        ret.stdin_buffering = true;
      }
    }

    if stdout_pipe {
      ret.stdout_pipe = true;
      try {
        var stdout_file = new file(stdout_fd, own=true);
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
        ret.stderr_file = new file(stderr_fd, own=true);
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
                 should be handled. It could be :type:`pipeStyle`
                 ``pipeStyle.forward``, ``pipeStyle.close``,
                 ``pipeStyle.pipe``, or a file descriptor number to use.
                 Defaults to ``pipeStyle.forward``.

     :arg stdout: indicates how the standard output of the child process
                  should be handled. It could be :type:`pipeStyle`
                  ``pipeStyle.forward``, ``pipeStyle.close``,
                  ``pipeStyle.pipe``, or a file descriptor number to use.
                  Defaults to ``pipeStyle.forward``.

     :arg stderr: indicates how the standard error of the child process
                  should be handled. It could be :type:`pipeStyle`
                  ``pipeStyle.forward``, ``pipeStyle.close``,
                  ``pipeStyle.pipe``, ``pipeStyle.stdout``, or a file
                  descriptor number to use. Defaults to
                  ``pipeStyle.forward``.

     :arg executable: By default, the executable argument is "/bin/sh".
                      That directs the subprocess to run the /bin/sh shell
                      in order to interpret the command string.

     :arg shellarg: An argument to pass to the shell before
                    the command string. By default this is "-c".

     :arg kind: What kind of channels should be created when
                :type:`pipeStyle` ``pipeStyle.pipe`` is used. This
                argument is used to set :attr:`subprocess.kind` in
                the resulting subprocess.  Defaults to
                :type:`IO.iokind` ``iokind.dynamic``.

     :arg locking: Should channels created use locking?
                   This argument is used to set :attr:`subprocess.locking`
                   in the resulting subprocess. Defaults to `true`.

     :returns: a :record:`subprocess` with kind and locking set according
               to the arguments.

     :throws IllegalArgumentError: Thrown when ``command`` is an empty string.
  */
  proc spawnshell(command:string, env:[] string=Subprocess.empty_env,
                  stdin:?t = pipeStyle.forward, stdout:?u = pipeStyle.forward,
                  stderr:?v = pipeStyle.forward,
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

     :throws InterruptedError: when the poll was interrupted by
                               a signal.
     :throws SystemError: if something else has gone wrong when polling the
                          subprocess.
   */
  proc subprocess.poll() throws {
    try _throw_on_launch_error();

    var err:errorCode = 0;
    on home {
      // check if child process has terminated.
      var done:c_int = 0;
      var exitcode:c_int = 0;
      err = qio_waitpid(pid, 0, done, exitcode);
      if done {
        this.running = false;
        this.exitCode = exitcode;
      }
    }
    // Note: `err` can be ECHILD or EINVAL, in which case we've made a mistake
    // in our implementation.  But if it is EINTR, the user should be able to
    // catch an InterruptedError and respond to it.
    if err then try ioerror(err, "in subprocess.poll");
  }

  /*
    Wait for a child process to complete. After this function
    returns, :attr:`~subprocess.running` is `false` and
    :attr:`~subprocess.exitCode` stores the exit code returned
    by the subprocess.

    If `buffer` is `true`, then this call will handle cases in which
    stdin, stdout, or stderr for the child process is :type:`pipeStyle`
    ``pipe`` by writing any input to the child process and buffering up
    the output of the child process as necessary while waiting for it
    to terminate. It will do so in the same manner as
    :proc:`subprocess.communicate`.


    .. note::

        Do not use `buffer` `false` when using ``pipe`` for stdin,
        stdout, or stderr.  If `buffer` is `false`, this function does not
        try to send any buffered input to the child process and so could result
        in a hang if the child process is waiting for input to finish.
        Similarly, this function does not consume the output from the child
        process and so the child process could hang while waiting to write data
        to its output while the parent process is waiting for it to complete
        (and not consuming its output).

    :arg buffer: if `true`, buffer input and output pipes (see above).

    :throws BlockingIoError: when there weren't sufficient resources to perform
                             one of the required actions
    :throws InterruptedError: when the call was interrupted in some way.
    :throws BrokenPipeError: when a pipe for the subprocess closed early.
    :throws SystemError: when invalid values were passed to the subprocess's
                         stdin, or something else went wrong when
                         shutting down the subprocess.
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

    var stdin_err:errorCode  = 0;
    var wait_err:errorCode   = 0;
    var stdout_err:errorCode = 0;
    var stderr_err:errorCode = 0;

    on home {
      // Close stdin.
      if this.stdin_pipe {
        // send data to stdin
        _stop_stdin_buffering();
        try {
          // Can return EACCES, EAGAIN, EBADF, EDEADLK, EFAULT, EFBIG, EINTR,
          // EINVAL, EIO, EISDIR, ELOOP, EMSGSIZE, ENAMETOOLONG, ENOENT, ENOMEM,
          // ENOSYS, ENOTDIR, ENOTRECOVERABLE, EOF, EOVERFLOW, EOWNERDEAD,
          // EPERM, EROFS, ETXTBSY.  Of these, only EAGAIN, EINTR and EINVAL are
          // something the user could respond to, the rest would likely only
          // occur as a result of bugs in the implementation.
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
      // Can return ECHILD, EINTR, or EINVAL.  Of these, only EINTR is something
      // the user should respond to, the rest would likely only occur as a
      // result of bugs in the implementation.
      wait_err = qio_waitpid(pid, 1, done, exitcode);
      if done {
        this.running = false;
        this.exitCode = exitcode;
      }

      // If these channels are to stay open, use buffer=true or communicate.
      // Close stdout channel.
      if this.stdout_pipe {
        try {
          // Can return EACCES, EAGAIN, EBADF, EDEADLK, EFAULT, EFBIG, EINTR,
          // EINVAL, EIO, EISDIR, ELOOP, EMSGSIZE, ENAMETOOLONG, ENOENT, ENOMEM,
          // ENOSYS, ENOTDIR, ENOTRECOVERABLE, EOF, EOVERFLOW, EOWNERDEAD,
          // EPERM, EROFS, ETXTBSY.  Of these, only EAGAIN and EINTR are
          // something the user could respond to, the rest would likely only
          // occur as a result of bugs in the implementation.
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
          // Can return EACCES, EAGAIN, EBADF, EDEADLK, EFAULT, EFBIG, EINTR,
          // EINVAL, EIO, EISDIR, ELOOP, EMSGSIZE, ENAMETOOLONG, ENOENT, ENOMEM,
          // ENOSYS, ENOTDIR, ENOTRECOVERABLE, EOF, EOVERFLOW, EOWNERDEAD,
          // EPERM, EROFS, ETXTBSY.  Of these, only EAGAIN and EINTR are
          // something the user could respond to, the rest would likely only
          // occur as a result of bugs in the implementation.
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

  /*

    Wait for a child process to complete. After this function
    returns, :attr:`~subprocess.running` is `false` and
    :attr:`~subprocess.exitCode` stores the exit code returned
    by the subprocess.

    This function handles cases in which stdin, stdout, or stderr
    for the child process is :type:`pipeStyle` ``pipe`` by writing any
    input to the child process and buffering up the output
    of the child process as necessary while waiting for
    it to terminate.

    :throws BlockingIoError: when there weren't sufficient resources to perform
                             one of the required actions
    :throws InterruptedError: when the call was interrupted in some way.
    :throws BrokenPipeError: when a pipe for the subprocess closed early.
    :throws SystemError: when something went wrong when shutting down the
                         subprocess
   */
  proc subprocess.communicate() throws {
    try _throw_on_launch_error();

    if !running {
      if this.spawn_error then
        try ioerror(this.spawn_error, "in subprocess.communicate");

      // Otherwise, do nothing, since the child process already ended.
      return;
    }

    var err:errorCode = 0;
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
      this.exitCode = exitcode;
    }
    if err then try ioerror(err, "in subprocess.communicate");
  }

  /*
    Close any open channels and pipes for interacting with a subprocess.  This
    function does not wait for the subprocess to complete.  Note that it is
    generally not necessary to call this function since these channels will be
    closed when the subprocess record goes out of scope.
   */
  proc subprocess.close() throws {
    // TODO: see subprocess.wait() for more on this error handling approach
    var err: errorCode = 0;

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

  private extern proc qio_send_signal(pid: int(64), sig: c_int): errorCode;

  /*
    Send a signal to a child process.

    Declarations for POSIX.1.2008 signals are provided in the OS.POSIX module.
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
  proc subprocess.sendPosixSignal(signal:int) throws {
    try _throw_on_launch_error();

    var err: errorCode = 0;
    on home {
      err = qio_send_signal(pid, signal:c_int);
    }
    if err then try ioerror(err, "in subprocess.sendPosixSignal, with signal " + signal:string);
  }

  /*
    Request an abnormal termination of the child process.  The
    associated signal, `SIGABRT`, may be caught and handled by
    the child process. See :proc:`subprocess.sendPosixSignal`.
   */
  proc subprocess.abort() throws {
    try _throw_on_launch_error();
    try this.sendPosixSignal(POSIX.SIGABRT);
  }

  /* Send the child process an alarm signal. The associated signal,
     `SIGALRM`, may be caught and handled by the child process. See
     :proc:`subprocess.sendPosixSignal`.
   */
  proc subprocess.alarm() throws {
    try _throw_on_launch_error();
    try this.sendPosixSignal(POSIX.SIGALRM);
  }

  /*
    Unconditionally kill the child process.  The associated signal,
    `SIGKILL`, cannot be caught by the child process. See
    :proc:`subprocess.sendPosixSignal`.
   */
  proc subprocess.kill() throws {
    try _throw_on_launch_error();
    try this.sendPosixSignal(POSIX.SIGKILL);
  }

  /*
    Request termination of the child process.  The associated signal,
    `SIGTERM`, may be caught and handled by the child process. See
    :proc:`subprocess.sendPosixSignal`.
   */
  proc subprocess.terminate() throws {
    try _throw_on_launch_error();
    try this.sendPosixSignal(POSIX.SIGTERM);
  }
}
