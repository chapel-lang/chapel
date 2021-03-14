/* The goal of this test is to show that our beautify code had a bug
   in it that chopped lines into 1024-char chunks and inserted #line
   directives between the chunks incorrectly.  This relies on the
   assumption that the internal types used for domain names are long
   enough to exceed this 1024-character limit for the function
   prototype of foo().  */

const Dom: domain(1) = {1..5};

const A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z: [Dom] real;

config const verbose = false;

proc foo(X1, X2, X3, X4, X5, X6, X7, X8, X9, X10, X11, X12, X13, X14, X15,
        X16, X17, X18, X19, X20, X21, X22, X23, X24, X25, X26) {
  if (verbose) {
    writeln("X1 is: ", X1);
    writeln("X2 is: ", X2);
    writeln("X3 is: ", X3);
    writeln("X4 is: ", X4);
    writeln("X5 is: ", X5);
    writeln("X6 is: ", X6);
    writeln("X7 is: ", X7);
    writeln("X8 is: ", X8);
    writeln("X9 is: ", X9);
    writeln("X10 is: ", X10);
    writeln("X11 is: ", X11);
    writeln("X12 is: ", X12);
    writeln("X13 is: ", X13);
    writeln("X14 is: ", X14);
    writeln("X15 is: ", X15);
    writeln("X16 is: ", X16);
    writeln("X17 is: ", X17);
    writeln("X18 is: ", X18);
    writeln("X19 is: ", X19);
    writeln("X20 is: ", X20);
    writeln("X21 is: ", X21);
    writeln("X22 is: ", X22);
    writeln("X23 is: ", X23);
    writeln("X24 is: ", X24);
    writeln("X25 is: ", X25);
    writeln("X26 is: ", X26);
  }
  writeln("In foo()");
}

foo(A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z);
