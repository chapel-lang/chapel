/* Basic overloading where the supertype function is defined after the subtype
   That ordering is unimportant.
*/
f (a : float ) : 2;
f (a) : 1;
print f(0);
print f(0.0);
