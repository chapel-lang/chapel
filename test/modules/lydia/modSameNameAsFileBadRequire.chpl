require "modSameNameAsFileGood.chpl";

module modSameNameAsFileBadRequire {
  use modSameNameAsFileGood;

  var inner = locVar + 2;
}
