This is a supplement to the main Readme file. The reader should consult
that file for more information.


Table of Contents
-----------------

1. MPP vs. Workstation Cluster
2. Usage
3. iPSC/860
4. Paragon
5. CM5
6. IBM SP2
	AIX 3.x
	AIX 4.x
7. Shared-memory Systems (pvmd and internal)
	Solaris 2.X+
8. Shared-memory Systems (separate daemon)
	ATT SysV shmop supported systems


1. Massively Parallel Systems vs. Workstation Cluster
-----------------------------------------------------

	In the workstation environment, there is usually one (or a few at
the most) task(s) and one PVM Daemon (pvmd) on each host. On a MPP
machine, however, only one pvmd runs on the front-end and it has to
support all the tasks running on the back-end nodes. On an SP, the
node on which PVM is started becomes the front-end.

	On a MPP machine tasks are always spawned on the back-end nodes.
If you want to start a task on the front-end you have to run it as a 
Unix process, which then connects to the pvmd by calling pvm_mytid(). 
Tasks to be run on the front-end should be linked with the socket-
based libpvm3.a (i.e., compiled with the option "-lpvm3"), while tasks
to be spawned on the back-end must be linked with libpvm3pe.a (i.e.,
compiled with the option "-lpvm3pe").

	On the Paragon, tasks running on the back-end communicate with
one another directly, but packets going to another machine or to a
task on the front-end must be relayed by pvmd. To get the best 
performance, the entire program should run on the back-end. If you 
must have a master task running on the front-end (to read input from 
the user terminal, for example), the master should avoid doing any 
compute-intensive work.

    On the CM5 and SP, each batch of tasks created by pvm_spawn() 
form a unit. Within the same unit, message are passed directly. For 
each unit, PVM spawns an additional task to relay messages to and from 
pvmd, because tasks in the unit cannot communicate with pvmd directly. 
Communication between different units must go through pvmd. To get the
best performance, try to spawn all your tasks together.

	The system supports on nodes of some MPP machines are limited.
For example if your program forks on a node, the behavior of the new
process is machine-dependent. In any event it would not be allowed
to become a new PVM task. 

	Inter-host task-to-task direct routing via a TCP socket connection 
has not been implemented on the nodes. Setting the PvmRouteDirect option
on a node has no effect. 

	PVM message tags above 999999000 are reserved for internal use.

    You should not have to make any changes in your source code when you
move your PVM program from a workstation cluster to a MPP, unless your
program contains machine-dependent code. You do need to, however, modify
the makefile to link in the approriate libraries. In the examples 
directory, you'll find a special makefile for each MPP architecture, 
hidden under the ARCH directory. On some MPP you must use a special 
compiler or loader to compile your program.



2. Usage
--------

    Once properly installed, PVM can be started on a MPP by simply typing 
pvm or pvmd&. Although a MPP has many nodes, it is treated as a single 
host in PVM, just like a workstation. PVM has access to all the nodes in
the default partition. To run a program in parallel, the user spawns a 
number of tasks, which will be loaded onto different nodes by PVM. The 
user has no control of how the tasks are distributed among the nodes.
So running a parallel program on a MPP is like running it on a very 
powerful uniprocessor machine with multitasking. For example, to run 
nntime program on the IBM SP, we first start PVM on the high performance
switch, and then spawn 2 copies of nntime from the console:

r25n15% pvm3/lib/pvm -nr25n15-hps
pvm> conf
1 host, 1 data format
                    HOST     DTID     ARCH   SPEED
              r25n15-hps    40000   SP2MPI    1000
pvm> spawn -2 -> nntime
[1]
2 successful
t60000
t60001
pvm> [1:t60000] t60001: 100000 doubles received correctly
[1:t60000] 
[1:t60000] 
[1:t60000] t60000: 100000 doubles received correctly
[1:t60000] 
[1:t60000] 
[1:t60000] Node-to-node Send/Ack
[1:t60000] 
[1:t60000] Roundtrip T = 90 (us)  (0.0000 MB/s)      Data size: 0
[1:t60000] Roundtrip T = 105 (us)  (1.5238 MB/s)      Data size: 80
[1:t60000] Roundtrip T = 207 (us)  (7.7295 MB/s)      Data size: 800
[1:t60000] Roundtrip T = 920 (us)  (17.3913 MB/s)      Data size: 8000
[1:t60000] Roundtrip T = 5802 (us)  (27.5767 MB/s)      Data size: 80000
[1:t60000] Roundtrip T = 47797 (us)  (33.4749 MB/s)      Data size: 800000
[1:t60001] EOF
[1:t60000] EOF
[1] finished
pvm> halt
r25n15%

