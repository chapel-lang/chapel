proc foo(const ref x) ref {
  return x;
}

const answer = 42;
ref alias = foo(answer);
alias += 1;
writeln(alias);
writeln(answer);
