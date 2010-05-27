CHPL = chpl

FLAGS += --conditional-dynamic-dispatch-limit 100

ifeq ($(CHPL_DEVELOPER), true)

FLAGS += $(F) --devel --gen-ids -g --no-checks --print-commands --print-passes

else 

FLAGS += $(F) --no-devel -O --fast --no-checks --print-commands --print-passes

endif

TARGETS = \
	omitElseCondUsingAtomicFlag \
	recursiveFns \
	serializeWholeArrayAssignment \
	test_array_local_uint64 \
	test_associative_domain \
	test_atomic_expr_in_return_stmt \
	test_atomic_multiloc_simple \
	test_atomic_stmt_in_local_block \
	test_atomic_stmt_in_on_stmt \
	test_atomic_on_clause \
	test_atomic_on_clause_in_coforall \
	test_class_var_atomic_access \
	test_fail_type_mismatch_in_atomic \
	test_fn_arg_global_var \
	test_fn_arg_inout_intent \
	test_fn_arg_inout_intent_recursive \
	test_fn_return_value \
	test_fn_with_multiple_atomic \
	test_prim_get_member \
	test_prim_get_member_value \
	test_serial_stmt_in_atomic \
	test_stmt_return_in_atomic \
	test_sync_lock_in_atomic \
	test_var_global_uint64 \
	test_var_local_bool8 \
	test_var_local_int32 \
	test_var_local_int32_wo \
	test_var_local_int64 \
	test_var_local_uint64 \
	test_whole_array_assign_in_atomic \
	atomic_arithmetic_operators_local_var \
	atomic_class_member_class_access \
	atomic_dynamic_dispatch \
	atomic_incr_cobegin_sync \
	atomic_stack_vs_heap_tracking \
	atomic_swap \
	atomic_trigger_dynamic_dispatch \
	simple_fn2 \
	simple_fn3 \
	simple_fn4 \
	simple_fn \
	test_dynamic_dispatch2 \
	test_dynamic_dispatch \

default: all 

all: $(TARGETS)

%: %.chpl
	$(CHPL) --savec c_$@ $(FLAGS) -o $@ $^

clean: 
	rm -f $(TARGETS) *_real 

clobber: clean
	rm -rf *_c c_*
