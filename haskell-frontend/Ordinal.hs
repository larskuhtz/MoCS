{- *** MOCS-COPYRIGHT-NOTICE-BEGIN ***
 -
 - This copyright notice is auto-generated by ./add-copyright-notice.
 - Additional copyright notices must be added below the last line of this notice.
 -
 - MoCS (https://lewis.cs.uni-saarland.de/tools/mocs/): "haskell-frontend/Ordinal.hs".
 - The content of this file is copyright of Saarland University -
 - Copyright (C) 2009 Saarland University, Reactive Systems Group, Lars Kuhtz <kuhtz@cs.uni-sb.de>.
 -
 - This file is part of MoCS (https://lewis.cs.uni-saarland.de/tools/mocs/).
 -
 - License: three-clause BSD style license.
 - The license text can be found in the file LICENSE.
 -
 - *** MOCS-COPYRIGHT-NOTICE-END *** -}

module Ordinal (Ordinal(Infty,Finite),isFinite,isInfty,parseOrdinal,finite) where

import qualified Text.ParserCombinators.Parsec as P
import PSLLexer
import ToString

data Ordinal = Finite Integer
             | Infty
  deriving (Ord,Eq,Show)

-- TODO: make sure all finite values are allways positive

instance Num Ordinal where
  (Infty) + _ = Infty
  _ + (Infty) = Infty
  (Finite a) + (Finite b) = Finite (a+b)

  (Infty) - _ = Infty
  _ - (Infty) = Finite 0
  (Finite a) - (Finite b) = Finite (max (a-b) 0)

  negate _ = Finite 0

  (Infty) * _ = Infty
  _ * (Infty) = Infty
  (Finite a) * (Finite b) = Finite (a*b)

  abs x = x -- there are no negative values!
  signum (Finite 0) = 0
  signum _ = 1

  fromInteger x | x < 0     = error "Can not build ordinal from negaive Integer"
                | otherwise = Finite x

instance ToString Ordinal where
  toString (Finite x) = show x
  toString Infty = "infty"

isInfty (Infty) = True
isInfty _       = False

isFinite = not . isInfty

parseOrdinal :: P.Parser Ordinal
parseOrdinal = (natural >>= return . fromInteger) P.<|> (P.string "infty" >> return Infty)

finite (Finite i) = i
finite _ = error "Can not extract integer from infty value"
