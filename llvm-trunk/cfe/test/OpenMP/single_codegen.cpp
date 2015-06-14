// RUN: %clang_cc1 -verify -fopenmp -x c++ -triple x86_64-unknown-unknown -emit-llvm %s -fexceptions -fcxx-exceptions -o - | FileCheck %s
// RUN: %clang_cc1 -fopenmp -x c++ -std=c++11 -triple x86_64-unknown-unknown -fexceptions -fcxx-exceptions -emit-pch -o %t %s
// RUN: %clang_cc1 -fopenmp -x c++ -triple x86_64-unknown-unknown -fexceptions -fcxx-exceptions -std=c++11 -include-pch %t -verify %s -emit-llvm -o - | FileCheck %s
// RUN: %clang_cc1 -verify -triple x86_64-apple-darwin10 -fopenmp -fexceptions -fcxx-exceptions -gline-tables-only -x c++ -emit-llvm %s -o - | FileCheck %s --check-prefix=TERM_DEBUG
// RUN: %clang_cc1 -verify -fopenmp -x c++ -std=c++11 -DARRAY -triple x86_64-apple-darwin10 -emit-llvm %s -o - | FileCheck -check-prefix=ARRAY %s
// expected-no-diagnostics

#ifndef ARRAY
#ifndef HEADER
#define HEADER

class TestClass {
public:
  int a;
  TestClass() : a(0) {}
  TestClass(const TestClass &C) : a(C.a) {}
  TestClass &operator=(const TestClass &) { return *this;}
  ~TestClass(){};
};

// CHECK-DAG:   [[TEST_CLASS_TY:%.+]] = type { i{{[0-9]+}} }
// CHECK:       [[IDENT_T_TY:%.+]] = type { i32, i32, i32, i32, i8* }
// CHECK:       [[IMPLICIT_BARRIER_SINGLE_LOC:@.+]] = private unnamed_addr constant %{{.+}} { i32 0, i32 322, i32 0, i32 0, i8*

// CHECK:       define void [[FOO:@.+]]()

TestClass tc;
TestClass tc2[2];
#pragma omp threadprivate(tc, tc2)

void foo() {}

