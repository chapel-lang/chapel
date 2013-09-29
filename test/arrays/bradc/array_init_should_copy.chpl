var A: [1..10] [1..3] real;

 {
    var B = A[1];
    var C = B;

    writeln("C is: ", C);
    writeln("B is: ", B);
    writeln("A[1] is: ", A[1]);

    B = 1.0;
    C = 2.0;
    A[1] = 3.0;

    writeln("C is: ", C);
    writeln("B is: ", B);
    writeln("A[1] is: ", A[1]);
  }

writeln("Outside B's scope");
writeln("A[1] is: ", A[1]);
