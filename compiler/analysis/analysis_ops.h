#ifndef _analysis_ops_H_
#define _analysis_ops_H_

#ifndef _EXTERN
#define _EXTERN extern
#define _INIT
#endif

class AnalysisOp;

_EXTERN AnalysisOp *unimplemented_analysis_op _INIT;
_EXTERN AnalysisOp *init_analysis_op _INIT;
_EXTERN AnalysisOp *return_bool_analysis_op _INIT;
_EXTERN AnalysisOp *return_int_analysis_op _INIT;
_EXTERN AnalysisOp *return_string_analysis_op _INIT;
_EXTERN AnalysisOp *write_analysis_op _INIT;
_EXTERN AnalysisOp *read_analysis_op _INIT;
_EXTERN AnalysisOp *coerce_analysis_op _INIT;
_EXTERN AnalysisOp *array_index_analysis_op _INIT;
_EXTERN AnalysisOp *array_set_analysis_op _INIT;
_EXTERN AnalysisOp *index_object_analysis_op _INIT;
_EXTERN AnalysisOp *set_index_object_analysis_op _INIT;
_EXTERN AnalysisOp *array_pointwise_op_analysis_op _INIT;
_EXTERN AnalysisOp *unary_minus_analysis_op _INIT;
_EXTERN AnalysisOp *unary_plus_analysis_op _INIT;
_EXTERN AnalysisOp *unary_bnot_analysis_op _INIT;
_EXTERN AnalysisOp *unary_not_analysis_op _INIT;
_EXTERN AnalysisOp *add_analysis_op _INIT;
_EXTERN AnalysisOp *subtract_analysis_op _INIT;
_EXTERN AnalysisOp *mult_analysis_op _INIT;
_EXTERN AnalysisOp *div_analysis_op _INIT;
_EXTERN AnalysisOp *mod_analysis_op _INIT;
_EXTERN AnalysisOp *equal_analysis_op _INIT;
_EXTERN AnalysisOp *notequal_analysis_op _INIT;
_EXTERN AnalysisOp *lessorequal_analysis_op _INIT;
_EXTERN AnalysisOp *greaterorequal_analysis_op _INIT;
_EXTERN AnalysisOp *less_analysis_op _INIT;
_EXTERN AnalysisOp *greater_analysis_op _INIT;
_EXTERN AnalysisOp *and_analysis_op _INIT;
_EXTERN AnalysisOp *or_analysis_op _INIT;
_EXTERN AnalysisOp *xor_analysis_op _INIT;
_EXTERN AnalysisOp *land_analysis_op _INIT;
_EXTERN AnalysisOp *lor_analysis_op _INIT;
_EXTERN AnalysisOp *exp_analysis_op _INIT;
_EXTERN AnalysisOp *cast_analysis_op _INIT;
_EXTERN AnalysisOp *alloc_analysis_op _INIT;

#endif
