class Base {
  proc ff(ref ipos : int) { };
}

class A : Base {
  override proc ff(ref ipos : int) {
    ipos += 1;
  }
}

var x : owned Base? = new owned A();

var ipos1 = 0;
x!.ff(ipos1);
writeln(ipos1);

// Why do I get a different answer here?
{
  var ipos2 = 0;
  x!.ff(ipos2);
  writeln(ipos2);
}
