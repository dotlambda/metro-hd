#!/usr/bin/env python
# -*- coding: utf-8 -*- #
from __future__ import unicode_literals

AUTHOR = 'Daniela Kilian, Stefan Müller, Robert Schütz'
SITENAME = 'Metro HD'
SITEURL = ''

PATH = 'content'

PAGES_SORT_ATTRIBUTE = 'order'
PAGE_URL = '{slug}.html'
PAGE_SAVE_AS = '{slug}.html'	

STATIC_PATHS = ['images', 'downloads']

TIMEZONE = 'Europe/Berlin'

DEFAULT_LANG = 'en'

# Feed generation is usually not desired when developing
FEED_ALL_ATOM = None
CATEGORY_FEED_ATOM = None
TRANSLATION_FEED_ATOM = None
AUTHOR_FEED_ATOM = None
AUTHOR_FEED_RSS = None

DEFAULT_PAGINATION = False

# Uncomment following line if you want document-relative URLs when developing
RELATIVE_URLS = True

THEME = './pelican-blueidea'

PLUGIN_PATHS = ['pelican-plugins']
PLUGINS = ['pandoc_reader', 'render_math']
PANDOC_ARGS = [
    '--mathjax',
    '--highlight-style', 'pygments',
]

