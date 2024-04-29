/* This is a utility module of convenience features that arguably
   should be reviewed and added to LayoutCS proper. */
   
module LayoutCSUtil {
  use LayoutCS;

  proc CSDom.rows() {
    return this.rowRange;
  }

  proc CSDom.cols() {
    return this.colRange;
  }

  iter CSDom.colUidsInRow(r) {
    if this.compressRows == false then
      compilerError("Can't (efficiently) iterate over columns using a CSC layout");
    for uid in startIdx[r]..<startIdx[r+1] do
      yield uid;
  }

  iter CSDom.rowUidsInCol(c) {
    if this.compressRows == true then
      compilerError("Can't (efficiently) iterate over columns using a CSR layout");
    for uid in startIdx[c]..<startIdx[c+1] do
      yield uid;
  }

}