function foo() : int {
  var i : int = 0;
  while(i < 10) {
    i = i + 1;
    writeln(i);
    if (i > 5) {
      return i;
    }
  }

  writeln(i*10);
  return i;    
}

function main() {
 foo();
 return;
}
