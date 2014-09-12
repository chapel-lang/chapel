
class foo {
  type elType;
  var temp : elType;
  var data : int;

  proc this(i : int) ref where setter {
    return data;
  }

  proc this(i : int) {
    return (data, true);
  }
}

var f = new foo(int);
f(1) += 5; // foo.data = 5
writeln(f(1)); // 5
