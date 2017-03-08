use Futures;

config const X = 1234;

var F = async(lambda(x:int) { return x:string; }, X)
  .andThen(lambda(x:string) { return(x.length); });

writeln(F.get());
