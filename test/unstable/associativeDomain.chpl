class X {
  var x : int;
}

record Y {
  var y : real;
}

var y1 = new Y(), y2 = new Y();

{
  // Default parSafe
  var A1 : domain(int);
  var B1 : domain(string);
  var C1 : domain(real);
  var D1 : domain(borrowed X);
  var E1 : domain(Y);
  var F1 : domain(string) = {"bar", "foo"};
  var G1 = {y1, y2};
  var Days = {"Sunday", "Wednesday", "Saturday"};
  var intSet = {1, 2, 4, 5, 100};
  var A = [A1];
  var B = [B1];
  var C = [C1];
  var D = [D1];
  var E = [E1];
  var F = [F1];
  var G = [G1];
}


// The warnings for -sparSafeDefault seem to be optimized away
// But they reappear if you comment out the above block?
{
 // parSafe=true
  var A2 : domain(int, parSafe=true);
  var B2 : domain(string, parSafe=true);
  var C2 : domain(real, parSafe=true);
  var D2 : domain(borrowed X, parSafe=true);
  var E2 : domain(Y, parSafe=true);
  var F2 : domain(string, parSafe=true) = {"bar", "foo"}; // parSafe=true warning gets optimized away, but its there if you remove the other vars in this block
  var A = [A2];
  var B = [B2];
  var C = [C2];
  var D = [D2];
  var E = [E2];
  var F = [F2]; // parSafe=true warning gets optimized away, but its there if you remove the other vars in this block
}

{
  // parSafe=false
  var A3 : domain(int, parSafe=false);
  var B3 : domain(string, parSafe=false);
  var C3 : domain(real, parSafe=false);
  var D3 : domain(borrowed X, parSafe=false);
  var E3 : domain(Y, parSafe=false);
  var F3 : domain(string, parSafe=false) = {"bar", "foo"};
  var A = [A3];
  var B = [B3];
  var C = [C3];
  var D = [D3];
  var E = [E3];
  var F = [F3];
}
