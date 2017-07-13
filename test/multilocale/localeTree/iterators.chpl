config const debug = false;

forall loc in localeTree() {
  writeln('Gather: ', here.id, ' -> ', loc.id);
}

forall loc in localeTreeDown() {
  writeln('Broadcast: ', here.id, ' -> ', loc.id);
}
