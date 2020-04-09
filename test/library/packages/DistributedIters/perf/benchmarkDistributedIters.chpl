/*
  Benchmarks for testing the DistributedIters module performance.

  ..
    Part of a 2017 Cray summer intern project by Sean I. Geronimo Anderson
    (github.com/s-geronimoanderson) as mentored by Ben Harshbarger
    (github.com/benharsh).
*/
use BlockDist,
    DistributedIters,
    Math,
    Random,
    ReplicatedDist,
    Time;

/*
  If true, write out only the test time result (ideal for shell scripts).
*/
config const timing:bool = false;

/*
  Toggle debugging output.
*/
config const debug:bool = false;

/*
  Benchmark mode (iterator):

  - ``default``
    The block-distributed default iterator.

  - ``guided``
    The distributed guided load-balancing iterator.
*/
enum iterator
{
  default,
  dynamic,
  guided
};

/*
  Used to select the benchmark mode (which iterator to use). Defaults to
  ``guided``. See :data:`iterator` for more information.
*/
config const mode:iterator = iterator.guided;

/*
  Test cases for array values:

  - ``constant``
    All array elements have the same constant value.

  - ``kryptonite``
    Large values in the beginning and decreasing exponentially.

  - ``normal``
    Normally distributed, scaled to [0,1].

  - ``outlier``
    Most values close to average with handful of outliers.

  - ``rampdown``
    Values follow a linearly decreasing function.

  - ``rampup``
    Values follow a linearly increasing function.

  - ``stacked``
    Small values mostly but increasing exponentially near the end.

  - ``uniform``
    Uniformly random.
*/
enum testCase
{
  constant,
  kryptonite,
  normal,
  outlier,
  rampdown,
  rampup,
  stacked,
  uniform
};

/*
  Used to select test case. Defaults to ``uniform``. See :data:`testCase` for
  more information.
*/
config const test:testCase = testCase.uniform;

/*
  If ``coordinated`` is ``true``, then the iterator dedicates one locale
  to distributing work to the remaining locales (if applicable).
*/
config const coordinated:bool = false;

/*
  Dynamic-iterator--specific options.
*/
config const localeChunkSize:int = 0;
config const chunkSize:int = 1;

/*
  Guided-iterator--specific option.
*/
config const minChunkSize:int = 1;

/*
  Used to determine the iteration count and total work per iteration.
*/
config const n:int = 1000;

const controlRange:range = 0..#n;
const controlDomain:domain(1) = {controlRange};
const globalRandomSeed:int = 13;
var timeResult:real;

select mode
{
  when iterator.default do timeResult = testControlWorkload();
  when iterator.dynamic do timeResult = testDynamicWorkload();
  when iterator.guided do timeResult = testGuidedWorkload();
}

if timing
then writeln("%dr".format(timeResult));
else
{
  writeln("test = ", test);
  writeln("mode = ", mode);
  writeln("n = ", n);
  writeln("nl = ", numLocales);
  writeln("Time = ", timeResult);
}

/*
  Testing procedures.
*/
pragma "no doc"
private proc testDynamicWorkload()
{
  var timer:Timer;

  const replicatedDomain:domain(1) dmapped Replicated() = controlDomain;
  var array:[controlDomain]real;
  var replicatedArray:[replicatedDomain]real;

  fillArray(array);

  // Ensure all locales have the same array.
  coforall L in Locales
  do on L
  do for i in controlDomain
  do replicatedArray[i] = array[i];

  timer.start();
  forall i in distributedDynamic(controlRange,
                                 chunkSize=chunkSize,
                                 localeChunkSize=localeChunkSize,
                                 coordinated=coordinated)
  {
    const k:real = (array[i] * n):int;

    // Simulate work.
    isPerfect(k:int);
  }
  timer.stop();

  const timerElapsed:real = timer.elapsed();
  timer.clear();
  return timerElapsed;
}

pragma "no doc"
private proc testGuidedWorkload()
{
  var timer:Timer;

  const replicatedDomain:domain(1) dmapped Replicated() = controlDomain;
  var array:[controlDomain]real;
  var replicatedArray:[replicatedDomain]real;

  fillArray(array);

  // Ensure all locales have the same array.
  coforall L in Locales
  do on L
  do for i in controlDomain
  do replicatedArray[i] = array[i];

  timer.start();
  forall i in distributedGuided(controlRange,
                                minChunkSize=minChunkSize,
                                coordinated=coordinated)
  {
    const k:real = (array[i] * n):int;

    // Simulate work.
    isPerfect(k:int);
  }
  timer.stop();

  const timerElapsed:real = timer.elapsed();
  timer.clear();
  return timerElapsed;
}

