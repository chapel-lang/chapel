module SSCA2_main {
  use analyze_torus_graphs;
  proc main() { writeln("SSCA2_main.main()"); }
}

module analyze_torus_graphs {
  use SSCA2_driver;
  writeln("analyze_torus_graphs init");
}

module SSCA2_driver {
  proc execute_SSCA2(G) {
    use SSCA2_kernels;
  }
}

module SSCA2_kernels { 
  use PrivateDist;
  var pa: [PrivateSpace] int;
  writeln("SSCA2_kernels init");
}
