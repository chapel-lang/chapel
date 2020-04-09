// Code from reduceIntents.chpl tech note

/* Implements + reduction over numeric data. */
class PlusReduceOp: ReduceScanOp {

  /* the type of the elements to be reduced */
  type eltType;

  /* task-private accumulator/reduction state */
  var value: eltType;

  /* identity w.r.t. the reduction operation */
  proc identity         return 0: eltType;

  /* accumulate a single element onto the accumulator */
  proc accumulate(elm)  { value = value + elm; }

  /* accumulate a single element onto the state */
  proc accumulateOntoState(ref state, elm)  { state = state + elm; }

  /* accumulate the value of the outer variable at the entry to the loop */
  // Note: this method is optional. If it is not provided,
  // accumulate(outerVar) is used instead.
  proc initialAccumulate(outerVar) { value = value + outerVar: eltType; }

  // Note: 'this' can be accessed by multiple calls to combine()
  // concurrently. The Chapel implementation serializes such calls
  // with a lock on 'this'.
  // 'other' will not be accessed concurrently.
  /* combine the accumulations in 'this' and 'other' */
  proc combine(other: borrowed PlusReduceOp)   { value = value + other.value; }

  /* Convert the accumulation into the value of the reduction
     that is reported to the user. This is trivial in our case. */
  proc generate()       return value;

  /* produce a new instance of this class */
  proc clone()          return new unmanaged PlusReduceOp(eltType=eltType);
}

{
  // Use the above class.
  var A = [1000, 200, 30, 4];
  var sum: int;
  forall elm in A with (PlusReduceOp reduce sum) {
    sum += elm;  // equivalently:  sum reduce= elm;
  }
  writeln(sum);
}

{
  // To have different input/accumulator/result types of the reduction,
  // specify the input type explicitly, e.g. PlusReduceOp(int) below:
  var A = [false, false, true, false, true];
  var sum: real;
  forall elm in A with (PlusReduceOp(int) reduce sum) {
    sum reduce= elm;   // bools are implicitly coerced to 'int' input type
    writeln(sum.type:string); // This varies from actual tech note
    // writeln(sum);      // accumulation state: int
  }
  writeln(sum);        // result: real
}