To run nntime from command line and not from the console you can use
the starter program provided in the examples directory.
r25n15% pvm3/lib/pvm -nr25n15-hps
pvm> conf
1 host, 1 data format
                    HOST     DTID     ARCH   SPEED
              r25n15-hps    40000   SP2MPI    1000
pvm> quit

r25n15% bin/SP2MPI/starter -n 2 nntime

    There is no need to "add" any nodes, "conf" showed only one host. We 
note that nntime is a true "hostless" (SPMD) program in the sense that it has
no master task. The spmd example, on the other hand, has a master
task which spawns other slave tasks. Hostless programs perform better
than master-slave programs because all the tasks can be spawned together
onto back-end nodes.

    PVM does not distinguish between a multiprocessor system and a
uniprocessor system. While machine-specific information such as the
number of processors and the power of individual processor would be 
useful for load-balancing purpose, PVM neither supplies nor makes use
of such information. 



3. iPSC/860
-----------

	This port was developed on an iPSC/860 with 128 nodes, using an
SRM as its front-end. Compiling and installing on a remote host may
require extensive knowledge of Intel software.


INSTALLATION

	Type "make" on the SRM front-end to build pvmd and libpvm3.a, and 
		make PVM_ARCH=CUBE
to build libpvm3pe.a on a machine (SRM or workstation) that has the 
icc/if77 i860 cross-compilers.

	To make the examples, repeat the above steps in the "examples"
directory but use ../lib/aimk instead of make. (Note that aimk ---
and pvmd, pvmgetarch, etc. --- is a sh script. You have to run them
under sh instead of csh. The i386 frontend running sysV3.2 does not 
understand #!, so you have to put a blank line at the beginning of
the scripts.)

	To build pvmd and libpvm3.a on a remote host (Sun/SGI) instead of
the SRM, try 
		make PVM_ARCH=I860
You may have to add a -L option to the definition of ARCHDLIB and a 
-I option to ARCHCFLAGS in conf/I860SUN4.def (or conf/I860SGI.def) 
to include the path of libhost.a and cube.h .



APPLICATION PROGRAMS

	Host (master) programs should be linked with pvm3/lib/I860/libpvm3.a 
and the SRM system library libsocket.a, node (slave) programs with 
pvm3/lib/ARCH/libpvm3pe.a and libnode.a .  Fortran programs should
also be linked with pvm3/lib/I860/libfpvm3.a (host program) or
pvm3/lib/I860/libfpvm3pe.a (node program).

	The programs in the gexamples directory have NOT been ported. The
user must modify the makefiles to link to the appropriate libraries.


BUGS AND TIPS

	a) On the SRM pvm is slow to start and it may sometimes time out and
print out the message "can't start pvmd", while in fact pvmd has already
been started. When this happens, just run pvm again and it'll connect
to the pvmd. If you start the pvm console on another machine and add the
I860 host to the configuration then this should not occur.

	b) To use PVM on a remote host you have to start "pvm3/lib/I860/pvmd"
manually in the background, because PVM has no way of knowing that your
workstation is being used as a front-end to the hypercube. PVM relies on
the shell script "pvm3/lib/pvmd" to determine the architecture of your
system, you can hack the script to make it do what you want.

	c) If PVM cannot obtain a cube of the size you specify because none
is available, it will return the error "Out of resources".

	d) The function pvm_spawn() will NOT return until all spawned tasks
check in with PVM. Therefore slave tasks should call pvm_mytid() before
starting any heavy duty computation.

	e) The only signal that can be sent to tasks running on the cube is
