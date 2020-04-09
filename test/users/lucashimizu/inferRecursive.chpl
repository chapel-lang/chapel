proc fibDecl( a ): int {
  if( a == 0 || a == 1){
    return a;
  } else {
    return fibDecl( a - 1 ) + fibDecl( a - 2 );
  }
}

proc fibInfer( a ) {
  if (a == 0 || a == 1) {
    return a;
  } else{
    return fibInfer( a - 1 ) + fibInfer( a - 2 );
  }
}

writeln("fib(7) = ", fibDecl(7));
writeln("fib(7) = ", fibInfer(7));
