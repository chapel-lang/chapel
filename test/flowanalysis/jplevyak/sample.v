aa : 0;
bb : { a: 1; b: 2; aa; 3; };
f a b : {
  if a then {
    if b.a then
      a = b.b
    else
      a = 4;
  } else {
   a = 5;
  };
  aa = a;
};
f aa bb;
