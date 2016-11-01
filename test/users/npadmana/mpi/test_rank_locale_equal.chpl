use MPI;

proc main() {
  writef("Testing ranks and locale ids...\n");
  coforall loc in Locales do on loc {
    var rank = commRank(comm=CHPL_COMM_WORLD);
    if rank != here.id then writef("Locale %i --> Rank %i\n",here.id, rank);
  }
}
