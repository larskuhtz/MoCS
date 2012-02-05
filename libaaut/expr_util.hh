/* *** MOCS-COPYRIGHT-NOTICE-BEGIN ***
 *
 * This copyright notice is auto-generated by ./add-copyright-notice.
 * Additional copyright notices must be added below the last line of this notice.
 *
 * MoCS (https://lewis.cs.uni-saarland.de/tools/mocs/): "libaaut/expr_util.hh".
 * The content of this file is copyright of Saarland University -
 * Copyright (C) 2009 Saarland University, Reactive Systems Group, Lars Kuhtz <kuhtz@cs.uni-sb.de>.
 *
 * This file is part of MoCS (https://lewis.cs.uni-saarland.de/tools/mocs/).
 *
 * License: three-clause BSD style license.
 * The license text can be found in the file LICENSE.
 *
 * *** MOCS-COPYRIGHT-NOTICE-END *** */

#ifndef _EXPR_UTIL_HH
#define _EXPR_UTIL_HH

#include "config.h"

#ifdef NOREC_DISTR
#include <queue>
#endif

#include "expr.hh"


namespace expr { namespace util
{
    RUNTIME_ERROR(expr_type_error);

    /* Makes an expression weak. recurses on states, detects cycles. */
    inline expr_t weak(const expr_t& e);

    /* Dualizes an expression. recurses on states, detects cycles.*/
    expr_t dual(const expr_t& e);

    
    /* Collects all conjuncts of a conjunction */
    template<class T> void get_conj(const expr_t& e, std::back_insert_iterator<T> i);

    /* Collects all states from a disjunction over states */
    template<class T> void get_disj_states(const expr_t& e, std::insert_iterator<T> i);

    /* splits a Disjunction into a disjunction of states and the rest */
    std::pair<expr_t,expr_t> split_disj(const expr_t& e, const efac_ptr& state_efac, const efac_ptr& rest_efac);

    /* Recursively distributes disjuncts over a conjunction (used by mkCNF) */
    expr_t distr_disj(const expr_t& a, const expr_t& b);

    /* Transforms an expr into CNF */
    expr_t mkCNF(const expr_t& e, const efac_ptr& out_efac);


    /* Collects all disjuncts of a conjunction */
    template<class T> void get_disj(const expr_t& e, std::back_insert_iterator<T> i);

    /* Collects all states from a conjunction over states */
    template<class T> void get_disj_states(const expr_t& e, std::insert_iterator<T> i);

    /* splits a Conjunction into a conjcution of states and the rest */
    std::pair<expr_t,expr_t> split_disj(const expr_t& e, const efac_ptr& state_efac, const efac_ptr& rest_efac);

    /* Recursively distributes conjuncts over a disjunction (used by mkCNF) */
    expr_t distr_disj(const expr_t& a, const expr_t& b);

    /* Transforms an expr into DNF */
    expr_t mkDNF(const expr_t& e, const efac_ptr& out_efac);

    
    /* Collects all successor states */
    template<class T>
    void successor_states(const state_t& s, std::back_insert_iterator<T>&);
    
    std::size_t successor_states_count(const state_t& s);
    std::size_t successor_states_count(const expr_t& e);

    /* ************************************************************************** */
    /* expr hash map */

    // from http://www.boost.org/doc/libs/1_38_0/libs/multi_index/doc/tutorial/techniques.html#emulate_assoc_containers
    template <typename T1,typename T2>
    struct mutable_pair
    {
        typedef T1 first_type;
        typedef T2 second_type;

        mutable_pair():first(T1()),second(T2()){}
        mutable_pair(const T1& f,const T2& s):first(f),second(s){}
        mutable_pair(const std::pair<T1,T2>& p):first(p.first),second(p.second){}

        T1         first;
        mutable T2 second;
    };

    struct expr_hasher {
        std::size_t operator() (const expr_t& e) const
        {
            boost::hash<const void*>()(e.uid());
        }
    };

#define EXPR_HASH_MAP(Value) multi_index_container< mutable_pair<expr_t,Value>, indexed_by<hashed_unique<member<mutable_pair<expr_t,Value>,expr_t,&mutable_pair<expr_t,Value>::first>,expr_hasher > > > 


    /* ********************************************************************** */
    /* Implementation */

    namespace 
    {
        struct expr_dualizer
        {
            typedef std::map<state_t,state_t> state_map_t;
            expr_dualizer() : state_map() {}

            expr_t operator()(const expr_t& e) { return rec(e); }

            expr_t rec(const expr_t& x)
            {
                efac_ptr c = x.efac();
                switch (x.op()) {
                case And: return rec(x.left()) || rec(x.right());
                case Or: return rec(x.left()) && rec(x.right());
                case Prop: return c->mkNProp(x.prop());
                case NProp: return c->mkProp(x.prop());
                case Const: return c->mkConst(! x.con());
                case State: state_map_t::const_iterator r = state_map.find(x.state());
                            if (r != state_map.end()) return c->mkState(r->second);
                            state_t s = new_state();
                            state_map.insert(std::make_pair(x.state(),s));
                            *s = rec(*x.state());
                            s.set_final(! x.state().final());
                            return c->mkState(s);
                }
            }
            state_map_t state_map;
        };

        struct expr_rewriter
        {
            typedef std::map<state_t, state_t> state_map_t;
            expr_rewriter(efac_ptr ctx = efac_ptr()) : state_map(), c(ctx) {}

            virtual ~expr_rewriter() { }

            virtual expr_t operator()(const expr_t& e) {
                if (! c ) { 
                    c = e.efac();
                    return rec(e);
                    c = efac_ptr();
                } else {
                    return rec(e);
                }
            }

            virtual expr_t and_rec(const expr_t& x) { return rec(x.left()) && rec(x.right()); }
            virtual expr_t or_rec(const expr_t& x) { return rec(x.left()) || rec(x.right()); }
            virtual expr_t prop_rec(const expr_t& x) { return c->mkProp(x.prop()); }
            virtual expr_t nprop_rec(const expr_t& x) { return c->mkNProp(x.prop()); }
            virtual expr_t const_rec(const expr_t& x) { return c->mkConst(x.con()); }
            virtual expr_t state_rec(const expr_t& x, state_t& new_state) 
            {
                *new_state = rec(*x.state());
                new_state.set_final(x.state().final());
                return c->mkState(new_state);
            }
            virtual expr_t cache_conv(const expr_t& x, const state_t& new_state)
            {
                return c->mkState(new_state);
            }

            expr_t rec(const expr_t& x)
            {
                switch (x.op()) {
                case And: return and_rec(x);
                case Or: return or_rec(x);
                case Prop: return prop_rec(x);
                case NProp: return nprop_rec(x);
                case Const: return const_rec(x);
                case State: state_map_t::const_iterator r = state_map.find(x.state());
                            if (r != state_map.end()) return cache_conv(x,r->second);
                            state_t s = new_state();
                            state_map.insert(std::make_pair(x.state(),s));
                            return state_rec(x,s);
                }
            }
            state_map_t state_map;
            efac_ptr c;
        };
    };

    inline expr_t dual(const expr_t& e) 
    {
        return expr_dualizer()(e);
    }

    // make expr (automaton) weak
    namespace 
    {
        struct expr_weaker : expr_rewriter
        {
            expr_t state_rec(const expr_t& x, state_t& ns)
            {
                *ns = rec(*x.state());
                ns.set_final();
                return c->mkState(ns);
            }
        };
    };

    inline expr_t weak(const expr_t& e) 
    {
        return expr_weaker()(e);
    }

/* ************************************************************************** */

    // CNF
    // TODO do we need to distribut state-less expressions?
    // would be more efficient to not touch them at all???
    // TODO check naming of subroutines (e.g. split_disj is missleading or at least ambigious)

    template<class T>
    void get_conj(const expr_t& e, std::back_insert_iterator<T> i)
    {
        switch (e.op()) {
        case And: get_conj(e.left(),i); get_conj(e.right(),i); break;
        default: i = e; break;
        }
    }

    template<class T>
    void get_disj_states(const expr_t& e, std::insert_iterator<T> i)
    {
        switch (e.op()) {
        case Or: get_disj_states(e.left(),i); get_disj_states(e.right(),i); break;
        case State: i = e.state(); break;
        default: throw expr_type_error("Expr should be a disjunction of states.");
        }
    }

    // first: states, second: rest
    inline std::pair<expr_t,expr_t> split_disj(const expr_t& e, 
                                               const efac_ptr& state_efac, 
                                               const efac_ptr& rest_efac)
    {
        std::pair<expr_t,expr_t> r,l;
        switch (e.op()) {
        case And: throw expr_type_error("In a CNF a conjunct must not contain conjunctions");
        case Or: l = split_disj(e.left(),state_efac,rest_efac);
                 r = split_disj(e.right(),state_efac,rest_efac);
                 return std::make_pair(l.first || r.first, l.second || r.second);
        case Prop: return std::make_pair(expr_t(),rest_efac->mkProp(e.prop()));
        case NProp: return std::make_pair(expr_t(),rest_efac->mkNProp(e.nprop()));
        case Const: return std::make_pair(expr_t(),rest_efac->mkConst(e.con()));
        case State: return std::make_pair(state_efac->mkState(e.state()),expr_t());
        }
    }

#ifdef NOREC_DISTR
    inline expr_t distr_disj(const expr_t& a, const expr_t& b)
    {
        std::queue<std::pair<expr_t,expr_t> > q;
        q.push(std::make_pair(a,b));
        expr_t result;
        while (! q.empty()) {
            expr_t a = q.front().first;
            expr_t b = q.front().second;
            q.pop();
            if (a.op() == And) {
                q.push(std::make_pair(a.left(),b));
                q.push(std::make_pair(a.right(),b));
            } else if (b.op() == And) {
                q.push(std::make_pair(a,b.left()));
                q.push(std::make_pair(a,b.right()));
            } else {
                result = result && (a || b);
            }
        }
        return result;
    }
#else
    inline expr_t distr_disj(const expr_t& a, const expr_t& b)
    {
        if (a.op() == And) {
            return distr_disj(a.left(),b) && distr_disj(a.right(),b);
        } else if (b.op() == And) {
            return distr_disj(a,b.left()) && distr_disj(a,b.right());
        } else {
            return a || b;
        }
    }
#endif


