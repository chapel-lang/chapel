#include <iostream>

#include "udf2.h"

int main(int argc, char **argv) {
  chpl_library_init(argc, argv);
  chpl__init_Foo(0, 0);

  int32_t x[] = {5, 1, 4}; // these should be allocated on the GPU
  int32_t y[] = {5, 7, 8}; // but that's irrelevant for the issue here
  int32_t result[3];

  chpl_external_array col_result{result, 3, nullptr};
  chpl_external_array col_a{x, 3, nullptr};
  chpl_external_array col_b{y, 3, nullptr};

  add_int32(&col_result, &col_a, &col_b);

  for (int i = 0; i < 3; i++) {
    std::cout << "result[" << i << "] = " << result[i] << std::endl;
  }

  chpl_library_finalize();
  return 0;
}
