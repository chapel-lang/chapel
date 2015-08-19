// Example 2 of use of VisualDebug module and chplvis tool.

use BlockDist;
use VisualDebug;

config var ncells = 10;

proc main() {

   // Create a couple of domains and a block mapped data array.
   const Domain = { 1 .. ncells };
   const mapDomain = Domain dmapped Block(Domain);

   var  data : [mapDomain] int = 1;

   // Start VisualDebug here
   startVdebug ("E2");

   // First computation step ... a simple forall
   forall i in Domain do data[i] += here.id + 1;
   
   // Write the result, we want to see the results of the above
   // so we tag before we continue.
   tagVdebug("writeln 1");
   writeln("data= ", data);

   // Second computation step ... using the distributed domain
   tagVdebug("step 2");
   forall i in mapDomain do data[i] += here.id+1;

   // Don't capture for the writeln
   pauseVdebug();
   writeln("data2= ", data);

   // Reduction step
   tagVdebug("reduce");
   var i = + reduce data;

   // done with visual debug
   stopVdebug();

   writeln ("sum is " + i + ".");
}
   
