use IO, List, Map;

config const debug = false;

const Pairs  = ['('=>')', '['=>']', '{'=>'}',  '<'=>'>' ];
const Scores = [')'=>3,   ']'=>57,  '}'=>1197, '>'=>25137];

iter readlines() {
  var line: string;
  while readLine(line) {
    yield line.strip();
  }
}

var navSubSys = readlines();

var score: atomic int;

forall (command, lineno) in zip(navSubSys, 1..) {
  var stack: list(string);
  for ch in command {
    if Pairs.domain.contains(ch) then
      stack.pushBack(ch);
    else {
      const tail = stack.last();
      const expected = Pairs[tail];
      if ch == expected then
        stack.popBack();
      else {
        if debug then
          writeln("line ", lineno, ": expected ", expected, " but got ", ch);
        score.add(Scores[ch]);
        break;
      }
    }
  }
}
writeln(score.read());
