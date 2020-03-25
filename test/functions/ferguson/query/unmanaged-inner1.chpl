proc f(arg:unmanaged ?t) {
  writeln(t:string);
}
class MyClass { }
f(new unmanaged MyClass());
