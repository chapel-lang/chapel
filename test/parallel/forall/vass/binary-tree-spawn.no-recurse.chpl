
const topLocale = numLocales-1;

proc main {
  var result: int;

  forall aaa in AAA() with (+ reduce result) {
    result += aaa;
  }

  writef("%064bi\n", result);
}

inline proc ln return 2**here.id;

iter AAA() {
  halt("do not invoke me");
  yield 1;
}

iter AAA(param tag) where tag == iterKind.standalone {
  assert(here == Locales[0]);
  if topLocale >= 2 {
    cobegin {
      yield ln;
      on Locales[1] {
        if topLocale >= 6 {
          cobegin {
            yield ln;
            on Locales[3] {
              if topLocale >= 14 {
                cobegin {
                  on Locales[07] do yield ln;
                  on Locales[08] do yield ln;
                  yield ln;
                }
              } else {
                yield ln;
              }
            }
            on Locales[4] {
              if topLocale >= 14 {
                cobegin {
                  on Locales[09] do yield ln;
                  on Locales[10] do yield ln;
                  yield ln;
                }
              } else {
                yield ln;
              }
            }
          }
        } else {
          yield ln;
        }
      }
      on Locales[2] {
        if topLocale >= 6 {
          cobegin {
            yield ln;
            on Locales[5] {
              if topLocale >= 14 {
                cobegin {
                  on Locales[11] do yield ln;
                  on Locales[12] do yield ln;
                  yield ln;
                }
              } else {
                yield ln;
              }
            }
            on Locales[6] {
              if topLocale >= 14 {
                cobegin {
                  on Locales[13] do yield ln;
                  on Locales[14] do yield ln;
                  yield ln;
                }
              } else {
                yield ln;
              }
            }
          }
        } else {
          yield ln;
        }
      }
    }
  } else {
    yield ln;
  }
}
