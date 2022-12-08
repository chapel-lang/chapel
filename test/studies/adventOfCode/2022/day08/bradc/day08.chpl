use IO;

const ForestAsText = readForest();
const numrows = ForestAsText.size,
      numcols = ForestAsText[0].size;

const ForestSpace = {0..<numrows, 0..<numcols};
var Forest: [ForestSpace] int;

for (r,c) in ForestSpace do
  Forest[r,c] = ForestAsText[r][c]:int;

writeln(Forest);

writeln(+ reduce visible(Forest, ForestSpace, Forest));

proc visible(Trees, (myR,myC), myHeight: int) {
  var visible = true;  // let's be optimistic

  if myR == 0 || myR == numrows-1 || myC == 0 || myC == numcols-1 then
    return true;

  if && reduce (Trees[..<myR, myC] < myHeight) then return true;
  if && reduce (Trees[myR+1.., myC] < myHeight) then return true;
  if && reduce (Trees[myR, ..<myC] < myHeight) then return true;
  if && reduce (Trees[myR, myC+1..] < myHeight) then return true;


  return false;
}

iter readForest() {
  var line: string;
  while readLine(line, stripNewline=true) do
    yield line;
}
