// RUN: %clang_cc1 %s -verify -pedantic -fsyntax-only

constant sampler_t glb_smp = 5;

void foo(sampler_t); 

void kernel ker(sampler_t argsmp) {
  local sampler_t smp; // expected-error {{sampler type cannot be used with the __local and __global address space qualifiers}}
  const sampler_t const_smp = 7;
  foo(glb_smp);
  foo(const_smp);
  foo(5); // expected-error {{sampler_t variable required - got 'int'}}
}
