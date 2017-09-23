#!/usr/bin/env runghc

import Text.Pandoc.JSON
import Data.Maybe
import Data.List

slice from to xs = take (to - from + 1) (drop from xs)

doInclude :: Block -> IO Block
doInclude cb@(CodeBlock (id, classes, namevals) contents) =
  case lookup "include" namevals of
       Nothing -> return cb
       Just f  -> do filecontent <- readFile f
                     let startline = (fromMaybe 0 . fmap (subtract 1) . fmap read) (lookup "startline" namevals)
                     let endline = (subtract 1 . fromMaybe (length (lines filecontent)) . fmap read) (lookup "endline" namevals)
                     return (CodeBlock (id, classes, namevals) ((intercalate "\n" . slice startline endline . lines) filecontent))
doInclude x = return x

main :: IO ()
main = toJSONFilter doInclude
