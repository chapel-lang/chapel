import argparse
import chapel
import chapel.core
from bisect import bisect_left

parser = argparse.ArgumentParser( prog='replace', description='A tool to search-and-replace Chapel expressions with others')
parser.add_argument('filename')
args = parser.parse_args()

# Build the line number -> file position map
with open(args.filename, "r") as file:
    content = file.read()
lines = {1: 0}
lines_back = {}
line = 1
for (i, char) in enumerate(content):
    lines_back[i] = line
    if char == '\n':
        line += 1
        lines[line] = i+1 # the next characrer is the start of the next line

def get_file_idx(loc):
    (row, col) = loc
    return lines[row] + (col - 1)

def get_node_range(node):
    loc = node.location()

    replace_from = get_file_idx(loc.start())
    replace_to = get_file_idx(loc.end())
    return (replace_from, replace_to)

def get_node_exact_string(node):
    (replace_from, replace_to) = get_node_range(node)
    return content[replace_from:replace_to]

def get_node_indent(node):
    (replace_from, _) = get_node_range(node)
    return replace_from - lines[lines_back[replace_from]]

def rename_formals(fn, renames):
    for child in fn:
        if not isinstance(child, chapel.core.Formal): continue

        name = child.name()
        if name not in renames: continue

        child_text = get_node_exact_string(child)
        yield (child, child_text.replace(name, renames[name]))

def rename_named_actuals(call, renames):
    for actual in call.actuals():
        if isinstance(actual, tuple):
            (name, actual) = actual
            if name not in renames: continue

            actual_text = get_node_exact_string(actual)

            # TODO: but there's no node that includes the name = in its location...
            # yield (actual, actual_text.replace(name, renames[name]))
            yield from []

def do_replace(finder):
    ctx = chapel.core.Context()
    asts = ctx.parse(args.filename)
    new_content = content

    # First, store all the replacements in a map; then, walk the tree in a
    # reverse-postorder traversal (child nodes in reverse order, then parent)
    # and apply the transformations.

    nodes_to_replace = {}
    for ast in asts:
        for (node, replace_with) in finder(ast):
            nodes_to_replace[node.unique_id()] = replace_with

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
            (replace_from, replace_to) = get_node_range(node)
            yield (replace_from, replace_to, my_replace)

        # We have a callable replacement, which means we should apply child
        # substitutions to our text and then call the replacement with that.
        else:
            (replace_from, replace_to) = get_node_range(node)
            my_text = get_node_exact_string(node)
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

    return new_content

# -------------------------
# Application-Specific Code
# -------------------------

def find_this_complete(root):
    # pattern for x._()
    methodcall = [chapel.core.FnCall, ["?dot",  chapel.core.Dot, ["?ident", chapel.core.Identifier]], chapel.rest]

    for (node, variables) in chapel.each_matching(root, methodcall):
        if variables["dot"].field() == "complete" and variables["ident"].name() == "this":
            yield (node, 'init this')

def rename_x_y_to_a_b(root):
    for (fn, _) in chapel.each_matching(root, chapel.core.Function):
        if fn.name() != "changeMe": continue

        yield from rename_formals(fn, { "x": "a", "y": "b" })

    # pattern for x(...)
    fncall = [chapel.core.FnCall, ["?ident", chapel.core.Identifier], chapel.rest]
    for (call, variables) in chapel.each_matching(root, fncall):
        if variables["ident"].name() != "changeMe": continue

        yield from rename_named_actuals(call, { "x": "a", "y": "b" })

def tag_all_nodes_assert_on_gpu(root):
    # pattern for x()
    noargcall = [chapel.core.FnCall, ["?ident", chapel.core.Identifier]]

    for (foreach, _) in chapel.each_matching(root, chapel.core.Foreach):
        has_assert_on_gpu = False

        loop_body = list(foreach)[-1];
        for child in loop_body:
            variables = chapel.match_pattern(child, noargcall)
            if variables is not None and variables["ident"].name() == "assertOnGpu":
                has_assert_on_gpu = True
                yield (child, '')

        indent = get_node_indent(foreach)
        if has_assert_on_gpu:
            yield (foreach, lambda text, i = indent: "@assertOnGpu\n" + (" " * i) + text)

def all_transforms(root):
    yield from find_this_complete(root)
    yield from rename_x_y_to_a_b(root)
    yield from tag_all_nodes_assert_on_gpu(root)

print(do_replace(all_transforms));
