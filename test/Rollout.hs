{- *** MOCS-COPYRIGHT-NOTICE-BEGIN ***
 -
 - This copyright notice is auto-generated by ./add-copyright-notice.
 - Additional copyright notices must be added below the last line of this notice.
 -
 - MoCS (https://lewis.cs.uni-saarland.de/tools/mocs/): "test/Rollout.hs".
 - The content of this file is copyright of Saarland University -
 - Copyright (C) 2009 Saarland University, Reactive Systems Group, Lars Kuhtz.
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
-- import qualified Data.Map as M

import IO

import PSL
import ToString
import T2Automaton

import CExpr
import CAut

main = do

  -- Parse Formula
  s <- getLine
  f <- either (error . show) (return) (parsePSL s)
  -- putStrLn $ toString f

  -- Get steps
  i <- read `liftM` getLine

  -- Automaton
  (ctx,_,a) <- createAut f
  -- putStrLn $ "automaton:"
  -- putStrLn $ toString $ ptable
  
  autToDot "aut" a "automaton"
  sctx <- contextSize ctx
  putStrLn $ "size of context: " ++ (show sctx)
 
  e <- autPrefixRollout (fromInteger i) a
  exprToDot "rollout" e ("rollout of " ++ toString f)
  se <- getExprCtx e >>= contextSize
  putStrLn $ "size of rollout context: " ++ (show se)
  es <- exprSize e
  putStrLn $ "size of expr:    " ++ (show es)

