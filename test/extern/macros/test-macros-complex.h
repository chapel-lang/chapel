#ifndef TEST_MACROS_COMPLEX_H
#define TEST_MACROS_COMPLEX_H

static int macroLimit = 3;
static int* macroLimitPtr(void) { return &macroLimit; }
static int readMacroLimit(void) { return macroLimit; }
#define MACRO_LIMIT (*macroLimitPtr())

static int macroState = 0;
static int readMacroState(void) { return macroState; }
#define MACRO_RESET() do { macroState = 10; macroState += 2; } while (0)
#define MACRO_ACCUMULATE(p, x) do { *(p) += (x); macroState += (x); } while (0)
#define MACRO_DISCARD_SUM(x) ((x) + 1)
#define MACRO_CALLER_RETURN() do { return; } while (0)
#define MACRO_NOT_LVALUE (macroLimit + 1)
#define MACRO_LOCAL_LVALUE ((int){3})
#define MACRO_LOCAL_POINTER() ((int[]){1, 2})

typedef struct macro_result {
  int x;
  double y;
} macro_result;

static macro_result makeMacroResult(int x) {
  macro_result result = {x, 2.5};
  macroState = x;
  return result;
}

#define MACRO_RECORD_RESULT(x) makeMacroResult(x)

static int readMacroResultX(macro_result result) { return result.x; }

#endif
