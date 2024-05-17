// This module defines convenience features that arguably should be
// reviewed and added to LayoutCS proper
//
module LayoutCSUtil {
  use LayoutCS;

  proc CSDom.rows() {
    return this.rowRange;
  }

  proc CSDom.cols() {
    return this.colRange;
  }

  proc CSArr.rows() {
    return this.dom.rows();
  }

  proc CSArr.cols() {
    return this.dom.cols();
  }

  @chpldoc.nodoc
  iter CSDom.uidsInRowCol(rc) {
    for uid in startIdx[rc]..<startIdx[rc+1] do
      yield uid;
  }
  
  iter CSDom.colUidsInRow(r) {
    if this.compressRows == false then
      compilerError("Can't (efficiently) iterate over rows using a CSC layout");
    for uid in uidsInRowCol(r) do
      yield uid;
  }

  iter CSDom.rowUidsInCol(c) {
    if this.compressRows == true then
      compilerError("Can't (efficiently) iterate over columns using a CSR layout");
    for uid in uidsInRowCol(c) do
      yield uid;
  }

  iter CSArr.indsAndVals(rc) {
    ref dom = this.dom;
    for uid in dom.uidsInRowCol(rc) do
      yield (dom.idx[uid], this.data[uid]);
  }
  
  iter CSArr.colsAndVals(r) {
    if this.dom.compressRows == false then
      compilerError("Can't (efficiently) iterate over rows using a CSC layout");
    for colVal in indsAndVals(r) do
      yield colVal;
  }

  iter CSArr.rowsAndVals(c) {
    if this.dom.compressRows == true then
      compilerError("Can't (efficiently) iterate over columns using a CSR layout");
    for rowVal in indsAndVals(c) do
      yield rowVal;
  }
}