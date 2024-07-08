use Futures;

config const X = 1234;

var F = async(proc(x:int) { return x:string; }, X)
  .andThen(proc(x:string) { return(x.size); });

writeln(F.get());
