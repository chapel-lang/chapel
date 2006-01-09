#ifndef _INTERPRETER_OPS_H_
#define _INTERPRETER_OPS_H_

class InterpreterOp;

#ifndef _EXTERN
#define _EXTERN extern
#define _INIT
#endif

_EXTERN InterpreterOp *unimplemented_interpreter_op _INIT;
_EXTERN InterpreterOp *init_interpreter_op _INIT;
_EXTERN InterpreterOp *alloc_interpreter_op _INIT;
_EXTERN InterpreterOp *fopen_interpreter_op _INIT;
_EXTERN InterpreterOp *fclose_interpreter_op _INIT;
_EXTERN InterpreterOp *strerror_interpreter_op _INIT;
_EXTERN InterpreterOp *fprintf_interpreter_op _INIT;
_EXTERN InterpreterOp *fscanf_interpreter_op _INIT;
_EXTERN InterpreterOp *array_index_interpreter_op _INIT;
_EXTERN InterpreterOp *array_set_interpreter_op _INIT;
_EXTERN InterpreterOp *unary_minus_interpreter_op _INIT;
_EXTERN InterpreterOp *unary_plus_interpreter_op _INIT;
_EXTERN InterpreterOp *unary_bnot_interpreter_op _INIT;
_EXTERN InterpreterOp *unary_not_interpreter_op _INIT;
_EXTERN InterpreterOp *add_interpreter_op _INIT;
_EXTERN InterpreterOp *subtract_interpreter_op _INIT;
_EXTERN InterpreterOp *mult_interpreter_op _INIT;
_EXTERN InterpreterOp *div_interpreter_op _INIT;
_EXTERN InterpreterOp *mod_interpreter_op _INIT;
_EXTERN InterpreterOp *equal_interpreter_op _INIT;
_EXTERN InterpreterOp *notequal_interpreter_op _INIT;
_EXTERN InterpreterOp *elssorequal_interpreter_op _INIT;
_EXTERN InterpreterOp *greaterorequal_interpreter_op _INIT;
_EXTERN InterpreterOp *less_interpreter_op _INIT;
_EXTERN InterpreterOp *greater_interpreter_op _INIT;
_EXTERN InterpreterOp *and_interpreter_op _INIT;
_EXTERN InterpreterOp *or_interpreter_op _INIT;
_EXTERN InterpreterOp *xor_interpreter_op _INIT;
_EXTERN InterpreterOp *land_interpreter_op _INIT;
_EXTERN InterpreterOp *lor_interpreter_op _INIT;
_EXTERN InterpreterOp *exp_interpreter_op _INIT;
_EXTERN InterpreterOp *get_member_interpreter_op _INIT;
_EXTERN InterpreterOp *set_member_interpreter_op _INIT;
_EXTERN InterpreterOp *ptr_eq_interpreter_op _INIT;
_EXTERN InterpreterOp *ptr_neq_interpreter_op _INIT;
_EXTERN InterpreterOp *cast_interpreter_op _INIT;
_EXTERN InterpreterOp *to_string_interpreter_op _INIT;
_EXTERN InterpreterOp *copy_string_interpreter_op _INIT;
_EXTERN InterpreterOp *string_index_interpreter_op _INIT;
_EXTERN InterpreterOp *string_concat_interpreter_op _INIT;
_EXTERN InterpreterOp *string_equal_interpreter_op _INIT;
_EXTERN InterpreterOp *string_select_interpreter_op _INIT;
_EXTERN InterpreterOp *string_strided_select_interpreter_op _INIT;
_EXTERN InterpreterOp *string_length_interpreter_op _INIT;
_EXTERN InterpreterOp *done_interpreter_op _INIT;


#endif
