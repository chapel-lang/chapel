use DynamicIters;

config const numTasks = here.maxTaskPar;

proc testIters(iterand, ref A) {
  for    i in iterand                                                 do A[i] += 1;
  forall i in iterand                                                 with (ref A) do A[i] += 1;
  forall (i, j) in zip(iterand, iterand)                              with (ref A) do A[i] += 1;

  for    i in dynamic(iterand, numTasks=numTasks)                     do A[i] += 1;
  forall i in dynamic(iterand, numTasks=numTasks)                     with (ref A) do A[i] += 1;
  forall (i, j) in zip(dynamic(iterand, numTasks=numTasks), iterand)  with (ref A) do A[i] += 1;

  for    i in guided(iterand, numTasks=numTasks)                      do A[i] += 1;
  forall i in guided(iterand, numTasks=numTasks)                      with (ref A) do A[i] += 1;
  forall (i, j) in zip(guided(iterand, numTasks=numTasks), iterand)   with (ref A) do A[i] += 1;

  for    i in adaptive(iterand, numTasks=numTasks)                    do A[i] += 1;
  forall i in adaptive(iterand, numTasks=numTasks)                    with (ref A) do A[i] += 1;
  forall (i, j) in zip(adaptive(iterand, numTasks=numTasks), iterand) with (ref A) do A[i] += 1;
}

// test dynamic iters with an explicit numTasks
var r = 1..10;
var d = {r};
var A: [d] int;

testIters(r, A);
testIters(d, A);

writeln(A);
