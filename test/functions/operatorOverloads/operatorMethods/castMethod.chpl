record R {
  var x: int;
  
  operator :(x: int, type t:R) {
    return new R(x);
  }
}
  
var r = 3: R;
writeln(r);
