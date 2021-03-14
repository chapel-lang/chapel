var s$: sync int = 0;

writeln("coforall: ");
coforall loc in Locales {
  on loc {
    while (s$.readFF() != here.id) { }
    s$.readFE();
    writeln("chpl_nodeID: ", chpl_nodeID, ", here.id = ", here.id);
    s$.writeEF(here.id+1);
  }
}
writeln();
s$.writeXF(0);

writeln("forall: ");
forall loc in Locales {
  on loc {
    while (s$.readFF() != here.id) { }
    s$.readFE();
    writeln("chpl_nodeID: ", chpl_nodeID, ", here.id = ", here.id);
    s$.writeEF(here.id+1);
  }
}
