pragma "inline" function foo() {
  var i : integer = 0;
  while(i < 10) {
    i = i + 1;
    writeln(i);
    if (i > 5) {
      return;
    }
    if (i > 8) {
      return;
    }
  }

  writeln(i*10);
  return;    
}

function main() {
 foo();
 foo();
 return;
}