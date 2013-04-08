use Futures;

proc main(): void {
  var aFuture : future(string) = begin {
    var s = "hello-world";
    s;
  };
  writeln("Printing...");
  writeln(aFuture.get());
}
