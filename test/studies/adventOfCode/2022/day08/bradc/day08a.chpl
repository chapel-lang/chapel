use IO;

const ForestAsText = readForest();
const numrows = ForestAsText.size,
      numcols = ForestAsText[0].size;

const ForestSpace = {0..<numrows, 0..<numcols};
var Forest: [ForestSpace] int;

for (r,c) in ForestSpace do
  Forest[r,c] = ForestAsText[r][c]:int;

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



writeln(max reduce (scenicScore(Forest, ForestSpace, Forest)));


proc scenicScore(Trees, (myR, myC), myHeight: int) {
  var score = 1;

  {
    var count = 0;
    for r in 0..<myR by -1 {
      count += 1;
      const treeHeight = Trees[r, myC];
      if treeHeight >= myHeight then
        break;
    }
    score *= count;
  }

{
  var  count = 0;
  for r in myR+1..<numrows {
      count += 1;
    const treeHeight = Trees[r, myC];

    if treeHeight >= myHeight then
      break;
  }
  score *= count;
}

{
  var  count = 0;
  for c in 0..<myC by -1 {
      count += 1;
    const treeHeight = Trees[myR, c];
    if treeHeight >= myHeight then
      break;
   }
  score *= count;
}

{
  var  count = 0;
  for c in myC+1..<numcols {
      count += 1;
    const treeHeight = Trees[myR, c];
    if treeHeight >= myHeight then
      break;
  }
  score *= count;
}

  return score;
}

iter readForest() {
  var line: string;
  while readLine(line, stripNewline=true) do
    yield line;
}
