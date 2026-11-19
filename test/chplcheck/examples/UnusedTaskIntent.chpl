/*
  Task intents (such as ``ref``) in a ``with`` clause should be used inside
  the task.
*/

var A: [1..10] int;

// This forall has an unused task intent which violates the rule.
forall i in 1..10 with (ref A) {
  writeln(i);
}

// This forall uses its task intent which is correct.
forall i in 1..10 with (ref A) {
  A[i] = i;
}
