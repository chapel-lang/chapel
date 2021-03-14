config param testConstAssigns = 0;
config param writelocals = true;

proc foo(const in x) {
  writeln("in foo");
  if testConstAssigns == 1 then
    x = "5";
  if writelocals then
    writeln("local x is: ", x);
}

proc bar(const ref x) {
  writeln("in bar");
  if testConstAssigns == 2 then
    x = "7";
  if writelocals then
    writeln("local x is: ", x);
}

proc baz(in x) {
  writeln("in baz");
  x = "11";
  if writelocals then
    writeln("local x is: ", x);
}

proc boo(ref x) {
  writeln("in boo");
  x = "13";
  if writelocals then
    writeln("local x is: ", x);
}

proc goo(const x) {
  writeln("in goo");
  if testConstAssigns == 3 then
    x = "17";
  if writelocals then
    writeln("local x is: ", x);
}

proc hoo(x) {
  writeln("in hoo");
  if testConstAssigns == 4 then
    x = "19";
  if writelocals then
    writeln("local x is: ", x);
}

proc moo(inout x) {
  writeln("in moo");
  x = "21";
  if writelocals then
    writeln("local x is: ", x);
}

proc zoo(out x) {
  writeln("in zoo");
  x = "23";
  if writelocals then
    writeln("local x is: ", x);
}

var x = "3";

foo(x);
writeln(x);
bar(x);
writeln(x);
baz(x);
writeln(x);
boo(x);
writeln(x);
goo(x);
writeln(x);
hoo(x);
writeln(x);
moo(x);
writeln(x);
zoo(x);
writeln(x);
