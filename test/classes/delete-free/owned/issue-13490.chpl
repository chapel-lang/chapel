class MyClass { }

proc f(arg: record) {
  writeln("f(arg: record)");
} 
proc f(arg) {
  writeln("f(arg)");
}
writeln(isRecordType(owned MyClass));
var x = new owned MyClass(); 
f(x);
