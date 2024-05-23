---
name: Bug Report
about: Report any bug observed within your code.
title: '[Bug]: '
labels: 'type: Bug'
---

### Summary of Problem

**Description:**
<!--
What behavior did you observe when encountering this issue?
What behavior did you expect to observe?
Describe the workarounds you've tried, if any.
-->

**Is this issue currently blocking your progress?**
<!-- Answer 'yes', 'no' or feel free to provide additional detail. -->

### Steps to Reproduce

**Source Code:**

```chapel
// Please provide source code that will reproduce the problem.
// You can insert your code inline if it's not too long.
// Otherwise, you can attach it as a file or provide a URL to it.
// To the extent possible, providing simplified programs demonstrating the
// problem will be appreciated.
// You can also replace this section with "Associated Future Test(s)" below.
```

**Compile command:**
<!-- e.g. `chpl foo.chpl` -->

**Execution command:**
<!--
e.g. `./foo -nl 4`
If an input file is required, include it as well.
-->

**Associated Future Test(s):**
<!--
Are there any tests in Chapel's test system that demonstrate this issue?
e.g. [`test/path/to/foo.chpl`](
      https://github.com/chapel-lang/chapel/blob/main/test/path/to/foo.chpl
      ) #1234
-->

### Configuration Information

- Output of `chpl --version`:
- Output of `$CHPL_HOME/util/printchplenv --anonymize`:
  <!--
  When `$CHPL_HOME` is not set, you can use `chpl --print-chpl-settings`
  instead, though be aware that this does not anonymize the output
  -->
- Back-end compiler and version, e.g. `gcc --version` or `clang --version`:
- (For Cray systems only) Output of `module list`:
