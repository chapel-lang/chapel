// Generated with c2chapel version 0.1.0

// Header given to c2chapel:
require "enum.h";

// Note: Generated with fake std headers

extern const TEST_STATUS_PASSED :c_int;
extern const TEST_STATUS_FAILED :c_int;
extern const PASSED :c_int;
extern const FAILED :c_int;
extern proc test_file(e : test_error) : c_int;

extern record test_status {
  var current_status : test_error;
}

// ==== c2chapel typedefs ====

// typedef enum
extern type test_error = c_int;
