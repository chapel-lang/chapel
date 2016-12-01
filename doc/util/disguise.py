from docutils import nodes

def disguise_role(typ, rawtext, text, lineno, inliner, options={}, content=[]):
    """
    Role to obfuscate e-mail addresses using DISGUISE comments.
    """

    obfuscated = '<!-- DISGUISE -->'.join(list(text))

    obfuscated = '<b>' + obfuscated
    obfuscated = obfuscated + '</b>'

    node = nodes.raw('', obfuscated, format='html')
    return [node], []

def setup(app):
    app.add_role('disguise', disguise_role)
