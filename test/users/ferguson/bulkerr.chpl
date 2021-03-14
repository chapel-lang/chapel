/* This program passes, but gives errors
   when run under valgrind like:
==16157== Source and destination overlap in memcpy(0x4c28ea0, 0x4c28ea0, 24)
==16157==    at 0x4A07F24: memcpy (mc_replace_strmem.c:628)
==16157==    by 0x481234: chpl_comm_get (comm-none.c:89)
==16157==    by 0x42AB52: doiBulkTransfer (in /home/mppf/chapel/svn_trunk_clean/a.out)
==16157==    by 0x43C0EB: myread (in /home/mppf/chapel/svn_trunk_clean/a.out)
==16157==    by 0x43B662: chpl__init_bulkerr (in /home/mppf/chapel/svn_trunk_clean/a.out)
==16157==    by 0x441A12: chpl_gen_main (in /home/mppf/chapel/svn_trunk_clean/a.out)
==16157==    by 0x45EB27: chpl_main (main.c:75)
==16157==    by 0x47E739: chpl_task_callMain (tasks-fifo.c:299)
==16157==    by 0x45ECA6: main (main.c:161)
*/

proc myread(inout args ...?k) {
  for param i in 0..k-1 {
    writeln(args(i));
  }
}

/*
   Note - valgrind does not produce the error
   if we use the following function instead.
proc myread(inout arg) {
  writeln(arg);
}
*/

var A: [1..3] real;

myread(A);

writeln("A is: ", A);
