/* Basic RBC model with full depreciation.
 * 
 * Thomas Van Doren, Chapel Team
 * Seattle, July 7, 2014
 */

use Math;
use Time;

// When true, print the elapsed time at the end.
config const printElapsed = true,
  gridCapital = 17820;

proc main() {
  // 0. Housekeeping
  var timer: Timer;
  timer.start();

  // 1. Calibration
  var aalpha = 1.0 / 3.0,  // Elasticity of output w.r.t. capital
    bbeta = 0.95;          // Discount factor

  // Productivity values
  var vProductivity = [0.9792, 0.9896, 1.0000, 1.0106, 1.0212];

  // Transition matrix
  // This creates a 1 dimensional array and then "reshapes" it into a 2
  // dimensional domain, 5x5 array.
  var mTransition = reshape([0.9727, 0.0273, 0.0000, 0.0000, 0.0000,
                             0.0041, 0.9806, 0.0153, 0.0000, 0.0000,
                             0.0000, 0.0082, 0.9837, 0.0082, 0.0000,
                             0.0000, 0.0000, 0.0153, 0.9806, 0.0041,
                             0.0000, 0.0000, 0.0000, 0.0273, 0.9727],
                            {1..5, 1..5});

  // 2. Steady State
  var capitalSteadyState = (aalpha * bbeta) ** (1 / (1 - aalpha)),
    outputSteadyState = capitalSteadyState ** aalpha,
    consumptionSteadyState = outputSteadyState - capitalSteadyState;

  writeln("Output = ", outputSteadyState,
          ", Capital = ", capitalSteadyState,
          ", Consumption = ", consumptionSteadyState, "\n");

  // We generate the grid of capital
  var nGridCapital = gridCapital,
    nGridProductivity = 5;
  var vGridCapital: [1..nGridCapital] real;
  forall (value, nCapital) in zip(vGridCapital, 0..) {
    value = 0.5*capitalSteadyState + 0.00001 * nCapital;
  }

  // 3. Required matrices and vectors.
  var gridProductivityDomain = {1..nGridCapital, 1..nGridProductivity};
  var mOutput, mValueFunction, mValueFunctionNew, mPolicyFunction,
    expectedValueFunction: [gridProductivityDomain] real;

  // 4. We pre-build output for each point in the grid.

  for (valueProductivity, nProductivity) in zip(vProductivity, 1..) {
    mOutput[1..nGridCapital, nProductivity] =
      valueProductivity * (vGridCapital ** aalpha);
  }

  // 5. Main iteration

  var maxDifference = 10.0,
    tolerance = 0.0000001,
    iteration = 0;

  var gridCapitalNextPeriod: int,
    valueHighSoFar, valueProvisional, consumption, capitalChoice: real;

  // Intermediate array, to be reused in matMultiply() call inside loop.
  var mTransitionTranspose: [1..5, 1..5] real;
  transpose(mTransitionTranspose, mTransition);

  while (maxDifference > tolerance) {
    // expectedValueFunction = mValueFunction * transpose(mTransition)
    matMultiply(expectedValueFunction, mValueFunction, mTransitionTranspose);

    for nProductivity in 1..nGridProductivity {

      // We start from previous choice (monotonicity of policy function)
      gridCapitalNextPeriod = 1;

      for nCapital in 1..nGridCapital {
        valueHighSoFar = -100000.0;
        capitalChoice = vGridCapital[1];

        for nCapitalNextPeriod in gridCapitalNextPeriod..nGridCapital {
          consumption = mOutput[nCapital, nProductivity] - vGridCapital[nCapitalNextPeriod];
          valueProvisional = (1 - bbeta) * log(consumption) + bbeta *
            expectedValueFunction[nCapitalNextPeriod, nProductivity];

          if valueProvisional > valueHighSoFar {
            valueHighSoFar = valueProvisional;
            capitalChoice = vGridCapital[nCapitalNextPeriod];
            gridCapitalNextPeriod = nCapitalNextPeriod;
          } else {
            break;
          }

          mValueFunctionNew[nCapital, nProductivity] = valueHighSoFar;
          mPolicyFunction[nCapital, nProductivity] = capitalChoice;
        }
      }
    }

    maxDifference = max reduce abs(mValueFunctionNew - mValueFunction);
    mValueFunction = mValueFunctionNew;
    mValueFunctionNew = 0.0;

    iteration += 1;
    if (iteration % 10 == 0 || iteration == 1) {
      writeln("Iteration = ", iteration, ", Sup Diff = ", maxDifference);
    }
  }

  writeln("Iteration = ", iteration, ", Sup Diff = ", maxDifference, "\n");
  writeln("My check = ", mPolicyFunction[1000, 3], "\n");

  timer.stop();
  if printElapsed {
    writeln("Elapsed time is: ", timer.elapsed());
  }
}


// Transpose inputMatrix and store in outputMatrix.
proc transpose(ref outputMatrix: [?D] real, ref inputMatrix: [?E] real)
  where D.rank == 2 && E.rank == 2
{
  // If not running with --fast (e.g. boundsChecking == true), ensure the array
  // dimensions are correct.
  if boundsChecking {
    assert(D.dim(0) == E.dim(1) && D.dim(1) == E.dim(0),
           "Dimensions of outputMatrix are not transpose of inputMatrix.");
  }

  forall (row, col) in D {
    outputMatrix[row, col] = inputMatrix[col, row];
  }
}


// Calculate dot product (matrix multiplication) of A and B and store result in
// C. Specifically, it does C = A * B.
proc matMultiply(ref C: [?DC] real, ref A: [?DA] real, ref B: [?DB] real)
  where DC.rank == 2 && DA.rank == 2 && DB.rank == 2
{
  // If not running with --fast (e.g. boundsChecking == true), ensure the array
  // dimensions are correct.
  if boundsChecking {
    assert(DC.dim(0) == DA.dim(0) &&
           DC.dim(1) == DB.dim(1),
           "Outer dimensions for C, A, or B do not match.");
    assert(DA.dim(1) == DB.dim(0),
           "Inner array dimensions do not match.");
  }

  // There is a more Chapelerific way to implement 2D matrix dot product, but
  // it suffers from poor performance. See tests in
  // $CHPL_HOME/test/performance/thomasvnadoren/ for more details.
  forall (row, col) in DC {
    C[row, col] = 0.0;
    for z in DA.dim(1) do
      C[row, col] += A[row, z] * B[z, col];
  }
}
