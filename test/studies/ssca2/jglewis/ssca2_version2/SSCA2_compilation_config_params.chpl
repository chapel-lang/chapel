module SSCA2_compilation_config_params

// +==========================================================================+
// |  compilation time configuration parameters                               |
// |                                                                          |
// |  These control code generation like "IFDEF" in languages with            |
// |  preprocessors.  In Chapel the conditional expressions are evaluated at  |
// |  at compile time and dead code is eliminated.                            |
// |                                                                          |
// |  The most important turns "filtering" on and off (edges are "filtered"   |
// |  Y the value of the edge weight determines whether the edge is live or   |
// |  not).                                                                   |
// |                                                                          |
// |  "VALIDATE_BC" causes the code to do additional work to demonstrate      |
// |  correct functioning.                                                    |
// |                                                                          |
// |  "REPRODUCIBLE_PROBLEMS" causes all random number streams to start from  |
// | fixed (deterministic) seeds.                                             |
// |                                                                          |
// |  "PRINT_TIMING_STATISTICS" enables or disables timing measurement and    |
// |  documentation.  This mode is normally used with the preceding mode to   |
// |                                                                          |
// |  create deterministic output for compiler regression testing.            |
// |                                                                          |
// |  "DISTRIBUTION_TYPE" selects among different distribution possibilities. |
// |  At present, only "BLOCK" distributions are supported in the compiler.   |
// |  The only alternative to "BLOCK" is "none" (undistributed).              |
// |  Only the vertex domain is distributed in this current code.             |
// +==========================================================================+

{
  config param FILTERING = false;

  config param VALIDATE_BC = false;

  config param DEBUG_GRAPH_GENERATOR = false;

  config param DEBUG_WEIGHT_GENERATOR = false;

  config param DEBUG_KERNEL2 = false;

  config param DEBUG_KERNEL3 = false;

  config param DEBUG_KERNEL4 = false;

  config param REPRODUCIBLE_PROBLEMS = true;

  config param PRINT_TIMING_STATISTICS = true;

  config param DISTRIBUTION_TYPE = "BLOCK";

}