    inline expr_t mkCNF(const expr_t& e, const efac_ptr& out_efac)
    {
        switch (e.op()) {
        case And: return mkCNF(e.left(),out_efac) && mkCNF(e.right(),out_efac);
        case Or: return distr_disj (mkCNF(e.left(),out_efac), mkCNF(e.right(),out_efac));
        case Prop: return out_efac->mkProp(e.prop());
        case NProp: return out_efac->mkNProp(e.nprop());
        case Const: return out_efac->mkConst(e.con());
        case State: return out_efac->mkState(e.state()); 
        }
    }

/* ************************************************************************** */

    template<class T>
    void get_disj(const expr_t& e, std::back_insert_iterator<T> i)
    {
        switch (e.op()) {
        case Or: get_disj(e.left(),i); get_disj(e.right(),i); break;
        default: i = e; break;
        }
    }

    template<class T>
    void get_conj_states(const expr_t& e, std::insert_iterator<T> i)
    {
        switch (e.op()) {
        case And: get_conj_states(e.left(),i); get_conj_states(e.right(),i); break;
        case State: i = e.state(); break;
        default: throw expr_type_error("Expr should be a conjunction of states.");
        }
    }

    // first: states, second: rest
    inline std::pair<expr_t,expr_t> split_conj(const expr_t& e, 
                                               const efac_ptr& state_efac, 
                                               const efac_ptr& rest_efac)
    {
        std::pair<expr_t,expr_t> r,l;
        switch (e.op()) {
        case Or: throw expr_type_error("in a DNF a disjunct must not contain disunctions");
        case And: l = split_conj(e.left(),state_efac,rest_efac);
                  r = split_conj(e.right(),state_efac,rest_efac);
                  return std::make_pair(l.first && r.first, l.second && r.second);
        case Prop: return std::make_pair(expr_t(),rest_efac->mkProp(e.prop()));
        case NProp: return std::make_pair(expr_t(),rest_efac->mkNProp(e.nprop()));
        case Const: return std::make_pair(expr_t(),rest_efac->mkConst(e.con()));
        case State: return std::make_pair(state_efac->mkState(e.state()),expr_t());
        }
    }

#ifdef NOREC_DISTR
    inline expr_t distr_conj(const expr_t& a, const expr_t& b)
    {
        std::queue<std::pair<expr_t,expr_t> > q;
        q.push(std::make_pair(a,b));
        expr_t result;
        while (! q.empty()) {
            expr_t a = q.front().first;
            expr_t b = q.front().second;
            q.pop();
            if (a.op() == Or) {
                q.push(std::make_pair(a.left(),b));
                q.push(std::make_pair(a.right(),b));
            } else if (b.op() == Or) {
                q.push(std::make_pair(a,b.left()));
                q.push(std::make_pair(a,b.right()));
            } else {
                result = result || (a && b);
            }
        }
        return result;
    }
#else
    inline expr_t distr_conj(const expr_t& a, const expr_t& b)
    {
        if (a.op() == Or) {
            return distr_conj(a.left(),b) || distr_conj(a.right(),b);
        } else if (b.op() == Or) {
            return distr_conj(a,b.left()) || distr_conj(a,b.right());
        } else {
            return a && b;
        }
    }
#endif

    inline expr_t mkDNF(const expr_t& e, const efac_ptr& out_efac)
    {
        switch (e.op()) {
        case Or: return mkDNF(e.left(),out_efac) || mkDNF(e.right(),out_efac);
        case And: return distr_conj (mkDNF(e.left(),out_efac), mkDNF(e.right(),out_efac));
        case Prop: return out_efac->mkProp(e.prop());
        case NProp: return out_efac->mkNProp(e.nprop());
        case Const: return out_efac->mkConst(e.con());
        case State: return out_efac->mkState(e.state()); 
        }
    }

/* ************************************************************************** */

    namespace {
        template<class T>
        void suc_state_rec(const expr_t& e, std::back_insert_iterator<T>& i)
        {
            switch (e.op()) {
            case And: 
            case Or: suc_state_rec(e.left(),i); suc_state_rec(e.right(),i); break;
            case State: i = e.state(); break;
            }
        }

        std::size_t suc_state_count_rec(const expr_t& e)
        {
            switch (e.op()) {
            case And: 
            case Or: return suc_state_count_rec(e.left()) + suc_state_count_rec(e.right());
            case State: return 1;
            default: return 0;
            }
        }
    };

    template<class T>
    inline void successor_states(const state_t& s, std::back_insert_iterator<T>& i)
    {
        suc_state_rec(*s, i);
    }

    inline std::size_t successor_states_count(const state_t& s)
    {
        return suc_state_count_rec(*s);
    }

    inline std::size_t successor_states_count(const expr_t& e)
    {
        return suc_state_count_rec(e);
    }

};}; // namespace expr::util

#endif // _EXPR_UTIL_HH