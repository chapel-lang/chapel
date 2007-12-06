config const n = 10;

/* sequential for loop expression with if-then filtering: */
writeln(" odd values from 1..", n, ": ", 
        for i in 1..n do if isOdd(i) then i);

writeln("even values from 1..", n, ": ", 
        for i in 1..n do if !isOdd(i) then i);


def isOdd(x) {
  return (x%2 == 1);
}
