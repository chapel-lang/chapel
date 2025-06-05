use CTypes;

extern proc getenv(const name: c_ptrConst(c_char)): c_ptrConst(c_char);
extern proc printf(const fmt: c_ptrConst(c_char), const arg1: c_ptrConst(c_char));
for loc in Locales do on loc do printf('%s\n', getenv('GETENV_ENV_VAR'));
