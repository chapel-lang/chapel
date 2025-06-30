//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
/* START_EXAMPLE */
use Collectives;

config const numTasks = here.maxTaskPar;
var b = new barrier(numTasks);

coforall tid in 1..numTasks {
  writeln("Task ", tid, " is entering the barrier");
  b.barrier();
  writeln("Task ", tid, " is past the barrier");
}
/* STOP_EXAMPLE */
