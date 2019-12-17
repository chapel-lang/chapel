# -*- coding: utf-8 -*-

"""
    sphinxcontrib.chapeldomain
    ~~~~~~~~~~~~~~~~~~~~~~~~~~

    The Chapel language domain.

    :copyright: Copyright 2015 by Chapel Team
    :license: Apache v2.0, see LICENSE for details.

    Chapel website: http://chapel.cray.com/
    Chapel spec:    http://chapel.cray.com/language.html

"""

import re

from docutils import nodes
from docutils.parsers.rst import directives
from six import iteritems

from sphinx import addnodes
from sphinx.directives import ObjectDescription
from sphinx.domains import Domain, Index, ObjType
from sphinx.locale import l_, _
from sphinx.roles import XRefRole
from docutils.parsers.rst import Directive
from sphinx.util.docfields import Field, GroupedField, TypedField
from sphinx.util.nodes import make_refnode


VERSION = '0.0.15'


# regex for parsing proc, iter, class, record, etc.
chpl_sig_pattern = re.compile(
    r"""^ ((?:\w+\s+)*                        # optional: prefixes
           (?:proc|iter|class|record)\s+      #   must end with keyword
           (?:type\s+|param\s+)?              # optional: type or param method
          )?
          ([\w$.]*\.)?                        # class name(s)
          ([\w\+\-/\*$\<\=\>\!]+)  \s*        # function or method name
          (?:\((.*?)\))?                      # optional: arguments
          (\s+(?:const\s)? \w+|               #   or return intent
           \s* : \s* [^:]+|                   #   or return type
           \s+(?:const\s)? \w+\s* : \s* [^:]+ #   or return intent and type
          )?
          $""", re.VERBOSE)

# regex for parsing attribute and data directives.
chpl_attr_sig_pattern = re.compile(
    r"""^ ((?:\w+\s+)*)?          # optional: prefixes
          ([\w$.]*\.)?            # class name(s)
          ([\w$]+)                # const, var, param, etc name
          (\s* [:={] \s* .+)?     # optional: type, default value
          $""", re.VERBOSE)


# This would be the ideal way to create a chapelerific desc_returns similar to
# addnodes.desc_returns. However, due to some update issue, the
# nodes._add_node_class_names() call does not seem to make chapel_desc_returns
# to the sphinx html write. So, we'll just use addnodes.desc_returns
# directly. :-\ (thomasvandoren, 2015-02-19)
# class chapel_desc_returns(addnodes.desc_type):
#     """Node for a "returns" annotation."""
# nodes._add_node_class_names([chapel_desc_returns.__name__])

class ChapelTypedField(TypedField):
    """Override TypedField in order to change output format."""

    def make_field(self, types, domain, items, env=None):
        """Copy+Paste of TypedField.make_field() from Sphinx version 1.2.3. The first
        and second nodes.Text() instance are changed in this implementation to
        be ' : ' and '' respectively (instead of ' (' and ')').

        TODO: Ask sphinx devs if there is a better way to support
              this that is less copy+pasty. (thomasvandoren, 2015-03-17)
        """
        def handle_item(fieldarg, content):
            par = nodes.paragraph()
            par += self.make_xref(
                self.rolename, domain, fieldarg, nodes.strong)
            if fieldarg in types:
                par += nodes.Text(' : ')
                # NOTE: using .pop() here to prevent a single type node to be
                # inserted twice into the doctree, which leads to
                # inconsistencies later when references are resolved
                fieldtype = types.pop(fieldarg)
                if (len(fieldtype) == 1 and
                        isinstance(fieldtype[0], nodes.Text)):
                    typename = u''.join(n.astext() for n in fieldtype)
                    par += self.make_xref(self.typerolename, domain, typename)
                else:
                    par += fieldtype
                par += nodes.Text('')
            par += nodes.Text(' -- ')
            par += content
            return par

        fieldname = nodes.field_name('', self.label)
        if len(items) == 1 and self.can_collapse:
            fieldarg, content = items[0]
            bodynode = handle_item(fieldarg, content)
        else:
            bodynode = self.list_type()
            for fieldarg, content in items:
                bodynode += nodes.list_item('', handle_item(fieldarg, content))
        fieldbody = nodes.field_body('', bodynode)
        return nodes.field('', fieldname, fieldbody)


