---
name: Documentation
about: Report any issue observed within the Chapel documentation or the `chpldoc` tool.
title: '[Documentation]: '
labels: 'area: Docs'
---

### Summary of Problem

**Description:**
<!--
What did you observe when encountering this issue?
What did you expect to observe?
-->

### Steps to Reproduce

**Source Code:**

```chapel
// Please provide source code that will reproduce the problem if it seems to be
// with chpldoc itself. You can insert your code inline if it's not too long.
// Otherwise, you can attach it as a file or provide a URL to it.
// To the extent possible, providing simplified programs demonstrating the
// problem will be appreciated.
// You can also replace this section with "Associated Future Test(s)" below.
```

**Compile command:**
<!-- e.g. `chpldoc foo.chpl` -->

**Additional steps:**
<!-- Are there any additional steps needed to demonstrate the issue beyond calling `chpldoc`? -->

**Associated Future Test(s):**
<!--
Are there any tests in Chapel's test system that demonstrate this issue?
e.g. [`test/path/to/foo.chpl`](
      https://github.com/chapel-lang/chapel/blob/main/test/path/to/foo.chpl
      ) #1234
-->

### Configuration Information

- Output of `chpldoc --version`:
- Output of `$CHPL_HOME/util/printchplenv --anonymize`:
  <!--
  When `$CHPL_HOME` is not set, you can use `chpl --print-chpl-settings`
  instead, though be aware that this does not anonymize the output
  -->
- Back-end compiler and version, e.g. `gcc --version` or `clang --version`:
- (For Cray systems only) Output of `module list`:
