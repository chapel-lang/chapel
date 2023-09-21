#
# Copyright 2020-2023 Hewlett Packard Enterprise Development LP
# Copyright 2004-2019 Cray Inc.
# Other additional copyright holders may be indicated within.
#
# The entirety of this work is licensed under the Apache License,
# Version 2.0 (the "License"); you may not use this file except
# in compliance with the License.
#
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

import argparse
import chapel
import chapel.core
from collections import defaultdict
import os
import sys

class ReplacementContext:
    def __init__(self, path):
        # Build the line number -> file position map
        with open(path, "r") as file:
            self.content = file.read()
        self.lines = {1: 0}
        self.lines_back = {}
        line = 1
        for (i, char) in enumerate(self.content):
            self.lines_back[i] = line
            if char == '\n':
                line += 1
                self.lines[line] = i+1 # the next characrer is the start of the next line

    def loc_to_idx(self, loc):
        (row, col) = loc
        return self.lines[row] + (col - 1)


    def node_idx_range(self, node):
        loc = node.location()

        range_start = self.loc_to_idx(loc.start())
        range_end = self.loc_to_idx(loc.end())
        return (range_start, range_end)

    def node_exact_string(self, node):
        (range_start, range_end) = self.node_idx_range(node)
        return self.content[range_start:range_end]

    def node_indent(self, node):
        (range_start, _) = self.node_idx_range(node)
        return range_start - self.lines[self.lines_back[range_start]]

def rename_formals(rc, fn, renames):
    for child in fn:
        if not isinstance(child, chapel.core.Formal): continue

        name = child.name()
        if name not in renames: continue

        child_text = rc.node_exact_string(child)
        yield (child, child_text.replace(name, renames[name]))

def rename_named_actuals(rc, call, renames):
    for actual in call.actuals():
        if isinstance(actual, tuple):
            (name, actual) = actual
            if name not in renames: continue

            actual_text = rc.node_exact_string(actual)

            # TODO: but there's no node that includes the name = in its location...
            # yield (actual, actual_text.replace(name, renames[name]))
            yield from []

def do_replace(finder, ctx, filename, suffix, inplace):
    asts = ctx.parse(filename)
    rc = ReplacementContext(filename)
    new_content = rc.content

    # First, store all the replacements in a map; then, walk the tree in a
    # reverse-postorder traversal (child nodes in reverse order, then parent)
    # and apply the transformations.

    nodes_to_replace = {}
    for ast in asts:
        for (node, replace_with) in finder(rc, ast):
            uid = node.unique_id()
            # Old result doesn't matter or doesn't exist; throw it out.
            if not callable(replace_with) or uid not in nodes_to_replace:
                nodes_to_replace[uid] = replace_with
            # replace_with is a callable, which means it transforms the previous
            # text. See if a transformation for this node has already been
            # requested.
            elif uid in nodes_to_replace:
                # Old substitution is also a callable; need to create composition.
                if callable(nodes_to_replace[uid]):
                    nodes_to_replace[uid] = lambda text: replace_with(nodes_to_replace[uid](text))
                # Old substitution is a string; we can apply the callable to get
                # another string.
                else:
                    nodes_to_replace[uid] = replace_with(nodes_to_replace[uid])

    def recurse(node):
        my_replace = None
        if node.unique_id() in nodes_to_replace:
            my_replace = nodes_to_replace[node.unique_id()]

        # If we don't have our own substitution, we can just propagate the
        # child substitutions.
        if my_replace is None:
            for child in reversed(list(node)):
                yield from recurse(child)
        # If it's not callable, it must be a string; we don't care about child
        # replacements, since our own target is constant.
        elif not callable(my_replace):
            (replace_from, replace_to) = rc.node_idx_range(node)
            yield (replace_from, replace_to, my_replace)

        # We have a callable replacement, which means we should apply child
        # substitutions to our text and then call the replacement with that.
        else:
            (replace_from, replace_to) = rc.node_idx_range(node)
            my_text = rc.node_exact_string(node)
            for child in reversed(list(node)):
                for (child_from, child_to, child_str) in recurse(child):
                    # Child is not inside this node, so it can be replaced as before
                    if child_from >= replace_to:
                        yield (child_from, child_to, child_str)

                    # Otherwise, child is inside, and we have to apply the patch
                    # to our own content before handing it to the transformer.
                    else:
                        child_from -= replace_from
                        child_to -= replace_from

                        my_text = my_text[:child_from] + child_str + my_text[child_to:]
            yield (replace_from, replace_to, my_replace(my_text))

    for ast in reversed(asts):
        for (replace_from, replace_to, replace_with) in recurse(ast):
            new_content = new_content[:replace_from] + replace_with + new_content[replace_to:]

    if inplace:
        store_into = filename
    else:
        store_into = filename + suffix
    with open(store_into, "w") as newfile:
        newfile.write(new_content)

def run(finder, name='replace', description='A tool to search-and-replace Chapel expressions with others'):
    parser = argparse.ArgumentParser(prog=name, description=description)
    parser.add_argument('filenames', nargs='*')
    parser.add_argument('--suffix', dest='suffix', action='store', default='.new')
    parser.add_argument('--in-place', dest='inplace', action='store_true', default=False)
    args = parser.parse_args()

    # Some files might have the same name, which Dyno really doesn't like.
    # Strateify files into "buckets"; within each bucket, all filenames are
    # unique. Between each bucket, re-create the Dyno context to avoid giving
    # it complicting files.

    basenames = defaultdict(lambda: 0)
    buckets = defaultdict(lambda: [])
    for filename in args.filenames:
        filename = os.path.realpath(os.path.expandvars(filename))

        basename = os.path.basename(filename)
        bucket = basenames[basename]
        basenames[basename] += 1
        buckets[bucket].append(filename)

    for bucket in buckets:
        ctx = chapel.core.Context()
        to_replace = buckets[bucket]

        for filename in to_replace:
            do_replace(finder, ctx, filename, args.suffix, args.inplace)

def fuse(*args):
    def fused(rc, root):
        for arg in args:
            yield from arg(rc, root)
    return fused
