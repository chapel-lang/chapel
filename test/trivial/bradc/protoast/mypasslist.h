PassInfo passlist[] = {
  FIRST,

  RUN(FilesToAST, ""),
  RUN(ResolveDomainRanks, ""),
  RUN(PrintProgram, ""),

  LAST
};
