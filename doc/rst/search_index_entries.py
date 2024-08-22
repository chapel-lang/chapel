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
JSON = 'searchindex.js'

# For Python 3.9 and later, just use removeprefix / removesuffix
def remove_prefix(text, prefix):
    if text.startswith(prefix):
        return text[len(prefix):]
    return text

def remove_suffix(text, suffix):
    if text.endswith(suffix):
        return text[:-len(suffix)]
    return text

def process_index_nodes(app: Sphinx, doctree: nodes.document, docname: str):
    if app.builder.format != 'html':
        return

    terms = { }

    for node in doctree.traverse(index):
        for entry in node['entries']:
            (_, entryname, target, _, _) = entry
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

    dest_dir = app.outdir
    fname = os.path.join(dest_dir, JSON)

    logger.info(f'Preparing {JSON}', color='green')

    # load up the searchindex.js file
    searchindex = ""
    with open(fname, 'r') as f:
        searchindex = f.read()

    # remove 'Search.setIndex(' as well as the trailing )
    searchindex = remove_prefix(searchindex, 'Search.setIndex(')
    searchindex = remove_suffix(searchindex, ')');

    # parse the result as a json object
    index = json.loads(searchindex)

    # compute a map from docname to idx
    doc_to_idx = { } # from docname to idx
    for idx, docname in enumerate(index['docnames']):
        doc_to_idx[docname] = idx

    forward = { }
    for docname, terms in app.env.search_index_docname_to_terms.items():
        doc_idx = doc_to_idx[docname]
        for entryname, target in terms.items():
            if entryname not in forward:
                forward[entryname] = [ ]
            forward[entryname].append(str(doc_idx) + "#" + target)

    # print ("forward", forward)

    # save the map in the searchindex
    index['index_entries_to_locations'] = forward

    nkeys = len(forward)
    logger.info(f'Writing {nkeys} new entries to {JSON}', color='green')

    os.makedirs(dest_dir, exist_ok=True)

    with open(fname, 'w') as f:
        f.write("Search.setIndex(")
        json.dump(index, f)
        f.write(")")


def setup(app):
    app.connect('doctree-resolved', process_index_nodes)
    app.connect('build-finished', on_build_finish)
    app.connect('env-purge-doc', purge_index_env)
    app.connect('env-merge-info', merge_index_env)

    return {'version': '0.1', 'parallel_read_safe': True}
