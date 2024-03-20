record S {
  type t1;
  type t2;
  var x: (t1, t2);
}

type t = S(x=(int, real));
writeln(t:string);
