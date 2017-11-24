module SSCA2_main {
  // if 'use' were here instead, everything would work:
  //use SSCA2_kernels;
  proc main () {
    use SSCA2_kernels;
    writeln("SSCA2_main.main()");
  }
}

module SSCA2_kernels { 
  use PrivateDist;
  var pa: [PrivateSpace] int;
  writeln("SSCA2_kernels init");
}
