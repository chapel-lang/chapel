proc write_maxTaskPar() {
  writeln(here.maxTaskPar);
  const numChildren = here.getChildCount();
  if localeModelHasSublocales && numChildren > 0 {
    for child in 0..#numChildren {
      on here.getChild(child) do write_maxTaskPar();
    }
  }
}

write_maxTaskPar();
