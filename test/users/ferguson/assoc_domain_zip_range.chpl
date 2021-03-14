var ids:domain(int);

ids += 1;
/*ids += 2;
ids += 3;
ids += 5;
ids += 7;
*/
writeln(ids);

forall (id, i) in zip(ids, 1..) {
  writeln( "id ", id, " i ", i);
}

