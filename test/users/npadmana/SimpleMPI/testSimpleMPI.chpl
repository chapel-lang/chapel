use SimpleMPI;

coforall loc in Locales {
  on loc {
    var myrank = SimpleMPI.rank();
    var mysize = SimpleMPI.size();
    writeln("Locale ",here.id," is MPI rank ",myrank," of ",mysize);
  }
}

SimpleMPI.finalize();

