// Ensure that param formals retain their param-ness inside loop expressions.

const RR = 1..2;

proc make(param ARG, type TYP) param {
  return ARG:string + "::" + TYP:string;
}

proc doit(param ARG, type TYP) {
  var XX = for jjj in RR do make(ARG,TYP);
  writeln(XX);
}

proc main {
  doit(666, int);
}
