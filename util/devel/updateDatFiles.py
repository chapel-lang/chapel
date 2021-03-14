#!/usr/bin/env python3

import csv
import re
import sys
import time

#
# Helper script for adding, removing, renaming, or moving keys and/or data for
# 1 or more .dat files. See the comments above "insert_key", "remove_key",
# "move_key", "rename_key", and "remove_data"
#
# Some example usage is in "test" function below and for info on updating
# nightly .dat files is in chapel-docs/bestPractices/updatingDatFiles.rst 
#

def main():
    files = sys.argv[1:]
    for f in files:
        with DatFile(f) as dat_file:

            # replace `pass` with whatever .dat file manipulation you need
            pass

# parse string as float if it's parse-able, returning 0.0 if it's not
def maybe_float(val):
    try:
        return float(val)
    except ValueError:
        return 0.0

# DatFile is an object to help manipulate (insert, remove, move, rename) keys
# and/or data from .dat files
class DatFile:
    def __init__ (self, dat_file):
        self.dat_file = dat_file
        self.data = None

    # open the file and read in the data stripping empty lines
    def __enter__(self):
        with open(self.dat_file, 'r') as f:
            self.data = list(csv.reader(f, delimiter='\t'))
            self.data = [x for x in self.data if x != []]
            # TODO check for malformed .dat files, possibly with
            # computePerfStats:verify_data_file()
            return self

    # write the data back out to the .dat file
    def __exit__(self, exception_type, exception_val, trace):
        with open(self.dat_file, 'w') as f:
            writer = csv.writer(f, delimiter='\t', lineterminator="\n")
            for row in self.data:
                writer.writerow(row)

    # returns true if the line starts with a comment (#) exclusing the header,
    # which starts with "# Date"
    def _is_comment_line(self, line):
        first_col = line[0].strip()
        if first_col.startswith('#'):
            if not first_col.startswith('# Date'):
                return True

        return False

    # return a list of the dates (not including the "#Date" key, but including
    # any comments  (e.g. "# 08/09/13    - ### EXECUTION TIMED OUT ###")
    def _get_dates(self):
        dates = []
        for row in self.data[1:]:
            dates.append(row[0].strip())
        return dates

    def _num_dates(self):
        return len(self._get_dates())

    # get the column number (index) of a key from its name
    def _get_col_num(self, key_name):
        key_name = key_name.strip()
        known_keys = self.data[0]

        if key_name in known_keys:
            return known_keys.index(key_name)
        else:
           raise ValueError('Key "{0}" not found for: {1}. Known keys are: '
                            '{2}'.format(key_name, self.dat_file, known_keys))

    # rename a key (by name) with a new name
    def rename_key(self, old_name, new_name):
        col = self._get_col_num(old_name)
        self.data[0][col] = new_name

    # remove a key and its data (by name). returns key/data removed
    def remove_key(self, key_name):
        col = self._get_col_num(key_name)
        old_data = []
        for row in self.data:
            if self._is_comment_line(row):
                old_data.append('')
            else:
                old_data.append(row.pop(col))
        return old_data

    # insert a key (by name) after another key (also by name). If no key_data
    # is supplied, adds a '-' for each date, otherwise the key_data is added
    def insert_key(self, key_name, after_key, key_data=None):
        after_key_col = self._get_col_num(after_key)
        new_col = []
        if key_data == None:
            key_data = ["-"] * self._num_dates()
        key_data.insert(0, key_name)
        for row, data in zip(self.data, key_data):
            if not self._is_comment_line(row):
                row.insert(after_key_col+1, data)

    # move a key (by name) after another key (also by name)
    def move_key(self, key_name, after_key):
        key_col = self.remove_key(key_name)
        self.insert_key(key_name, after_key, key_col[1:])

    # remove data for all keys in the range [start_date, end_date]. dates must
    # be of the form MM/DD/YY (same as the .dat file)
    def remove_data_in_range(self, start_date, end_date):

        # helper to convert a date to a stuct_time. optionally allows comments
        # to be returned
        def convert_date(date, comments=False):
            dat_date_format = '%m/%d/%y'
            date = date.strip()
            if comments and date.startswith("#"):
                return date
            return time.strptime(date, dat_date_format)

        start_time = convert_date(start_date)
        end_time = convert_date(end_date)
        dat_times = [convert_date(d, comments=True) for d in self._get_dates()]

        if start_time > end_time:
            raise ValueError('start_date: "{0}" is later than end_date: '
                             '"{1}"'.format(start_date, end_date))

        start_index = end_index = 0
        if start_time in dat_times and end_time in dat_times:
            start_index = dat_times.index(start_time)
            end_index = len(dat_times) - 1 - dat_times[::-1].index(end_time)
        else:
            raise ValueError('start_date: "{0}" or end_date: "{1}" not found '
                             'in .dat file "{2}"'.format(start_date,
                             end_date,self.dat_file))

        for i in reversed(range(start_index, end_index+1)):
            # `i+1` as _get_dates() doesn't include '#Date', but self.data does
            self.data.pop(i+1)

    # replace values that are over some threshold (default 1 day in seconds)
    # with the values from the previous day
    def replace_values_over_threshold(self, threshold=(24*60*60)):
        num_rows = len(self.data)
        num_cols = len(self.data[0])

        # Replace a bogus value with the previous day's value. Assumes row 1
        # has good data and is not a comment
        last_non_comment_row = 1
        for r in range(2, num_rows):
            if len(self.data[r]) != num_cols:
                continue # skip comment lines
            for c in range(1, num_cols):
                if maybe_float(self.data[r][c]) > threshold:
                    self.data[r][c] = self.data[last_non_comment_row][c]
            last_non_comment_row = r


# This exist just to test this from our testing system
def test(f):
    with DatFile(f) as dat_file:
        # Replace any bogus times
        dat_file.replace_values_over_threshold()

        # Remove the data for all keys in an inclusive range
        dat_file.remove_data_in_range("01/11/16", "01/12/16")

        # Insert a new key after the "total time" key and backfill all
        # existing dates with a "-"
        dat_file.insert_key("newkey", "total time")

        # Move LICM to after insert wide references
        dat_file.move_key("loopInvariantCodeMotion", "total time")

        # Add new key before "init" (first key listed)
        dat_file.insert_key("pre-init", "# Date")

        # Remove parse
        dat_file.remove_key("parse")

        # Rename LICM
        dat_file.rename_key("loopInvariantCodeMotion", "elliotsAwesomePass")


if __name__ == '__main__':
    main()
