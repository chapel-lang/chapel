// task intent examples for 04-Chapel-more-parallelism slide deck.

config const n = 100;
config const m = 10;

///////////////////////
// Task intents in 'forall' loops: scalar variables

/*
// Default intent of scalars is 'const in' so this is illegal (and avoids a race)
// error: cannot assign to const variable
{
  var sum: real;
  forall i in 1..n do
    sum += computeMyResult(i);
  writeln("sum = ", sum); 
}
*/

// with 'ref' intent, we are requesting a race
// NOTE: To see 'ref' intent sum and 'reduce' intent sum being different, try to 
// force a race by setting higher numbers of n
//      ./04-task-intents-forall --n 10000
{
  var sum: real;
  forall i in 1..n with (ref sum) do
    sum += computeMyResult(i);
  writeln("\n'ref' intent\n  sum = ", sum,
          ", can't trust this result due to a possible race"); 
}

// 'reduce' intent, each task accumulates into its own copy and
// on loop exit, all tasks combine their results into original ‘sum3’ scalar
{
  var sum: real;
  forall i in 1..n with (+ reduce sum) do
    sum += computeMyResult(i);
  writeln("\n'reduce' intent\n  sum = ", sum,
          ", can trust this result due to 'reduce' intent"); 
}


///////////////////////
// Task intents in 'forall' loops: scalar variables

// 'ref' intent avoids array copies, but can result in data races
{
  var bucketCount: [0..<m] real;
  forall i in 1..n with (ref bucketCount) do
    bucketCount[i % m] += 1;
  writeln("\n'ref' intent\n  bucketCount = ", bucketCount, 
          ", can't trust this result due to a possible race"); 
}

// 'in' intent will result in each task having its own copy
{
  var bucketCount: [0..<m] real;
  forall i in 1..n with (in bucketCount) do
    bucketCount[i % m] += 1;
  writeln("\n'in' intent\n  bucketCount = ", bucketCount,
          ", not being copied out of tasks so all zeros"); 
}

// 'reduce' intent results in each task having own copy, but then on loop exit 
// tasks combine their results into the original 'bucketCount' variable
{
  var bucketCount : [0..<m] real;
  forall i in 1..n with (+ reduce bucketCount) do   // request reduce on exit
    bucketCount[i % m] += 1;                     // each task has its own copy now
  writeln("\n'reduce' intent\n  bucketCount = ", bucketCount,
          ", should have a valid count of all "); 
}

///////////////////////
// helper routines

// this can do whatever, but simplest thing is to return the value of i
proc computeMyResult(i) {
  return i;
}
