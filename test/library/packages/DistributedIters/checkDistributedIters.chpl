/*
  Tests for verifying the DistributedIters module iterator correctness.

  Test cases:
  - Range inputs
  - Domain inputs
  - Specific work locales
  - Coordinated mode
*/
use DistributedIters;

/*
  Test mode (iterator):

  - ``dynamic``
    The distributed dynamic load-balancing iterator.

  - ``guided``
    The distributed guided load-balancing iterator.
*/
enum iterator
{
  dynamic,
  guided
};

/*
  Used to select the test mode (which iterator to use). Defaults to ``guided``.
  See :data:`iterator` for more information.
*/
config const mode:iterator = iterator.guided;

/*
  Control variables. These determine the test variables (defined later) and
  help us check correctness.
*/
config const n:int = 1000;

const controlRange:range = 1..n;
const controlRangeStrided = (controlRange by 2);

const controlDomain:domain(1) = {controlRange};
const controlDomainStrided = (controlDomain by 2);

/*
  Default tests.
*/
writeln("Default tests, serial:");
testRangesAndDomainsSerial();
writeln();

writeln("Default tests, zippered:");
testRangesAndDomainsZippered();
writeln();

writeln("Default tests, coordinated mode:");
testRangesAndDomainsZippered(coordinated=true);
writeln();

/*
  Specific work locales.
*/
if numLocales > 1 then
{
  const evenLocales = [Locale in Locales] if (Locale.id % 2 == 0) then Locale;
  const oddLocales = [Locale in Locales] if (Locale.id % 2 != 0) then Locale;

  writeln("Even locales only:");
  testRangesAndDomainsZippered(workerLocales=evenLocales);
  writeln();

  writeln("Odd locales only:");
  testRangesAndDomainsZippered(workerLocales=oddLocales);
  writeln();

  // Coordinated mode.
  writeln("Even locales only, coordinated mode:");
  testRangesAndDomainsZippered(workerLocales=evenLocales, coordinated=true);
  writeln();

  writeln("Odd locales only, coordinated mode:");
  testRangesAndDomainsZippered(workerLocales=oddLocales, coordinated=true);
  writeln();
}

/*
  Main testing functions.
*/
proc testIteratorSerial(c)
{
  var array:[c]int;
  select mode
  {
    when iterator.dynamic do for i in distributedDynamic(c)
                             do array[i] = (array[i] + 1);
    when iterator.guided do for i in distributedGuided(c)
                            do array[i] = (array[i] + 1);
  }
  checkCorrectness(array, c);
}

proc testIteratorZippered(array,
                          target,
                          base,
                          workerLocales,
                          coordinated)
{
  select mode
  {
    when iterator.dynamic
    {
      forall (i,j) in zip(distributedDynamic(target,
                                             coordinated=coordinated,
                                             workerLocales=workerLocales),
                          base # target.size)
      do array[i,j] = (array[i,j] + 1);
    }
    when iterator.guided
    {
      forall (i,j) in zip(distributedGuided(target,
                                            coordinated=coordinated,
                                            workerLocales=workerLocales),
                          base # target.size)
      do array[i,j] = (array[i,j] + 1);
    }
  }
  checkCorrectnessZippered(array, target, base);
}

proc testRangesAndDomainsSerial()
{
  /*
    Range inputs.
  */
  writeln("Testing a range, non-strided (serial)...");
  testIteratorSerial(controlRange);

  writeln("Testing a range, strided (serial)...");
  testIteratorSerial(controlRangeStrided);

  /*
    Domain inputs.
  */
  writeln("Testing a domain, non-strided (serial)...");
  testIteratorSerial(controlDomain);

  writeln("Testing a domain, strided (serial)...");
  testIteratorSerial(controlDomainStrided);
}

proc testRangesAndDomainsZippered(workerLocales=Locales, coordinated=false)
{
  /*
    Range inputs.
  */
  writeln("Testing a range, non-strided (zippered)...");
  var arrayRNS:[controlRange, controlRange]int;
  testIteratorZippered(array=arrayRNS,
                       target=controlRange,
                       base=controlRange,
                       workerLocales=workerLocales,
                       coordinated=coordinated);

  writeln("Testing a range, strided (zippered)...");
  var arrayRS:[controlRangeStrided, controlRange]int;
  testIteratorZippered(array=arrayRS,
                       target=controlRangeStrided,
                       base=controlRange,
                       workerLocales=workerLocales,
                       coordinated=coordinated);

  /*
    Domain inputs.
  */
  writeln("Testing a domain, non-strided (zippered)...");
  var arrayDNS:[controlRange, controlRange]int;
  testIteratorZippered(array=arrayDNS,
                       target=controlDomain,
                       base=controlDomain,
                       workerLocales=workerLocales,
                       coordinated=coordinated);

  writeln("Testing a domain, strided (zippered)...");
  var arrayDS:[controlRangeStrided, controlRange]int;
  testIteratorZippered(array=arrayDS,
                       target=controlDomainStrided,
                       base=controlDomain,
                       workerLocales=workerLocales,
                       coordinated=coordinated);

}

/*
  Helper functions.
*/
proc checkCorrectness(array:[]int, c)
{
  var check:bool = true;
  for i in c do
  {
    if (array[i] != 1) then
    {
      check = false;
      writeln();
      writeln("Error in iteration ", i);
      writeln();
    }
  }
  writeln("Result: ",
          if (check == true)
          then "pass"
          else "fail");
}

proc checkCorrectnessZippered(array:[]int, cLeader, cFollower)
{
  var check:bool = true;
  for (i,j) in zip(cLeader, cFollower#cLeader.size) do
  {
    if (array(i,j) != 1)
    then
    {
      check = false;
      writeln();
      writeln("Error in iteration (", i, ",", j, ")");
      writeln();
    }
  }
  writeln("Result: ",
          if (check == true)
          then "pass"
          else "fail");
}

// EOF