class ChapelObject(ObjectDescription):
    """Base class for Chapel directives. It has methods for parsing signatures of
    any form, and generating target and index text.
    """

    option_spec = {
        'noindex': directives.flag,
        'module': directives.unchanged,
        'annotation': directives.unchanged,
    }

    doc_field_types = [
        ChapelTypedField('parameter', label=l_('Arguments'),
                         names=('param', 'parameter', 'arg', 'argument'),
                         typerolename='chplref',
                         typenames=('paramtype', 'type'),
                         can_collapse=True),
        Field('returnvalue', label=l_('Returns'), has_arg=False,
              names=('returns', 'return')),
        Field('yieldvalue', label=l_('Yields'), has_arg=False,
              names=('yields', 'yield')),
        Field('returntype', label=l_('Return type'), has_arg=False,
              names=('rtype',)),
        Field('yieldtype', label=l_('Yield type'), has_arg=False,
              names=('ytype',)),
        GroupedField('errorhandling', label=l_('Throws'),
                     names=('throw', 'throws'), can_collapse=True),
    ]

    @staticmethod
    def _pseudo_parse_arglist(signode, arglist):
        """Parse list of comma separated arguments.

        Arguments can have optional types.
        """
        paramlist = addnodes.desc_parameterlist()
        stack = [paramlist]
        try:
            for argument in arglist.split(','):
                argument = argument.strip()
                ends_open = 0
                ends_close = 0
                while argument.startswith('['):
                    stack.append(addnodes.desc_optional())
                    stack[-2] += stack[-1]
                    argument = argument[1:].strip()
                while argument.startswith(']'):
                    stack.pop()
                    argument = argument[1:].strip()
                while argument.endswith(']') and not argument.endswith('[]'):
                    ends_close += 1
                    argument = argument[:-1].strip()
                while argument.endswith('['):
                    ends_open += 1
                    argument = argument[:-1].strip()
                if argument:
                    stack[-1] += addnodes.desc_parameter(argument, argument)
                while ends_open:
                    stack.append(addnodes.desc_optional())
                    stack[-2] += stack[-1]
                    ends_open -= 1
                while ends_close:
                    stack.pop()
                    ends_close -= 1
            if len(stack) != 1:
                raise IndexError
        except IndexError:
            # If there are too few or too many elements on the stack, just give
            # up and treat the whole argument list as one argument, discarding
            # the already partially populated paramlist node.
            signode += addnodes.desc_parameterlist()
            signode[-1] += addnodes.desc_parameter(arglist, arglist)
        else:
            signode += paramlist

    def _get_attr_like_prefix(self, sig):
        """Return prefix text for attribute or data directive."""
        sig_match = chpl_attr_sig_pattern.match(sig)
        if sig_match is None:
            return ChapelObject.get_signature_prefix(self, sig)

        prefixes, _, _, _ = sig_match.groups()
        if prefixes:
            return prefixes.strip() + ' '
        elif self.objtype == 'type':
            return 'type' + ' '
        else:
            return ChapelObject.get_signature_prefix(self, sig)

    def _get_proc_like_prefix(self, sig):
        """Return prefix text for function or method directive
        (and similar).
        """
        sig_match = chpl_sig_pattern.match(sig)
        if sig_match is None:
            return ChapelObject.get_signature_prefix(self, sig)

        prefixes, _, _, _, _ = sig_match.groups()
        if prefixes:
            return prefixes.strip() + ' '
        elif self.objtype.startswith('iter'):
            return 'iter' + ' '
        elif self.objtype in ('method', 'function'):
            return 'proc' + ' '
        else:
            return ChapelObject.get_signature_prefix(self, sig)

    def _is_attr_like(self):
        """Returns True when objtype is attribute or data."""
        return self.objtype in ('attribute', 'data', 'type', 'enum')

    def _is_proc_like(self):
        """Returns True when objtype is *function or *method."""
        return (self.objtype in
                ('function', 'iterfunction', 'method', 'itermethod'))

    def _get_sig_prefix(self, sig):
        """Return signature prefix text. For attribute, data, and proc/iter directives
        this might be part of the signature. E.g. `type myNewType` will return
        a prefix of 'type' and `inline proc foo()` will return 'inline proc'.
        """
        if self._is_proc_like():
            return self._get_proc_like_prefix(sig)
        elif self._is_attr_like():
            return self._get_attr_like_prefix(sig)
        else:
            return ChapelObject.get_signature_prefix(self, sig)

    def get_signature_prefix(self, sig):
        """May return a prefix to put before the object name in
        the signature.
        """
        return ''

    def needs_arglist(self):
        """May return True if an empty argument list is to be generated even if the
        document contains none.
        """
        return False

    def handle_signature(self, sig, signode):
        """Parse the signature *sig* into individual nodes and append them to the
        *signode*. If ValueError is raises, parsing is aborted and the whole
        *sig* string is put into a single desc_name node.

        The return value is the value that identifies the object. IOW, it is
        the identifier that will be used to reference this object, datum,
        attribute, proc, etc. It is a tuple of "fullname" (including module and
        class(es)) and the classes. See also :py:meth:`add_target_and_index`.
        """
        if self._is_attr_like():
            sig_match = chpl_attr_sig_pattern.match(sig)
            if sig_match is None:
                raise ValueError('Signature does not parse: {0}'.format(sig))
            func_prefix, name_prefix, name, retann = sig_match.groups()
            arglist = None
        else:
            sig_match = chpl_sig_pattern.match(sig)
            if sig_match is None:
                raise ValueError('Signature does not parse: {0}'.format(sig))

            func_prefix, name_prefix, name, arglist, retann = \
                sig_match.groups()

        modname = self.options.get(
            'module', self.env.temp_data.get('chpl:module'))
        classname = self.env.temp_data.get('chpl:class')

        if classname:
            if name_prefix and name_prefix.startswith(classname):
                fullname = name_prefix + name
                # class name is given again in the signature
                name_prefix = name_prefix[len(classname):].lstrip('.')
            elif name_prefix:
                # class name is given in the signature, but different
                # (shouldn't happen)
                fullname = classname + '.' + name_prefix + name
            else:
                # class name is not given in the signature
                fullname = classname + '.' + name
        else:
            if name_prefix:
                classname = name_prefix.rstrip('.')
                fullname = name_prefix + name
            else:
                classname = ''
                fullname = name

        signode['module'] = modname
        signode['class'] = classname
        signode['fullname'] = fullname

        sig_prefix = self.get_signature_prefix(sig)
        if sig_prefix:
            signode += addnodes.desc_annotation(sig_prefix, sig_prefix)
        # if func_prefix:
        #     signode += addnodes.desc_addname(func_prefix, func_prefix)
        if name_prefix:
            signode += addnodes.desc_addname(name_prefix, name_prefix)

        anno = self.options.get('annotation')

        signode += addnodes.desc_name(name, name)

        if not arglist:
            # If this needs an arglist, and parens were provided in the
            # signature, add a parameterlist. Chapel supports paren-less
            # functions and methods, which can act as computed properties. If
            # arglist is the empty string, the signature included parens. If
            # arglist is None, it did not include parens.
            if self.needs_arglist() and arglist is not None:
                # for callables, add an empty parameter list
                signode += addnodes.desc_parameterlist()
            if retann:
                signode += addnodes.desc_type(retann, retann)
            if anno:
                signode += addnodes.desc_annotation(' ' + anno, ' ' + anno)
            return fullname, name_prefix

        self._pseudo_parse_arglist(signode, arglist)
        if retann:
            signode += addnodes.desc_type(retann, retann)
        if anno:
            signode += addnodes.desc_annotation(' ' + anno, ' ' + anno)
        return fullname, name_prefix

    def get_index_text(self, modname, name):
        """Return the text for the index entry of the object."""
        raise NotImplementedError('must be implemented in subclasses')

    def add_target_and_index(self, name_cls, sig, signode):
        """Add cross-reference IDs and entries to the index node, if
        applicable. *name_cls* is the return value of
        :py:meth:`handle_signature`.
        """
        modname = self.options.get(
            'module', self.env.temp_data.get('chpl:module'))
        fullname = (modname and modname + '.' or '') + name_cls[0]
        # note target
        if fullname not in self.state.document.ids:
            signode['names'].append(fullname)
            signode['ids'].append(fullname)
            signode['first'] = (not self.names)
            self.state.document.note_explicit_target(signode)
            objects = self.env.domaindata['chpl']['objects']
            if fullname in objects:
                self.state_machine.reporter.warning(
                    'duplicate object description of %s, ' % fullname +
                    'other instance in ' +
                    self.env.doc2path(objects[fullname][0]) +
                    ', use :noindex: for one of them',
                    line=self.lineno)
            objects[fullname] = (self.env.docname, self.objtype)

        indextext = self.get_index_text(modname, name_cls)
        if indextext:
            self.indexnode['entries'].append(('single', indextext,
                                              fullname, '', None))

    def before_content(self):
        """Called before parsing content. Set flag to help with class scoping.
        """
        self.clsname_set = False

    def after_content(self):
        """Called after parsing content. If any classes were added to the env
        temp_data, make sure they are removed.
        """
        if self.clsname_set:
            self.env.temp_data.pop('chpl:class', None)


