// chplvis: Tags

// Example 2 of use of the VisualDebug module and chplvis tool.
// Read the file doc/rst/tools/chplvis/chplvis.rst for full
// documentation on chplvis.

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
   // Even though the data is distributed, the computation is
   // on Locale 0.  chplvis shows no computation on locales
   // other than 0.   Domain is not dmapped.
   forall i in Domain do data[i] += here.id + 1;
   
   // Write the result, we want to see the results of the above
   // so we tag before we continue.  Computation only on locale 0.
   tagVdebug("writeln 1");
   writeln("data= ", data);

   // Second computation step ... using the distributed domain
   // in the forall and thus computation is distributed.  Again,
   // chplvis shows computation on all locales.
   tagVdebug("step 2");
   forall i in mapDomain do data[i] += here.id+1;

   // Don't capture the writeln
   pauseVdebug();
   writeln("data2= ", data);

   // Reduction step, computation on all locales.
   tagVdebug("reduce");
   var i = + reduce data;

   // done with visual debug
   stopVdebug();

   writeln ("sum is ", i, ".");
}
   
