use CTypes;

proc main()
{
  var t1: [0..9] [0..9] c_int;
  var t2: c_array(c_array(c_int, 9), 9);
  var t3: [0..9] c_array(c_int, 9);

  for i in 0..9 {
    for j in 0..9 {
      t1[i][j] = i:c_int;
      t2[i][j] = i:c_int;
      t3[i][j] = i:c_int;
    }
  }

  on here.gpus[0] {
    var t1_c = t1; writeln("t1_c = ", t1_c); // OK
    var t2_c = t2; writeln("t2_c = ", t2_c); // OK
    var t3_c = t3; writeln("t3_c = ", t3_c); // ERROR
  }

  return 0;
}

