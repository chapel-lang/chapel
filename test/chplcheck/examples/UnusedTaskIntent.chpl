/*
  Task intents in a ``with`` clause should be used inside the task.

  An unused task intent only adds noise and makes the task look like it
  depends on data that it never touches, so either use the intent inside the
  task or remove it from the ``with`` clause.
*/

var A: [1..10] int;

// This forall has an unused task intent, which violates the rule.
forall i in 1..10 with (ref A) {
  writeln(i);
}

// This forall uses its task intent, which is correct.
forall i in 1..10 with (ref A) {
  A[i] = i;
}

// Removing the unused task intent is also correct.
forall i in 1..10 {
  writeln(i);
}
