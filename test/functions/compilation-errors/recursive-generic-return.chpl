
// a generic type
record RR {
  type tt;
}

// non-recursive proc with a generic declared return type
proc nonRecProc() : RR {
  return new RR(int);
}

compilerWarning(nonRecProc().type:string);

// recursive proc with a generic declared return type
proc recProc() : RR {
  return recProc();
  return new RR(real); // ideally, this will define recProc's return type
}

compilerWarning(recProc().type:string);

compilerError("done");
