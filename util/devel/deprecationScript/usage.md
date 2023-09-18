# Usage for the deprecation scripts utility

This is a simple 2 part script that can be used to quickly replace deprecated
functions across multiple files in the chapel repository.


## extractLineNumbers.py

The first part of the script is called `extractLineNumbers.py`. It uses the
log from a `paratest`/`start_test` run (`log_file`) and scans it for a user specified
string (`search_string`), typically a deprecation warning.

```sh
python3 extractLineNumbers.py <log_file> <search_string> [output_file_name]
```

Since the paratest log gives us the file and line number for each error or
warning generated, we get the file and line number for where the deprecated
function lies based on the deprecation warning we searched for (doing its best
to have absolute paths for each file). It extracts these lines into a separate
file whose name can be customized using the optional `output_file_name` argument.
By default the output file is called `warning_lines`.

The script drops the matches in those lines that start with `<`.
Such lines typically come from `.good` files and so are irrelevant to the task.


## replaceDeprecatedLines.py

The second script then takes two strings and this file as input as follows:

```sh
python3 replaceDeprecatedLines.py <from_string> <to_string> <lines_file> [files_to_update...]
```
It reads the `lines_file` which is produced by the first script.
And then for each of the files and corresponding line numbers listed in the file,
it replaces all instances of `from_string` with `to_string`.

The `files_to_update` is an optional argument that runs the replacement only on the
specified files. Otherwise the script operates on all files in the `lines_file` file.


### deprecateFunctions.bash

The recommended way is to use the above two python scripts one by one manually,
but you can also use the `deprecateFunctions.bash` script to run them together or
to get an idea of how an example run would look like.


## Limitations

It is important to note the limitations of these two scripts combined.

1. Multiple occurences of a word on a line.
If a line has multiple instances of a function on a single line that has to be replaced, all instances will be replaced
This may not be the correct behavior in some cases.

2. Functions spanning across multiple lines
Since a function call may span multiple lines, the chapel compiler will report the error/warning to be on the first line.
But the actual offending call may be on the subsequent lines which requires manual replacement.

3. Aggressive matching
Running the script multiple times without well defined word boundaries may cause erronious replacements.
Be mindful of what words you pass into the script as replacements
