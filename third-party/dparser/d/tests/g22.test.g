E : E bop F | F;
bop: '+' $binary_op_left 2 | $binary_op_left 1;
F : uop F | 'x';
uop: '+' $unary_op_right 3;
