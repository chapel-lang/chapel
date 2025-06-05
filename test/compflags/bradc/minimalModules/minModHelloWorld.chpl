extern proc printf(f);
extern {
  const char* formatStr = "Hello, Minimal World!\n";
}

printf(formatStr);
