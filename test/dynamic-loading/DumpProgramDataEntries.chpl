proc main() {
  use ChapelProgramRegistration;
  for loc in Locales do on loc {
    chpl_programInfoHere.dump(showAddresses=false);
  }
}
