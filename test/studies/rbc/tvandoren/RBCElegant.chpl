/* Basic RBC model with full depreciation.
 * 
 * Thomas Van Doren, Chapel Team
 * Seattle, July 7, 2014
 */

use Math;
use MatrixHelpers;
use Time;

// When true, print the elapsed time at the end.
config const printElapsed = true;

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
  var nGridCapital = 17820,
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

  while (maxDifference > tolerance) {
    expectedValueFunction = dotProduct(mValueFunction, transpose(mTransition));

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
