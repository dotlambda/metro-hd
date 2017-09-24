#!/usr/bin/env python
# -*- coding: utf-8 -*- #
from __future__ import unicode_literals

AUTHOR = 'Daniela Kilian, Stefan Müller, Robert Schütz'
SITENAME = 'Metro HD'
SITEURL = ''
SITELOGO = 'images/splash.png'
FAVICON = 'images/samuslogo.png'

PATH = 'content'

PAGE_URL = '{slug}.html'
PAGE_SAVE_AS = '{slug}.html'	

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

PAGE_ORDER_BY = 'order'

THEME = './Flex'
JINJA_ENVIRONMENT = { }
