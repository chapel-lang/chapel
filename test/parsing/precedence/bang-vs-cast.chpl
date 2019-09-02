
class C {
  var x: int;
}

type tq = borrowed C?;
var bq:borrowed C? = nil;

// how is bq : tq ! intepreted?
//   bq : (tq!)   ->  throwing cast to non-nilable type
//   (bq : tq) !  ->  no-op cast and then halt if nil
try {
  writeln(bq:(tq!));
} catch e {
  writeln("case 1 error as expected");
}

//writeln((bq:tq)!); // halt

try {
  writeln(bq:tq!);  // hoping this will throw 
} catch e {
  writeln("case 2 error as expected");
}



// What is the precedence of ! vs cast?
// spec says that : binds more tightly than !

// how is ! true : string interpreted ?
//   (!true) : int -> 0
//   ! (true:int)  -> false     (spec says this)
writeln((!true):int);  // expect 0
writeln(!(true:int));  // expect false
writeln(! true : int); // expect false
