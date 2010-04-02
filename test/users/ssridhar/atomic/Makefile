CHPL = chpl

ifeq ($(CHPL_DEVELOPER), true)

FLAGS = --gen-ids --gdb -g --no-checks --print-commands --print-passes

else 

FLAGS = -O --fast --no-checks --print-commands --print-passes

endif

ifeq ($(CHPL_GEN_HTML), true)

FLAGS += -t

endif

TARGETS = \
	atomic_arithmetic_operators_local_var \
	atomic_call_function_return_value \
	atomic_class_member_class_access \
	atomic_dynamic_dispatch \
	atomic_incr_cobegin_sync \
	atomic_incr_global_var \
	atomic_incr_local_array \
	atomic_incr_local_array_in_on \
	atomic_incr_local_var \
	atomic_recursive_function2 \
	atomic_recursive_function \
	atomic_return_stmt \
	atomic_stack_vs_heap_tracking \
	atomic_swap \
	atomic_trigger_dynamic_dispatch \
	atomic_type_mismatch_unit64_int \
	return_atomic_expr \
	simple_fn2 \
	simple_fn3 \
	simple_fn4 \
	simple_fn \
	test_dynamic_dispatch2 \
	test_dynamic_dispatch \

default: all 

all: $(TARGETS)

%: %.chpl
	$(CHPL) --savec $@_c $(FLAGS) -o $@ $^

clean: 
	rm -f $(TARGETS) *_real

clobber: clean
	rm -rf *_c
