/* Basic RBC model with full depreciation.
 * 
 * Thomas Van Doren, Chapel Team
 * Seattle, July 2, 2014
 */

use Math;
use Time;

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

  // FIXME: What is the multidimensional array syntax? This produces a 5 elment
  //        1d array of 1day arrays with 5 elements.
  //        (thomasvandoren, 2014-07-02)
  var mTransition = [[0.9727, 0.0273, 0.0000, 0.0000, 0.0000],
                     [0.0041, 0.9806, 0.0153, 0.0000, 0.0000],
                     [0.0000, 0.0082, 0.9837, 0.0082, 0.0000],
                     [0.0000, 0.0000, 0.0153, 0.9806, 0.0041],
                     [0.0000, 0.0000, 0.0000, 0.0273, 0.9727]];

  // 2. Steady State
  var capitalSteadyState = (aalpha * bbeta) ** (1 / (1-aalpha)),
    outputSteadyState = capitalSteadyState ** aalpha,
    consumptionSteadyState = outputSteadyState - capitalSteadyState;

  writeln("Output = ", outputSteadyState,
          ", Capital = ", capitalSteadyState,
          ", Consumption = ", consumptionSteadyState, "\n");

  // We generate the grid of capital
  var nCapitalNextPeriod, gridCapitalNextPeriod,
    nProductivity, nProductivityNextPeriod: int;
  const nGridCapital = 17820,
    nGridProductivity = 5;

  var vGridCapital: [1..nGridCapital] real;

  forall (nValue, nCapital) in zip(vGridCapital, 0..) {
    nValue = 0.5 * capitalSteadyState + 0.00001 * nCapital;
  }

  // 3. Required matrices and vectors.
  var gridProductivityDomain = {1..nGridCapital, 1..nGridProductivity};
  var mOutput, mValueFunction, mValueFunctionNew, mPolicyFunction,
    expectedValueFunction: [gridProductivityDomain] real;

  // 4. We pre-build output for each point in the grid.

  // TODO: May be more performant to use "for param" as outer loop, since we
  //       know it is small. (thomasvandoren, 2014-07-02)
  forall (productivityValue, nProductivity) in zip(vProductivity, 1..) {
    forall (gridCapitalValue, nCapital) in zip(vGridCapital, 1..) {
      mOutput[nCapital, nProductivity] = productivityValue * gridCapitalValue ** aalpha;
    }
  }

  // 5. Main iteration

  var maxDifference = 10.0;
  var diff, diffHighSoFar: real;
  var tolerance = 0.0000001;
  var valueHighSoFar, valueProvisional, consumption, capitalChoice: real;
  var iteration = 0;

  // FIXME: Remove the parens here. They are not needed, but their presence
  //        works around a bug in the emacs major mode.
  //        (thomasvandore, 2014-07-02)
  while (maxDifference > tolerance) {

    // TODO: Rewrite these two forall loops as a single forall in
    //       expectedValueFunction. (thomasvandoren, 2014-07-02)
    forall nProductivity in 1..nGridProductivity {
      forall nCapital in 1..nGridCapital {
        expectedValueFunction[nCapital, nProductivity] = 0.0;

        // TODO: Try to rewrite this as some kind of + reduce (product
        //       expression). (thomasvandoren, 2014-07-02)
        for nProductivityNextPeriod in 1..nGridProductivity {
          expectedValueFunction[nCapital, nProductivity] +=
            mTransition[nProductivity][nProductivityNextPeriod] *
            mValueFunction[nCapital, nProductivityNextPeriod];
        }
      }
    }

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

    diffHighSoFar = -100000.0;

    for nProductivity in 1..nGridProductivity {
      for nCapital in 1..nGridCapital {
        diff = abs(mValueFunction[nCapital, nProductivity] -
                   mValueFunctionNew[nCapital, nProductivity]);
        if diff > diffHighSoFar {
          diffHighSoFar = diff;
        }
        mValueFunction[nCapital, nProductivity] = mValueFunctionNew[nCapital, nProductivity];
      }
    }
    maxDifference = diffHighSoFar;

    iteration += 1;
    if (iteration % 10 == 0 || iteration == 1) {
      writeln("Iteration = ", iteration, ", Sup Diff = ", maxDifference);
    }
  }

  writeln("Iteration = ", iteration, ", Sup Diff = ", maxDifference, "\n");
  writeln("My check = ", mPolicyFunction[999, 2], "\n");

  // Finish up and print elapsed time.
  timer.stop();
  writeln("Elapsed time is: ", timer.elapsed());
}