SIGKILL. Any other signals will be ignored.

	f) All spawned tasks should call pvm_exit() just before they exit.
Pvmd will release the cube after all tasks loaded into the cube have
called pvm_exit(). If any task attempts to continue execution after
it called pvm_exit(), it'll be terminated when the cube is released.

	g) There is a constant TIMEOUT in the file "pvmmimd.h" that controls 
the frequency at which the PVM daemon probes for packets from node tasks.
If you want it to respond more quickly you can reduce this value. 
Currently it is set to 10 millisecond.

	h) DO NOT use any native NX message passing calls in PVM or they
may interfere with pvm_send()/pvm_recv().

	i) Messages printed to "stderr" by node tasks do not end up in
the log file "/tmp/pvml.uid".

	j) The "argv" argument to pvm_spawn() is quietly ignored.



4. Paragon
----------

	This port has been tested on a Paragon XPS5 running R1.3. The support
and information provided by Intel engineers are gratefully acknowledged.


INSTALLATION

	Type "make" in this directory on the Paragon. To build on a cross-
compiler machine, you need to type
		setenv PVM_ARCH PGON
		make -e
The environmental variable PARAGON_XDEV must be set to the proper base 
directory in the later case.

	To make the examples, go to the "examples" directory and repeat the
above procedure but use ../lib/aimk instead of make.

STARTING THE PVMD WITH PEXEC

Many Paragon sites use the program "pexec" to manage partitions for 
application programs. The pvmd startup script has been modified to
use pexec, if it exists.  The environment variable NX_DFLT_SIZE should
be set in the user's .cshrc file and is the size (with a default of 1) 
of the partition that the pvmd will manage. The partition size can also be 
set by invoking the pvmd with the command
       % pvmd [pvmd options] -sz <part_size> 

The "pexec" program should exist in /bin/pexec. If it exists in another
location, modify the  pvm3/lib/pvmd startup script to point at the
site-specific location or have the system administrator make the 
appropriate link.

If a PGON is the first ("master") host in the virtual machine, the
pvmd should be started first, and then the console may be started:
       % pvmd -sz <partsize> & 
       % pvm
Other hosts, including other Paragons, may then be added using the console.
       pvm> add other_pgon
       pvm> add my_workstation
       
APPLICATION PROGRAMS

    Host (master) programs should be linked with pvm3/lib/PGON/libpvm3.a
and the system library librpc.a, node (slave) programs with
pvm3/lib/PGON/libpvm3pe.a, libnx.a, and librpc.a . Fortran programs should
also be linked with pvm3/lib/PGON/libfpvm3.a .

	The programs in the gexamples directory have NOT been ported. The
user must modify the makefiles to link to the appropriate libraries.

NATIVE MODE COLLECTIVE OPERATIONS
    
    If possible, native mode collective operations such as gsync,
gisum, and gdsum are used for group operations.  The native operations
will be used if all of following hold 
      a) A native collective operation exists on the Paragon
      b) All the nodes in the paragon compute partition are participating
         in the collective operation (pvm_barrier, pvm_reduce, etc.)
      c) The group has been made static with pvm_freezegroup

If all three conditions do not hold, the collective operation still functions
correctly, but does not use native operations. If all the PGON nodes are part
of a larger group, the native operations are used for the PGON part of the 
collective operation. 

BUGS AND CAVEATS

	a) Tasks spawned onto the Paragon run on the compute nodes by default.
Host tasks run on the service nodes and should be started from a Unix
prompt.

	b) By default PVM spawns tasks in your default partition. You can use
the NX command-line options such as "-pn partition_name" to force it to
run on a particular partition or "-sz number_of_nodes" to specify the
number of nodes you want it to use. Setting the environmental variable 
NX_DFLT_SIZE would have the same effect. For example starting pvmd with
the following command
		pvmd -pn pvm -sz 33
would force it to run on the partition "pvm" using only 33 nodes (there
must be at least that many nodes in the partition).

	c) The current implementation only allows one task to be spawned on
each node.

	d) There is a constant TIMEOUT in the file "pvmmimd.h" that controls 
