var lock: atomic int;
const numtasks = 5;

lock.write(0); //The main task set lock to zero

coforall id in 1..numtasks
{
  writeln("Greetings from task ",id,"... I am waiting for all tasks to say hello");
  lock.add(1);            // task id says hellow and atomically adds 1 to lock
  lock.waitFor(numtasks); // then it waits for lock to be equal to numtasks
                          // (which will happen when all tasks say hello)
  writeln("Task ",id," is done...");
}
