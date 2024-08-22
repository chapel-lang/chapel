record myRec: hashable {
  var x: int;

  proc hash(salt = 0): uint throws {
    // This is not meant to be a good hash function, just to test throwing
    if (salt == 0) then throw new IllegalArgumentError("salt too obvious!");
    else return x*salt;
  }
}

proc main() {
  var myR = new myRec(15);
  try {
    var hash1 = myR.hash();
  } catch e: IllegalArgumentError {
    writeln(e.message());
    var hash2 = myR.hash(3);
    writeln(hash2);
  } catch e {
    halt(e.message());
  }
}
