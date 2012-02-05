{- *** MOCS-COPYRIGHT-NOTICE-BEGIN ***
 -
 - This copyright notice is auto-generated by ./add-copyright-notice.
 - Additional copyright notices must be added below the last line of this notice.
 -
 - MoCS (https://lewis.cs.uni-saarland.de/tools/mocs/): "haskell-frontend/PSLLexer.hs".
 - The content of this file is copyright of Saarland University -
 - Copyright (C) 2009 Saarland University, Reactive Systems Group, Lars Kuhtz <kuhtz@cs.uni-sb.de>.
 -
 - This file is part of MoCS (https://lewis.cs.uni-saarland.de/tools/mocs/).
 -
 - License: three-clause BSD style license.
 - The license text can be found in the file LICENSE.
 -
 - *** MOCS-COPYRIGHT-NOTICE-END *** -}

module PSLLexer where

--import Prelude
import Text.ParserCombinators.Parsec
import qualified Text.ParserCombinators.Parsec.Token as T

-------------------------------------------------------------------------------
-- Lexer

lexer = T.makeTokenParser lang
  where
  lang = T.LanguageDef {
    T.commentStart = "{-",
    T.commentEnd = "-}",
    T.commentLine = "--",
    T.nestedComments = True,
    T.identStart = (letter <|> char '_'),
    T.identLetter = (alphaNum <|> char '_'),
--    T.opStart = oneOf "\\*;:|&",
    T.opStart = letter,
    T.opLetter = pzero,
    T.reservedNames = ["true","false","epsilon"],
    T.reservedOpNames = [ 
                          -- PSL
                          "\\strong"

                        , "\\not"
                        , "\\and"
                        , "\\or"

                        , "\\implies"
                        , "\\equiv"
                        
                        , "\\N"
                        , "\\BN"
                        , "\\wN"
                        , "\\wBN"
                        
                        , "\\G"
                        , "\\BG"
                        , "\\sBG"
                        , "\\F"
                        , "\\BF"
                        , "\\wBF"

                        , "\\U"
                        , "\\W"
                        , "\\BU"
                        , "\\BW"

                        , "\\R"
                        , "\\sR"
                        , "\\BR"
                        , "\\sBR"

                        , "\\simplies"
                        , "\\abort"

                        -- Expression
                        , "+"
                        , "^"
                        , "-"

                        -- SERE
                        , "|"
                        , "*"
                        , "&&"
                        , ";"
                        , ":"

                        -- bound Expression
                        , "++"
                        ],
    T.caseSensitive = False
    }

identifier = T.identifier lexer
parens = T.parens lexer
brackets = T.brackets lexer
reserved = T.reserved lexer
reservedOp = T.reservedOp lexer
natural = T.natural lexer
whiteSpace = T.whiteSpace lexer
squares = T.squares lexer
