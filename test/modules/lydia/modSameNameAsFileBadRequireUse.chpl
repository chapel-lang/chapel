use modSameNameAsFileGood;
require "someHeader2.h";

module modSameNameAsFileBadRequireUse {
  extern var locVar2: int(32);

  var inner = locVar + locVar2;
}