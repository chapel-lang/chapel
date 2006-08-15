pragma "fast" def foo(type t, x:t) { 
  pragma "now" return 1; 
}
writeln(foo(int, 0));