class ChapelModule(Directive):
    """Directive to make description of a new module."""

    has_content = False
    required_arguments = 1
    optional_arguments = 1
    final_argument_whitespace = False
    option_spec = {
        'platform': lambda x: x,
        'synopsis': lambda x: x,
        'noindex': directives.flag,
        'deprecated': directives.flag,
    }

    def run(self):
        """Custom execution for chapel module directive. This class is instantiated by
        the directive implementation and then this method is called. It parses
        the options on the module directive, updates the environment according,
        and creates an index entry for the module.

        Based on the python domain module directive.
        """
        env = self.state.document.settings.env
        modname = self.arguments[0].strip()
        noindex = 'noindex' in self.options
        env.temp_data['chpl:module'] = modname
        ret = []
        if not noindex:
            env.domaindata['chpl']['modules'][modname] = \
                (env.docname, self.options.get('synopsis', ''),
                 self.options.get('platform', ''),
                 'deprecated' in self.options)

            # Make a duplicate entry in 'objects' to facilitate searching for
            # the module in ChapelDomain.find_obj().
            env.domaindata['chpl']['objects'][modname] = (
                env.docname, 'module')
            targetnode = nodes.target('', '', ids=['module-' + modname],
                                      ismod=True)
            self.state.document.note_explicit_target(targetnode)

            # The platform and synopsis are not printed. In fact, they are only
            # used in the modindex currently.
            ret.append(targetnode)
            indextext = _('%s (module)') % modname
            inode = addnodes.index(entries=[('single', indextext,
                                             'module-' + modname, '', None)])
            ret.append(inode)
        return ret


