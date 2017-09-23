index.html: index.md includes.hs
	pandoc index.md -s -t html --toc --toc-depth 1 --mathjax --filter includes.hs --highlight-style pygments -o index.html
