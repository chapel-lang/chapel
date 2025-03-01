extern char** environ;
char** getEnvironment(void);
char** getEnvironment(void) {
  return environ;
}