class ChapelCurrentModule(Directive):
    """this directive is just to tell Sphinx that we're documenting stuff in module
    foo, but links to module foo won't lead here.
    """

    has_content = False
    required_arguments = 1
    optional_arguments = 0
    final_argument_whitespace = False
    option_spec = {}

    def run(self):
        """See :py:meth:`ChapelModule.run`"""
        env = self.state.document.settings.env
        modname = self.arguments[0].strip()
        if modname == 'None':
            env.temp_data['chpl:module'] = None
        else:
            env.temp_data['chpl:module'] = modname
        return []


class ChapelClassMember(ChapelObject):
    """Description of Chapel class members, including attributes, procs,
    and iters.
    """

    @property
    def chpl_type_name(self):
        """Returns iterator or method or '' depending on object type."""
        if not self.objtype.endswith('method'):
            return ''
        elif self.objtype.startswith('iter'):
            return 'iterator'
        elif self.objtype == 'method':
            return 'method'
        else:
            return ''

    def get_signature_prefix(self, sig):
        """Return signature prefix based on sig. May include portion of the sig text,
        if relevant (e.g. `proc foo()` will return 'proc' here.
        """
        return self._get_sig_prefix(sig)

    def needs_arglist(self):
        """Procs and iters need arglists. Attributes do not."""
        return self.objtype.endswith('method')

    def get_index_text(self, modname, name_cls):
        """Return text for index entry based on object type."""
        name, cls = name_cls
        add_modules = self.env.config.add_module_names
        if self.objtype.endswith('method'):
            try:
                clsname, methname = name.rsplit('.', 1)
            except ValueError:
                if modname:
                    return _('%s() (in module %s)') % (name, modname)
                else:
                    return _('%s()') % name
            if modname and add_modules:
                return _('%s() (%s.%s %s)') % \
                    (methname, modname, clsname, self.chpl_type_name)
            else:
                return _('%s() (%s %s)') % \
                    (methname, clsname, self.chpl_type_name)
        elif self.objtype == 'attribute':
            try:
                clsname, attrname = name.rsplit('.', 1)
            except ValueError:
                if modname:
                    return _('%s (in module %s)') % (name, modname)
                else:
                    return name
            if modname and add_modules:
                return _('%s (%s.%s attribute)') % (attrname, modname, clsname)
            else:
                return _('%s (%s attribute)') % (attrname, clsname)
        else:
            return ''


