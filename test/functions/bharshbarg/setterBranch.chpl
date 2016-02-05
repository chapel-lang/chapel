
class foo {
  var data : int;
  
  proc this(i : int) ref {
    return data;
  }
  proc this(i : int) {
    return (data,42);
  }

}

var f = new foo();
f(1) += 5; // foo.data = 5
writeln(f(1)); // (5, 42)
