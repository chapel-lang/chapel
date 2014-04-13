// do not run this program :)

proc iloop() {
  writeln("in an infinite loop");
}

const intconst = 5;
const boolconst = true;
var i = 0;

while intconst do  // need warning
  iloop();

while boolconst do  // need warning
  iloop();

while intconst {  // no warning
  i += 1;
  if i < 10 then
    break;
}

while intconst { // need warning
  i += 1;
  if i < 10 then 
    continue;
}
i = 0;

procdriver(3, true);

proc procdriver(intarg: int, boolarg: bool) {
  while intarg do  // need warning
    iloop();
  while boolarg do  // need warning
    iloop();
  for iterdriver(33, true) do
    iloop();
  while intarg {  // no warning
    i += 1;
    if i < 10 then
      break;
  }
  while intconst {  // need warning
    i += 1;
    if i < 10 then
      continue;
  }
  i = 0;

  while intarg {  // no warning
    i += 1;
    if i < 10 then
      return;
  }
}

iter iterdriver(intarg: int, boolarg: bool) {
  while intarg do  // no warning
    yield 3;
  while boolarg do  // no warning
    yield 4;
  while intarg {  // no warning
    i += 1;
    if i < 10 then
      return;
  }
}

var x: bool = true;
cobegin {
  while x do; // need warning
  writeln(x);
}
