class Domain {
}
class MyDomain : Domain {
  var x;
}

proc foo(type t) where t:unmanaged Domain
{
  writeln("In foo where");
}

proc foo(type t)
{
  writeln("In fallback foo");
}


proc test() {
  foo(unmanaged MyDomain(int));
  foo(MyDomain(int));
  foo(unmanaged Domain);
  foo(Domain);
}
test();
