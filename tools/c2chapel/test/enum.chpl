// Generated with c2chapel version 0.1.0

// Header given to c2chapel:
require "enum.h";

// Note: Generated with fake std headers

use CPtr;
// Enum: anonymous
extern const TEST_STATUS_PASSED :c_int;
extern const TEST_STATUS_FAILED :c_int;


// Enum: direction
extern const NORTH :c_int;
extern const SOUTH :c_int;
extern const EAST :c_int;
extern const WEST :c_int;


// Enum: anonymous
extern const PASSED :c_int;
extern const FAILED :c_int;


extern proc test_file(e : test_error) : c_int;

extern record test_status {
  var current_status : test_error;
}

// ==== c2chapel typedefs ====

// test_error enum
extern type test_error = c_int;
extern const ENUM_ERROR :test_error;
extern const ENUM_WVALUES_ERROR :test_error;
extern const TD_ENUM_ERROR :test_error;
extern const FUNC_ENUM_ERROR :test_error;
extern const STRUCT_ENUM_ERROR :test_error;


