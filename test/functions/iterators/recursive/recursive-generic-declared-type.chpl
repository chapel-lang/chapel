
// a generic type
record RR {
  type tt;
}

// non-recursive iterator with a generic declared return/yield type
iter nonRecIter() : RR {
  yield new RR(int);
}

for INR in nonRecIter() {
  compilerWarning(INR.type:string);
}

// recursive iterator with a generic declared return/yield type
iter recIter() : RR {
  for jjj in recIter() do
    yield jjj;

  yield new RR(real); // ideally, this will define recIter's yield type
}

for IRE in recIter() {
  compilerWarning(IRE.type:string);
}

compilerError("done");