// CHECK-LABEL: @main
// TERM_DEBUG-LABEL: @main
int main() {
  // CHECK-DAG: [[A_ADDR:%.+]] = alloca i8
  // CHECK-DAG: [[A2_ADDR:%.+]] = alloca [2 x i8]
  // CHECK-DAG: [[C_ADDR:%.+]] = alloca [[TEST_CLASS_TY]]
  char a;
  char a2[2];
  TestClass c;

// CHECK:       [[GTID:%.+]] = call i32 @__kmpc_global_thread_num([[IDENT_T_TY]]* [[DEFAULT_LOC:@.+]])
// CHECK-DAG:   [[DID_IT:%.+]] = alloca i32,
// CHECK-DAG:   [[COPY_LIST:%.+]] = alloca [5 x i8*],

// CHECK:       [[RES:%.+]] = call i32 @__kmpc_single([[IDENT_T_TY]]* [[DEFAULT_LOC]], i32 [[GTID]])
// CHECK-NEXT:  [[IS_SINGLE:%.+]] = icmp ne i32 [[RES]], 0
// CHECK-NEXT:  br i1 [[IS_SINGLE]], label {{%?}}[[THEN:.+]], label {{%?}}[[EXIT:.+]]
// CHECK:       [[THEN]]
// CHECK-NEXT:  store i8 2, i8* [[A_ADDR]]
// CHECK-NEXT:  call void @__kmpc_end_single([[IDENT_T_TY]]* [[DEFAULT_LOC]], i32 [[GTID]])
// CHECK-NEXT:  br label {{%?}}[[EXIT]]
// CHECK:       [[EXIT]]
// CHECK-NOT:   call {{.+}} @__kmpc_cancel_barrier
#pragma omp single nowait
  a = 2;
// CHECK:       [[RES:%.+]] = call i32 @__kmpc_single([[IDENT_T_TY]]* [[DEFAULT_LOC]], i32 [[GTID]])
// CHECK-NEXT:  [[IS_SINGLE:%.+]] = icmp ne i32 [[RES]], 0
// CHECK-NEXT:  br i1 [[IS_SINGLE]], label {{%?}}[[THEN:.+]], label {{%?}}[[EXIT:.+]]
// CHECK:       [[THEN]]
// CHECK-NEXT:  store i8 2, i8* [[A_ADDR]]
// CHECK-NEXT:  call void @__kmpc_end_single([[IDENT_T_TY]]* [[DEFAULT_LOC]], i32 [[GTID]])
// CHECK-NEXT:  br label {{%?}}[[EXIT]]
// CHECK:       [[EXIT]]
// CHECK:       call{{.*}} @__kmpc_cancel_barrier([[IDENT_T_TY]]* [[IMPLICIT_BARRIER_SINGLE_LOC]], i32 [[GTID]])
#pragma omp single
  a = 2;
// CHECK:       store i32 0, i32* [[DID_IT]]
// CHECK:       [[RES:%.+]] = call i32 @__kmpc_single([[IDENT_T_TY]]* [[DEFAULT_LOC]], i32 [[GTID]])
// CHECK-NEXT:  [[IS_SINGLE:%.+]] = icmp ne i32 [[RES]], 0
// CHECK-NEXT:  br i1 [[IS_SINGLE]], label {{%?}}[[THEN:.+]], label {{%?}}[[EXIT:.+]]
// CHECK:       [[THEN]]
// CHECK-NEXT:  invoke void [[FOO]]()
// CHECK:       to label {{%?}}[[CONT:.+]] unwind
// CHECK:       [[CONT]]
// CHECK:       store i32 1, i32* [[DID_IT]]
// CHECK:       call void @__kmpc_end_single([[IDENT_T_TY]]* [[DEFAULT_LOC]], i32 [[GTID]])
// CHECK-NEXT:  br label {{%?}}[[EXIT]]
// CHECK:       [[EXIT]]
// CHECK:       [[A_PTR_REF:%.+]] = getelementptr inbounds [5 x i8*], [5 x i8*]* [[COPY_LIST]], i{{[0-9]+}} 0, i{{[0-9]+}} 0
// CHECK:       store i8* [[A_ADDR]], i8** [[A_PTR_REF]],
// CHECK:       [[C_PTR_REF:%.+]] = getelementptr inbounds [5 x i8*], [5 x i8*]* [[COPY_LIST]], i{{[0-9]+}} 0, i{{[0-9]+}} 1
// CHECK:       [[C_PTR_REF_VOID_PTR:%.+]] = bitcast [[TEST_CLASS_TY]]* [[C_ADDR]] to i8*
// CHECK:       store i8* [[C_PTR_REF_VOID_PTR]], i8** [[C_PTR_REF]],
// CHECK:       [[TC_PTR_REF:%.+]] = getelementptr inbounds [5 x i8*], [5 x i8*]* [[COPY_LIST]], i{{[0-9]+}} 0, i{{[0-9]+}} 2
// CHECK:       [[TC_THREADPRIVATE_ADDR_VOID_PTR:%.+]] = call{{.*}} i8* @__kmpc_threadprivate_cached
// CHECK:       [[TC_THREADPRIVATE_ADDR:%.+]] = bitcast i8* [[TC_THREADPRIVATE_ADDR_VOID_PTR]] to [[TEST_CLASS_TY]]*
// CHECK:       [[TC_PTR_REF_VOID_PTR:%.+]] = bitcast [[TEST_CLASS_TY]]* [[TC_THREADPRIVATE_ADDR]] to i8*
// CHECK:       store i8* [[TC_PTR_REF_VOID_PTR]], i8** [[TC_PTR_REF]],
// CHECK:       [[A2_PTR_REF:%.+]] = getelementptr inbounds [5 x i8*], [5 x i8*]* [[COPY_LIST]], i{{[0-9]+}} 0, i{{[0-9]+}} 3
// CHECK:       [[BITCAST:%.+]] = bitcast [2 x i8]* [[A2_ADDR]] to i8*
// CHECK:       store i8* [[BITCAST]], i8** [[A2_PTR_REF]],
// CHECK:       [[TC2_PTR_REF:%.+]] = getelementptr inbounds [5 x i8*], [5 x i8*]* [[COPY_LIST]], i{{[0-9]+}} 0, i{{[0-9]+}} 4
// CHECK:       [[TC2_THREADPRIVATE_ADDR_VOID_PTR:%.+]] = call{{.*}} i8* @__kmpc_threadprivate_cached
// CHECK:       [[TC2_THREADPRIVATE_ADDR:%.+]] = bitcast i8* [[TC2_THREADPRIVATE_ADDR_VOID_PTR]] to [2 x [[TEST_CLASS_TY]]]*
// CHECK:       [[TC2_PTR_REF_VOID_PTR:%.+]] = bitcast [2 x [[TEST_CLASS_TY]]]* [[TC2_THREADPRIVATE_ADDR]] to i8*
// CHECK:       store i8* [[TC2_PTR_REF_VOID_PTR]], i8** [[TC2_PTR_REF]],
// CHECK:       [[COPY_LIST_VOID_PTR:%.+]] = bitcast [5 x i8*]* [[COPY_LIST]] to i8*
// CHECK:       [[DID_IT_VAL:%.+]] = load i32, i32* [[DID_IT]],
// CHECK:       call void @__kmpc_copyprivate([[IDENT_T_TY]]* [[DEFAULT_LOC]], i32 [[GTID]], i64 40, i8* [[COPY_LIST_VOID_PTR]], void (i8*, i8*)* [[COPY_FUNC:@.+]], i32 [[DID_IT_VAL]])
// CHECK-NOT:   call {{.+}} @__kmpc_cancel_barrier
#pragma omp single copyprivate(a, c, tc, a2, tc2)
  foo();
// CHECK-NOT:   call i32 @__kmpc_single
// CHECK-NOT:   call void @__kmpc_end_single
  return a;
}

