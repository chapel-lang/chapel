
/* Header file for enum test script */
/* Enum without values */
enum 
{
  TEST_STATUS_PASSED,
  TEST_STATUS_FAILED
};


enum direction {
  NORTH,
  SOUTH,
  EAST,
  WEST
};


/* Enum with values */
enum 
{
  PASSED = 1,
  FAILED = 2
};

typedef enum 
{
  ENUM_ERROR = 1,
  ENUM_WVALUES_ERROR = 2,
  TD_ENUM_ERROR = 3,
  FUNC_ENUM_ERROR = 4,
  STRUCT_ENUM_ERROR = 5
}test_error;


/* Function with enum as argument */
int test_file(test_error e);


/* struct with enum as a field */
struct test_status { test_error current_status; };
