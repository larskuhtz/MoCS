/* *** MOCS-COPYRIGHT-NOTICE-BEGIN ***
 *
 * This copyright notice is auto-generated by ./add-copyright-notice.
 * Additional copyright notices must be added below the last line of this notice.
 *
 * MoCS (https://lewis.cs.uni-saarland.de/tools/mocs/): "libaaut/state-machine.hh".
 * The content of this file is copyright of Saarland University -
 * Copyright (C) 2009 Saarland University, Reactive Systems Group, Lars Kuhtz <kuhtz@cs.uni-sb.de>.
 *
 * This file is part of MoCS (https://lewis.cs.uni-saarland.de/tools/mocs/).
 *
 * License: three-clause BSD style license.
 * The license text can be found in the file LICENSE.
 *
 * *** MOCS-COPYRIGHT-NOTICE-END *** */

#ifndef _STATE_MACHINE_HH_
#define _STATE_MACHINE_HH_

#include "config.h"

#include "expr.hh"
#include "aa.hh"

/* A state machine
 *
 * Essentially the same data-structure as an (universal) automaton.
 * However, the suc (successors/ postcondition) member of states is interpreted as 
 * pre (predecessor-relation/ precondition).
 *
 * This allows for a straight forward implementation into circuits.
 *
 */

struct state_machine_t
{
    state_machine_t(const automaton_t& a) : init(), states() {init_state_machine(a);}
    state_t init;
    std::list<state_t> states;
    std::map<const void*,const void*> smap;

    void init_state_machine(const automaton_t& a);
};

// pretty-printing
std::ostream& state_machine2dot(std::ostream& o, const state_machine_t& a, const std::string& name = "state machine");


#endif // _STATE_MACHINE_HH_
