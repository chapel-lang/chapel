#ifndef _INTERPRETER_OPS_H_
#define _INTERPRETER_OPS_H_

class InterpreterOp;

#ifndef _EXTERN
#define _EXTERN extern
#define _INIT
#endif


#define INTERPRETER_OP(_fun)                                    \
  _EXTERN InterpreterOp* _fun ## _interpreter_op _INIT


INTERPRETER_OP( unimplemented);
INTERPRETER_OP( init);
INTERPRETER_OP( alloc);
INTERPRETER_OP( fopen);
INTERPRETER_OP( fclose);
INTERPRETER_OP( strerror);
INTERPRETER_OP( fprintf);
INTERPRETER_OP( fscanf);
INTERPRETER_OP( fflush);
INTERPRETER_OP( array_init);
INTERPRETER_OP( array_index);
INTERPRETER_OP( array_set);
INTERPRETER_OP( unary_minus);
INTERPRETER_OP( unary_plus);
INTERPRETER_OP( unary_not);
INTERPRETER_OP( unary_lnot);
INTERPRETER_OP( add);
INTERPRETER_OP( subtract);
INTERPRETER_OP( mult);
INTERPRETER_OP( div);
INTERPRETER_OP( mod);
INTERPRETER_OP( lsh);
INTERPRETER_OP( rsh);
INTERPRETER_OP( equal);
INTERPRETER_OP( notequal);
INTERPRETER_OP( elssorequal);
INTERPRETER_OP( greaterorequal);
INTERPRETER_OP( less);
INTERPRETER_OP( greater);
INTERPRETER_OP( and);
INTERPRETER_OP( or);
INTERPRETER_OP( xor);
INTERPRETER_OP( land);
INTERPRETER_OP( lor);
INTERPRETER_OP( pow);
INTERPRETER_OP( abs);
INTERPRETER_OP( acos);
INTERPRETER_OP( acosh);
INTERPRETER_OP( asin);
INTERPRETER_OP( asinh);
INTERPRETER_OP( atan);
INTERPRETER_OP( atan2);
INTERPRETER_OP( atanh);
INTERPRETER_OP( cbrt);
INTERPRETER_OP( ceil);
INTERPRETER_OP( cos);
INTERPRETER_OP( cosh);
INTERPRETER_OP( exp);
INTERPRETER_OP( exp2);
INTERPRETER_OP( erf);
INTERPRETER_OP( erfc);
INTERPRETER_OP( expm1);
INTERPRETER_OP( fabs);
INTERPRETER_OP( floor);
INTERPRETER_OP( lgamma);
INTERPRETER_OP( log);
INTERPRETER_OP( log2);
INTERPRETER_OP( log10);
INTERPRETER_OP( log1p);
INTERPRETER_OP( nearbyint);
INTERPRETER_OP( rint);
INTERPRETER_OP( round);
INTERPRETER_OP( sin);
INTERPRETER_OP( sinh);
INTERPRETER_OP( sqrt);
INTERPRETER_OP( tan);
INTERPRETER_OP( tanh);
INTERPRETER_OP( tgamma);
INTERPRETER_OP( trunc);
INTERPRETER_OP( get_member);
INTERPRETER_OP( set_member);
INTERPRETER_OP( ptr_eq);
INTERPRETER_OP( ptr_neq);
INTERPRETER_OP( cast);
INTERPRETER_OP( to_string);
INTERPRETER_OP( copy_string);
INTERPRETER_OP( string_index);
INTERPRETER_OP( string_concat);
INTERPRETER_OP( string_equal);
INTERPRETER_OP( string_select);
INTERPRETER_OP( string_strided_select);
INTERPRETER_OP( string_length);
INTERPRETER_OP( done);

/*
INTERPRETER_OP( getreal);
INTERPRETER_OP( getimag);
INTERPRETER_OP( setreal);
INTERPRETER_OP( setimag);
*/

#endif
