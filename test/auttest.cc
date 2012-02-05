/* *** MOCS-COPYRIGHT-NOTICE-BEGIN ***
 *
 * This copyright notice is auto-generated by ./add-copyright-notice.
 * Additional copyright notices must be added below the last line of this notice.
 *
 * MoCS (https://lewis.cs.uni-saarland.de/tools/mocs/): "test/auttest.cc".
 * The content of this file is copyright of Saarland University -
 * Copyright (C) 2009 Saarland University, Reactive Systems Group, Lars Kuhtz <kuhtz@cs.uni-sb.de>.
 *
 * This file is part of MoCS (https://lewis.cs.uni-saarland.de/tools/mocs/).
 *
 * License: three-clause BSD style license.
 * The license text can be found in the file LICENSE.
 *
 * *** MOCS-COPYRIGHT-NOTICE-END *** */

#include <fstream>
#include "aa.hh"

prop_t prop(int i)
{
    return prop_t(1,i);
}

int main(int argsc, char ** args)
{
    typedef automaton_t A;
    efac_ptr c = efac::newEfac();

    A b0 = prop_aut(c,prop(0));
    A b1 = prop_aut(c,prop(1));
    A b2 = prop_aut(c,prop(2));
    A b3 = prop_aut(c,prop(3));
    A b4 = prop_aut(c,prop(4));

    std::ofstream f0("x.dot");
    std::ofstream f1("ux.dot");

    A a5 = ( + * b0);
    std::string n = "(F G b0)";
    if (argsc > 1) {
        std::string arg(args[1]);
        if (arg == "G") {
            a5 = *b0;
            n = "G(0)";
        } else if ( arg == "F") {
            a5 = +b0;
            n = "F(0)";
        } else if ( arg == "GF") {
            a5 = *+b0;
            n = "GF(0)";
        } else if ( arg == "FG") {
            a5 = +*b0;
            n = "FG(0)";
        } else if ( arg == "p") {
            a5 = b0;
            n = "0";
        } else if ( arg == "or") {
            a5 = b0 || b1;
            n = "0 or 1";
        } else if ( arg == "andor") {
            a5 = b0 && (b1 || b2);
            n = "0 and (1 or 2)";
        } else if ( arg == "orand") {
            a5 = b0 || (b1 && b2);
            n = "0 or (1 and 2)";
        } else if ( arg == "until") {
            a5 = until_aut(b0,b1);
            n = "0 until 1";
        } else if ( arg == "untilGF") {
            a5 = until_aut( *b0, +b1);
            n = "(G 0) until (F 1)";
        }
    }

    automaton2dot(f0,a5, n);

    A a6 = universal_aut( a5  );
    automaton2dot(f1,a6, "universal " + n);

    f0.close();
    f1.close();
    return 0;
}