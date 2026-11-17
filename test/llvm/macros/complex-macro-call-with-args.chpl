use CTypes;

extern proc ComplexFnLikeArgs(arg);

proc main() {
  ComplexFnLikeArgs(1:c_int);
}
