// Until May 13, 2017 it was possible for top-level modules to intercept
// lexical lookups within internal modules in certain situations.

// This occurred because
//   a) chpl__Program 'used' ChapelStandard
//
//   b) Some internal modules implicitly found needed names in other
//      modules by reaching chpl__Program and then following the use.

// For example there were lookup paths for the type 'list' that would
// be blocked if a user innocently created a file named 'list.chpl'.

// This test demonstrates that one version of this bug has been
// resolved.
//
// As of May 13, 2017 there are still problems with the scoping of
// certain primitive types and primitive variables.


writeln("Module 'list' can be defined");
