proc show(msg, rng) {
  writeln("amb", if rng.isAmbiguous() then "+" else "-", " ",
          msg, " = ", rng);
}

// ambiguously aligned
show("(1.. by -2)(0..5)",
      (1.. by -2)(0..5) );

show("(1.. by -2)(1..5)",
      (1.. by -2)(1..5) );

show("(1.. by -2)(2..5)",
      (1.. by -2)(2..5) );

show("(.. by 2)(1..5)",
      (.. by 2)(1..5) );

show("(.. by -2)(1..5)",
      (.. by -2)(1..5) );

// assuming the present "relatively prime" clause
show("(.. by -2)(1..5 by -3)",
      (.. by -2)(1..5 by -3) );

// not ambiguously aligned, to contrast the above
writeln();

show("(1.. by 2)(1..5)",
      (1.. by 2)(1..5) );

show("1..5 by 2",
      1..5 by 2 );

show("(..)(1..5 by 2)",
      (..)(1..5 by 2) );
