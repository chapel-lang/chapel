/* 
  Nested functions are accessible in outer scopes?
*/

g a : {
  f (a : int) : 4;
  0
};

print f(0);
print f(0.0);
f (a) : 1;
f (a : float) : 2;
