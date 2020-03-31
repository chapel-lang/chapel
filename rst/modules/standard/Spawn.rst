.. default-domain:: chpl

.. module:: Spawn
   :synopsis: Support launching and interacting with other programs.

Spawn
=====
**Usage**

.. code-block:: chapel

   use Spawn;



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

 

.. record:: subprocess

   
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
   


   .. attribute:: param kind: iokind

      The kind of a subprocess is used to create the types
      for any channels that are necessary. 

   .. attribute:: param locking: bool

      As with kind, this value is used to create the types
      for any channels that are necessary. 

   .. attribute:: var pid: int(64)

      The Process ID number of the spawned process 

   .. attribute:: var running: bool

      `false` if this library knows that the subprocess is not running 

   .. attribute:: var exit_status: int

      The exit status from the subprocess, or possibly a value >= 256
      if there was en error when creating the subprocess 

   .. method:: proc stdin throws

      
      Access the stdin pipe for the subprocess. The parent process
      can write to the subprocess through this pipe if the subprocess
      was created with stdin=PIPE.
      
      :throws SystemError: If the subprocess does not have a stdin pipe open.
      

   .. method:: proc stdout throws

      
      Access the stdout pipe for the subprocess. The parent process
      can read from the subprocess through this pipe if the subprocess
      was created with stdout=PIPE.
      
      :throws SystemError: If the subprocess does not have a stdout pipe open.
      

   .. method:: proc stderr throws

      
      Access the stderr pipe for the subprocess. The parent process
      can read from the subprocess through this pipe if the subprocess
      was created with stderr=PIPE.
      
      :throws SystemError: If the subprocess does not have a stderr pipe open.
      

.. data:: const FORWARD = QIO_FD_FORWARD

   
   FORWARD indicates that the child process should inherit
   the stdin/stdout/stderr of this process.
   

.. data:: const CLOSE = QIO_FD_CLOSE

   
   CLOSE indicates that the child process should close
   its stdin/stdout/stderr.
   

.. data:: const PIPE = QIO_FD_PIPE

   
   PIPE indicates that the spawn operation should set up
   a pipe between the parent process and the child process
   so that the parent process can provide input to the
   child process or capture its output.
   

.. data:: const STDOUT = QIO_FD_TO_STDOUT

   
   STDOUT indicates that the stderr stream of the child process
   should be forwarded to its stdout stream.
   

.. data:: const BUFFERED_PIPE = QIO_FD_BUFFERED_PIPE

   
   BUFFERED_PIPE is the same as PIPE, but when used for stdin causes all data
   to be buffered and sent on the communicate() call. This avoids certain
   deadlock scenarios where stdout or stderr are PIPE. In particular, without
   BUFFERED_PIPE, the sub-process might block on writing output which will not
   be consumed until the communicate() call.
   

.. function:: proc spawn(args: [] string, env: [] string = Spawn.empty_env, executable = "", stdin: ?t = FORWARD, stdout: ?u = FORWARD, stderr: ?v = FORWARD, param kind = iokind.dynamic, param locking = true) throws

   
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
   

.. function:: proc spawnshell(command: string, env: [] string = Spawn.empty_env, stdin: ?t = FORWARD, stdout: ?u = FORWARD, stderr: ?v = FORWARD, executable = "/bin/sh", shellarg = "-c", param kind = iokind.dynamic, param locking = true) throws

   
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
   

.. method:: proc subprocess.poll() throws

   
   Check to see if a child process has terminated.
   If the child process has terminated, after this
   call, :attr:`~subprocess.running` will be `false`.
   

.. method:: proc subprocess.wait(buffer = true) throws

   
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
   

.. method:: proc subprocess.communicate() throws

   
   
   Wait for a child process to complete. After this function
   returns, :attr:`~subprocess.running` is `false` and
   :attr:`~subprocess.exit_status` stores the exit code returned
   by the subprocess.
   
   This function handles cases in which stdin, stdout, or stderr
   for the child process is :const:`PIPE` by writing any
   input to the child process and buffering up the output
   of the child process as necessary while waiting for
   it to terminate.
   

.. method:: proc subprocess.close() throws

   
   Close any open channels and pipes for interacting with a subprocess.  This
   function does not wait for the subprocess to complete.  Note that it is
   generally not necessary to call this function since these channels will be
   closed when the subprocess record goes out of scope.
   

.. method:: proc subprocess.send_signal(signal: int) throws

   
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
   

.. method:: proc subprocess.kill() throws

   
   Unconditionally kill the child process.  The associated signal,
   `SIGKILL`, cannot be caught by the child process. See
   :proc:`subprocess.send_signal`.
   

.. method:: proc subprocess.terminate() throws

   
   Request termination of the child process.  The associated signal,
   `SIGTERM`, may be caught and handled by the child process. See
   :proc:`subprocess.send_signal`.
   

