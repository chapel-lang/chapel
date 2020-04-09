config const n = 100;

/* sequential for loop expression with if-then filtering: */
writeln("odd values from 1..", n, ": ", 
        for i in 1..n do if isOdd(i) then i);

proc isOdd(x) {
  return (x%2 == 1);
}


/* "parallel" forall loop expression: */
writeln("doubled values: ", [i in 1..n] 2*i);
