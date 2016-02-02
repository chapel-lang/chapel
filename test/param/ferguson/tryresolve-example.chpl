use Reflection;

proc foo(x:int) {
  writeln("foo(int)");
}

proc bar(x:int) {
  writeln("bar(int)");
}

proc bar(x:string) {
  writeln("bar(string)");
}


proc callFooOrBar(type t)
{
  var x:t;

  if __primitive("call resolves", "foo", x) then foo(x);
  else bar(x);
}

proc main() {

  callFooOrBar(int); // prints foo(int)
  callFooOrBar(string); // prints bar(string)

}

