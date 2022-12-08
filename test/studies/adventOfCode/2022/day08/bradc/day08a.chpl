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



/*
scenicScore(Forest, (1,2), 5);
writeln("----");
scenicScore(Forest, (3,2), 5);
writeln("----");
scenicScore(Forest, (2,3), 3);
writeln("----");
*/
/*
for (r,c) in ForestSpace {
  writeln("------", (r,c));
  writeln(scenicScore(Forest, (r,c), Forest(r,c)));
}
*/
writeln(max reduce (scenicScore(Forest, ForestSpace, Forest)));


proc scenicScore(Trees, (myR, myC), myHeight: int) {
  writeln("Considering: ", ((myR, myC), myHeight));
  var score = 1;

  {
    var count = 0;
    for r in 0..<myR by -1 {
      count += 1;
      const treeHeight = Trees[r, myC];
      writeln("Considering ", (r, myC), " with height ", treeHeight, " vs. ", myHeight);
      if treeHeight >= myHeight then
        break;
    }
    score *= count;
    writeln((count, score));
  }

{
  var  count = 0;
  for r in myR+1..<numrows {
      count += 1;
    const treeHeight = Trees[r, myC];
    writeln("Considering ", (r, myC), " with height ", treeHeight, " vs. ", myHeight);
    if treeHeight >= myHeight then
      break;
  }
  score *= count;
    writeln((count, score));
}

{
  var  count = 0;
  for c in 0..<myC by -1 {
      count += 1;
    const treeHeight = Trees[myR, c];
    writeln("Considering ", (myR, c), " with height ", treeHeight, " vs. ", myHeight);
    if treeHeight >= myHeight then
      break;
   }
  score *= count;
    writeln((count, score));
}

{
  var  count = 0;
  for c in myC+1..<numcols {
      count += 1;
    const treeHeight = Trees[myR, c];
    writeln("Considering ", (myR, c), " with height ", treeHeight, " vs. ", myHeight);
    if treeHeight >= myHeight then
      break;
  }
  score *= count;
    writeln((count, score));
}

  writeln("Score = ", score);
  return score;
}

iter readForest() {
  var line: string;
  while readLine(line, stripNewline=true) do
    yield line;
}