pragma "no doc"
private proc testControlWorkload():real
{
  var timer:Timer;

  const D:domain(1) dmapped Block(boundingBox=controlDomain) = controlDomain;
  var array:[D]real;

  fillArray(array);

  timer.start();
  forall i in D
  {
    const k:real = (array[i] * n):int;

    // Simulate work.
    isPerfect(k:int);
  }
  timer.stop();

  const timerElapsed:real = timer.elapsed();
  timer.clear();
  return timerElapsed;
}

/*
  Array fills.
*/
pragma "no doc"
private proc fillArray(array)
{ // Call the appropriate array-filling function based on config const ``test``
  select test
  {
    when testCase.constant do fillConstant(array);
    when testCase.kryptonite do fillKryptonite(array);
    when testCase.normal do fillNormallyDistributed(array);
    when testCase.outlier do fillCubicOutliers(array);
    when testCase.rampdown do fillRampDown(array);
    when testCase.rampup do fillRampUp(array);
    when testCase.stacked do fillStacked(array);
    when testCase.uniform do fillUniformlyRandom(array);
  }
  if debug then writeArrayStatistics(array);
}

pragma "no doc"
private proc fillConstant(array, constant=1)
{
  const arrayDomain = array.domain;
  forall i in arrayDomain do array[i] = constant;
}

pragma "no doc"
private proc fillLinear(array, slope, yIntercept)
{
  const arrayDomain = array.domain;
  forall i in arrayDomain do array[i] = ((slope * i) + yIntercept);
  normalizeSum(array);
}

pragma "no doc"
private proc fillRampDown(array) { fillLinear(array, (-1.0/n:real), 1.0); }

pragma "no doc"
private proc fillRampUp(array) { fillLinear(array, (1.0/n:real), 0); }

pragma "no doc"
private proc fillCubicOutliers(array)
{
  const arrayDomain = array.domain;
  fillRandom(array, globalRandomSeed);
  /*
    Creating outliers: a_3 through a_0 are coefficients for a cubic function
    extrapolation for these (x,y) points:

      {(0, 0.5), (0.25, 0.5), (0.5, 0.5), (0.75, 0.5), (1, 1)}.

    Thus the function

      a_3*x^3 + a_2*x^2 + a_1*x + a_0

    translates x values in the interval [0,1] into values that follow this
    distribution:

      ~80% are between 0.45 and 0.55,
      ~10% are between 0.55 and 0.75,
      ~5% are between 0.75 and 0.85,
      ~5% are between 0.85 and 1.
  */
  const a_3:real = 2.66667;
  const a_2:real = -2.85714;
  const a_1:real = 0.690476;
  const a_0:real = 0.492857;
  forall i in arrayDomain
  {
    const x:real = array[i];
    const xSquared:real = (x ** 2);
    const xCubed:real = (x * xSquared);
    const translatedX:real = ((a_3 * xCubed)
                              + (a_2 * xSquared)
                              + (a_1 * x)
                              + a_0);
    array[i] = (0.8 * translatedX);
  }
  normalizeSum(array);
}

/*
  fillKryptonite uses the guided iterator's behavior against it by putting
  almost all the work into the first work unit, using the same closed-form
  expression that the iterator uses for dividing work units.
*/
pragma "no doc"
private proc fillKryptonite(array, desiredProcessorCount:int=0)
{
  const processorCount:int = if desiredProcessorCount == 0
                             then if dataParTasksPerLocale == 0
                                  then here.maxTaskPar
                                  else dataParTasksPerLocale
                             else desiredProcessorCount;
  const arrayDomain = array.domain;
  const arraySize:int = array.size;

  const lengthOverProcessorCount:real = (arraySize:real
                                         / processorCount:real);
  const commonRatio:real = (1.0 - (1.0 / processorCount:real));

  forall i in arrayDomain do array[i] = (lengthOverProcessorCount
                                         * (commonRatio ** i));
  normalizeSum(array);
}

/*
  fillStacked is the opposite of fillKryptonite, and supports the guided
  iterator's behavior by making the work per iteration approximately equal
  (work per iteration increases exponentially).
*/
pragma "no doc"
private proc fillStacked(array, desiredProcessorCount:int=0)
{
  const processorCount:int = if desiredProcessorCount == 0
                             then if dataParTasksPerLocale == 0
                                  then here.maxTaskPar
                                  else dataParTasksPerLocale
                             else desiredProcessorCount;
  const arrayDomain = array.domain;
  const arraySize:int = array.size;

  const lengthOverProcessorCount:real = (arraySize:real
                                         / processorCount:real);
  const commonRatio:real = (1.0 - (1.0 / processorCount:real));

  forall i in arrayDomain do array[i] = (lengthOverProcessorCount
                                         * (commonRatio ** (arraySize - i)));
  normalizeSum(array);
}