class ChapelClassObject(ChapelObject):
    """Chapel class and record description."""

    def get_signature_prefix(self, sig):
        """Return class or record according to object type."""
        return self.objtype + ' '

    def get_index_text(self, modname, name_cls):
        """Return index entry text based on object type."""
        if self.objtype in ('class', 'record'):
            if not modname:
                return _('%s (built-in %s)') % (name_cls[0], self.objtype)
            return _('%s (%s in %s)') % (name_cls[0], self.objtype, modname)
        else:
            return ''

    def before_content(self):
        """Called before parsing content. Push the class name onto the class name
        stack. Used to construct the full name for members.
        """
        ChapelObject.before_content(self)
        if self.names:
            self.env.temp_data['chpl:class'] = self.names[0][0]
            self.clsname_set = True


class ChapelModuleLevel(ChapelObject):
    """Chapel module level functions, types, and variables (i.e. data directives)
    descriptions.
    """

    @property
    def chpl_type_name(self):
        """Returns type, iterator, or procedure or '' depending on
        object type.
        """
        if self.objtype == 'type':
            return 'type'
        elif not self.objtype.endswith('function'):
            return ''
        elif self.objtype.startswith('iter'):
            return 'iterator'
        elif self.objtype == 'function':
            return 'procedure'
        else:
            return ''

    def get_signature_prefix(self, sig):
        """Return signature prefix based on sig. May include portion of the sig text,
        if relevant (e.g. `proc foo()` will return `proc` here.
        """
        return self._get_sig_prefix(sig)

    def needs_arglist(self):
        """Procs and iters need arglists. Data directives do not."""
        return self.objtype.endswith('function')

    def get_index_text(self, modname, name_cls):
        """Return text for index entry based on object type."""
        if self.objtype.endswith('function'):
            if not modname:
                return _('%s() (built-in %s)') % \
                    (name_cls[0], self.chpl_type_name)
            return _('%s() (in module %s)') % (name_cls[0], modname)
        elif self.objtype in ('data', 'type', 'enum'):
            if not modname:
                type_name = self.objtype
                if type_name == 'data':
                    type_name = 'variable'
                return _('%s (built-in %s)') % (name_cls[0], type_name)
            return _('%s (in module %s)') % (name_cls[0], modname)
        else:
            return ''


class ChapelXRefRole(XRefRole):
    """Chapel cross-referencing role. Extends base XRefRole with special link
    processing method. The Chapel link processing knows how to match a chapel
    xref expression to the known objects, data, and modules in the current
    project/documents.
    """

    def process_link(self, env, refnode, has_explicit_title, title, target):
        """Called after parsing title and target text, and creating the reference
        node. Alter the reference node and return it with chapel module and
        class information, if relevant.
        """
        refnode['chpl:module'] = env.temp_data.get('chpl:module')
        refnode['chpl:class'] = env.temp_data.get('chpl:class')
        if not has_explicit_title:
            # Only has a meaning for the target.
            title = title.lstrip('.')

            # Only has a meaning for the title.
            target = target.lstrip('~')

            if title[0:1] == '~':
                title = title[1:]
                dot = title.rfind('.')
                if dot != -1:
                    title = title[dot+1:]

        # IF the first character is a dot, search more specific names
        # first. Else, search builtins first.
        if target[0:1] == '.':
            target = target[1:]
            refnode['refspecific'] = True

        return title, target


