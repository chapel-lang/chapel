/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testreduce.c $
 * Description: test of user-defined reductions
 * Copyright 2018, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#define SCRATCH_SIZE (2*1024*1024)

#ifndef TEST_SEGSZ
#define TEST_SEGSZ (PAGESZ + SCRATCH_SIZE) // even team's scratch + pad
#endif

#include <gasnetex.h>
#include <gasnet_coll.h>
#include <test.h>

#include <string.h>
#include <stdint.h>
#include <float.h>
#include <math.h>

// Size of vector
static int Nelem;
#define NELEM_DEFAULT 64

// Polymorphic operator for a commutative operation: addition
void op_ADD(const void * arg1,
            void *       arg2_and_out,
            size_t       count,
            const void * cdata)
{
  gex_DT_t dt = (gex_DT_t)(uintptr_t)cdata;
  switch (dt) {
    case GEX_DT_I32:
    case GEX_DT_U32: {
      const uint32_t * restrict x = arg1;
            uint32_t * restrict y = arg2_and_out;
      for (size_t i = 0; i < count; ++i) y[i] += x[i];
      break;
    }
    case GEX_DT_I64:
    case GEX_DT_U64: {
      const uint64_t * restrict x = arg1;
            uint64_t * restrict y = arg2_and_out;
      for (size_t i = 0; i < count; ++i) y[i] += x[i];
      break;
    }
    case GEX_DT_FLT: {
      const float * restrict x = arg1;
            float * restrict y = arg2_and_out;
      for (size_t i = 0; i < count; ++i) y[i] += x[i];
      break;
    }
    case GEX_DT_DBL: {
      const double * restrict x = arg1;
            double * restrict y = arg2_and_out;
      for (size_t i = 0; i < count; ++i) y[i] += x[i];
      break;
    }
  }
}

// Type and operator for a non-commutative operation: strcat
// "Post-concatenates" two null-terminated strings (e.g. "arg2 .= arg1" in perl)
static size_t myDT_sz;
void op_concat(const void * arg1,
               void *       arg2_and_out,
               size_t       count,
               const void * cdata)
{
  const size_t dt_sz = (size_t)(uintptr_t)cdata;
  const char * restrict x = arg1;
        char * restrict y = arg2_and_out;
  for (size_t i = 0; i < count; ++i, x+=dt_sz, y+=dt_sz) {
    strcat(y,x);
  }
}

// Macros to ease other macros
#define I32_TYPE  int32_t
#define U32_TYPE uint32_t
#define I64_TYPE  int64_t
#define U64_TYPE uint64_t
#define FLT_TYPE    float
#define DBL_TYPE   double

// Convenience to avoid calling TEST_RAND with lo==hi
#define RAND_ROOT(size) ((size == 1) ? 0 : TEST_RAND(0, size-1))

