class MyClass { var x:int; }
record R { var field: owned MyClass = new owned MyClass(1); }
const x = new R();
const y = x; // should be an error
