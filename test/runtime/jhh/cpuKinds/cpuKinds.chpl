// This is a file used to create an otherwise empty Chapel library. Its main
// purpose is to provide access into the runtime. However, since it
// initializes the runtime in a non-standard way, it also needs to manually
// prepare program information so that the runtime can function.
export proc setupProgramInfoHereManually() {
  use ChapelProgramRegistration;
  chpl_programInfoHere.registerAsRoot();
}
