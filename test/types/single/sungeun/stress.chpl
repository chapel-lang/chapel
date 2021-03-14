config type myType = int;
config const initval: myType = 71:myType;
config const iters = 250; // < GASNet limit on number of threads

var sx: single myType;

sync for i in 1..(if iters%2==0 then iters else iters+1) {
  if i==iters/2 then
    begin {
      sx.writeEF(initval);
      if !sx.isFull then writeln("invalid state (empty)");
      if sx.readXX() != initval then writeln("incorrect value: ", sx.readXX());
    }
  else
    begin {
      var x: myType = sx.readFF();
      if x != initval then writeln("incorrect value: ", x);
    }  

}

writeln("DONE");
