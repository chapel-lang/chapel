proc foo( arg : domain(?) ) {
  var bar = proc( i : int ) : int { return 1 - i; };

  for i in arg do writeln( bar( i ) );
}

foo( {1..10} );
