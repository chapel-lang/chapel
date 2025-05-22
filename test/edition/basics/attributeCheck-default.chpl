// Validate that "default" is not a valid edition name for the attribute
// (it is okay for the compilation flag, but I don't think there's a good reason
//  to support it for the attribute, since it is expected to change over time
//  the point of the attribute is to define where a change happens for all time)
@edition(first="default")
proc foo() {
  writeln("uh oh!");
}

proc main() {
  foo();
}
