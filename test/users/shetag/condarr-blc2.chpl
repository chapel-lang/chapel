config const natom = 10;

proc bas_init(i,j) {
  return 5*(i/2) + 10*((i-1)/2) + if (j==1) then 1 else 10/(i%2+1);
}

const bas_info : [1..natom, 1..2] int = [(i,j) in {1..natom, 1..2}] bas_init(i,j);

writeln("bas_info is: ", bas_info);
