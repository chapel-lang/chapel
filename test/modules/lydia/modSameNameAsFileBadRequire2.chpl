require "someHeader.h";

module modSameNameAsFileBadRequire2 {
  extern var locVar: int(32);

  var inner = locVar + 2;
}
