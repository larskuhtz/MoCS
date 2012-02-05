OVERVIEW
========

MoCS -- Monitor Circuits Synthesis for PSL specifications

MoCS compiles monitor circuits from linear time temporal logic
specifications. The input logic is similar to PSL by allowing nesting of
regular expressions within temporal modalities. The output of MoCS is a
synthesizable VHDL description of the monitor circuit. The resulting monitors
implement truncated path semantics. On sequentially reading an input trace a
monitor outputs at each input position whether the current prefix of the trace
satisfies the monitored property. MoCS is particularly careful about generating
small monitors form specifications containing sub-properties of bounded
temporal scope.

Reference: [Bernd Finkbeiner and Lars Kuhtz. Monitor Circuits for LTL with
Bounded and Unbounded Future. Proceedings of RV 09, LNCS 5779, 2009.](http://react.cs.uni-sb.de/publications/FK09.html)

Author: Lars Kuhtz <kuhtz@cs.uni-sb.de>

Original project page: [https://lewis.cs.uni-saarland.de/tools/mocs/]()

IMPORTANT NOTE
--------------

Please note that MoCS is still prototypic phase. Not all operators are
supported in all contexts yet. This is not due to limitations of the approach
but to missing bits in the implementation. If you rely on such a missing bit
please let us know. Simliar statement holds for the technical quality of the
generated VHDL Model.

Currently I am not actively developing MoCS. I release the source code as

-   a reference for the corresponding paper[^1],
-   an inspiration for any related projects, and
-   a starting point if someone want's to pick up MoCS and
    continue development.

[^1]: [Bernd Finkbeiner and Lars Kuhtz. Monitor Circuits for LTL with Bounded 
and Unbounded Future. Proceedings of RV 09, LNCS 5779, 2009.](http://react.cs.uni-sb.de/publications/FK09.html)

COPYRIGHT AND LICENSE
=====================

All files in the orginial release of  MoCS are copyright of Saarland University.

Copyright (C) 2009 Saarland University, Reactive Systems Group, Lars Kuhtz <kuhtz@cs.uni-sb.de>.

License: three-clause BSD style.

The license text can be found in the file LICENSE.

INSTALLATION FROM SOURCE
========================

1.  Edit the file config.make

2.  To build everything run:

        make all

USAGE
=====

    MoCS [--ouput FILE] FILE
    MoCS [--output FILE] --formula STRING
    MoCS [--output FILE]"
    MoCS --help
    MoCS --version

Syntax for a formulas:
----------------------

    Formula ::= '(' Formula ')' | RegEx | '\strong' Regex 
             | PrefixOp Formula | BPrefixOp BoundExpr Formula 
             | Formula BinaryOp Formula | Formula BBinaryOp BoundExpr Formula
             | Formula '\abort' Prop | RegEx '\simplies' Formula

    PrefixOp ::= '\not' | '\N' | '\wN' | '\G' | '\F'
    BPreifxOp ::= '\BN' | '\wBN' | '\BG' | '\BF' | '\sBG' | '\wBF' 

    BinaryOp ::= '\and' | '\or' | '\implies' | '\equiv' | '\U' | '\W' | '\R' | '\sR'
    BBinaryOp ::= '\BU' | '\BR' | '\BW' | '\SBR'

    Prop ::= '(' Prop ')' | Var | 'true' | 'false' | '-' Prop 
           | Prop '+' Prop | Prop '^' Prop

    RegEx ::= '(' RegEx ')' | Prop | 'epsilon' 
            | RegEx '&&' RegEx | RegEx '|' RegEx | RegEx ':' RegEx | RegEx ';' RegEx
            | RegEx '*' | RegEx '[' Ordinal ']'

    Var ::= "[_a-zA-Z][_a-zA-Z0-9]*"

    BoundExpr ::= '(' BoundExpr ')' | Ordinal
                | BoundExpr '*' BoundExpr | '-' BoundExpr | BoundExpr '++' 

    Ordinal ::= [0-9]+ | 'infty'"

