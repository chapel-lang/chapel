#!/usr/bin/env python

"""Add license and copyright text to source files."""

from __future__ import print_function

import optparse
import os.path


class LicenseCommenter(object):
    __doc__ = __doc__

    raw_comment_text_file = os.path.abspath(os.path.join(
        os.path.dirname(__file__), 'license_text_for_comment.txt'))

    def __init__(self):
        super(LicenseCommenter, self).__init__()
        with open(self.raw_comment_text_file) as fp:
            raw_comment_text = fp.read()
        self.comment_text_lines = raw_comment_text.splitlines()

    def main(self):
        """Parse arguments, which should be a list of source files, for each file
        insert the copyright/license text, as comment, at the beginning.
        """
        source_files = self.parse_args()
        for source_file in source_files:
            self.add_comment(source_file)

    def add_comment(self, source_filename):
        """Add comment with copyright and source text to the given file.

        :type source_filename: str
        :arg source_filename: name of source file to prepend copyright/license comment
        """
        with open(source_filename, 'r') as fp:
            orig_cont = fp.read()

        comment_text = self.get_comment_text(source_filename)

        with open(source_filename, 'w') as fp:
            fp.write(comment_text)
            fp.write(orig_cont)

    def get_comment_text(self, source_filename):
        """Returns comment text with copyright/license for the given files based on its
        extension and/or name.

        :type source_filename: str
        :arg source_filename: name of source file to prepend copyright/license comment
        """
        if 'Makefile' in source_filename:
            # Add "# " to each line.
            return '\n'.join(map(lambda l: '# {0}'.format(l), self.comment_text_lines)) + '\n\n'

        c_style_comments = ['.c', '.chpl', '.cpp', '.h', '.lex', '.ypp', '.y']

        root, extension = os.path.splitext(source_filename)
        if extension in c_style_comments:
            # Use this form:
            #
            # /*
            #  * License line one
            #  * ...
            #  * last license line
            #  */
            license_lines = map(lambda l: ' * {0}'.format(l), self.comment_text_lines)
            comment_lines = ['/*'] + license_lines + [' */', '\n']
            return '\n'.join(comment_lines)
        else:
            raise ValueError('Cannot figure out comment style for: {0}'.format(source_filename))

    def parse_args(self):

        """Parse arguments, which should be list of source files to update."""
        parser = optparse.OptionParser('%prog [filename] ...')
        _, args = parser.parse_args()
        return args


if __name__ == '__main__':
    LicenseCommenter().main()
