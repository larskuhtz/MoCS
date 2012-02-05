{- *** MOCS-COPYRIGHT-NOTICE-BEGIN ***
 -
 - This copyright notice is auto-generated by ./add-copyright-notice.
 - Additional copyright notices must be added below the last line of this notice.
 -
 - MoCS (https://lewis.cs.uni-saarland.de/tools/mocs/): "haskell-frontend/Regular.hs".
 - The content of this file is copyright of Saarland University -
 - Copyright (C) 2009 Saarland University, Reactive Systems Group, Lars Kuhtz.
 -
 - This file is part of MoCS (https://lewis.cs.uni-saarland.de/tools/mocs/).
 -
 - License: three-clause BSD style license.
 - The license text can be found in the file LICENSE.
 -
 - *** MOCS-COPYRIGHT-NOTICE-END *** -}

-- TODO Use submodule for r2aut
module Regular where

import qualified Text.ParserCombinators.Parsec as P
import Text.ParserCombinators.Parsec.Expr

import Data.List

import PSLLexer
import ToString
import PSLFragment
import Ordinal

data Expression prop 
  = Prop prop
  | Conc (Expression prop) (Expression prop)
  | OConc (Expression prop) (Expression prop)
  | Union (Expression prop) (Expression prop)
  | Intersection (Expression prop) (Expression prop)
  | Epsilon
  | Kleene Ordinal (Expression prop)
  deriving (Eq,Ord,Show)

mkProp e = Prop e
mkConc x y = Conc x y
mkOConc x y = OConc x y
mkUnion x y | x < y =  Union x y
            | otherwise = Union y x
mkIntersection x y  | x < y = Intersection x y
                    | otherwise = Intersection y x
mkEpsilon = Epsilon
mkKleene b x = Kleene b x

instance (ToString prop) => ToString (Expression prop) where
  toString r = case r of
    (Prop e)           -> toString e
    (Conc x y)         -> bin ";" x y
    (OConc x y)        -> bin ":" x y
    (Union x y)        -> bin "|" x y
    (Intersection x y) -> bin "&&" x y
    (Epsilon)          -> "epsilon"
    (Kleene Infty x)   -> toString x ++ "*"
    (Kleene b x)       -> toString x ++ "[" ++ toString b ++ "]"
    where
    bin o f g   = "(" ++ toString f ++ " " ++ o ++ " " ++ toString g ++ ")"

-- TODO: dual, horizon, subf, subfmap
instance (PSLFragment prop) => PSLFragment (Expression prop) where
  dual = id
  horizon = horizon'
  subf x = subf' x
  subfmap _ = id
  parser = formula
  constant = mkProp . constant
  constr = constr'

horizon' (Conc x y) = horizon' x + horizon' y + 1
horizon' (OConc x y) = horizon' x + horizon' y
horizon' (Prop y) = horizon y
horizon' (Union x y) = max (horizon' x) (horizon' y)
horizon' (Intersection x y) = max (horizon' x) (horizon' y)
horizon' (Epsilon) = 0
horizon' (Kleene b x) = (((horizon' x)+1) * b) - 1

-------------------------------------------------------------------------------

subf' (Conc x y) = [x,y]
subf' (OConc x y) = [x,y]
subf' (Union x y) = [x,y]
subf' (Intersection x y) = [x,y]
subf' (Kleene _ x) = [x]
subf' _ = []

allProps :: (PSLFragment prop) => Expression prop -> [Expression prop]
allProps h = case h of
  (Prop _) -> return h
  _        -> subf h >>= allProps

constr' f = case f of
    (Prop _) -> const f
    (Conc _ _)         -> bin mkConc
    (OConc _ _)        -> bin mkOConc
    (Union _ _)        -> bin mkUnion
    (Intersection _ _) -> bin mkIntersection
    (Epsilon)          -> const f
    (Kleene b _)       -> bun mkKleene b
    where
    bun c b = \(x:_) -> c b x
    bin c = \(l:r:_) -> c l r

-------------------------------------------------------------------------------
-- Expression-Parser

formula :: (PSLFragment prop) => P.CharParser () (Expression prop)
formula =
  buildExpressionParser table aexpr
  P.<?> "Expression: formula"

aexpr :: (PSLFragment prop) => P.CharParser () (Expression prop)
aexpr =  P.try (parser >>= return . mkProp)
        P.<|> (reserved "epsilon" >> return mkEpsilon)
        P.<|> parens formula
        P.<?> "Expression: atomic expression"


table :: (PSLFragment prop) => [[Operator Char () (Expression prop)]]
table = [ [repetition]
        , [kleene]
        , [binary "|" mkUnion AssocLeft, binary "&&" mkIntersection AssocLeft]
        , [binary ";" mkConc AssocLeft, binary ":" mkOConc AssocLeft]
        ]

binary name fun assoc  = Infix (reservedOp name >> return fun) assoc
aBinary name fun assoc = Infix (reservedOp name >> return fun) assoc
repetition             = Postfix (squares parseOrdinal >>= return . mkKleene)
kleene                 = Postfix (reservedOp "*" >> return (mkKleene Infty))

