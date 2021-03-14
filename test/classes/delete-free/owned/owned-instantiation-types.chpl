class MyClass {
  var x:int;
  proc init() { }
}

proc blank_untyped(x) {
  writeln("blank_untyped ", x.type:string);
}
blank_untyped(new owned MyClass());

proc blank_owned(x:owned) {
  writeln("blank_owned ", x.type:string);
}
blank_owned(new owned MyClass());

proc blank_class(x:MyClass) {
  writeln("blank_class ", x.type:string);
}
blank_class(new owned MyClass());


proc const_untyped(const x) {
  writeln("const_untyped ", x.type:string);
}
const_untyped(new owned MyClass());

proc const_owned(const x:owned) {
  writeln("const_owned ", x.type:string);
}
const_owned(new owned MyClass());

proc const_class(const x:MyClass) {
  writeln("const_class ", x.type:string);
}
const_class(new owned MyClass());


proc in_untyped(in x) {
  writeln("in_untyped ", x.type:string);
}
in_untyped(new owned MyClass());

proc in_owned(in x:owned) {
  writeln("in_owned ", x.type:string);
}
in_owned(new owned MyClass());

proc in_class(in x:MyClass) {
  writeln("in_class ", x.type:string);
}
in_class(new owned MyClass());


proc const_in_untyped(const in x) {
  writeln("const_in_untyped ", x.type:string);
}
const_in_untyped(new owned MyClass());

proc const_in_owned(const in x:owned) {
  writeln("const_in_owned ", x.type:string);
}
const_in_owned(new owned MyClass());

proc const_in_class(const in x:MyClass) {
  writeln("const_in_class ", x.type:string);
}
const_in_class(new owned MyClass());

proc ref_untyped(ref x) {
  writeln("ref_untyped ", x.type:string);
}
{ var x = new owned MyClass(); ref_untyped(x); }

proc ref_owned(ref x:owned) {
  writeln("ref_owned ", x.type:string);
}
{ var x = new owned MyClass(); ref_owned(x); }

// compilation error
/*proc ref_class(ref x:MyClass) {
  writeln("ref_class ", x.type:string);
}
{ var x = new owned MyClass(); ref_class(x); }*/

proc const_ref_untyped(const ref x) {
  writeln("const_ref_untyped ", x.type:string);
}
const_ref_untyped(new owned MyClass());

proc const_ref_owned(const ref x:owned) {
  writeln("const_ref_owned ", x.type:string);
}
const_ref_owned(new owned MyClass());

// compilation error
/*proc const_ref_class(const ref x:MyClass) {
  writeln("const_ref_class ", x.type:string);
}
const_ref_class(new owned MyClass());*/
