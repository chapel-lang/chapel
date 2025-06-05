class C{}
record R {
  var x: owned C?;
}

iter foo() do yield new R(new owned C?());
var A1 = foo();

var D = {1..10};
var A2 = for D do new R(new owned C?());
var A3 = forall D do new R(new owned C?());
var A4 = foreach D do new R(new owned C?());

var A5 = for A2 do new R(new owned C?());
var A6 = forall A3 do new R(new owned C?());
var A7 = foreach A4 do new R(new owned C?());

writeln((A1, A2, A3, A4, A5, A6, A7).type : string);
