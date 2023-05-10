proc test(type t) {
  var vals = [Types.max(t), Types.max(t) - 1, Types.min(t), Types.min(t) + 1 ]:t;

  writeln("Type: ", t:string);
  for v1 in vals {
    for v2 in vals {
      if v2 != 0 {
        var r = v1 % v2;
        writef("%24n %% %24n = %24n\n", v1, v2, r);
      }
    }
  }
}

type STs = (int(8), int(16), int(32), int(64));
for param i in 0..<STs.size  {
  type ST = STs(i);
  test(ST);
}
