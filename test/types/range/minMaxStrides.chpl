
proc test(type t) {
  type ST = int(Types.numBits(t));
  type UT = uint(Types.numBits(t));
  writeln("Testing signed ", ST:string, " and unsigned ", UT:string);


  for by_ in (Types.min(ST), Types.max(ST), Types.max(ST):UT) {
    for align_ in (Types.min(ST), Types.max(ST), Types.max(UT)) {
      var r = (0:align_.type)..(100:align_.type) by by_ align align_;
      writef("by_=%20i:%-10s; align_=%20i:%-10s; range=%t\n", by_, by_.type:string, align_, align_.type:string, r);
    }
  }
}

test(int(8));
test(int(16));
test(int(32));
test(int(64));
