var x = 1;
config var numoftasks = 2;

writeln("This is the main task: x = ",x);

coforall taskid in 1..numoftasks do
{
  var c = taskid + 1;
  writeln("This is task ",taskid,": x + ",taskid," = ",x+taskid,". My value of c is: ",c);
}

writeln("This message won't appear until all tasks are done...");
