/* *** MOCS-COPYRIGHT-NOTICE-BEGIN ***
 *
 * This copyright notice is auto-generated by ./add-copyright-notice.
 * Additional copyright notices must be added below the last line of this notice.
 *
 * MoCS (https://lewis.cs.uni-saarland.de/tools/mocs/): "libaaut/state-machine.cc".
 * The content of this file is copyright of Saarland University -
 * Copyright (C) 2009 Saarland University, Reactive Systems Group, Lars Kuhtz.
 *
 * This file is part of MoCS (https://lewis.cs.uni-saarland.de/tools/mocs/).
 *
 * License: three-clause BSD style license.
 * The license text can be found in the file LICENSE.
 *
 * *** MOCS-COPYRIGHT-NOTICE-END *** */

#include "state-machine.hh"

/* Precondition: The input automaton is universal
 *
 * FIXME: documentation is outdated!
 *
 * We need to "invert" suc-expressions:
 * Do a dfs where at
 * -> state:
 *    * get all conj (a "maximal/ first" non-and-node)
 *    * register at each conj
 * -> conj (register carrying node)
 *    * split into (se,re) ( (state-expression, rest), disjunction)
 *    * if se is empty (se == expr_t()) then FIXME
 *    * build (not re) and register at conj
 *    * register conj at se-input-or
 *    * insert (not re) into input-logic-list
 *
 * state machine generation:
 * state:
 *   -> input: state-input-or
 *
 * state-input-or:
 *   -> or-expr:
 *      input: all registered exprs
 *
 * conj (register carrying node):
 *   -> and-expr:
 *      input: registered exprs
 *
 * each expr in input-logic-list:
 *   -> copy
 *
 */

namespace 
{
    typedef const void* reg_key_t;
    typedef std::map<reg_key_t,state_t> state_map_t;
    typedef std::set<reg_key_t> visited_t;

    state_t new_state(const state_t& old, state_map_t& state_map, const efac_ptr& new_efac)
    {
        // new state
        state_map_t::const_iterator it = state_map.find(old.uid()); 
        if (it != state_map.end()) {
            return it->second;
        } else {
            state_t nstate = new_state(new_efac->mkFalse(),old.final());
            state_map[old.uid()] = nstate;
            return nstate;
        }
    }

    void invert_dfs1(const expr_t& cur, const expr_t& state_expr, visited_t& visited, state_map_t& state_map, const efac_ptr& new_efac);

    void invert_dfs0(const state_t& cur, visited_t& visited, state_map_t& state_map, const efac_ptr& new_efac)
    {
        // check for recursion end
        if (visited.find(cur.uid()) != visited.end()) {
            return;
        } else {
            visited.insert(cur.uid());
        }

        state_t nstate = new_state(cur,state_map,new_efac);

        // get conjuncts
        std::list<expr_t> l;
        expr::util::get_conj(*cur,std::back_inserter(l));

        // recurse on conjuncts
        foreach(expr_t& c, l) {
            invert_dfs1(c,new_efac->mkState(nstate),visited,state_map,new_efac);
        }
    }

    void invert_dfs1(const expr_t& cur, const expr_t& state_expr, visited_t& visited, state_map_t& state_map, const efac_ptr& new_efac)
    {
        // split conjunct into state and rest
        expr_t se, re;
        boost::tie(se,re) = expr::util::split_disj(cur,cur.efac(),new_efac);
        if (se == expr_t()) {
            std::cerr << "FIXME, conjunct without state ..." << std::endl;
        } else {
            ASSERT(se.op() == State); // is se a singelton ?
            expr_t tmp = expr::util::dual(new_efac->mkFalse() || re); // why the disjunction?

            state_t nse = new_state(se.state(),state_map,new_efac);
            nse.set_suc(*nse || (state_expr && tmp));

            // recurse on se
            invert_dfs0(se.state(), visited, state_map, new_efac);
        }
    }
}; // annonymous


void state_machine_t::init_state_machine(const automaton_t& a)
{
    // new_efac
    efac_ptr new_efac = efac::newEfac();

    // run dfs
    state_map_t state_map;
    visited_t visited;
    invert_dfs0(a.init,visited,state_map,new_efac);

    foreach (const state_map_t::value_type& v, state_map) {
        // std::cerr << (long) v.second.uid() << ": " << *v.second << std::endl;
        states.push_back(v.second);
        smap[v.second.uid()] = v.first;
    }
    init = state_map[a.init.uid()];
}

std::ostream& state_machine2dot(std::ostream& o, const state_machine_t& a, const std::string& name)
{
    std::list<expr_t> l;
    l.push_back(a.init->efac()->mkState(a.init));
    foreach(const std::list<state_t>::value_type& s, a.states) {
        if (s != a.init) l.push_back(a.init->efac()->mkState(s));
    }
    return exprs2dot(o,l,name); 
}