// CHECK: void [[COPY_FUNC]](i8*, i8*)
// CHECK: store i8* %0, i8** [[DST_ADDR_REF:%.+]],
// CHECK: store i8* %1, i8** [[SRC_ADDR_REF:%.+]],
// CHECK: [[DST_ADDR_VOID_PTR:%.+]] = load i8*, i8** [[DST_ADDR_REF]],
// CHECK: [[DST_ADDR:%.+]] = bitcast i8* [[DST_ADDR_VOID_PTR]] to [5 x i8*]*
// CHECK: [[SRC_ADDR_VOID_PTR:%.+]] = load i8*, i8** [[SRC_ADDR_REF]],
// CHECK: [[SRC_ADDR:%.+]] = bitcast i8* [[SRC_ADDR_VOID_PTR]] to [5 x i8*]*
// CHECK: [[DST_A_ADDR_REF:%.+]] = getelementptr inbounds [5 x i8*], [5 x i8*]* [[DST_ADDR]], i{{[0-9]+}} 0, i{{[0-9]+}} 0
// CHECK: [[DST_A_ADDR:%.+]] = load i8*, i8** [[DST_A_ADDR_REF]],
// CHECK: [[SRC_A_ADDR_REF:%.+]] = getelementptr inbounds [5 x i8*], [5 x i8*]* [[SRC_ADDR]], i{{[0-9]+}} 0, i{{[0-9]+}} 0
// CHECK: [[SRC_A_ADDR:%.+]] = load i8*, i8** [[SRC_A_ADDR_REF]],
// CHECK: [[SRC_A_VAL:%.+]] = load i8, i8* [[SRC_A_ADDR]],
// CHECK: store i8 [[SRC_A_VAL]], i8* [[DST_A_ADDR]],
// CHECK: [[DST_C_ADDR_REF:%.+]] = getelementptr inbounds [5 x i8*], [5 x i8*]* [[DST_ADDR]], i{{[0-9]+}} 0, i{{[0-9]+}} 1
// CHECK: [[DST_C_ADDR_VOID_PTR:%.+]] = load i8*, i8** [[DST_C_ADDR_REF]],
// CHECK: [[DST_C_ADDR:%.+]] = bitcast i8* [[DST_C_ADDR_VOID_PTR]] to [[TEST_CLASS_TY]]*
// CHECK: [[SRC_C_ADDR_REF:%.+]] = getelementptr inbounds [5 x i8*], [5 x i8*]* [[SRC_ADDR]], i{{[0-9]+}} 0, i{{[0-9]+}} 1
// CHECK: [[SRC_C_ADDR_VOID_PTR:%.+]] = load i8*, i8** [[SRC_C_ADDR_REF]],
// CHECK: [[SRC_C_ADDR:%.+]] = bitcast i8* [[SRC_C_ADDR_VOID_PTR]] to [[TEST_CLASS_TY]]*
// CHECK: call{{.*}} [[TEST_CLASS_TY_ASSIGN:@.+]]([[TEST_CLASS_TY]]* [[DST_C_ADDR]], [[TEST_CLASS_TY]]* {{.*}}[[SRC_C_ADDR]])
// CHECK: [[DST_TC_ADDR_REF:%.+]] = getelementptr inbounds [5 x i8*], [5 x i8*]* [[DST_ADDR]], i{{[0-9]+}} 0, i{{[0-9]+}} 2
// CHECK: [[DST_TC_ADDR_VOID_PTR:%.+]] = load i8*, i8** [[DST_TC_ADDR_REF]],
// CHECK: [[DST_TC_ADDR:%.+]] = bitcast i8* [[DST_TC_ADDR_VOID_PTR]] to [[TEST_CLASS_TY]]*
// CHECK: [[SRC_TC_ADDR_REF:%.+]] = getelementptr inbounds [5 x i8*], [5 x i8*]* [[SRC_ADDR]], i{{[0-9]+}} 0, i{{[0-9]+}} 2
// CHECK: [[SRC_TC_ADDR_VOID_PTR:%.+]] = load i8*, i8** [[SRC_TC_ADDR_REF]],
// CHECK: [[SRC_TC_ADDR:%.+]] = bitcast i8* [[SRC_TC_ADDR_VOID_PTR]] to [[TEST_CLASS_TY]]*
// CHECK: call{{.*}} [[TEST_CLASS_TY_ASSIGN]]([[TEST_CLASS_TY]]* [[DST_TC_ADDR]], [[TEST_CLASS_TY]]* {{.*}}[[SRC_TC_ADDR]])
// CHECK: [[DST_A2_ADDR_REF:%.+]] = getelementptr inbounds [5 x i8*], [5 x i8*]* [[DST_ADDR]], i{{[0-9]+}} 0, i{{[0-9]+}} 3
// CHECK: [[DST_A2_ADDR:%.+]] = load i8*, i8** [[DST_A2_ADDR_REF]],
// CHECK: [[SRC_A2_ADDR_REF:%.+]] = getelementptr inbounds [5 x i8*], [5 x i8*]* [[SRC_ADDR]], i{{[0-9]+}} 0, i{{[0-9]+}} 3
// CHECK: [[SRC_A2_ADDR:%.+]] = load i8*, i8** [[SRC_A2_ADDR_REF]],
// CHECK: call void @llvm.memcpy.{{.+}}(i8* [[DST_A2_ADDR]], i8* [[SRC_A2_ADDR]], i{{[0-9]+}} 2, i{{[0-9]+}} 1, i1 false)
// CHECK: [[DST_TC2_ADDR_REF:%.+]] = getelementptr inbounds [5 x i8*], [5 x i8*]* [[DST_ADDR]], i{{[0-9]+}} 0, i{{[0-9]+}} 4
// CHECK: [[DST_TC2_ADDR_VOID_PTR:%.+]] = load i8*, i8** [[DST_TC2_ADDR_REF]],
// CHECK: [[DST_TC2_ADDR:%.+]] = bitcast i8* [[DST_TC2_ADDR_VOID_PTR]] to [[TEST_CLASS_TY]]*
// CHECK: [[SRC_TC2_ADDR_REF:%.+]] = getelementptr inbounds [5 x i8*], [5 x i8*]* [[SRC_ADDR]], i{{[0-9]+}} 0, i{{[0-9]+}} 4
// CHECK: [[SRC_TC2_ADDR_VOID_PTR:%.+]] = load i8*, i8** [[SRC_TC2_ADDR_REF]],
// CHECK: [[SRC_TC2_ADDR:%.+]] = bitcast i8* [[SRC_TC2_ADDR_VOID_PTR]] to [[TEST_CLASS_TY]]*
// CHECK: br i1
// CHECK: call{{.*}} [[TEST_CLASS_TY_ASSIGN]]([[TEST_CLASS_TY]]* %{{.+}}, [[TEST_CLASS_TY]]* {{.*}})
// CHECK: br i1
// CHECK: ret void

