extern proc getenv(const name: c_string): c_string;
writeln(string.createCopyingBuffer(getenv('DASH_E_ENV_VAR')));
