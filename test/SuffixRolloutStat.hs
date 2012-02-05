{- *** MOCS-COPYRIGHT-NOTICE-BEGIN ***
 -
 - This copyright notice is auto-generated by ./add-copyright-notice.
 - Additional copyright notices must be added below the last line of this notice.
 -
 - MoCS (https://lewis.cs.uni-saarland.de/tools/mocs/): "test/SuffixRolloutStat.hs".
 - The content of this file is copyright of Saarland University -
 - Copyright (C) 2009 Saarland University, Reactive Systems Group, Lars Kuhtz <kuhtz@cs.uni-sb.de>.
 -
 - This file is part of MoCS (https://lewis.cs.uni-saarland.de/tools/mocs/).
 -
 - License: three-clause BSD style license.
 - The license text can be found in the file LICENSE.
 -
 - *** MOCS-COPYRIGHT-NOTICE-END *** -}

-- plot: echo "plot 'data.gplot'" | gnuplot -persist
module Main where

import Control.Monad
import Data.List
import IO

import PSL
import T2Automaton

import CExpr
import CAut

main = do

  -- Parse Formula
  -- s <- getLine
  let i = 150
  -- putStrLn $ toString f

  -- Automaton
  -- (ctx,ptable,a) <- createAut f
  -- putStrLn $ "automaton:"
  -- putStrLn $ toString $ ptable
  
  --autToDot "aut" a "automaton"
  -- sctx <- contextSize ctx
  -- putStrLn $ "size of context: " ++ (show sctx)
 
  l <- mapM rsize [1..i]
  mapM (\(x,y) -> putStrLn ((show x) ++ " " ++ (show y))) l

  where
  rsize i = do 
    a <- aut i
    el <- autSuffixRollout (fromInteger i) a
    se <- getExprCtx (head el) >>= contextSize
    --mapM (\x -> getExprCtx x >>= contextSize >>= (putStrLn.show)) el
    return (i,se) 
  
  --s i = "(a \\BU " ++ show i ++ " b) \\U (c \\BR " ++ show i ++ " d)"
  s i = let j = i `div` 2 in 
          "(a \\BU " ++ show j ++ " b) \\BU " ++ show j ++ " (((b*);(a[" ++ show j ++ "]);(true*)) \\BR " ++ show j ++ " d)"

  aut i = do 
    formula <- either (error . show) (return) (parsePSL (s i))
    (_,_,a) <- createAut formula
    return a
 
