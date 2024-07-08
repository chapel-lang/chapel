proc foo(n: int(?w)) {
	proc helper(a : [?D] int) {}
  helper([0,1]);
}

proc main() {
 foo(1);
}

