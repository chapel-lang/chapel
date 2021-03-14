// What is the precedence of ! vs cast?
// spec says that : binds more tightly than !

// how is ! true : string interpreted ?
//   (!true) : int -> 0
//   ! (true:int)  -> false     (spec says this)
writeln((!true):int);  // expect 0
writeln(!(true:int));  // expect false
writeln(! true : int); // expect false
