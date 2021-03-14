param c1_2:complex = 1.0+2.0i;
param c2_1:complex = 2.0+1.0i;
param c_2:complex  = 2.0i;
param c1:complex   = 1.0;
param cn1:complex  = -1.0;
param cn_2:complex = -2.0i;

param cn1_2:complex = -1.0-2.0i;

proc checkYes(param arg:bool) {
  if arg == false then
    compilerWarning("assert failed");
}

checkYes(c1_2 != c2_1);
checkYes(c1_2 != c_2);
checkYes(c1_2 != c1);
checkYes(c1_2 != cn1);
checkYes(c1_2 != cn_2);
checkYes(c2_1 != c_2);
checkYes(c2_1 != c1);
checkYes(c2_1 != cn1);
checkYes(c2_1 != cn_2);
checkYes(c_2 != c1);
checkYes(c_2 != cn1);
checkYes(c_2 != cn_2);
checkYes(c1 != cn1);
checkYes(c1 != cn_2);

checkYes(c1 + c_2 == c1_2);
checkYes((+cn1)+cn_2 == cn1_2);
checkYes((-c1_2) == cn1_2);
checkYes((-c1) - c_2 == cn1_2);
