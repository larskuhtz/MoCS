/* *** MOCS-COPYRIGHT-NOTICE-BEGIN ***
 *
 * This copyright notice is auto-generated by ./add-copyright-notice.
 * Additional copyright notices must be added below the last line of this notice.
 *
 * MoCS (https://lewis.cs.uni-saarland.de/tools/mocs/): "c-bindings/aa_c.cc".
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
#include "vhdl.hh"

#include "aa.h"

namespace
{
    std::vector<long> mk_cpp_prop(const c_prop_t p, long dim)
    {
        std::vector<long> ret(dim);
        for (int i = 0; i < dim; ++i) ret[i] = p[i];
        return ret;
    }

};

using namespace c::context;

// constructors/ destructors
void free_automaton(automaton_ptr a)
{
    delete a;
}

// Get Context
context_ptr get_aut_context(automaton_ptr a)
{
    return new context_t(a->init->efac());
}

// SImplies
automaton_ptr simplies_aut(const automaton_ptr a, const automaton_ptr b)
{
    return new automaton_t(simplies_aut(*a,*b));
}

// Abort 
automaton_ptr abort_aut(const automaton_ptr a, const automaton_ptr b)
{
    return new automaton_t(abort_aut(*a,*b));
}

// Concatenation
automaton_ptr conc_aut(const automaton_ptr a, const automaton_ptr b)
{
    return new automaton_t(conc_aut(*a,*b));
}

// Overlapping Concatenation
automaton_ptr oconc_aut(const automaton_ptr a, const automaton_ptr b)
{
    return new automaton_t(oconc_aut(*a,*b));
}

// Kleene-Closure
automaton_ptr kleene_aut(const automaton_ptr a)
{
    return new automaton_t(kleene_aut(*a));
}

// Union
automaton_ptr union_aut(const automaton_ptr a, const automaton_ptr b)
{
    return new automaton_t(union_aut(*a,*b));
}

// Empty
automaton_ptr empty_aut(context_ptr c)
{
    return new automaton_t(empty_aut(c->impl));
}

// Proposition
automaton_ptr prop_aut(context_ptr c, const c_prop_t p, int dim)
{
    return new automaton_t(prop_aut(c->impl, mk_cpp_prop(p,dim)));
}

// Negated Proposition
automaton_ptr nprop_aut(context_ptr c, const c_prop_t p, int dim)
{
    return new automaton_t(nprop_aut(c->impl, mk_cpp_prop(p,dim)));
}

// Constant
automaton_ptr const_aut(context_ptr c, int p)
{
    return new automaton_t(const_aut(c->impl,(bool) p));
}

// Conjunction
automaton_ptr and_aut(const automaton_ptr a, const automaton_ptr b)
{
    return new automaton_t(*a && *b);
}

// Disjunction
automaton_ptr or_aut(const automaton_ptr a, const automaton_ptr b)
{
    return new automaton_t(*a || *b);
}

// Negation (dualization)
automaton_ptr dual_aut(const automaton_ptr a)
{
    return new automaton_t(~ *a);
}

// Implication
automaton_ptr implies_aut(const automaton_ptr a, const automaton_ptr b)
{
    return new automaton_t(*a >> *b);
}


// Next
automaton_ptr next_aut(const automaton_ptr a) 
{
    return new automaton_t(next_aut(*a));
}

automaton_ptr bnext_aut(int i, const automaton_ptr a) 
{
    return new automaton_t(next_aut(i,*a));
}

automaton_ptr wnext_aut(const automaton_ptr a)
{
    return new automaton_t(wnext_aut(*a));
}

automaton_ptr bwnext_aut(int i, const automaton_ptr a)
{
    return new automaton_t(wnext_aut(i,*a));
}


// Globally
automaton_ptr g_aut(const automaton_ptr a) 
{
    return new automaton_t(g_aut(*a));
}
automaton_ptr bg_aut(int i, const automaton_ptr a) 
{
    return new automaton_t(g_aut(i,*a));
}
automaton_ptr strong_bg_aut(int i, const automaton_ptr a) 
{
    return new automaton_t(strong_g_aut(i,*a));
}

// Eventually
automaton_ptr f_aut(const automaton_ptr a)
{
    return new automaton_t(f_aut(*a));
}
automaton_ptr bf_aut(int i, const automaton_ptr a)
{
    return new automaton_t(f_aut(i,*a));
}
automaton_ptr weak_bf_aut(int i, const automaton_ptr a)
{
    return new automaton_t(weak_f_aut(i,*a));
}

// Until
automaton_ptr until_aut(const automaton_ptr a,const automaton_ptr b)
{
    return new automaton_t(until_aut(*a,*b));
}

automaton_ptr buntil_aut(int i, const automaton_ptr a,const automaton_ptr b)
{
    return new automaton_t(until_aut(i,*a,*b));
}

automaton_ptr waitfor_aut(const automaton_ptr a,const automaton_ptr b)
{
    return new automaton_t(waitfor_aut(*a,*b));
}

automaton_ptr bwaitfor_aut(int i, const automaton_ptr a,const automaton_ptr b)
{
    return new automaton_t(waitfor_aut(i,*a,*b));
}


// Release
automaton_ptr release_aut(const automaton_ptr a,const automaton_ptr b)
{
    return new automaton_t(release_aut(*a,*b));
}

automaton_ptr brelease_aut(int i, const automaton_ptr a,const automaton_ptr b)
{
    return new automaton_t(release_aut(i,*a,*b));
}

automaton_ptr strong_release_aut(const automaton_ptr a,const automaton_ptr b)
{
    return new automaton_t(strong_release_aut(*a,*b));
}

automaton_ptr strong_brelease_aut(int i, const automaton_ptr a,const automaton_ptr b)
{
    return new automaton_t(strong_release_aut(i,*a,*b));
}

// make weak
automaton_ptr weak_aut(const automaton_ptr a)
{
    return new automaton_t(weak_aut(*a));
}

// Universalization
automaton_ptr universal_aut(const automaton_ptr a)
{
    return new automaton_t(universal_aut(*a));
}

// Rollout
//template<class T>
//context_t suffix_rollout(const automaton_t&, int, std::back_insert_iterator<T>);
// returns null-terminated array of expression_ptr
expr_ptr* aut_suffix_rollout(const automaton_ptr a, int b)
{
    std::list<expr_t> l;
    suffix_rollout(*a,b,std::back_inserter(l));
    expr_ptr* res = (expr_ptr*) malloc((sizeof(expr_ptr) * (l.size() + 1)));
    int i = 0;
    foreach(const expr_t& e, l) {
        res[i++] = new expr_t(e);
    }
    res[i] = 0;
    return res;
}


expr_ptr aut_prefix_rollout(const automaton_ptr a, int b)
{
    return new expr_t(prefix_rollout(*a,b).first);
}

// pretty-printing
void automaton2dot(const char* file_name, const automaton_ptr a, const char* name)
{
    std::ofstream file(file_name);
    if (file) {
        automaton2dot(file,*a,name);
    } else {
        std::cerr << "Error: Failed to open file for writing." << std::endl;
    }
}


void vhdl_build_monitor(const char* file_name,
                        int b,
                        int pc,
                        int sc,
                        expr_ptr po,      // prefix automaton output expr
                        automaton_ptr pa, // prefix automaton
                        expr_ptr** so)       // array of array of suffix output expr
{
    std::ofstream file(file_name);
    if (file) {
        std::list<std::list<expr_t> > ll;
        while(*so) {
            std::list<expr_t> l;
            expr_ptr* e = *so++;
            while (*e) {
                l.push_back(**e++);
            }
            ll.push_back(l);
        }
        vhdl::monitor(file,b,pc,sc,*po,*pa,ll);
    } else {
        std::cerr << "Error: Failed to open file " << file_name << " for writing." << std::endl;
    }
}