pragma "no doc"
private proc fillNormallyDistributed(array)
{
  const arrayDomain = array.domain;
  fillRandom(array, globalRandomSeed);
  /*
    https://en.wikipedia.org/wiki/Normal_distribution#Alternative_parameterizations

    sigma is standard deviation (which should be "mean by three").
    precision is 1/sigma.
  */
  const mean:real = 0.5;
  const precision:int = 6;
  const precisionByRootTwoPi:real = (precision:real/((2.0 * Math.pi) ** 0.5));
  const minusPrecisionSquaredByTwo:real = (-1.0 * ((precision:real ** 2.0)
                                                   /2.0));
  forall i in arrayDomain
  {
    const x:real = array[i];
    const power:real = (minusPrecisionSquaredByTwo
                        * ((x - mean) ** 2.0));
    const translatedX:real = (precisionByRootTwoPi * (Math.e ** power));
    array[i] = translatedX;

  }
  normalizeSum(array);
}

pragma "no doc"
private proc fillUniformlyRandom(array)
{
  fillRandom(array, globalRandomSeed);
  normalizeSum(array);
}

/*
  Helpers.
*/
private proc normalizeSum(array, desiredSum=0)
{ // Make an array's sum equal its size by scaling all values appropriately.
  const arrayDomain = array.domain;
  const targetSum:int = if desiredSum == 0
                        then array.size
                        else desiredSum;
  var sum:real;

  for i in arrayDomain do sum += array[i];
  const scalingFactor:real = (targetSum:real / sum);
  array *= scalingFactor;
}

private proc properDivisors(n:int):domain(int)
{ // Return an associative domain of ``n``'s proper divisors.
  var result:domain(int) = {1};
  var quotient:real;
  for i in 2..n/2
  {
    quotient=n/i;
    if quotient == quotient:int
    then result += i;
  }
  return result;
}

private proc isPerfect(n:int):bool
{ /*
    Return whether ``n`` is a perfect number (i.e. equals its proper divisor
    sum).
  */
  var properDivisorsN:domain(int) = properDivisors(n);
  var sum:int;
  for p in properDivisorsN do sum += p;
  return sum == n;
}

private proc max(array:[]):real
{ // Return the maximum value in ``array``.
  var max:real;
  assert(array.size > 0, "max: array must have positive-size domain");
  const arrayLocalSubdomain = array.localSubdomain();
  const firstIndex = arrayLocalSubdomain.first;
  const initial = array[firstIndex];
  max = initial;
  for element in array do if element > max then max = element;
  return max;
}

private proc writeArrayStatistics(array:[]real)
{ // Write ``array``'s min, max, mean, sum, standard deviation, and histogram.
  var max,mean,min,squaredDeviationSum,stdDev,sum:real;
  const arraySize:int = array.size;
  if arraySize == 0
  then writeln("writeArrayStatistics: array has zero-size domain");
  else
  {
    // Initial.
    const arrayLocalSubdomain = array.localSubdomain();
    const firstIndex = arrayLocalSubdomain.first;
    const initial = array[firstIndex];
    min = initial;
    max = initial;

    // Mean.
    for element in array
    {
      if element < min then min = element;
      if element > max then max = element;
      sum += element;
    }
    mean = (sum/arraySize:real);

    // Standard deviation.
    for element in array do squaredDeviationSum += ((element - mean) ** 2.0);
    stdDev = ((squaredDeviationSum/arraySize:real) ** (0.5));

    writeln("writeArrayStatistics: min = ", min,
            ", max = ", max,
            ", mean = ", mean,
            ", stdDev = ", stdDev,
            ", sum = ", sum);

    // Histogram. Bin by stdDev.
    var bins:[0..7]int;
    const minusStdDev:real = (mean - stdDev);
    const minusTwoStdDev:real = (mean - 2.0*stdDev);
    const minusThreeStdDev:real = (mean - 3.0*stdDev);
    const plusStdDev:real = (mean + stdDev);
    const plusTwoStdDev:real = (mean + 2.0*stdDev);
    const plusThreeStdDev:real = (mean + 3.0*stdDev);
    for element in array
    {
      if element < mean
      then if element < minusTwoStdDev
           then if element < minusThreeStdDev
                then bins[0] += 1;
                else bins[1] += 1;
           else if element < minusStdDev
                then bins[2] += 1;
                else bins[3] += 1;
      else if element < plusTwoStdDev
           then if element < plusStdDev
                then bins[4] += 1;
                else bins[5] += 1;
           else if element < plusThreeStdDev
                then bins[6] += 1;
                else bins[7] += 1;
    }
    writeln("writeArrayStatistics: Histogram:");
    var current:real = minusThreeStdDev;
    for bin in bins
    {
      writeln((current - stdDev), " to ", current, ": ", bin);
      current += stdDev;
    }
  }
}

// EOF
