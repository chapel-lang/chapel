class C {
  var A: [1..5] int;
}

var GA: [1..5] int = [i in 1..5] i;

var c1 = new C(A=GA);
var c2 = new C(A=>GA);

writeln("global array:            ", GA);
writeln("class w/ copied array:   ", c1);
writeln("class w/ aliased array:  ", c2);

GA(3) = 0;

writeln("global array:            ", GA);
writeln("class w/ copied array:   ", c1);
writeln("class w/ aliased array:  ", c2);

c1.A(3) = -1;
c2.A(3) = -2;

writeln("global array:            ", GA);
writeln("class w/ copied array:   ", c1);
writeln("class w/ aliased array:  ", c2);

// comment these out because failure to do so causes valgrind
// problems as of PR #4762
//
// delete c2;
// delete c1;
