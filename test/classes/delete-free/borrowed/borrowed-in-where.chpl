class Domain {
}
class MyDomain : Domain {
  var x;
}

proc foo(type t) where isSubtype(t, borrowed Domain)
{
  writeln("In foo where");
}

proc foo(type t)
{
  writeln("In fallback foo");
}


proc test() {
  foo(unmanaged MyDomain(int));
  foo(borrowed MyDomain(int));
  foo(unmanaged Domain);
  foo(borrowed Domain);
}
test();
