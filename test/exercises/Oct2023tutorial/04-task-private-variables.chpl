config const n = 100;
config const numTasks = 10;

// Task-parallel features support task-private variables easily
coforall i in 1..numTasks {
  var mySum: real;  // each task gets its own copy of mySum
  for j in 1..n do
    mySum += 3; // FIXME
//    mySum += A[i][j];
}

// Forall loops need special support for task-private variables
{
  var oneSingleVariable: real;
  forall i in 1..n {
    var onePerIteration: real;
  }
}

// Forall loops need special support for task-private variables
{
  var oneSingleVariable: real;
  forall i in 1..n with (var onePerTask: real) {
    var onePerIteration: real;
  }
}

{
  var oneSingleVariable: real;
  forall i in 1..n with (var onePerTask = 3.14) {
    var onePerIteration: real;
  }
}
{
  var oneSingleVariable: real;
// FIXME: want to ask about this one
//  forall i in 1..n with (ref myLocArr = A[localInds]) {
//    var onePerIteration: real;
//  }
}

