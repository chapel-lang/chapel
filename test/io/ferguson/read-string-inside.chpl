record R {
  var str: string;
}

var r = new R("x");
var a = ["x", "x"];
var t = ("x", "x");
var s = "x";

readln(r);
readln(a);
readln(t);
readln(s);

assert(r.str == "string in record");
assert(a[1] == "string in array");
assert(a[2] == "other string in array");

assert(t(1) == "string in tuple");
assert(t(2) == "other string in tuple");

assert(s == "string");
