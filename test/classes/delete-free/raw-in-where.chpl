class Domain {
}
class MyDomain : Domain {
  var x;
}

proc foo(type t) where t:raw Domain
{
  writeln("In foo where");
}

proc foo(type t)
{
  writeln("In fallback foo");
}


proc test() {
  foo(raw MyDomain(int));
  foo(MyDomain(int));
  foo(raw Domain);
  foo(Domain);
}
test();
