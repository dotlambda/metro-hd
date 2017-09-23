index.html: index.md includes.hs
	pandoc index.md -s -t html5 --template templates/default.html5 --toc --toc-depth 1 --mathjax --filter includes.hs --highlight-style pygments -o index.html
