/*  This test uses Chapel's distributed data parallel features to
 *  create a parallel hello world program that utilizes multiple cores
 *  on multiple locales.  This test is very similar to
 *  hello-datapar.chpl, so we won't repeat the explanation of concepts
 *  covered there.
 */

//
// We need to use the standard CyclicDist[ribution] module in order
// to distribute the parallel iterations cyclically.
//
use CyclicDist;

//
// The number of messages to print
//
config const numMessages = 100;


//
// Here, we declare a domain (index set) named MessageSpace that
// represents the indices 1..numMessages and is 'domain mapped'
// (dmapped) using the standard Cyclic distribution.  This causes
// its indices to be distributed across the locales in a cyclic
// fashion starting with 'startIdx'.
//
const MessageSpace = {1..numMessages} dmapped Cyclic(startIdx=1);

//
// By using the distributed domain MessageSpace to drive this forall
// loop each iteration will be executed by the locale to which the
// corresponding index was distributed via the domain map above.
// By changing the domain map of MessageSpace above (whether by
// changing Cyclic's arguments or switching to another domain map
// altogether), we can change the distribution and scheduling of the
// forall loop's iterations without changing the loop itself.
//
forall msg in MessageSpace do
  writeln("Hello, world! (from iteration ", msg, " of ", numMessages, 
          " owned by locale ", here.id, " of ", numLocales, ")");


//
// For further examples of using distributions, refer to
// examples/primers/distributions.chpl
//
