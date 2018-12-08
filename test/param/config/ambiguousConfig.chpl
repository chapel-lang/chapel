module M {
  config const CV = 1;
  config param PV = 2;
}
config const CV = 3;
config param PV = 4;
writeln(M.CV, M.PV, CV, PV);
