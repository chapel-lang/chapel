pragma "fast" function foo(type t, x:t) { 
  pragma "now" return 1; 
}
writeln(foo(integer, 0));