class ChapelModuleIndex(Index):
    """Provides Chapel module index based on chpl:module."""

    name = 'modindex'
    localname = l_('Chapel Module Index')
    shortname = l_('modules')

    def generate(self, docnames=None):
        """Returns entries for index given by ``name``. If ``docnames`` is given,
        restrict to entries referring to these docnames.

        Retunrs tuple of ``(content, collapse)``. ``collapse`` is bool. When
        True, sub-entries should start collapsed for output formats that
        support collapsing.

        ``content`` is a sequence of ``(letter, entries)`` tuples. ``letter``
        is the "heading" for the given ``entries``, in this case the starting
        letter.

        ``entries`` is a sequence of single entries, where a single entry is a
        sequence ``[name, subtype, docname, anchor, extra, qualifier,
        description]``. These items are:

        * ``name`` - name of the index entry to be displayed
        * ``subtype`` - sub-entry related type:

          * 0 - normal entry
          * 1 - entry with sub-entries
          * 2 - sub-entry

        * ``docname`` - docname where the entry is located
        * ``anchor`` - anchor for the entry within docname
        * ``extra`` - extra info for the entry
        * ``qualifier`` - qualifier for the description
        * ``description`` - description for the entry

        Qualifier and description are not rendered in some output formats.
        """
        content = {}

        # list of prefixes to ignore
        ignores = self.domain.env.config['chapeldomain_modindex_common_prefix']
        ignores = sorted(ignores, key=len, reverse=True)

        # list of all modules, sorted by module name
        modules = sorted(iteritems(self.domain.data['modules']),
                         key=lambda x: x[0].lower())

        # sort out collapsible modules
        prev_modname = ''
        num_toplevels = 0
        for modname, (docname, synopsis, platforms, deprecated) in modules:
            # If given a list of docnames and current docname is not in it,
            # skip this docname for the index.
            if docnames and docname not in docnames:
                continue

            for ignore in ignores:
                if modname.startswith(ignore):
                    modname = modname[len(ignore):]
                    stripped = ignore
                    break
            else:
                stripped = ''

            # we stripped the whole module name?
            if not modname:
                modname, stripped = stripped, ''

            # Put the module in correct bucket (first letter).
            entries = content.setdefault(modname[0].lower(), [])

            package = modname.split('.')[0]
            if package != modname:
                # it's a submodule!
                if prev_modname == package:
                    # first submodule - make parent a group head
                    if entries:
                        entries[-1][1] = 1
                elif not prev_modname.startswith(package):
                    # submodule without parent in list, add dummy entry
                    entries.append([stripped + package, 1, '', '', '', '', ''])
                subtype = 2
            else:
                num_toplevels += 1
                subtype = 0

            qualifier = deprecated and _('Deprecated') or ''
            entries.append([stripped + modname, subtype, docname,
                            'module-' + stripped + modname, platforms,
                            qualifier, synopsis])
            prev_modname = modname

        # apply heuristics when to collapse modindex at page load: only
        # collapse if number of toplevel modules is larger than number of
        # submodules
        collapse = len(modules) - num_toplevels < num_toplevels

        # sort by first leter
        content = sorted(iteritems(content))

        return content, collapse


