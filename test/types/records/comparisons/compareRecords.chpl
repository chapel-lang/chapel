config param case = 1;

record State {
  var board : [1..2] int;

  proc init(board) {
    this.board = board;
  }
}

// Uncommenting this fixes things:
/*
proc ==(l: State, r: State) {
  return (&& reduce (l.board == r.board));
}
*/

var s1 = new State(board = 33);
var s2 = new State(board = 33);
var s3 = new State(board = 78);

select (case) {
  when 1 {
    writeln(s1 < s1);
    writeln(s1 < s2);
    writeln(s1 < s3);
  }
  when 2 {
    writeln(s1 <= s1);
    writeln(s1 <= s2);
    writeln(s1 <= s3);
  }
  when 3 {
    writeln(s1 > s1);
    writeln(s1 > s2);
    writeln(s1 > s3);
  }
  when 4 {
    writeln(s1 >= s1);
    writeln(s1 >= s2);
    writeln(s1 >= s3);
  }
  otherwise {
    compilerError("Unexpected case");
    halt("Unexpected case");
  }
}
