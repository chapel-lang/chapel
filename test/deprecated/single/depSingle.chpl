var x: single int;
var y: single int = 4;
var z: [1..10] single int;

proc p(x: single ) { }
proc p(x: single int ) { }
proc p(x: [] single int ) { }

type T = single int;
var t: T; 

proc decl(type T) {
  var t: T; 
}
decl(single int);
