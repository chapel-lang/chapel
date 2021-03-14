use driver_arrays;

for e in A1D do e = next();
for e in A2D do e = next();

for loc in Locales do on loc do
  writeln(here.id, ": ", A1D.localSlice[A1D.localSubdomain()]);

for loc in Locales do on loc do
  writeln(here.id, ": ", A2D.localSlice[A2D.localSubdomain()]);
