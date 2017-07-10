/*
  Test to check the correctness of the distributed() iterator from the
  DistributedIters module.

  Example usage:
  chpl -I. checkDistributedIters.chpl && time ./a.out --n=10000 -nl 4
*/
use DistributedIters,
    Time;

var timer:Timer;
/*
  Control variables. These determine the test variables (defined later) and
  provide control for checking correctness.
*/
config const n:int=1000;
var controlRange:range=1..n;

// Tests.
writeln("Testing a range (distributed guided iterator)...");
var testGuidedDistributedRangeArray:[controlRange] int=0;

timer.start();
forall i in guidedDistributed(controlRange) do
  testGuidedDistributedRangeArray[i] = testGuidedDistributedRangeArray[i]+1;
timer.stop();

writeln("Time: ", timer.elapsed());
timer.clear();

checkCorrectness(testGuidedDistributedRangeArray,controlRange);

// Helper functions.
proc checkCorrectness(Arr:[]int,c)
{
  var check=true;
  for i in c do
  {
    if Arr[i] != 1 then
    {
      check=false;
      writeln(" ");
      writeln("Error in iteration ", i);
      writeln(" ");
    }
  }
  writeln("Result: ",
          if check==true
          then "pass"
          else "fail");
}
