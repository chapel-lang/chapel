//  #include "stdchplrt.h"
#include "qio.h"
#include "qio_regexp.h"
#include <assert.h>
#include <stdio.h>

#include "re2/re2.h"

struct minmax_test {
  const char* pat;
  int min;
  int max;
};

void check_minmax_match(void)
{
  struct minmax_test tests[] = {
    {"a", 1, 1},
    {"(a)", 1, 1},
    {"(?:a)", 1, 1},
    {"ab", 2, 2},
    {"a?", 0, 1},
    {"(a)?", 0, 1},
    {"a+", 1, -1},
    {"a*", 0, -1},
    {"a{2,3}", 2, 3},
    {"a{2,}", 2, -1},
    {"a{2}", 2, 2},
    {"abc|d", 1, 3},
    {".*", 0, -1},
    {".+", 1, -1},
    {"wuwu", 4, 4},
    {"wuwu.*", 4, -1},
    {"wuwu.*(wuwu)?", 4, -1},
    {"(wuwu)?", 0, 4},
    {"(wuwu)*", 0, -1},
    {"(wuwu)+", 4, -1},
    // Lengths are returned in bytes,
    // and some unicode characters are multiple bytes...
    {"\xe2\x82\xac", 3, 3}, // Euro sign
    {"\xe2\x82\xac{2}", 6, 6}, // Euro sign{2}
  };
  int i;
  int ntests = sizeof(tests)/sizeof(struct minmax_test);
  for(i = 0; i < ntests; i++ ) {
    RE2 re(tests[i].pat);
    assert(re.min_match_length_bytes() == tests[i].min);
    assert(re.max_match_length_bytes() == tests[i].max);
  }
}


int main(int argc, char** argv)
{
  // Check that RE2 is functioning at all.
  assert(RE2::FullMatch("a", "aa*$"));
  assert(RE2::FullMatch("hello", "h.*o"));
  assert(RE2::FullMatch("", "$"));

  check_minmax_match();

  return 0;
}

