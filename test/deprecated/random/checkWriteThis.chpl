// This test locks in an odd bug with the point of instantiation, where calling
// another generic function with a first class function caused us to notice a
// missing use statement for PCGRandomStream's writeThis function, even though
// the writeThis function should not have been considered when resolving the
// other generic function.
module Foo {
  use Random;
  use Random.PCGRandom only PCGRandomStream;

  proc generateToken(len: int=32) : string {
    var alphanum = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
                    "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
                    "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
                    "a", "b", "c", "d", "e", "f", "g", "h", "i", "j","k", "l", "m",
                    "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"
                    ];
    var upper_bound = alphanum.size;
    var indices : [0..upper_bound-1] int;
    for i in 0..upper_bound-1 do
      indices[i] = i;

    var ret : [0..len-1] string;
    var r = new owned PCGRandomStream(int);
    var rindices = try! r.choice(indices, len);

    for i in 1..len-1 do
      ret[i] = alphanum[rindices[i]];
    return ''.join(ret);
  }
}

module Two {
  use IO;
  proc otherGenFunc(x) throws {
    writeln("%s".format(x));
  }
}
module Three {
  use Two;

  proc callGenFunc() {
    try! otherGenFunc(otherFunc); // Note missing parentheses here
  }

  proc otherFunc() {
    writeln("eh");
  }
}
module User {
  use Foo, Three;
  proc main() {
    callGenFunc();
    var token = generateToken(32);
    writeln(token);
  }
}
