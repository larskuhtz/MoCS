{- *** MOCS-COPYRIGHT-NOTICE-BEGIN ***
 -
 - This copyright notice is auto-generated by ./add-copyright-notice.
 - Additional copyright notices must be added below the last line of this notice.
 -
 - MoCS (https://lewis.cs.uni-saarland.de/tools/mocs/): "haskell-frontend/Main2.hs".
 - The content of this file is copyright of Saarland University -
 - Copyright (C) 2009 Saarland University, Reactive Systems Group, Lars Kuhtz.
 -
 - This file is part of MoCS (https://lewis.cs.uni-saarland.de/tools/mocs/).
 -
 - License: three-clause BSD style license.
 - The license text can be found in the file LICENSE.
 -
 - *** MOCS-COPYRIGHT-NOTICE-END *** -}

module Main where

import Control.Monad

import Data.List
import qualified Data.Map as M
import Foreign.C.Types
import System( getArgs )
import IO

import PSL
import ToString
import PSLFragment
import T2Automaton
import Ordinal

import CExpr
import CAut

{--
 main = do
 -  x <- newAutCtx
 -  -  a0 <- propAut x [1,2]
 -  -  a1 <- constAut x True
 -  -  a2 <- propAut x [2,3]
 -  -  a3 <- andAut a0 a1
 -  -  a4 <- orAut a3 a2
 -  -  a5 <- dualAut a4
 -  -  a6 <- nextAut =<< gAut =<< (untilAut a4 a3)
 -  -  autToDot "test.dot" a6 "aut"
 -  -  print "done"
 -
--}

d a = a 

main = do

  -- get arguments
  args <- getArgs
  let mfile = if args == [] then "monitor.vhdl" else (let (h:_) = args in h)

  -- Parse Formula
  s <- getContents
  f <- either (error . show) (return) (parsePSL s)
  --d putStrLn $ toString f
  --d putStrLn $ show $ horizon f

  -- Separate
  let (prefix,suffix) =  altSeparate f
  --putStrLn $ toString prefix
  --putStrLn $ concatMap toString suffix
  let hor = maximum $ map horizon suffix
  --d putSep
  --d putStrLn $ "prefix-formula:"
  --d putStrLn $ toString prefix

  ------------------------------------------------------------------------------
  -- Prefix Automaton
  (_,ptable,a) <- createAut prefix
  --d putStrLn $ "prefix-automaton (ptable):"
  --d putStrLn $ toString $ ptable

  -- autToDot "pre.dot" a "prefix-automaton"
  --sctx <- contextSize ctx
  --d putStrLn $ "size of prefix-automaton-context: " ++ (show sctx)

  -- Universal prefix automaton
  --d putStrLn $ "universal prefix-automaton:"
  (_,b) <- universalAut a
  -- autToDot "preu.dot" b "universal prefix-automaton"
  --suctx <- contextSize uctx
  --d putStrLn $ "size of universal prefix-automaton-context: " ++ (show suctx)

  -- Build state machine
  -- sm <- mkStateMachine b
  -- smToDot "sm.dot" sm "state machine"

  -- rollout
  --d putStrLn $ "rollout " ++ (show hor) ++ " steps:"
  e <- autPrefixRollout (fromInteger $ finite $ hor) b
  -- putStrLn $ "get size of rollout context"
  -- se <- getExprCtx e >>= contextSize
  --d putStrLn $ "size of rollout-context: " ++ (show $ se)

  ------------------------------------------------------------------------------
  -- Suffix Automata
  -- putSep
  --d putStrLn $ "number of suffixes: " ++ show (length suffix)
  tmpctx <- newAutCtx
  (_,stable,as) <- foldM doSuffix (tmpctx,M.empty,[]) (zip [0..] suffix)
  oexprs <- mapM (rollmap hor) as

  -- exprsToDot "suffix_exprs.dot" (concatMap snd oexprs) "suffix_expressions"

  -- oexprs is a map from suffix-formulas (toString as used in variables names of prefix)
  -- to its oexprs list

  -- print "build vhdl monitor"

  callVhdlBuildMonitor mfile (fromInteger $ finite $ hor) b ptable e oexprs stable

  -- print "done"
  --  putStrLn $ concat (parMap rnf doSuffix' suffix)


  where
  rollmap h (i,a) = do
    exprs <- autSuffixRollout (fromInteger $ finite $ h) a
    return (i,exprs)

doSuffix (ctx,table,as) (_,s) = do
  (nctx,ntable,sa) <- createAutWithCtxAndTable ctx table s
  --  putStrLn $ toString sa
  -- putStrLn $ A.stats sa
  -- autToDot ("suf" ++ show i ++ ".dot") sa ("suffix-automaton " ++ toString s)
  --bracket (openFile ("suf" ++ show i) WriteMode) hClose (\h -> A.doA2dot h $ mkInt sa)
  -- putSep
  return (nctx,ntable,as ++ [(toString s,sa)]) -- what does ntable look like?

putSep = putStrLn $ "--------------------"

callVhdlBuildMonitor :: String -> Int -> AutPtr -> PropTable -> ExprPtr -> [(String,[ExprPtr])] -> PropTable -> IO ()
callVhdlBuildMonitor
  fname -- filename
  hor    -- horizon
  prefix_automaton
  ptable -- variable index tabel for prefix automaton
  pa_expr -- output expression of prefix automaton
  suffix_expressions -- map from suffix formulae (variable name) to list of output expressions
  stable -- variable index tabel for suffix automata
  = do
    vhdlBuildMonitor fname hor (M.size stable) (M.size ptable) pa_expr prefix_automaton sexprs
    where
    sexprs :: [[ExprPtr]]
    sexprs =
      let
        swap (a,b) = (b,a)
        swap_sort_ptable :: [(CLong,String)]
        swap_sort_ptable = sortBy (comp) (map swap (M.toList ptable))
        comp a b = if a < b then LT else (if b < a then GT else EQ)
      in
        map (\(_,v) -> maybe (lookuperror v) id (lookup v suffix_expressions)) swap_sort_ptable

    lookuperror v = error $ "variable " ++ v ++ " in ptable but not in suffix expressions"

