/* Basic overloading where the supertype function is defined after the subtype
   and the functions defined after they are used. (2)
*/
print f(0);
print f(0.0);
f (a : int) : 3;
f (a) : 1;
f (a : float) : 2;
