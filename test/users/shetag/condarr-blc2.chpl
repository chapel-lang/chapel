config const natom = 10;

def bas_init(i,j) {
  return 5*(i/2) + 10*((i-1)/2) + if (j==1) then 1 else 10/(i%2+1);
}

const bas_info : [(i,j) in [1..natom, 1..2]] int = bas_init(i,j);

writeln("bas_info is: ", bas_info);
