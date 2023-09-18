import re
import sys
import os

TEST_LINE_PREFIX = '[test: '
TEST_LINE_CLOSING = ']'
CHPL_REGEX_PATTERN = r'.{0,3}\$CHPL_HOME/'
DIR_ABS_PREFIX = '$CHPL_HOME/test/'
CHPL_EXT = '.chpl'
DIFF_INDICATOR = '> '

def extractLineNumbers(file_name, warning_message, output_file_name):
  # Read in the text file
  with open(file_name, 'r') as f:
    lines = f.readlines()
  

  # Extract lines with a specific characteristic
  warning_lines = []
  current_sub_test_path = None

  for line in lines:
    # If the line begins with "[test: " then we're running a new subtest"
    if line.startswith(TEST_LINE_PREFIX):
      # Extract the current_sub_test_path from the line 
      # excluding the filename at the end of the path
      # Ex: [test: attributes/badAttribute.chpl] -> path = attributes/
      current_sub_test_path = line.split(TEST_LINE_CLOSING)[0].split(TEST_LINE_PREFIX)[1].rsplit('/', 1)[0] + '/'
      continue

    if line.startswith("< "):
      # Ignore matches in .good files
      continue

    if warning_message in line:
      warning_line = resolveToAbsPath(current_sub_test_path, line)
      if warning_line is not None:
        warning_lines.append(warning_line)

  # Remove duplicates efficiently
  warning_set = set(warning_lines)

  # Write them to a file
  with open(output_file_name, 'w') as f:
    for line in sorted(warning_set, key=lineKey):
      f.write(line)
  print("Total Unique Matches Found:", len(warning_set))
  return

def lineKey(line):
  split = line.split(":", 2)
  try: return split[0], int(split[1])
  except ValueError: return split[0], 0

def resolveToAbsPath(current_sub_test_path, line):

  # If the error comes from an internal chapel module it will be an absolute path already
  # The path will be something like '$CHPL_HOME/modules/internal/' 
  # We do a lose check to see if the beginning of the line approximately matches such a path
  # we use regex for an approximate match
  # Create the regex with room for 3 characters before matching starts
  # This is because the line will start with '> $CHPL_HOME'
  # We don't want to match the first 0 to 3 characters of the path
  # Ex: '> $CHPL_HOME/modules/internal/' -> '$CHPL_HOME/modules/internal/'
  if re.match(CHPL_REGEX_PATTERN, line):
    # If the line matches the regex then we return the after stripping the leading '> ' before the '$' if its there
    return '$'+line.split('$', 1)[-1]

  # Else we append the current_sub_test_path to the line along with '$CHPL_HOME/test/'
  else:
    # First we check if this is a garbage match that we actually need to discard
    # The line should have just one filename in it in the beginning
    # Ex: 'badAttribute.chpl:3: warning: range.boundsCheck() is deprecated, please use range.contains() instead'
    # We split the line by ':' and take the first element
    # If it does not end with '.chpl' then we discard the line
    
    test_name = line.split(':')[0]
    if not test_name.endswith(CHPL_EXT):
      return None

    result = DIR_ABS_PREFIX + current_sub_test_path + line.split(DIFF_INDICATOR, 1)[-1]

    # replace "XX/somedir/../YY" with "XX/YY"
    subs = 1
    while subs > 0: result, subs = re.subn(r'/[^/]+/\.\./', '/', result)

    return result


def main():
  # Print usage if the user does not provide the correct number of arguments
  if len(sys.argv) < 3:
    print("Usage: python3 extractLineNumbers.py <log_file> <search_string> [output_file_name]")
    print('output_file_name is "warning_lines" by default')
    exit(1)
  file_name = sys.argv[1]
  warning_message = sys.argv[2]
  
  # If the user provides an output file name then we write the warning lines to that file
  # Else we write to a default file name
  if len(sys.argv) == 4:
    output_file_name = sys.argv[3]
  else:
    output_file_name = 'warning_lines'

  # Check if the user provided file exists
  # If it doesn't then we exit
  if not os.path.isfile(file_name):
    print("File ", file_name," does not exist")
    exit(1)

  extractLineNumbers(file_name, warning_message, output_file_name)


if __name__ == '__main__':
  main()
