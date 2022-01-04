config type myType = int;
config const initval: myType = 71:myType;
config const iters = 250; // < GASNet limit on number of threads

var sx: sync myType;

sync for i in 1..(if iters%2==0 then iters else iters+1) {
  if i%2==0 then
    begin {
      var x: myType = sx.readFE();
      if x != initval then writeln("incorrect value: ", x);
    }  
  else
    begin {
      sx.writeEF(initval);
      if sx.readXX() != initval then writeln("incorrect value: ", sx.readXX());
    }
}

writeln("DONE");
