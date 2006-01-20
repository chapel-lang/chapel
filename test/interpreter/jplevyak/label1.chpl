
pragma "inline" function foo() {
  var i : integer = 0;
  while(i < 10) {
    i = i + 1;
    writeln(i);
    if (i > 5) {
      goto one;
    }
  }
  writeln(i*10);
  label one {}; 
  return;    
}

function main() {
 foo();
 return;
}

