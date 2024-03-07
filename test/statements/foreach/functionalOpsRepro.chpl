class MyClass {
  var prop = 42;
}

var x : shared MyClass = new shared MyClass();

foreach i in 0..0 {
  x.prop = 52;
}
writeln(x.prop);
