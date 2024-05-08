# -*- coding: utf-8 -*-
"""
    search_index_entries.py
    ~~~~~~~~~~~~~~~~~~~~~~~

    This file generates search-index-entries.js which can be used from
    the javascript side in order to more prominently feature search
    results that match index items.

"""

from __future__ import annotations

from typing import TYPE_CHECKING

import json
import os.path
from docutils import nodes
from sphinx.addnodes import index
from sphinx.util import logging

if TYPE_CHECKING:
    from sphinx.application import Sphinx

logger = logging.getLogger(__name__)
JSON = 'search-index-entries.js'


def process_index_nodes(app: Sphinx, doctree: nodes.document, docname: str):
    if app.builder.format != 'html':
        return

    terms = { }

    for node in doctree.traverse(index):
        for i, entry in enumerate(node['entries']):
            (entrytype, entryname, target, _, _) = node['entries'][i]
            # ignore C++ entries
            if not entryname.startswith('chpl::'):
                #print (entryname, " -> ", uri)
                terms[entryname] = target

    if hasattr(app.env, 'search_index_docname_to_terms'):
        app.env.search_index_docname_to_terms.update({docname: terms})
    else:
        app.env.search_index_docname_to_terms = {docname: terms}

def purge_index_env(app, env, docname):
    if not hasattr(env, 'search_index_docname_to_terms'):
        return
    fresh_search_index_docname_to_terms = { }
    for d, terms in env.search_index_docname_to_terms.items():
        if d != docname:
            fresh_search_index_docname_to_terms[docname] = terms
    env.search_index_docname_to_terms = fresh_search_index_docname_to_terms

def merge_index_env(app, env, docnames, other):
    if not hasattr(other, 'search_index_docname_to_terms'):
        return
    if not hasattr(env, 'search_index_docname_to_terms'):
        env.search_index_docname_to_terms = { }
    for d, terms in other.search_index_docname_to_terms.items():
        if d in env.search_index_docname_to_terms:
            raise ValueError # this should not be possible
        else:
            env.search_index_docname_to_terms[d] = terms

def on_build_finish(app, exc):
    if not hasattr(app.env, 'search_index_docname_to_terms'):
        return
    if not app.env.search_index_docname_to_terms:
        return

    logger.info(f'Preparing {JSON}', color='green')

    idx_to_doc = [ ] # from idx to docuri
    doc_to_idx = { } # from docname to idx
    for docname in sorted(app.env.search_index_docname_to_terms):
        docuri = app.builder.get_target_uri(docname)
        doc_to_idx[docname] = len(idx_to_doc)
        idx_to_doc.append(docuri)
        assert idx_to_doc[doc_to_idx[docname]] == docuri

    forward = { }
    for docname, terms in app.env.search_index_docname_to_terms.items():
        doc_idx = doc_to_idx[docname]
        for entryname, target in terms.items():
            if entryname not in forward:
                forward[entryname] = [ ]
            forward[entryname].append(str(doc_idx) + "#" + target)

    print ("forward", forward)

    logger.info(f'Writing {JSON}', color='green')

    dest_dir = app.outdir
    os.makedirs(dest_dir, exist_ok=True)

    with open(os.path.join(dest_dir, JSON), 'w') as f:
        f.write("Search.setManualIndex(")
        json.dump({'docuris': idx_to_doc,
                   'index_entries_to_locations': forward}, f)
        f.write(")")


def setup(app):
    app.connect('doctree-resolved', process_index_nodes)
    app.connect('build-finished', on_build_finish)
    app.connect('env-purge-doc', purge_index_env)
    app.connect('env-merge-info', merge_index_env)

    return {'version': '0.1', 'parallel_read_safe': True}
