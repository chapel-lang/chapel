/**
 * Unittest PPC64 is_plt_entry function by inspecting output at
 * different points in a mock PLT address space.
 */
/*
 * This file is part of libunwind.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "dwarf.h"
#include "libunwind_i.h"

enum
{
  ip_guard0,
  ip_std,
  ip_ld,
  ip_mtctr,
  ip_bctr,
  ip_guard1,

  ip_program_end
};

/* Mock access_mem implementation */
static int
access_mem (unw_addr_space_t as, unw_word_t addr, unw_word_t *val, int write,
            void *arg)
{
  if (write != 0)
    return -1;

  const size_t mem_size   = ip_program_end * sizeof(uint32_t);
  const void *mem_start   = arg;
  const void *mem_end     = (const char*) arg + mem_size;
  const unw_word_t *paddr = (const unw_word_t*) addr;

  if ((void*) paddr < mem_start || (void*) paddr > mem_end)
    {
      return -1;
    }

  *val = *paddr;
  return 0;
}

int
main ()
{
  if (target_is_big_endian())
    return 77;

  const uint32_t plt_instructions[ip_program_end] =
    {
      0xdeadbeef,
      0xf8410018, // std     r2,24(r1)
      0xe9828730, // ld      r12,-30928(r2)
      0x7d8903a6, // mtctr   r12
      0x4e800420, // bctr
      0xdeadbeef,
    };
  uint32_t test_instructions[ip_program_end];
  memcpy(test_instructions, plt_instructions, sizeof(test_instructions));

  struct unw_addr_space mock_address_space;
  mock_address_space.big_endian = 0;
  mock_address_space.acc.access_mem = &access_mem;

  struct cursor cursor;
  struct dwarf_cursor *dwarf = &cursor.dwarf;
  struct unw_cursor *c = (struct unw_cursor *)(&cursor);
  dwarf->as = &mock_address_space;
  dwarf->as_arg = &test_instructions;

  /* ip at std r2,24(r1) */
  dwarf->ip = (unw_word_t) (test_instructions + ip_std);
  if (!unw_is_plt_entry(c)) return -1;

  /* ld uses a different offset */
  test_instructions[ip_ld] = 0xe9820000;
  if (!unw_is_plt_entry(c)) return -1;
  memcpy(test_instructions, plt_instructions, sizeof(test_instructions));

  /* ip_ld is not a ld instruction */
  test_instructions[ip_ld] = 0xf154f00d;
  if (unw_is_plt_entry(c)) return -1;
  memcpy(test_instructions, plt_instructions, sizeof(test_instructions));

  /* ip_mtctr is not a mtctr instruction */
  test_instructions[ip_mtctr] = 0xf154f00d;
  if (unw_is_plt_entry(c)) return -1;
  memcpy(test_instructions, plt_instructions, sizeof(test_instructions));

  /* ip_bctr is not a bctr instruction */
  test_instructions[ip_bctr] = 0xf154f00d;
  if (unw_is_plt_entry(c)) return -1;
  memcpy(test_instructions, plt_instructions, sizeof(test_instructions));

  /* ip at ld r12,-30928(r2) */
  dwarf->ip = (unw_word_t) (test_instructions + ip_ld);
  if (!unw_is_plt_entry(c)) return -1;

  /* ip_std is not a std instruction */
  test_instructions[ip_std] = 0xf154f00d;
  if (unw_is_plt_entry(c)) return -1;
  memcpy(test_instructions, plt_instructions, sizeof(test_instructions));

  /* ip_mtctr is not a mtctr instruction */
  test_instructions[ip_mtctr] = 0xf154f00d;
  if (unw_is_plt_entry(c)) return -1;
  memcpy(test_instructions, plt_instructions, sizeof(test_instructions));

  /* ip_bctr is not a bctr instruction */
  test_instructions[ip_bctr] = 0xf154f00d;
  if (unw_is_plt_entry(c)) return -1;
  memcpy(test_instructions, plt_instructions, sizeof(test_instructions));

  /* ip at mtctr r12 */
  dwarf->ip = (unw_word_t) (test_instructions + ip_mtctr);
  if (!unw_is_plt_entry(c)) return -1;

  /* ip_std is not a std instruction */
  test_instructions[ip_std] = 0xf154f00d;
  if (unw_is_plt_entry(c)) return -1;
  memcpy(test_instructions, plt_instructions, sizeof(test_instructions));

  /* ip_ld is not a ld instruction */
  test_instructions[ip_ld] = 0xf154f00d;
  if (unw_is_plt_entry(c)) return -1;
  memcpy(test_instructions, plt_instructions, sizeof(test_instructions));

  /* ip_bctr is not a bctr instruction */
  test_instructions[ip_bctr] = 0xf154f00d;
  if (unw_is_plt_entry(c)) return -1;
  memcpy(test_instructions, plt_instructions, sizeof(test_instructions));

  /* ip at bctr */
  dwarf->ip = (unw_word_t) (test_instructions + ip_bctr);
  if (!unw_is_plt_entry(c)) return -1;

  /* ip_std is not a std instruction */
  test_instructions[ip_std] = 0xf154f00d;
  if (unw_is_plt_entry(c)) return -1;
  memcpy(test_instructions, plt_instructions, sizeof(test_instructions));

  /* ip_ld is not a ld instruction */
  test_instructions[ip_ld] = 0xf154f00d;
  if (unw_is_plt_entry(c)) return -1;
  memcpy(test_instructions, plt_instructions, sizeof(test_instructions));

  /* ip_mtctr is not a mtctr instruction */
  test_instructions[ip_mtctr] = 0xf154f00d;
  if (unw_is_plt_entry(c)) return -1;
  memcpy(test_instructions, plt_instructions, sizeof(test_instructions));

  /* ip at non-PLT instruction */
  dwarf->ip = (unw_word_t) (test_instructions + ip_guard0);
  if (unw_is_plt_entry(c)) return -1;

  /* ip at another non-PLT instruction */
  dwarf->ip = (unw_word_t) (test_instructions + ip_guard1);
  if (unw_is_plt_entry(c)) return -1;

  return 0;
}
