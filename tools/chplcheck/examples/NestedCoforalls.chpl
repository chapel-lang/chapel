/*
 Nested coforall loops can lead to performance issues because each coforall
 creates one task per iteration. A nested coforall creates N×M tasks, which
 can overwhelm the runtime with excessive task creation overhead.
*/

/*
 This creates 10 × 10 = 100 tasks (one for each (i,j) pair).
 With larger ranges, this quickly becomes problematic:
 e.g., 1000 × 1000 = 1,000,000 tasks
*/
coforall i in 1..10 {
  coforall j in 1..10 {
    writeln(i, j);
  }
}

/*
 This creates only 10 tasks (one per value of i).
 The inner loop runs serially within each task.
*/
coforall i in 1..10 {
  for j in 1..10 {
    writeln(i, j);
  }
}
