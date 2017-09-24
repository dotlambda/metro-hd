#!/usr/bin/env python
# -*- coding: utf-8 -*- #
from __future__ import unicode_literals

AUTHOR = 'Daniela Kilian, Stefan Müller, Robert Schütz'
SITENAME = 'Metro HD'
SITEURL = ''
SITELOGO = 'images/splash.png'
FAVICON = 'images/samuslogo.png'

PATH = 'content'

PAGES_SORT_ATTRIBUTE = 'order'
PAGE_URL = '{slug}.html'
PAGE_SAVE_AS = '{slug}.html'	

STATIC_PATHS = ['images', 'downloads', 'extra']

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

THEME = './Flex'
JINJA_ENVIRONMENT = { }
COPYRIGHT_NAME  = 'Daniela Kilian, Stefan Müller, Robert Schütz'
COPYRIGHT_YEAR = 2017
CUSTOM_CSS = 'custom.css'
EXTRA_PATH_METADATA = {'extra/custom.css': {'path': 'custom.css'}}

PLUGINS = ['pandoc_reader']
PANDOC_ARGS = [
    '--mathjax',
    '--highlight-style', 'pygments',
]

