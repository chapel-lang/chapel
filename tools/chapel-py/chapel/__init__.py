from . import core

def preorder(node):
    """
    Recursively visit the given AST node, going in pre-order (parent-then-children)
    """
    yield node
    for child in node:
        yield from preorder(child)

def postorder(node):
    """
    Recursively visit the given AST node, going in post-order (children-then-parent)
    """
    for child in node:
        yield from preorder(child)
    yield node

def parse_attribute(attr, attribute):
    """
    Given an Attribute AST node, and a description of the attribute in
    the form (name, formal_names), return a mapping of formal names to their
    values in the attribute call. Raise errors if the call is malformed in
    some way.
    """

    (name, formals) = attribute

    if attr.name() != name: return None

    parse_result = {}
    def save_kw(actual_tuple):
        (name, value) = actual_tuple
        if name not in formals:
            raise Exception("Invalid named argument for attribute!")
        elif name in parse_result:
            raise Exception("Duplicate occurrence of named argument!")
        else:
            parse_result[name] = value

    # First, process arguments in order
    actuals = attr.actuals()
    for (actual, formal) in zip(actuals, formals):
        # If we found the first named argument, save it and stop
        # processing positional arguments.
        if isinstance(actual, tuple):
            save_kw(actual)
            break

        # Otherwise, it's a positional argument, save it under the current formal.
        parse_result[formal] = actual

    # Finish up with the remaining kwargs, if any.
    for actual in actuals:
        if not isinstance(actual, tuple): raise Exception("Mixing named and positional arguments!")
        save_kw(actual)

    for formal in formals:
        if formal not in parse_result:
            parse_result[formal] = None

    return parse_result

rest = "rest"

def match_pattern(ast, pattern):
    """
    Matches the given AST against the given pattern. Patterns are specified
    as AST node classes, strings, or lists thereof. Here's a summary:

    A single node with any number of children:

      chapel.Nodetype

    Any node, saved in a variable:

      "?x"

    Any node at all, not saved in a variable:

      ""

    A single node with any number of children, saved in a variable:

      ("?x", chapel.Nodetype)

    A single node with no children at all:

      [chapel.Nodetype]

    A single node with no children, saved in a variable:

      ["?x", chapel.Nodetype]

    A single node with two children, expressed as patterns P1 and P2:

      [chapel.Nodetype, P1, P2]

    A single node with at least two children:

      [chapel.Nodetype, P1, P2, rest]

    An addition node in which both arguments are equal:

      [chapel.OpCall, "?x", "x"]

    """

    variables = {}
    counts = {}

    def fresh(metavar):
        new_var = metavar
        next_index = 0
        if metavar in counts:
            new_var = metavar + str(counts[metavar])
            next_index = counts[metavar]+1
        counts[metavar] = next_index
        return new_var

    def current(metavar):
        if metavar not in counts:
            # Varaible was never created; it's an error.
            raise Exception("Invalid variable in pattern")

        count = counts[metavar]
        return metavar if count == 0 else metavar + str(count-1)

    def check_var(ast, pat):
        # Empty pattern is wildcard
        if len(pat) == 0: return True

        if pat[0] == '?':
            # Fresh variable, guaranteed to match.
            variables[fresh(pat[1:])] = ast
            return True
        else:
            # Re-used variable; need to compare for equality.
            variable = current(pat)
            if variable not in variables:
                # Referring to a not-yet-bound variable.
                return False

            # Can't compare ASTs yet
            print("Equality constrait:", variables[variable], ast)
            return True

    def match_inner(ast, pat):
        if isinstance(pat, str): return check_var(ast, pat)
        elif isinstance(pat, tuple):
            (pat_name, node_type) = pat

            if not isinstance(ast, node_type): return False
            if not check_var(ast, pat_name): return False

            return True
        elif isinstance(pat, list):
            idx = 0
            node_type = pat[idx]
            pat_name = None
            if isinstance(node_type, str):
                pat_name = node_type
                idx = 1
                node_type = pat[idx]

            if not isinstance(ast, node_type): return False
            if pat_name is not None and not check_var(ast, pat_name): return False

            idx += 1
            children = list(ast)
            for child in children:
                # Too many children, pattern didn't expct
                if idx == len(pat): return False

                child_pat = pat[idx]
                if child_pat == "rest":
                    # Special case rest pattern; subsequent children allowed.
                    break
                if not match_inner(child, child_pat): return False
                idx += 1
            else:
                # Did not encounter a rest pattern.

                # If we didn't make it through all the expect patterns, we
                # ran out of children.
                if idx < len(pat) and pat[idx] != "rest": return False

            return True
        elif issubclass(pat, core.AstNode):
            # Just check if the AST node matches
            return isinstance(ast, pat)
        else:
            raise Exception("Invalid pattern!")

    return variables if match_inner(ast, pattern) else None
