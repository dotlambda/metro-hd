index.html: index.md templates/* includes.hs
	pandoc index.md -s -t html --toc --toc-depth 1 --katex --filter includes.hs --template templates/default.html --highlight-style pygments -o index.html