// CHECK-LABEL:      parallel_single
// TERM_DEBUG-LABEL: parallel_single
void parallel_single() {
#pragma omp parallel
#pragma omp single
  // TERM_DEBUG-NOT: __kmpc_global_thread_num
  // TERM_DEBUG:     call i32 @__kmpc_single({{.+}}), !dbg [[DBG_LOC_START:![0-9]+]]
  // TERM_DEBUG:     invoke void {{.*}}foo{{.*}}()
  // TERM_DEBUG:     unwind label %[[TERM_LPAD:.+]],
  // TERM_DEBUG-NOT: __kmpc_global_thread_num
  // TERM_DEBUG:     call void @__kmpc_end_single({{.+}}), !dbg [[DBG_LOC_END:![0-9]+]]
  // TERM_DEBUG:     [[TERM_LPAD]]
  // TERM_DEBUG:     call void @__clang_call_terminate
  // TERM_DEBUG:     unreachable
  foo();
}
// TERM_DEBUG-DAG: [[DBG_LOC_START]] = !DILocation(line: [[@LINE-12]],
// TERM_DEBUG-DAG: [[DBG_LOC_END]] = !DILocation(line: [[@LINE-3]],
#endif
#else
// ARRAY-LABEL: array_func
struct St {
  int a, b;
  St() : a(0), b(0) {}
  St &operator=(const St &) { return *this; };
  ~St() {}
};

void array_func(int n, int a[n], St s[2]) {
// ARRAY: call void @__kmpc_copyprivate(%ident_t* @{{.+}}, i32 %{{.+}}, i64 16, i8* %{{.+}}, void (i8*, i8*)* [[CPY:@.+]], i32 %{{.+}})
#pragma omp single copyprivate(a, s)
  ;
}
// ARRAY: define internal void [[CPY]]
// ARRAY: store i32* %{{.+}}, i32** %{{.+}},
// ARRAY: store %struct.St* %{{.+}}, %struct.St** %{{.+}},
#endif
