var lock$: sync int;

coforall i in 1..10 do
  atomic lock$ = lock$ + 1;

writeln("lock = ", lock$.readFF());

coforall i in 1..10 do
  lock$ = lock$ + 1;

writeln("lock = ", lock$.readFF());