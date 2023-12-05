config const n = 10;

/*
{
  // error: cannot assign to const variable
  var sum: real;
  coforall i in 1..n do          // default task intent of scalars is ‘const in’
    sum += computeMyResult(i);   // so this is illegal: (and avoids a race)
}
*/
{
  var sum: real;
  coforall i in 1..n with (ref sum) do  // override default task intent
    sum += computeMyResult(i);          // we’ve now requested a race
  writeln("\n'ref' intent\n  sum = ", sum,
          ", can't trust this result due to a possible race"); 
}
{
  var sum: real;
  coforall i in 1..n with (+ reduce sum) do  // override default intent
    sum += computeMyResult(i);  // per-task sums will be reduced on task exit
  writeln("\n'reduce' intent\n  sum = ", sum,
          ", can trust this result due to 'reduce' intent"); 
}
{
  var sum: atomic real;
  coforall i in 1..n do           // default task intent of atomics is ‘ref’
    sum.add(computeMyResult(i));  // so this is legal, meaningful, and safe
  writeln("\n'ref' intent with an atomic\n  sum = ", sum,
          ", can trust this result due to use of 'atomic' type"); 
}

///////////////////////
// helper routines

// this can do whatever, but simplest thing is to return the value of i
proc computeMyResult(i) {
  return i;
}