class ChapelDomain(Domain):
    """Chapel language domain."""

    name = 'chpl'
    labels = 'Chapel'

    object_types = {
        'data': ObjType(l_('data'), 'data', 'const', 'var', 'param', 'type'),
        'type': ObjType(l_('type'), 'type', 'data'),
        'function': ObjType(l_('function'), 'func', 'proc'),
        'iterfunction': ObjType(l_('iterfunction'), 'func', 'iter', 'proc'),
        'enum': ObjType(l_('enum'), 'enum'),
        'class': ObjType(l_('class'), 'class'),
        'record': ObjType(l_('record'), 'record'),
        'method': ObjType(l_('method'), 'meth', 'proc'),
        'itermethod': ObjType(l_('itermethod'), 'meth', 'iter'),
        'attribute': ObjType(l_('attribute'), 'attr'),
        'module': ObjType(l_('module'), 'mod'),
    }

    directives = {
        'data': ChapelModuleLevel,
        'type': ChapelModuleLevel,
        'function': ChapelModuleLevel,
        'iterfunction': ChapelModuleLevel,

        # TODO: Consider making enums ChapelClassObject, then each constant
        #       becomes an attribute on the class. Then xrefs to each constant
        #       would be possible, plus it would scale to large numbers of
        #       constants. (thomasvandoren, 2015-03-12)
        'enum': ChapelModuleLevel,

        'class': ChapelClassObject,
        'record': ChapelClassObject,
        'method': ChapelClassMember,
        'itermethod': ChapelClassMember,
        'attribute': ChapelClassMember,
        'module': ChapelModule,
        'currentmodule': ChapelCurrentModule,
    }

    roles = {
        'data': ChapelXRefRole(),
        'const': ChapelXRefRole(),
        'var': ChapelXRefRole(),
        'param': ChapelXRefRole(),
        'type': ChapelXRefRole(),
        'func': ChapelXRefRole(),
        'proc': ChapelXRefRole(),
        'iter': ChapelXRefRole(),
        'class': ChapelXRefRole(),
        'record': ChapelXRefRole(),
        'enum': ChapelXRefRole(),
        'meth': ChapelXRefRole(),
        'attr': ChapelXRefRole(),
        'mod': ChapelXRefRole(),
        'chplref': ChapelXRefRole(),
    }

    initial_data = {
        'objects': {},   # fullname -> docname, objtype
        'modules': {},   # modname -> docname, synopsis, platform, deprecated
        'labels': {      # labelname -> docname, labelid, sectionname
            'chplmodindex': ('chpl-modindex', '', l_('Chapel Module Index')),
        },
        'anonlabels': {  # labelname -> docname, labelid
            'chplmodindex': ('chpl-modindex', ''),
        },
    }

    indices = [
        ChapelModuleIndex,
    ]

    def clear_doc(self, docname):
        """Remove the data associated with this instance of the domain."""
        for fullname, (fn, x) in self.data['objects'].items():
            if fn == docname:
                del self.data['objects'][fullname]
        for modname, (fn, x, x, x) in self.data['modules'].items():
            if fn == docname:
                del self.data['modules'][modname]
        for labelname, (fn, x, x) in self.data['labels'].items():
            if fn == docname:
                del self.data['labels'][labelname]
        for anonlabelname, (fn, x) in self.data['anonlabels'].items():
            if fn == docname:
                del self.data['anonlabels'][anonlabelname]

    def find_obj(self, env, modname, classname, name, type_name, searchmode=0):
        """Find a Chapel object for "name", possibly with module or class/record
        name. Returns a list of (name, object entry) tuples.

        :arg int searchmode: If 1, search more specific names first. Otherwise,
            search built-ins first and then get more specific.
        """
        if name[-2:] == '()':
            name = name[:-2]

        if not name:
            return []

        objects = self.data['objects']
        matches = []

        newname = None
        if searchmode == 1:
            if type_name is None:
                objtypes = list(self.object_types)
            else:
                objtypes = self.objtypes_for_role(type_name)
            if objtypes is not None:
                if modname and classname:
                    fullname = modname + '.' + classname + '.' + name
                    if (fullname in objects and
                            objects[fullname][1] in objtypes):
                        newname = fullname
                if not newname:
                    if (modname and modname + '.' + name in objects and
                            objects[modname + '.' + name][1] in objtypes):
                        newname = modname + '.' + name
                    elif name in objects and objects[name][1] in objtypes:
                        newname = name
                    else:
                        # "Fuzzy" search mode.
                        searchname = '.' + name
                        matches = [(oname, objects[oname]) for oname in objects
                                   if oname.endswith(searchname) and
                                   objects[oname][1] in objtypes]
        else:
            # NOTE: Search for exact match, object type is not considered.
            if name in objects:
                newname = name
            elif type_name == 'mod':
                # Only exact matches allowed for modules.
                return []
            elif classname and classname + '.' + name in objects:
                newname = classname + '.' + name
            elif modname and modname + '.' + name in objects:
                newname = modname + '.' + name
            elif (modname and classname and
                    modname + '.' + classname + '.' + name in objects):
                newname = modname + '.' + classname + '.' + name

        if newname is not None:
            matches.append((newname, objects[newname]))
        return matches

    def resolve_xref(self, env, fromdocname, builder,
                     type_name, target, node, contnode):
        """Resolve the pending_xref *node* with give *type_name* and *target*. Returns
        None if xref node can not be resolved. If xref can be resolved, returns
        new node containing the *contnode*.
        """
        # Special case the :chpl:chplref:`chplmodindex` instances.
        if type_name == 'chplref':
            if node['refexplicit']:
                # Reference to anonymous label. The reference uses the supplied
                # link caption.
                docname, labelid = self.data['anonlabels'].get(
                    target, ('', ''))
                sectname = node.astext()
            else:
                # Reference to named label. The final node will contain the
                # section name after the label.
                docname, labelid, sectname = self.data['labels'].get(
                    target, ('', '', ''))

            if not docname:
                return None

            return self._make_refnode(
                fromdocname, builder, docname, labelid, sectname, contnode)

        modname = node.get('chpl:module')
        clsname = node.get('chpl:class')
        searchmode = 1 if node.hasattr('refspecific') else 0
        matches = self.find_obj(env, modname, clsname, target,
                                type_name, searchmode)

        if not matches:
            return None
        elif len(matches) > 1:
            env.warn_node(
                'more than one target found for cross-reference '
                '%r: %s' % (target, ', '.join(match[0] for match in matches)),
                node)
        name, obj = matches[0]

        if obj[1] == 'module':
            return self._make_module_refnode(
                builder, fromdocname, name, contnode)
        else:
            return make_refnode(builder, fromdocname, obj[0], name,
                                contnode, name)

    def resolve_any_xref(self, env, fromdocname, builder, target,
                         node, contnode):
        """Similar to :py:meth:`ChapelDomain.resolve_xref`, but applies to *any* or
        similar role where type is not known. This returns a list of tuples
        with ("domain:role", newnode).
        """
        modname = node.get('chpl:module')
        clsname = node.get('chpl:class')
        results = []

        # Always search in "refspecific" mode with the :any: role.
        matches = self.find_obj(env, modname, clsname, target, None, 1)
        for name, obj in matches:
            if obj[1] == 'module':
                results.append(('chpl:mod',
                                self._make_module_refnode(builder, fromdocname,
                                                          name, contnode)))
            else:
                results.append(
                    ('chpl:' + self.role_for_objtype(obj[1]),
                     make_refnode(builder, fromdocname, obj[0], name,
                                  contnode, name)))

        return results

    def _make_refnode(self, fromdocname, builder, docname, labelid, sectname,
                      contnode, **kwargs):
        """Return reference node for something like ``:chpl:chplref:``."""
        nodeclass = kwargs.pop('nodeclass', nodes.reference)
        newnode = nodeclass('', '', internal=True, **kwargs)
        innernode = nodes.emphasis(sectname, sectname)
        if docname == fromdocname:
            newnode['refid'] = labelid
        else:
            # Set more info on contnode. In case the get_relative_uri call
            # raises NoUri, the builder will then have to resolve these.
            contnode = addnodes.pending_xref('')
            contnode['refdocname'] = docname
            contnode['refsectname'] = sectname
            newnode['refuri'] = builder.get_relative_uri(fromdocname, docname)
            if labelid:
                newnode['refuri'] += '#' + labelid
        newnode.append(innernode)
        return newnode

    def _make_module_refnode(self, builder, fromdocname, name, contnode):
        """Helper function to generate new xref node based on
        current environment.
        """
        # Get additional info for modules.
        docname, synopsis, platform, deprecated = self.data['modules'][name]
        title = name
        if synopsis:
            title += ': ' + synopsis
        if deprecated:
            title += _(' (deprecated)')
        if platform:
            title += ' (' + platform + ')'
        return make_refnode(builder, fromdocname, docname,
                            'module-' + name, contnode, title)

    def merge_domaindata(self, docnames, otherdata):
        """Merge in data regarding *docnames* from a different domaindata inventory
        (coming froma subprocess in a parallel build).
        """
        for fullname, (fn, objtype) in otherdata['objects'].items():
            if fn in docnames:
                self.data['objects'][fullname] = (fn, objtype)
        for modname, data in otherdata['modules'].items():
            if data[0] in docnames:
                self.data['modules'][modname] = data
        for labelname, data in otherdata['labels'].items():
            if data[0] in docnames:
                self.data['labels'][labelname] = data
        for anonlabelname, data in otherdata['anonlabels'].items():
            if data[0] in docnames:
                self.data['anonlabels'][anonlabelname] = data

    def get_objects(self):
        """Return iterable of "object descriptions", which are tuple with these items:

        * `name`
        * `dispname`
        * `type`
        * `docname`
        * `anchor`
        * `priority`

        For details on each item, see
        :py:meth:`~sphinx.domains.Domain.get_objects`.
        """
        for modname, info in self.data['modules'].items():
            yield (modname, modname, 'module', info[0], 'module-' + modname, 0)
        for refname, (docname, type_name) in self.data['objects'].items():
            if type_name != 'module':  # modules are already handled
                yield (refname, refname, type_name, docname, refname, 1)


def setup(app):
    """Add Chapel domain to Sphinx app."""
    app.add_config_value('chapeldomain_modindex_common_prefix', [], 'html')
    app.add_domain(ChapelDomain)
