// This brings in the SSCA2_main module.
// Gotta be in a module declaration so that the following two modules
// are top-level (so SSCA2_main finds them).
//
module rmatalt {
  use SSCA2_main;
}

// We are not using any of the following for RMAT graphs.

module analyze_torus_graphs {
  proc generate_and_analyze_1D_torus {}
  proc generate_and_analyze_2D_torus {}
  proc generate_and_analyze_3D_torus {}
  proc generate_and_analyze_4D_torus {}
}

module SSCA2_driver {
  proc execute_SSCA2(G) { writeln("execute_SSCA2() skipped"); }
}