// Test ADD (both built-in and user-defined for a built-in DT
#define TEST_ADD(DT) do {                                                     \
  typedef DT##_TYPE TYPE;                                                     \
  size_t nbytes = Nelem * sizeof(TYPE);                                       \
  TYPE *answers = test_malloc(iters * nbytes);                                \
  TYPE *src = test_malloc(iters * nbytes);                                    \
  TYPE *dst = answers;                                                        \
  int failures = 0;                                                           \
  int local_count = 0;                                                        \
  for (int i = 0; i < Nelem; ++i) src[i] = (rank + i);                        \
  for (int i = 0; i < iters; ++i) {                                           \
    if (TEST_RAND_ONEIN(5)) {                                                 \
      memset(dst, 0xaa, nbytes);                                              \
      ev[i] = gex_Coll_ReduceToAllNB(tm, dst, src,                            \
                                     GEX_DT_##DT, sizeof(TYPE), Nelem,        \
                                     GEX_OP_ADD, NULL, NULL, 0);              \
      local_count += 1;                                                       \
      dst += Nelem;                                                           \
    } else {                                                                  \
      gex_Rank_t root = RAND_ROOT(size);                                      \
      if (root == rank) memset(dst, 0x55, nbytes);                            \
      ev[i] = gex_Coll_ReduceToOneNB(tm, root, dst, src,                      \
                                     GEX_DT_##DT, sizeof(TYPE), Nelem,        \
                                     GEX_OP_ADD, NULL, NULL, 0);              \
      if (root == rank) {                                                     \
        local_count += 1;                                                     \
        dst += Nelem;                                                         \
      }                                                                       \
    }                                                                         \
  }                                                                           \
  gex_Event_WaitAll(ev, iters, 0);                                            \
  dst = answers;                                                              \
  for (int i = 0; i < local_count; ++i) {                                     \
    for (int j = 0; j < Nelem; ++j) {                                         \
      uint64_t got = *dst++;                                                  \
      if (got != correct[j]) {                                                \
        MSG("Mismatch GEX_OP_ADD(GEX_DT_" #DT "), iter=%i, elem=%i"           \
            " (got=%" PRIu64 ", want=%" PRIu64 ")", i, j, got, correct[j]);   \
        ++failures;                                                           \
      }                                                                       \
    }                                                                         \
  }                                                                           \
  local_count = 0;                                                            \
  dst = answers;                                                              \
  for (int i = 0; i < iters; ++i) {                                           \
    if (TEST_RAND_ONEIN(5)) {                                                 \
      memset(dst, 0xee, nbytes);                                              \
      ev[i] = gex_Coll_ReduceToAllNB(tm, dst, src,                            \
                                     GEX_DT_##DT, sizeof(TYPE), Nelem,        \
                                     GEX_OP_USER, &op_ADD,                    \
                                     (void*)(uintptr_t)GEX_DT_##DT, 0);       \
      local_count += 1;                                                       \
      dst += Nelem;                                                           \
    } else {                                                                  \
      gex_Rank_t root = RAND_ROOT(size);                                      \
      if (root == rank) memset(dst, 0x33, nbytes);                            \
      ev[i] = gex_Coll_ReduceToOneNB(tm, root, dst, src,                      \
                                     GEX_DT_##DT, sizeof(TYPE), Nelem,        \
                                     GEX_OP_USER, &op_ADD,                    \
                                     (void*)(uintptr_t)GEX_DT_##DT, 0);       \
      if (root == rank) {                                                     \
        local_count += 1;                                                     \
        dst += Nelem;                                                         \
      }                                                                       \
    }                                                                         \
  }                                                                           \
  gex_Event_WaitAll(ev, iters, 0);                                            \
  dst = answers;                                                              \
  for (int i = 0; i < local_count; ++i) {                                     \
    for (int j = 0; j < Nelem; ++j) {                                         \
      uint64_t got = *dst++;                                                  \
      if (got != correct[j]) {                                                \
        MSG("Mismatch GEX_OP_USER(GEX_DT_" #DT "), iter=%i, elem=%i"          \
            " (got=%" PRIu64 ", want=%" PRIu64 ")", i, j, got, correct[j]);   \
        ++failures;                                                           \
      }                                                                       \
    }                                                                         \
  }                                                                           \
  test_free(src);                                                             \
  test_free(answers);                                                         \
  if (failures) ERR("GEX_DT_" #DT ": %d failures", failures);                 \
  gex_Event_Wait(gex_Coll_BarrierNB(tm,0));                                   \
} while (0)

static int iters = 0;

//
// GEX_OP ADD
// Test for every type which can exactly represent Sum(ranks).
// This tests both built-in and user-defines ADD operations
// TODO: split to make a team small enough to test instead of skipping?
// TODO: args to limit which tests?
//
void do_test_add(gex_TM_t tm)
{
    gex_Rank_t rank = gex_TM_QueryRank(tm);
    gex_Rank_t size = gex_TM_QuerySize(tm);

    uint64_t *correct = test_malloc(iters * Nelem * sizeof(uint64_t));
    gex_Event_t *ev = test_malloc(iters * sizeof(gex_Event_t));

    assert_always(size <= INT32_MAX - Nelem);
    for (int i = 0; i < Nelem; ++i) {
      correct[i] = ((size + i) * (size + i - 1) - i * (i - 1)) / 2;
    }

    uint64_t biggest = correct[Nelem-1];
    if (biggest <= (uint64_t) INT32_MAX) {
      MSG0("Running ADD:GEX_DT_I32 test...");
      TEST_ADD(I32);
    } else {
      MSG0("WARNING: skipping ADD:GEX_DT_I32 test (would overflow)");
    }
    if (biggest <= (uint64_t)UINT32_MAX) {
      MSG0("Running ADD:GEX_DT_U32 test...");
      TEST_ADD(U32);
    } else {
      MSG0("WARNING: skipping ADD:GEX_DT_U32 test (would overflow)");
    }
    MSG0("Running ADD:GEX_DT_I64 test...");
    TEST_ADD(I64);
    MSG0("Running ADD:GEX_DT_U64 test...");
    TEST_ADD(U64);
    if (biggest < (uint64_t)pow(FLT_RADIX, FLT_MANT_DIG)) {
      MSG0("Running ADD:GEX_DT_FLT test...");
      TEST_ADD(FLT);
    } else {
      MSG0("WARNING: skipping ADD:GEX_DT_FLT test (exceeds mantissa bits)");
    }
    if (biggest < (uint64_t)pow(FLT_RADIX, DBL_MANT_DIG)) {
      MSG0("Running ADD:GEX_DT_DBL test...");
      TEST_ADD(DBL);
    } else {
      MSG0("WARNING: skipping ADD:GEX_DT_DBL test (exceeds mantissa bits)");
    }

    test_free(ev);
    test_free(correct);
}

//
// Test "concat" operator on user-defined data type (a fixed-len char[])
//
void do_test_user(gex_TM_t tm)
{
    MSG0("Running GEX_DT_USER tests...");

    gex_Rank_t rank = gex_TM_QueryRank(tm);
    gex_Rank_t size = gex_TM_QuerySize(tm);

    // Use just 'A' - 'Z' for ease of debugging by humans
    #define BASE_CHAR 'A'
    #define NUM_CHAR  26
    size_t dt_sz = size + 1; // One for '\0' terminator
    char *InStrings  = test_malloc(Nelem * dt_sz * sizeof(char));
    char *OutStrings = test_malloc(Nelem * dt_sz * sizeof(char));
    for (size_t i = 0; i < Nelem; ++i) {
      char *p = InStrings + i * dt_sz;
      p[0] = BASE_CHAR + ((rank + i) % NUM_CHAR);
      p[1] = '\0';
    }

    // GEX_OP_USER(GEX_DT_USER): OP(x,y) := strcat(y,x)
    // Due to permitted assumption of commutativity, order is unspecified.
    // However, the reduction must preserve number of instances of each char.
    int failures = 0;
    for (int i = 0; i < iters; ++i) {
      gex_Rank_t root = RAND_ROOT(size);
      gex_Event_Wait(
        gex_Coll_ReduceToOneNB(tm, root, OutStrings, InStrings,
                               GEX_DT_USER, dt_sz, Nelem,
                               GEX_OP_USER, &op_concat,
                               (void*)(uintptr_t)dt_sz, 0));
      if (rank==root) {
        for (size_t j = 0; j < Nelem; ++j) {
          const char *str = OutStrings + j * dt_sz;
          // Count the number of occurances of each char
          gex_Rank_t tally[NUM_CHAR];
          memset(tally, 0, sizeof(tally));
          for (size_t k = 0; k < (dt_sz-1); ++k) {
            uint8_t idx = str[k] - BASE_CHAR;
            if (idx > 25) {
              ++failures;
            } else {
              ++tally[idx];
            }
          }
          // Check counts against the expected ones
          gex_Rank_t numer = size / NUM_CHAR;
          gex_Rank_t denom = size % NUM_CHAR;
          for (int k = 0; k < NUM_CHAR; ++k) {
            size_t expect = (numer + (k < denom));
            failures += (tally[(j + k) % NUM_CHAR] != expect);
          }
        }
      }
    }
    if (failures) ERR("GEX_OP_USER(GEX_DT_USER): %d failures", failures);
    gex_Event_Wait(gex_Coll_BarrierNB(tm,0));

    // GEX_OP_USER_NC(GEX_DT_USER): OP(x,y) := strcat(y,x)
    // Due to non-commutativity, order must be preserved.
  #if 0 // TODO: enabled once GEX_OP_USER_NC is implemented
    failures = 0;
    for (int i = 0; i < iters; ++i) {
      gex_Rank_t root = RAND_ROOT(size);
      gex_Event_Wait(
        gex_Coll_ReduceToOneNB(tm, root, OutStrings, InStrings,
                               GEX_DT_USER, dt_sz, Nelem,
                               GEX_OP_USER_NC, &op_concat,
                               (void*)(uintptr_t)dt_sz, 0));
      if (rank==root) {
        for (size_t j = 0; j < Nelem; ++j) {
          // validate result against properly ordered (lexically reversed) result
          const char *str = OutStrings + j * dt_sz;
          int idx = (j + (size - 1)) % NUM_CHAR;
          for (size_t k = 0; k < (dt_sz-1); ++k) {
            failures += (str[k] != BASE_CHAR + idx);
             // not using % since (-1 % x) is -1, not (x-1)
            idx = idx ? (idx - 1) : (NUM_CHAR - 1);
          }
        }
      }
    }
    if (failures) ERR("GEX_OP_USER_NC(GEX_DT_USER): %d failures", failures);
    gex_Event_Wait(gex_Coll_BarrierNB(tm,0));
  #endif

  test_free(OutStrings);
  test_free(InStrings);
}

void do_tests(gex_TM_t tm) {
  do_test_add(tm);    // GEX_OP_ADD
  do_test_user(tm);   // GEX_DT_USER

  //
  // TODO: test the remaining built-in operators.
  //
  // Validation:
  // Option 1: Assuming the user-defined ADD passed above, use a user-defined
  //           reduction as the reference for validation of each built-in
  //           operator.
  // Option 2: Follow the pattern of the ADD test above, and chose inputs that
  //           provide a locally computable reference for validation.
  //
  // GEX_OP_MULT:
  // There is concern over FP Mult and exact reproducibility.
  // If (and only if) we assume a "high-quality" implementation with full
  // reproducability of the order of evaliuation it *might* be reasonable to
  // exect bit-wise idenitcal results from the built-in and user-defined
  // versions of the same operator (use Option 1, above).  However, that sounds
  // risky.
  // An alternative to use Option 2, picking the input with care such that the
  // product will never (even at an intermediate value in the worst-case
  // application of commutativity) require more than the available mantissa
  // digits (24 for IEEE float).
  // Another alternative is to test for equality within "epsilon".  However,
  // without help from a skilled numerical analyst (or a text by one), it is
  // not obvious how that would need to scale with the number of ranks.
  //
  // GEX_OP_{MIN,MAX}:
  // No issues are anticipated, as long as we do not include Nan, Inf or
  // negative-0 among the FP inputs.
  //
  // GEX_OP_{AND,OR}:
  // Care is needed to avoid inputs which "saturate" the output.  Otherwise the
  // values 0 and ~0 could be arrived at "accidentally" and still pass
  // validation.
  //
  // GEX_OP_XOR:
  // No issues are anticipated.
  //
}

int main(int argc, char **argv)
{
  gex_Client_t      myclient;
  gex_EP_t          myep;
  gex_TM_t          myteam;
  gex_Segment_t     mysegment;

  gex_Client_Init(&myclient, &myep, &myteam, "testreduce", &argc, &argv, 0);

  int arg = 1;
  if (argc > arg) { iters = atoi(argv[arg]); ++arg; }
  if (!iters) iters = 1000;

  if (argc > arg) { Nelem = atoi(argv[arg]); ++arg; }
  if (!Nelem) Nelem = NELEM_DEFAULT;

  unsigned int seed = 0;
  if (argc > arg) { seed = atoi(argv[arg]); ++arg; }

  test_init("testreduce",0,"(iters) (nelem) (seed)");

  if (seed == 0) {
    seed = (((unsigned int)TIME()) & 0xFFFF);
    gex_Event_Wait(gex_Coll_BroadcastNB(myteam, 0, &seed, &seed, sizeof(seed), 0));
  }
  TEST_SRAND(seed); // SAME seed

  MSG0("Running %i iterations of %i-element reduction tests (seed = %u).", iters, Nelem, seed);

  GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ_REQUEST));
  BARRIER();

  gex_TM_t subtm = GEX_TM_INVALID;
  int color = gex_TM_QueryRank(myteam) & 1; // odds & evens
  size_t scratch_sz = gex_TM_Split(&subtm, myteam, color, 0, 0, 0, GEX_FLAG_TM_SCRATCH_SIZE_RECOMMENDED);
  gex_TM_Split(&subtm, myteam, color, 0, (void*)TEST_MYSEG(), scratch_sz, 0);

  MSG0("Testing over all ranks:");
  do_tests(myteam);

  MSG0("Testing over odd/even subteams:");
  do_tests(subtm);

  BARRIER();
  MSG0("done.");
  gasnet_exit(0);

  /* Not reached in most implementations */
  return 0;
}