the frequency at which the PVM daemon probes for packets from node tasks. 
If you want it to respond more quickly you can reduce this value. 
Currently it is set to 10 millisecond.

	e) DO NOT use any native NX message passing calls in PVM or they
may interfere with pvm_send()/pvm_recv().

	f) PVM programs compiled for versions earlier than 3.3.8 need to
be recompiled.  A small change in data passed to node tasks on startup
will cause earlier programs to break. 


5. CM5
------

	This port was developed on a 32-node CM5 running CMOST v7.2 and
CMMD3.0. We gratefully acknowledge the supports and information provided
by engineers of Thinking Machines Corp.


INSTALLATION

    Type "make" in this directory.

	To make the examples, go to the "examples" directory and type
		../lib/aimk default


APPLICATION PROGRAMS

    Host (master) programs should be linked with pvm3/lib/CM5/libpvm3.a .
Node (slave) programs need to be linked to pvm3/lib/CM5/libpvm3pe.a and 
joined with pvm3/lib/CM5/pvmhost.o and pvm3/lib/CM5/libpvm3.a using the
CMMD loader "cmmd-ld". Fortran programs should also be linked with 
pvm3/lib/CM5/libfpvm3.a .

    The programs in the gexamples directory have NOT been ported. The
user must modify the makefiles to link to the appropriate libraries.


BUGS

	a) The function pvm_kill() does not behave like a Unix kill: it
will delete the target task from PVM's task queue but doesn't actually
kill the task. 

	b) Signals sent by pvm_sendsig() are simply discarded, except for
SIGTERM and SIGKILL, which are equivalent to calling pvm_kill().

	c) You're not allowed to spawn more tasks than the number of nodes
available in your partition in one call to pvm_spawn().

	d) If you kill the node tasks from PVM (e.g., using pvm_kill), the
node processes will dump core files into your HOME directory. I don't
consider this to be a PVM bug.



6. IBM SP2
----------

	This implementation is built on top of MPI-F, IBM's version of the
Message Passing Interface. Information provided by Hubertus Franke of
IBM T. J. Watson Research Center is gratefully acknowledged.


KEY FOR OPERATING SYSTEM VERSION

	Based on the AIX version on your SP2, insert one of the following 
architecture designations for "<AIX_Arch>" in the discussion below.
                               ^^^^^^^^^^

	for AIX 3.x		use SP2MPI
	for AIX 4.x		use AIX4SP2


INSTALLATION

	Type "make PVM_ARCH=<AIX_Arch>" in this directory.  Make sure the
mpicc compiler is in your path.

	To make the examples, go to the "examples" directory, and type
		setenv PVM_ARCH <AIX_Arch>; ../lib/aimk


APPLICATION PROGRAMS

	Host (master) programs should be linked with:
		pvm3/lib/<AIX_Arch>/libpvm3.a .

	Node (slave) programs must be compiled with the mpicc compiler for
a C program and mpixlf compiler for a FORTRAN program, and then linked with
pvm3/lib/<AIX_Arch>/libpvm3pe.a . FORTRAN programs should also be linked with 
pvm3/lib/<AIX_Arch>/libfpvm3.a .

	The programs in the gexamples directory have NOT been ported. The
user must modify the makefiles to link to the appropriate libraries.


BUGS AND CAVEATS

	a) The user is required to set MP_PROCS and MP_RMPOOL environment
variables or provide a node file (MP_HOSTFILE) for the SP that lists
all the nodes the user intends to run PVM on. This is not to be confused 
with the PVM hostfile. Before starting PVM, the POE environment variable
MP_HOSTFILE should be set to the path of the user's SP node file. For
example I have the line "setenv MP_HOSTFILE ~/host.list" in my .cshrc file.
My host.list file contains the names of the high-performance switches on 
all the parallel nodes.  If you do not want to use the SP node file, then you
can set MP_PROCS to the maximum number of nodes you will need plus one.  The
number listed here should be equal to the number of nodes you would have
listed in the MP_HOSTFILE.  So if you need a 5 task job then you will need
to set MP_PROCS=6 and if you need spawned two 5 task jobs then you will
need to set MP_PROCS=12.  MP_RMPOOL is set to default value of "1" but
you may need to change it depending how your administrator has allocated
the SP nodes into different pools.  You can run "/usr/bin/jm_status -P"
to see what POOL id's and nodes are available.

	b) On a SP node only one process can use the switch (User Space)
