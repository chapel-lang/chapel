class X {
  var x : int;
}

record Y {
  var y : real;
}

var y1 = new Y(), y2 = new Y();

// Default parSafe
var A : domain(int);
var B : domain(string);
var C : domain(real);
var D : domain(borrowed X);
var E : domain(Y);
var F : domain(string) = {"bar", "foo"};
var G = {y1, y2};
var Days = {"Sunday", "Wednesday", "Saturday"};
var intSet = {1, 2, 4, 5, 100};

// parSafe=true
var A2 : domain(int, parSafe=true);
var B2 : domain(string, parSafe=true);
var C2 : domain(real, parSafe=true);
var D2 : domain(borrowed X, parSafe=true);
var E2 : domain(Y, parSafe=true);
var F2 : domain(string, parSafe=true) = {"bar", "foo"};

// parSafe=false
var A3 : domain(int, parSafe=false);
var B3 : domain(string, parSafe=false);
var C3 : domain(real, parSafe=false);
var D3 : domain(borrowed X, parSafe=false);
var E3 : domain(Y, parSafe=false);
var F3 : domain(string, parSafe=false) = {"bar", "foo"};
