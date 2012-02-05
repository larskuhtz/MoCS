/* *** MOCS-COPYRIGHT-NOTICE-BEGIN ***
 *
 * This copyright notice is auto-generated by ./add-copyright-notice.
 * Additional copyright notices must be added below the last line of this notice.
 *
 * MoCS (https://lewis.cs.uni-saarland.de/tools/mocs/): "libaaut/expr.tcc".
 * The content of this file is copyright of Saarland University -
 * Copyright (C) 2009 Saarland University, Reactive Systems Group, Lars Kuhtz.
 *
 * This file is part of MoCS (https://lewis.cs.uni-saarland.de/tools/mocs/).
 *
 * License: three-clause BSD style license.
 * The license text can be found in the file LICENSE.
 *
 * *** MOCS-COPYRIGHT-NOTICE-END *** */

/* ************************************************************************** */
/* expr_impl_t */

class expr_impl_t {

   friend const expr_impl_ptr check_hash(const expr_impl_t& expr);
   friend std::size_t hash_value(const expr_impl_t& e);
   friend std::ostream& operator<<(std::ostream& o, const expr_impl_t& e);
   friend expr_t mkProp(prop_t p);
   friend expr_t mkNProp(prop_t p);
   friend expr_t mkState(state_t state);
   friend class expr_t;
   friend std::ostream& rec_expr2dot(std::ostream& o, const expr_t& e);
   friend class dfs;

public:
    bool operator==(const expr_impl_t& r) const
    {
        /*
        debug();
        r.debug();
        std::cerr << "\n";
        */
        return 
            hash == r.hash &&
            op == r.op &&
            con == r.con &&
            state == r.state &&
            prop == r.prop &&
            left == r.left &&
            right == r.right;
    }

    void debug() const
    {
        std::cerr << "===============\n";
        std::cerr << "op:    " << op << "\n";
        std::cerr << "con:   " << con << "\n";
        std::cerr << "state: " << state.uid() << "\n";
        std::cerr << "prop:  " << prop << "\n";
        std::cerr << "left:  " << left << "\n";
        std::cerr << "right: " << right << "\n";
        std::cerr << "hash:  " << hash << "\n";
    }


//protected:

    const op_t op;
    const bool con;
    const state_t state;
    const prop_t prop;
    const expr_impl_ptr left;
    const expr_impl_ptr right;
    const std::size_t hash;

    mutable int ref_c;
    weak_efac_ptr eptr;

    ~expr_impl_t() {}

    expr_impl_t(const expr_impl_t& e) :
        op(e.op),
        con(e.con),
        state(e.state),
        prop(e.prop),
        left(e.left),
        right(e.right),
        hash(e.hash),
        ref_c(0),
        eptr(e.eptr)
    {}

    expr_impl_t(op_t op,
                bool con,
                state_t state,
                prop_t prop,
                weak_efac_ptr eptr,
                const expr_impl_ptr left = expr_impl_ptr(),
                const expr_impl_ptr right = expr_impl_ptr()) :
        op(op),
        con(con),
        state(state),
        prop(prop),
        left(left),
        right(right),
        hash(make_hash(op,con,state,prop,left,right)),
        ref_c(0),
        eptr(eptr)
    {
#if 0
        using namespace std::rel_ops;
        assert((left == expr_impl_ptr() && right == expr_impl_ptr()) ||
               (left != expr_impl_ptr() && right != expr_impl_ptr()));
        assert(left != expr_impl_ptr() || (eptr == left->eptr && eptr == right->eptr));
#endif
    }

    std::size_t make_hash(op_t op,
                          bool con,
                          state_t state,
                          prop_t prop,
                          const expr_impl_ptr left,
                          const expr_impl_ptr right)
    {
        std::size_t s = 0;
        boost::hash_combine(s, op);
        boost::hash_combine(s, con);
        boost::hash_combine(s, state);
        boost::hash_combine(s, prop);
        if (left) boost::hash_combine(s, *left);
        if (right) boost::hash_combine(s, *right);
        return s;
    }
};

inline std::size_t hash_value(const expr_impl_t& e)
{
    return e.hash;
}


/* ************************************************************************** */
/* state_t */

struct state_impl_t
{
    state_impl_t(bool final) : suc(), final(final) {}
    state_impl_t(const expr_t& e, bool final = false) : suc(e), final(final) {}
    expr_t suc;
    bool final;
};

inline state_t::state_t() : impl_ptr() { /* throw illegal_constructor_error(""); */ }
inline state_t::state_t(const state_t& s) : impl_ptr(s.impl_ptr) {}

inline state_t::state_t(bool final) : impl_ptr(new state_impl_t(final)) {}
inline state_t::state_t(const expr_t& e, bool final) : impl_ptr(new state_impl_t(e,final)) {}

inline state_t state_t::new_state(bool final) { return state_t(final); }
inline state_t state_t::new_state(const expr_t& e, bool final) { return state_t(e,final); }

