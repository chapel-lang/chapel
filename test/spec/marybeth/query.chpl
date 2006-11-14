
writeln(sumOfThree(1,2,3));
writeln(sumOfThree(4.0,5.0,3.0));

def sumOfThree(x: ?t, y:t, z:t):t {
   var sum: t;
   
   sum = x + y + z;
   return sum;
}
