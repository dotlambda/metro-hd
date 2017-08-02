index.html: index.md templates/* includes.hs
	pandoc index.md -s -t html --toc --filter includes.hs --template templates/default.html --highlight-style pygments -o index.html