inline state_t& state_t::operator=(const state_t& s)
{
    impl_ptr = s.impl_ptr;
    return *this;
}

inline const void* state_t::uid() const { return impl_ptr.get(); }
inline const expr_t& state_t::suc() const { return impl_ptr->suc; }
inline bool state_t::final() const { return impl_ptr->final; }

// syntactic sugar
inline expr_t& state_t::operator*() { return impl_ptr->suc; }
inline const expr_t& state_t::operator*() const { return impl_ptr->suc; }
inline expr_t* state_t::operator->() { return &impl_ptr->suc;}
inline const expr_t* state_t::operator->() const { return &impl_ptr->suc;}

inline void state_t::set_suc(const expr_t& e) { impl_ptr->suc = e; }
inline void state_t::set_final(bool f) { impl_ptr->final = f; }

// FIXME make a difference between semantic and syntactic equality
inline bool state_t::operator==(const state_t& r) const
{
    return uid() == r.uid();
    //return suc_ == r.suc_ && final_ == r.final_;
}

inline bool state_t::operator!=(const state_t& r) const
{
    return uid() != r.uid();
    //return suc_ != r.suc_ || final_ != r.final_;
}

inline bool state_t::operator<(const state_t& r) const
{
    return uid() < r.uid();
    //return suc_ < r.suc_ || (suc_ == r.suc_ && final_ < r.final_);
}

inline state_t new_state(bool final) { return state_t::new_state(final); }
inline state_t new_state(const expr_t& e, bool final) { return state_t::new_state(e,final); }

inline std::size_t hash_value(const state_t& state)
{
    return boost::hash<const void*>()(state.uid());
}

/* ************************************************************************** */
/* expr_t */

inline const void* expr_t::uid() const
{
    return (void*) e.get();
}

inline efac_ptr expr_t::efac() const { return ptr; }

#ifdef DEBUG
inline expr_impl_ptr expr_t::impl() const { return e; }
#endif 

#ifdef NOREC_GET_OPS
// non-recursive version
#include <queue>
template<op_t o, class T>
void expr_t::get_ops(const expr_t& e, std::insert_iterator<T> out)
{
    std::queue<expr_t> q;
    q.push(e);
    while (! q.empty()) {
        expr_t cur = q.front();
        q.pop();
        if (cur.op() == o) {
            q.push(cur.left());
            q.push(cur.right());
        } else {
            out = cur.e;
        }
    }
}
#else
template<op_t o, class T>
void expr_t::get_ops(const expr_t& e, std::insert_iterator<T> out)
{
    switch(e.op()) {
    case o: get_ops<o>(e.left(),out); get_ops<o>(e.right(),out); break;
    default: out = e.e;
    }
}
#endif

inline expr_t::expr_t() : e(), ptr() {}
inline expr_t::expr_t(const expr_impl_ptr e, const efac_ptr& ptr) : e(e), ptr(ptr) {}
inline expr_t::expr_t(const expr_impl_ptr e, const weak_efac_ptr& ptr) : e(e), ptr(ptr.lock()) {}

inline bool expr_t::is_false() const
{
    return e->op == Const && ! e->con;
}

inline bool expr_t::is_true() const
{
    return e->op == Const && e->con;
}

inline expr_t& expr_t::operator=(const expr_t& r)
{
    e = r.e;
    ptr = r.ptr;
    return *this;
}

inline bool expr_t::operator==(const expr_t& r) const
{
    // TODO check ptr == r.ptr
    return (e == 0 && r.e == 0) || (e != 0 && r.e != 0 && *e == *r.e);
}

inline std::list<expr_t> expr_t::childs() const
{
    std::list<expr_t> l;
    if (e->op == And || e->op == Or) {
        l.push_back(expr_t(e->left,ptr));
        l.push_back(expr_t(e->right,ptr));
    }
    return l;
}

inline op_t expr_t::op() const { return e->op; }

inline expr_t expr_t::left() const
{
    // TODO check!
    return expr_t(e->left,ptr);
}

inline expr_t expr_t::right() const
{
    // TODO check!
    return expr_t(e->right,ptr);
}

inline prop_t expr_t::prop() const
{
    // TODO check!
    return e->prop;
}

inline prop_t expr_t::nprop() const
{
    // TODO check!
    return e->prop;
}

inline state_t expr_t::state() const
{
    // TODO check!
    return e->state;
}

inline bool expr_t::con() const
{
    // TODO check!
    return e->con;
}

inline std::size_t expr_t::size() const
{
    struct counter : dfs {
        counter() : c(0) {}
        void node_hook(const expr_t& e) { c++; }
        void edge_hook(const expr_t& e1, const expr_t& e2) { c++; }
        std::size_t c;
    };

    counter c;
    c.run(*this);
    return c.c;
}

