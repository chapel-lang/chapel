use Map;

class B {
  var name:string;
}

record R {
  var bar: map(string, B);
}

var x = new R();
writeln(x);

/*
  warn of partial instantiation, see github.com/chapel-lang/chapel/issues/18005
*/
