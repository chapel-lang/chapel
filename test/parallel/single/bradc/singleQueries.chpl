var x$: single bool;

writeln("x$.isFull? = ", x$.isFull);
writeln("x$.readXX = ", x$.readXX());

x$.writeEF(true);

writeln("x$.isFull? = ", x$.isFull);
writeln("x$.readXX = ", x$.readXX());
