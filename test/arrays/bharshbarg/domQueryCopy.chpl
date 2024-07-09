
record Tracker {
  proc init=(other: Tracker) {
    init this;
    writeln("copied 'Tracker' instance");
  }
}

proc main() {
    var a: [0..<10] Tracker;
    var aw = new ArrayWrapper(a);
    writeln(aw.a);
}

record ArrayWrapper {
    type etype;
    param rank: int;
    var shape: rank*int;
    var a = makeDefaultArray(shape, etype);
}

proc ArrayWrapper.init(in a: [?d] ?t) {
    this.etype = t;
    this.rank = d.rank;
    this.shape = d.shape;
    this.a = a;

    writeln(d); // <- this line should not cause an extra copy of 'a'
}

proc makeDefaultArray(shape: ?N*int, type t) {
    var rngs: N*range;
    for param i in 0..<N do rngs[i] = 0..<shape[i];
    var a: [{(...rngs)}] t;
    return a;
}
