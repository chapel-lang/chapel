f x : {
  a : x;
  if (a) then
    a = 2 + a
  else
    a = 3 + a;
  print(a);
};
f 1;
