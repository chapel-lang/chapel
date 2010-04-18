CHPL = chpl

ifeq ($(CHPL_DEVELOPER), true)

FLAGS = $(F) --devel --gen-ids -g --no-checks --print-commands --print-passes

else 

FLAGS = $(F) --nodevel -O --fast --no-checks --print-commands --print-passes

endif

TARGETS = \
	test_local_var_int32 \
	test_local_var_int32_wo \
	test_local_var_int64 \
	test_local_var_uint64 \
	test_global_var_uint64 \
	test_fn_arg_inout_intent \
	test_fn_arg_global_var \
	test_fn_with_multiple_atomic \
	test_return_atomic_expr \
	atomic_arithmetic_operators_local_var \
	atomic_call_function_return_value \
	atomic_class_member_class_access \
	atomic_dynamic_dispatch \
	atomic_incr_cobegin_sync \
	atomic_incr_local_array \
	atomic_incr_local_array_in_on \
	atomic_recursive_function2 \
	atomic_recursive_function \
	atomic_return_stmt \
	atomic_stack_vs_heap_tracking \
	atomic_swap \
	atomic_trigger_dynamic_dispatch \
	atomic_type_mismatch_unit64_int \
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
