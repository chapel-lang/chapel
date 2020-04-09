module M {
  config const CV = 1;
  config type PV = int;
}
config const CV = 3;
config type PV = real;
writeln(M.CV, M.PV:string, CV, PV:string);
