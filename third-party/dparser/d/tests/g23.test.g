${declare set_op_priority_from_rule}
S : S '+' S $left 2 | S S $left 1 | '+' S $unary_right 3 | 'x';
