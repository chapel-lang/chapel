function foo() : integer {
  var i : integer = 0;
  while(i < 10) {
    i = i + 1;
    writeln(i);
    if (i > 5) {
      return i;
    }
    if (i > 8) {
      return 7;
    }
  }

  writeln(i*10);
  return i;    
}

function main() {
 foo();
 return;
}