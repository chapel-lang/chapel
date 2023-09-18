var A : domain(int);
var B : domain(string);
var C : domain(real);

class X {
  var x : int;
}
var D : domain(borrowed X);

record Y {
  var y : real;
}
var E : domain(Y);
var y1 = new Y(), y2 = new Y();
var G = {y1, y2};
var F : domain(string) = {"bar", "foo"};
var Days = {"Sunday", "Wednesday", "Saturday"};
var intSet = {1, 2, 4, 5, 100};