at any given time.  If you want to share the switch between different
users then you will need to run in IP mode over the switch.  To do this, 
you must set "setenv MP_EUILIB ip" before starting PVM.  The default value 
for "MP_EUILIB" is "us".  These environment variables should be set
before starting PVM and this configuration will last until PVM is halted.
To run your program you need one node for each PVM task, plus one for the 
PVM host process that relays messages for pvmd. So if you want to spawn 8 
PVM tasks, for instance, you'll need at least 9 node names in your SP 
node file. Remember there is a host process for each group of tasks spawned
together. If you spawn 8 tasks in two batches, you'll need 10 nodes instead
of 9.

	c) Signals sent by pvm_sendsig() are simply discarded, except for
SIGTERM and SIGKILL, which are equivalent to calling pvm_kill().

	d) If you kill one PVM task using pvm_kill(), all the tasks spawned
together will also die in sympathy.

	e) To get the best performance for communications that involves pvmd,
for example when you pass messages between two group of tasks spawned
separately, use switch names in the node file instead of the host names,
and start pvm with the -nswitchname option. This would force any socket
communication to go over the high-performance switch rather than Ethernet.



7. Shared-memory Systems
------------------------

	These ports achieve better performance by using shared memory
instead of sockets to pass messages.  They're fully compatible with
the socket version.  In addition, tasks linked to different PVM
libraries can coexist and communicate with each other.  (But these
messages are routed through the PVM daemon instead of using shared
memory.)

On architectures that support shared memory operations, the default
PVM architecture selection is still non-shmem, therefore to turn on
selection of the appropriate *MP architecture the user must set the
environment variable "PVM_SHMEM" to "ON", as in:

  % setenv PVM_SHMEM ON

(Note that the old "$SGIMP" environment variable has been subsumed
with the new "$PVM_SHMEM" environment variable.)

The original socket library in PVM 3.4 is now (consistently) named
libpvm3.a, whether on a shared memory (*MP) architecture or not.
The new shared memory version of the PVM library is now named
"libpvm3s.a", and users can link their programs with -lpvm3s if
they would rather use shared memory for local communication.
(Note that this naming convention is the *reverse* of the PVM 3.3
naming convention - sorry.)

There is also now a new PVM shmd shared memory daemon, to control
and monitor the use of shared memory.  This alleviates a significant
number of bugs as compared to the PVM 3.3 implementation.

BUGS AND CAVEATS

	a) Messages are sent directly, pvm_setopt(PvmRoute, PvmRouteDirect)
has no effect.  The sender will block if the receiving buffer is full.
This could lead to deadlocks.

	b) The buffer size used by PVM, SHMBUFSIZE, is defined in 
pvm3/src/pvmshmem.h, with a default of 1MBytes.  You can change the
buffer size by setting the environment variable PVMBUFSIZE before
starting PVM.  Note that the default system limit on shared-memory
segment size (1 MB on Sun's) may have to be raised as well.

    c) PVM uses the last 10 bits of the Unix user ID as the base for
the shared-memory keys.  If there is a conflict with keys used by
another user or application, you can set the environment variable
PVMSHMIDBASE to a different value.

    d) If PVM crashes for any reason, you may have to remove the
leftover shared-memory segments and/or semaphores manually.  Use the
Uhix command `ipcs` to list them and `ipcrm` to delete them.  Or,
use the script `ipcfree` in $PVM_ROOT/lib.

    e) For best performance, use psend/precv.  If you must use the
standard send/recv, the InPlace encoding should be used where
appropiate.


Solaris2.3+
----------

APPLICATION PROGRAMS

	All PVM programs must be linked with the thread library (-lthread).
Refer to the example Makefile (pvm3/examples/SUNMP/Makefile) for 
details.

BUGS AND CAVEATS

	a) There is a system limit on the number of shared-memory segments