template<class T>
std::size_t size(const T& exprs)
{
    struct counter : dfs {
        counter() : c(0) {}
        void node_hook(const expr_t& e) { c++; }
        void edge_hook(const expr_t& e1, const expr_t& e2) { c++; }
        std::size_t c;
    };

    counter c;
    c.run(exprs.begin(),exprs.end());
    return c.c;
}


    
/* ************************************************************************** */
/* efac */

inline boost::shared_ptr<efac> efac::newEfac()
{
    boost::shared_ptr<efac> e(new efac());
    e->ptr = boost::weak_ptr<efac>(e);
    return e;
}

inline boost::shared_ptr<efac> efac::newEfac(unsigned int n)
{
    boost::shared_ptr<efac> e(new efac(n));
    e->ptr = boost::weak_ptr<efac>(e);
    return e;
}

// copy expr from a different efac
inline expr_t efac::copy(const expr_t& e)
{
    switch (e.op()) {
    case And: return copy(e.left()) && copy(e.right());
    case Or: return copy(e.left()) || copy(e.right());
    case Const: return mkConst(e.con());
    case Prop: return mkProp(e.prop());
    case NProp: return mkNProp(e.nprop());
    case State: return mkState(e.state());
    }
}

inline expr_t efac::mkProp(prop_t p)
{
    return expr_t(check_hash(expr_impl_t(Prop,0,state_t(),p,ptr)),ptr);
}

inline expr_t efac::mkNProp(prop_t p)
{
    return expr_t(check_hash(expr_impl_t(NProp,0,state_t(),p,ptr)),ptr);
}

inline expr_t efac::mkState(const state_t& state)
{
    return expr_t(check_hash(expr_impl_t(State,0,state,prop_t(),ptr)),ptr);
}

inline expr_t efac::mkConst(bool b)
{
    return expr_t(check_hash(expr_impl_t(Const,b,state_t(),prop_t(),ptr)),ptr);
}

inline expr_t efac::mkTrue()
{
    return expr_t(check_hash(expr_impl_t(Const,true,state_t(),prop_t(),ptr)),ptr);
}

inline expr_t efac::mkFalse()
{
    return expr_t(check_hash(expr_impl_t(Const,false,state_t(),prop_t(),ptr)),ptr);
}

inline std::size_t efac::size() const { return cache.size(); }

#ifdef EFAC_HASH_INTERFACE
inline std::size_t efac::bucket_count() const { return cache.bucket_count(); }
inline float efac::load_factor() const { return cache.load_factor(); }
inline float efac::max_load_factor() const { return cache.max_load_factor(); }
inline void efac::max_load_factor(float z) { cache.max_load_factor(z); }
inline void efac::rehash(unsigned int n) { cache.rehash(n); }

inline std::size_t efac::max_buck() const 
{
    std::size_t m = 0;
    for (int i = 0; i < cache.bucket_count(); ++i) {
        m = std::max(m,cache.bucket_size(i));
    }
    return m;
}
#endif

inline efac::efac() : cache(), ptr() {}
inline efac::efac(unsigned int n) : cache(), ptr() { cache.rehash(n); }

inline const expr_impl_ptr efac::check_hash(const expr_impl_t& expr)
{
    expr_impl_hash_t::const_iterator r = cache.find(expr);
    if (r != cache.end()) {
        return expr_impl_ptr(*r);
    } else {
        const expr_impl_ptr n = expr_impl_ptr(new expr_impl_t(expr));
        cache.insert(n.get());
        return n;
    }
}


/* ************************************************************************** */
/* dfs */
inline void dfs::run(const expr_t& x)
{
    pre_hook();
    init_hook(x);
    rec(x);
    post_hook();
}

inline void dfs::rec(const expr_t& x)
{
    if (hash.find(x) != hash.end()) return;
    hash.insert(x);
    node_hook(x);
    switch (x.e->op) {
    case And: and_hook(x); break;
    case Or: or_hook(x); break;
    case State: state_hook(x); break;
    case Prop: prop_hook(x); break;
    case NProp: nprop_hook(x); break;
    case Const: const_hook(x); break;
    }
    if (x.op() == State) {
        edge_hook(x,*x.e->state);
        rec(*(x.e->state));
    } else {
        foreach(const expr_t& y, x.childs()) {
            edge_hook(x,y);
            rec(y);
        }
    }
}

template<class I>
void dfs::run(I a, I e)
{
    pre_hook();
    foreach(typename I::value_type const& x, std::make_pair(a,e))
    {
        init_hook(x);
        rec(x);
    }
    post_hook();
}

/* ************************************************************************** */
/* expr_impl_t */

inline void intrusive_ptr_add_ref(const expr_impl_t* e)
{
    ++(e->ref_c);
}

inline void intrusive_ptr_release(const expr_impl_t* e)
{
#ifndef NO_EXPR_DEALLOC
    if ( --(e->ref_c) == 0) {
        if (efac_ptr tmp = e->eptr.lock()) {
            tmp ->cache.erase(*e);
        }
        delete e;
    }
#endif
}

