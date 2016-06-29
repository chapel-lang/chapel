config const numTasks = 8;

coforall tid in 1..numTasks do
  writeln("Hello from task ", tid, " of ", numTasks);

writeln("Signing off...");
