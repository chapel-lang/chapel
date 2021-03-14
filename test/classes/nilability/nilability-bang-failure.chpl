module test {
  class Parent { var p:int; }

  class Child : Parent { var c:int; }

  config const option: int = 1;

  proc main() {
    var nbq:borrowed Child? = nil;
    var nuq:unmanaged Child? = nil;

    if option == 1 {
      writeln("nbq!");
      var x = nbq!;
      writeln(x.type:string, " ", x);
    }
    if option == 2 {
      writeln("nuq!");
      var x = nuq!;
      writeln(x.type:string, " ", x);
    }
  }
}
