// Distributed-memory data-parallel hello, world

/* This program uses Chapel's distributed data parallel features to
   create a parallel hello world program that utilizes multiple cores
   on multiple `locales` (compute nodes).  The number of locales on
   which to run is specified on the executable's command line using
   the `-nl` or `--numLocales` flag (e.g., ``./hello -nl 64``).  This
   test is very similar to :ref:`hello-datapar.chpl
   <primers-hello3-datapar>`, so we won't repeat the explanation of
   concepts introduced there.
 */

//
// To start, we'll 'use' the standard Cyclic distribution module
// (`CyclicDist`) to access a `domain map` that supports the
// round-robin distribution of indices to locales.
//
use CyclicDist;

//
// Declare the number of messages to print:
//
config const numMessages = 100;


//
// Here, we declare a `domain` (an index set) named `MessageSpace`
// that represents the indices ``1..numMessages`` and is `domain
// mapped` (``dmapped``) using the standard `Cyclic` distribution.
// This causes its indices to be distributed across the locales in a
// round-robin fashion where `startIdx` is mapped to locale #0.
//
const MessageSpace = {1..numMessages} dmapped Cyclic(startIdx=1);

//
// By using the distributed domain `MessageSpace` to drive the
// following forall-loop, each iteration will be executed by the
// locale which owns that index, resulting in the distribution of the
// work across all the program's compute nodes.  In addition, each
// locale will also use its available processing units (cores) to
// execute its local iterations in parallel.
//
forall msg in MessageSpace do
  writeln("Hello, world! (from iteration ", msg, " of ", numMessages, 
          " owned by locale ", here.id, " of ", numLocales, ")");


//
// Note that by changing the domain map of `MessageSpace` above
// (either by changing the arguments to `Cyclic` or switching to
// another domain map altogether), we can alter the distribution and
// scheduling of the forall-loop's iterations without changing the
// loop itself.
//
// For further examples of using distributions, refer to
// :ref:`examples/primers/distributions.chpl <primers-distributions>`
// in the Chapel :ref:`primers`.
//
