proc f(arg:owned ?t) {
  writeln(t:string);
}
class MyClass { }
f(new owned MyClass());
