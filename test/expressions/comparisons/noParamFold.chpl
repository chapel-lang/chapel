config var x: uint;

if x < 0 {
  compilerWarning("This will never happen, but should still be resolved");
} else {
  compilerWarning("This will always happen and be resolved");
}

if x < -1 {
  compilerWarning("This will never happen, but should still be resolved");
} else {
  compilerWarning("This will always happen and be resolved");
}

if 0 > x {
  compilerWarning("This will never happen, but should still be resolved");
} else {
  compilerWarning("This will always happen and be resolved");
}

if -2 > x {
  compilerWarning("This will never happen, but should still be resolved");
} else {
  compilerWarning("This will always happen and be resolved");
}


if x >= 0 {
  compilerWarning("This will always happen and be resolved");
} else {
  compilerWarning("This will never happen, but should still be resolved");
}

if 0 <= x {
  compilerWarning("This will always happen and be resolved");
} else {
  compilerWarning("This will never happen, but should still be resolved");
}
