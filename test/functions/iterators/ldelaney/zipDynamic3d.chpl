use DynamicIters;
proc main(){
  var r1 = 1..5;
  var r2 = -7..5;
  var r3 = 3..4;

  var dom = {r1, r2, r3};
  var bigArr : [r1][r2][r3] int;

  //initialize bigArr
  forall (x, y, z) in dom do
    bigArr[x][y][z] = 0;

  //test that dynamic(domain, chunksize, numTasks, parDim) can lead
  var count : atomic int;
  count.write(0);

  forall(xyz1, xyz2) in zip(dynamic(dom, 1, 0, 1), dom)
  {
    if (xyz1 != xyz2) then writeln("ZIPPED WRONG");
    else{
      count.add(1);
      bigArr[xyz1(0)][xyz1(1)][xyz1(2)] += 1;
    }

  }
  writeln("Dynamic multidimensional iter led ", count, " correct zippered iterations slicing across dimension 1.");
  var prevCount = count.read();
  count.write(0);

  //test that the dynamic iter can lead when parallelizing across dim2
  forall(xyz1, xyz2) in zip(dynamic(dom, 1, 0, 2), dom)
  {
    if (xyz1 != xyz2) then writeln("ZIPPED WRONG");
    else{
      count.add(1);
      bigArr[xyz1(0)][xyz1(1)][xyz1(2)] += 2;
    }
  }
  prevCount += count.read();
  writeln("Dynamic multidimensional iter led ", count.read(), " correct zippered iterations slicing across dimension 2.");
  count.write(0);

  //test that the dynamic iter can lead when the chunk size is > 1
  forall(xyz1, xyz2) in zip(dynamic(dom, 4, 0, 1), dom)
  {
    if (xyz1 != xyz2) then writeln("ZIPPED WRONG");
    else{
      count.add(1);
      bigArr[xyz1(0)][xyz1(1)][xyz1(2)] += 4;
    }
  }
  prevCount += count.read();
  writeln("Dynamic multidimensional iter led ", count.read(), " correct zippered iterations with chunk size 4.");
  count.write(0);

  //test that dynamic(domain, chunksize, numTasks, parDim) can follow
  forall(xyz1, xyz2) in zip(dom, dynamic(dom, 1, 0, 1))
  {
    if (xyz1 != xyz2) then writeln("ZIPPED WRONG");
    else{
      count.add(1);
      bigArr[xyz1(0)][xyz1(1)][xyz1(2)] += 8;
    }
  }
  writeln("Dynamic multidimensional iter followed ", count.read(), " correct zippered iterations.");

  //confirm that each element has been incremented by 1+2+4+8
  for (x, y, z) in dom do
    assert(bigArr[x][y][z] == 15, "bigArr didnt increment properly");
}