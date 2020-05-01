var ids:domain(int);

ids += 1;
/*ids += 2;
ids += 3;
ids += 5;
ids += 7;
*/
writeln(ids);

var sz = ids.size;

//forall (id, i) in zip(ids, 1..) {
forall (i, id) in zip(1..sz, ids) {
  writeln( "id ", id, " i ", i);
}

