proc show(msg, rng) {
  writeln("amb", if rng.isAligned() then "- " else "+ ",
          msg, " = ", rng);
}

// ambiguously aligned
show("(1.. by -2)",
      (1.. by -2) );

show("(..1 by 2)",
      (..1 by 2) );

show("(.. by 2)",
      (.. by 2) );

// not ambiguously aligned, to contrast the above
writeln();

show("(1.. by 2)(1..5)",
      (1.. by 2)(1..5) );

show("1..5 by 2",
      1..5 by 2 );

show("(..)(1..5 by 2)",
      (..)(1..5 by 2) );
