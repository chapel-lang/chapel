const init_arr: [1..2] int = (0, 0);

class C {
  const arr : [1..2] int = init_arr;
}

const a : [1..2] int = (1, 2);

var c2 = new borrowed C(a);

var c1 : borrowed C?;
c1 = new borrowed C(a);

//var c3 = new C();

var c4: borrowed C?;

writeln("c1 is: ", c1);
writeln("c2 is: ", c2);
//writeln("c3 is: ", c3);
writeln("c4 is: ", c4);
