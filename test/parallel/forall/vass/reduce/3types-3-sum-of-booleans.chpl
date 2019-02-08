
  var A = [false, false, true, false, true];
  var sum: real;
  forall elm in A with (PlusReduceOp(int) reduce sum) {
    sum reduce= elm;   // bools are implicitly coerced to 'int' input type
    writeln(sum);      // accumulation state: int
  }
  writeln(sum);        // result: real

  /* Implements + reduction over numeric data. */
  class PlusReduceOp: ReduceScanOp {

    /* the type of the elements to be reduced */
    type eltType;

    /* task-private accumulator state */
    var value: eltType;

    /* identity w.r.t. the reduction operation */
    proc identity         return 0: eltType;

    /* accumulate a single element onto the accumulator */
    proc accumulate(elm)  { value = value + elm; }

    /* accumulate a single element onto the state */
    proc accumulateOntoState(ref state, elm)  { state = state + elm; }

    /* accumulate the initial value of the outer variable onto the state */
    // make it an error if things don't work smoothly
    proc initialAccumulate(outerVar)  { assert(outerVar==(0:outerVar.type)); }

    // Note: 'this' can be accessed by multiple calls to combine()
    // concurrently. The Chapel implementation serializes such calls
    // with a lock on 'this'.
    // 'other' will not be accessed concurrently.
    /* combine the accumulations in 'this' and 'other' */
    proc combine(other)   { value = value + other.value; }

    /* Convert the accumulation into the value of the reduction
       that is reported to the user. This is trivial in our case. */
    proc generate()       return value;

    /* produce a new instance of this class */
    proc clone()          return new unmanaged PlusReduceOp(eltType=eltType);
  }