a process can attach to. This in turn imposes a limit on the number of
PVM tasks allowed on a single host. According to Sun Microsystem, the
system parameter can be set by a system administrator.

	b) Some of the newer PVM shared memory systems prefer to use semaphores
instead of mutex and conditional variables. Some of these systems also support
the V8plus instruction set. The semaphore version has been made the default
version at the request of Sun MicroSystems.
	For older systems that need to use the Mutex version, edit the
pvm3/conf/SUNMP.def file and add the -DPVMUSEMUTEX flag to the ARCHFLAGS line
and null out the PLOCKFILE line.


8. Shared-memory Systems (separate daemon)

This version is an add-on that is used to speed up particular application classes at
the expense of flexibility, while avoiding many of the un-welcome feature in the 
generic PVM shared memory system described above.

The system is designed to allow programs that use the pvm_psend and pvm_precv calls
to be accelerated through the use of a self monitoring shared memory daemon
that takes the burden of resource handling away from the PVM host daemons (pvmds).

In the generic version of shared memory all messages, even those received through forien
pvmds were placed into shared memory, and each task handled its own memory, even though
monitoring of processes was still via Unix pipes. This method had a number of know race 
conditions and other problems once the shared memory was full, or tasks forgot to cleanup
their own resources when exiting. This new version hands all the management to a single 
daemon, and allows the task-daemon operations to continue via pipes/sockets in the time proven
way.

Uses:
This version only uses shared memory for passing messages internal to a single SMP when using
pvm_psend, pvm_precv, and NOT pvm_send / pvm_recv with any packed buffers.
Messages off the machine (if enabled*) use sockets etc. *See readme in the shmd directory for 
more info.

To use the new version:
cd into the shmd directory and do an aimk.
This should build two executables, the shared memory daemon (pvm_shmd) and a program to probe
the daemon about its state (pvm_shmd_stat). The extra program is required as tickle from the PVM
console only tickles the pvmd and not other pvm tasks. (The new program is much like pvm_gstat
in the PVM group directory).
One other file will have been built,
libpvm3shmd.a
This is the library file that replaces libpvm3.a when you want to use the shared memory versions
of pvm_psend/pvm_precv. If you don't link to this you won't get any benefits.

Running the system:
Start the pvm_shmd,
host% pvm_shmd &

The pvm_shmd can be started automatically by calls to psend/precv in the libpvm3shmd.a, but this
has to be enabled at compile time. This is currently off as you probably want to monitor
how the pvm_shmd works so that you can tune it for your specific applications.

Now that the pvm_shmd is running you can use the shared memory calls. More info on the daemon
is in the manual pages under pvm_shmd(1PVM).

Other options and restrictions are listed in the shmd directory readme file.

Bugs: 
Only know bug to date appears on some linux boxes running RedHat.
When *somestimes* first starting the pvm_shmd, no output appears and the shmd daemon spins,
it appears that the kernel is creating the SysV resources and holding the daemon on a 
shmget() call. Just CTRL C the daemon, and start it again!

Warnings:
As mentioned in the pvm_shmd(1PVM) manual page, don't give all your virtual memory to the
pvm_shmd for use in message passing, as you'll need some running actual applications. 
Under Solaris, this is usually very restricted (6-8 segments 1MB each).
IRIX6 on the otherhand can be very very genereous.... and make all the VM shared memory for 
message passing!

Supported Architectures:
This system was made using only standard SysV calls. So it should work on *any* platform that
supports these calls. (Pigs have been known to fly).

The systems that shmd has been tested under include:
SGI IRIX64 6.5 
SGI IRIX64 origin 6.5 (Origin 2000)
SunOS 5.5.1 sun4u sparc SUNW,Ultra-2
SunOS 5.6 sun4u sparc SUNW,Ultra-2
SunOS sol 5.5.1 sun4m sparc SUNW,SPARCstation-10 (4 processor Sun)
IBM AIX 2 4 (SP2 4 processor high node)
HP V-Class 2250 under HP-UX 11.0
Linux 2.2.2 #9 SMP Sun Feb 28 14:19:40 EST 1999 i686 unknown (Dell Dual PII Linux box)

