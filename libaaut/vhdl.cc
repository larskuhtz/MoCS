/* *** MOCS-COPYRIGHT-NOTICE-BEGIN ***
 *
 * This copyright notice is auto-generated by ./add-copyright-notice.
 * Additional copyright notices must be added below the last line of this notice.
 *
 * MoCS (https://lewis.cs.uni-saarland.de/tools/mocs/): "libaaut/vhdl.cc".
 * The content of this file is copyright of Saarland University -
 * Copyright (C) 2009 Saarland University, Reactive Systems Group, Lars Kuhtz <kuhtz@cs.uni-sb.de>.
 *
 * This file is part of MoCS (https://lewis.cs.uni-saarland.de/tools/mocs/).
 *
 * License: three-clause BSD style license.
 * The license text can be found in the file LICENSE.
 *
 * *** MOCS-COPYRIGHT-NOTICE-END *** */

#include "vhdl.hh"
#include "state-machine.hh"

namespace
{
    using namespace vhdl;

    /* ********************************************************************** */
    /* utilities */

    typedef long sig_t;
    typedef std::set<sig_t> sig_set_t;
    typedef std::map<sig_t,sig_set_t> reg_t;
    typedef std::list<expr_t> logic_list_t;

    std::string output_port_name(int formula_idx, int pos_idx)
    {
        return "output" + STRCAST(formula_idx) + "(" + STRCAST(pos_idx) + ")";
    }

    std::string input_port_name(int prop, int pos_idx)
    {
        return "input" + STRCAST(prop) + "(" + STRCAST(pos_idx) + ")";
    }

    std::string input_port_name(const prop_t& p, bool array = false)
    {
        if (p.size() == 1 && ! array) {
            return "input" + STRCAST(p[0]);
        } else if (p.size() == 1 && array) {
            return "input(" + STRCAST(p[0]) + ")";
        } else if (p.size() == 2) {
            return input_port_name(p[0],p[1]);
        }
    }

    sig_t signal(const expr_t& s)
    {
        return (long) s.uid();
    }

    sig_t signal(const state_t& s)
    {
        return (long) s.uid();
    }

    std::string signal_name(const void* s)
    {
        return "s" + STRCAST((long)s);
    }

    std::string signal_name(const state_t& s)
    {
        return "s" + STRCAST(signal(s));
    }

    std::string signal_name(const expr_t& s)
    {
        if (s.op() != State) {
            return "s" + STRCAST(signal(s));
        } else {
            return signal_name(s.state());
        }
    }

    std::string signal_name(const sig_t& s)
    {
        return "s" + STRCAST(s);
    }

    // out-port-field iterator: iterators over (2 dimensional) out_ports_field_t
    struct opf_it : 
        public std::iterator<std::forward_iterator_tag, 
                             const out_ports_field_t::const_iterator::value_type::const_iterator::value_type>
    {
        typedef out_ports_field_t f_t;
        typedef opf_it self_t;
        typedef f_t::const_iterator o_it;
        typedef o_it::value_type i_it;
        
        opf_it(const f_t& f) : 
            f(f),
            o(f.begin()),
            i(f.begin()->begin())
        {}

        static self_t begin(const f_t& f)
        {
            return self_t(f);
        }

        static self_t end(const f_t& f)
        {
            self_t it = self_t(f);
            it.o = f.end();
            it.i = f.back().end(); // FIXME
            return it;
        }

        self_t& operator++()
        {
            ++i;
            if (i == o->end()) {
                ++o;
                if (o != f.end()) i = o->begin();
            }
            return *this;
        }

        reference operator*() const
        {
            return *i;
        }

        pointer operator->() const
        {
            return i.operator->();
        }

        bool operator==(const self_t& r) const
        {
            return  f == r.f && o == r.o && i == r.i;
        }

        const f_t& f;
        f_t::const_iterator o;
        f_t::const_iterator::value_type::const_iterator i;
    };

    /* generate function for a signal (except state)
     */
    std::ostream& signal_func(std::ostream& o, const expr_t& s, bool array = false)
    {
        o << "    " << signal_name(s) << " <= ";
        switch (s.op()) {
        case Prop: o << input_port_name(s.prop(),array); break;
        case NProp: o << " not " << input_port_name(s.nprop(),array); break;
        case And: o << signal_name(s.left()) << " and " << signal_name(s.right()); break;
        case Or: o << signal_name(s.left()) << " or " << signal_name(s.right()); break;
        case Const: if (s.con() ) o << "'1'"; else o << "'0'"; break;
        default: expr_type_error("a boolean circuit must not contain States."); // FIXME
        }
        return o << ";\n";
    }

    std::ostream& sensitives(std::ostream& o, const expr_t& s, bool array = false)
    {
        if (s.op() != Const) {
            o << "(";
            switch (s.op()) {
            case Prop: o << input_port_name(s.prop(),array); break;
            case NProp: o << input_port_name(s.nprop(),array); break;
            case And: o << signal_name(s.left()) << "," << signal_name(s.right()); break;
            case Or: o << signal_name(s.left()) << "," << signal_name(s.right()); break;
            case Const: o << ""; break;
            case State: o << signal_name(*s.state()); break;
            }
            return o << ")";
        } else {
            return o;
        }
    }

    struct signal_decl_dfs : public dfs
    {
        signal_decl_dfs(std::ostream& o) : o(o) {}
        void node_hook(const expr_t& s)
        {
            o << "  signal " << signal_name(s) << " : STD_LOGIC;\n";
        }
        std::ostream& o;
    };

    template<class T>
    std::ostream& signal_decl(std::ostream& o, const T& l)
    {
        foreach(const typename T::value_type& s, l) {
            o << "  signal " << signal_name(s) << " : STD_LOGIC;\n";
        }
        return o;
    }

    std::ostream& signal_decl(std::ostream& o, const sig_t& s)
    {
            o << "  signal " << signal_name(s) << " : STD_LOGIC;\n";
            return o;
    }

    std::ostream& signal_decl(std::ostream& o, const std::string& s)
    {
            o << "  signal " << s << " : STD_LOGIC;\n";
            return o;
    }

    // build process
    struct build_logic_dfs : public dfs
    {
        build_logic_dfs(std::ostream& o, bool array = false) : o(o), array(array) {}
        void node_hook(const expr_t& s)
        {
            if (s.op() == Const) {
                signal_func(o,s,array);
                o << "\n\n";

            } else if (s.op() == State) {
                o << "  state" << signal_name(s) << " : entity work.dff\n";
                o << "    port map(CLOCK  => CLOCK,\n";
                o << "             input  => " << signal_name(*s.state()) << ",\n";
                o << "             output => " << signal_name(s) << ");\n\n";

            } else if (s.op() == Or || s.op() == And || s.op() == Prop || s.op() == NProp){
                o << "  process";
                sensitives(o,s,array); 
                o << "\n";
                o << "  begin\n";
                signal_func(o,s,array);
                o << "  end process;\n\n";

            } else {
                /* ERROR */
                ASSERT(false);
            }
        }

        std::ostream& o;
        bool array;
    };

    std::ostream& create_header(std::ostream& o)
    {
        o << "use work.general.all;\n";
        o << "library IEEE;\n";
        o << "use IEEE.STD_LOGIC_1164.ALL;\n";
        o << "use IEEE.numeric_std.all;\n";
        return o;
    }

    std::string in_port_decl(const prop_t& p)
    {
       return "    " + input_port_name(p) + " : in STD_LOGIC";
    }

    /* ********************************************************************** */
    /* st - suffic transducer */

    std::ostream& create_stoc_entity(std::ostream& o, 
                                std::size_t prop_count, 
                                const out_ports_field_t& out_ports, 
                                const std::string& name)
    {
        o << "entity " << name << " is\n";
        o << "  Port (\n";

        // input ports
        for (std::size_t i = 0; i < prop_count; ++i) {
            o << "input" << i << " : in unsigned(hor_range);\n";
        }

        // output ports
        std::size_t subf_count = out_ports.size();
        for(std::size_t i = 0; i < subf_count; ++i)
        {
            o << "output" << i << " : out unsigned(hor_range)";
            if (i < subf_count - 1) o << ";\n";
        }

        o << "\n  );\n";
        o << "end " << name << ";\n\n";
        return o;
    }

    std::ostream& create_stoc_architecture(std::ostream& o,
                                      const out_ports_field_t& out_ports, 
                                      const std::string& name)
    {
        o << "architecture Behavioral of " << name << " is\n";

        signal_decl_dfs(o).run(opf_it::begin(out_ports),opf_it::end(out_ports));

        o << "\nbegin\n\n";
        
        build_logic_dfs(o).run(opf_it::begin(out_ports),opf_it::end(out_ports));

        o << "\n";

        // FIXME to complicated: it suffices to know the sizes!
        std::size_t i = 0;
        foreach(const out_ports_t& l, out_ports) {
            std::size_t j = 0;
            foreach(const expr_t& x, l) {
                o << "    " << output_port_name(i,j) << " <= " << signal_name(x) << ";\n";
                ++j;
            }
            ++i;
        }

        o << "\nend Behavioral;\n";
        return o;
    }

    /* Suffix Transducer:
     *
     * use work.general.all;
     * library IEEE;
     * use IEEE.STD_LOGIC_1164.ALL;
     * use IEEE.numeric_std.all;
     *
     * entity suffix_transducer is
     *   Port( CLOCK : in STD_LOGIC;
     *         input : in unsigned(prop_range);  -- props from 0 to prop_count-1
     *         output0: out unsigned(hor_range); -- for subf 0 and each pos
     *         output1: out unsigned(hor_range)
     *         ...);
     * end suffix_transducer;
     *
     * architecture Behavioral of suffix_transducer is
     *   signal s0 : unsigned(hor_range); -- connects prop0 pipline with trans
     *   ...
     *
     * begin
     *   pipe0 : entity work.pipeline
     *     portmap(CLOCK => CLOCK;
     *             input => input(0),
     *             output => s0);
     *  ...
     *
     *  trans : stoc 
     *    portmap(input0 => s0,
     *            input1 => s1,
     *            ...
     *            output0 => output0
     *            output1 => output1
     *            ...);
     *  end Behavioral;
     *
     */
    std::ostream& st(std::ostream& o, std::size_t prop_count, std::size_t subf_count)
    {
     o << "use work.general.all;\n";
     o << "library IEEE;\n";
     o << "use IEEE.STD_LOGIC_1164.ALL;\n";
     o << "use IEEE.numeric_std.all;\n";
     
     o << "\n";

     o << "entity suffix_transducer is\n";
     o << "  Port( CLOCK : in STD_LOGIC;\n";
     o << "        input : in unsigned(prop_range);\n";
    
     for (std::size_t i = 0; i < subf_count; ++i) {
     o << "        output" << i << " : out unsigned(hor_range)";
                           if (i < subf_count -1) o << ";\n";
                           else o << ");\n";
     }
     
     o << "end suffix_transducer;\n";
     
     o << "\n";

     o << "architecture Behavioral of suffix_transducer is\n";
     
     for (std::size_t i = 0; i < prop_count; ++i) {
     o << "  signal s" << i << " : unsigned(hor_range); -- connects prop0 pipline with trans\n";
     }
     
     o << "\n";

     o << "begin\n";


     for (std::size_t i = 0; i < prop_count; ++i) {
     o << "  pipe" << i << " : entity work.pipeline\n";
     o << "    generic map(LENGTH => hor)\n";
     o << "    port map(CLOCK => CLOCK,\n";
     o << "             input => input(" << i << "),\n";
     o << "             output => s" << i << ");\n";
     }
     
     o << "\n";

     o << " trans : entity work.stoc \n";
     o << "   port map(\n";
     
     for (std::size_t i = 0; i < prop_count; ++i) {
     o << "            input" << i << " => s" << i <<",\n";
     }

     for (std::size_t i = 0; i < subf_count; ++i) {
     o << "            output" << i << " => output" << i;
                           if (i < subf_count -1) o << ",\n";
                           else o << ");\n";
     }

     o << " end Behavioral;\n";
     
     o << "\n";

     return o;

    }

    /* ********************************************************************** */
    /* general declarations */

    /* library IEEE;
     * use IEEE.STD_LOGIC_1164.ALL;
     * use IEEE.numeric_std.all;
     *
     * package general is
     *
     *   constant prop_count : natural := XXX;
     *   constant hor : natural := XXX;
     *   constant subf_count : natural := XXX;
     *         
     *   subtype prop_range is natural range 0 to prop_count - 1;
     *   subtype subf_range is natural range 0 to subf_count - 1;
     *   subtype ptsm_range is natural range 0 to subf_count;
     *   subtype hor_range is natural range 0 to hor;
     *                 
     *   subtype prop_array is unsigned(0 to (prop_count - 1));
     *
     *   end package general;
     */

    std::ostream& general(std::ostream& o,
                          std::size_t prop_count,
                          std::size_t horizon,
                          std::size_t subf_count)
    {
        o << "library IEEE;\n";
        o << "use IEEE.STD_LOGIC_1164.ALL;\n";
        o << "use IEEE.numeric_std.all;\n";

        o << "package general is\n\n";

        o << "  constant prop_count : natural := " << prop_count << ";\n";
        o << "  constant hor : natural := " << horizon << ";\n";
        o << "  constant subf_count : natural := " << subf_count << ";\n\n";

        o << "  subtype prop_range is natural range 0 to prop_count - 1;\n";
        o << "  subtype subf_range is natural range 0 to subf_count - 1;\n";
        o << "  subtype ptsm_range is natural range 0 to subf_count;\n";
        o << "  subtype hor_range is natural range 0 to hor;\n\n";

        o << "  subtype prop_array is unsigned(0 to (prop_count - 1));\n\n";

        o << "end package general;\n\n";
        return o;
    }

    /* ********************************************************************** */
    /* ptoc */

    template<class T>
    std::ostream& ptoc_entity(std::ostream& o, 
                              std::size_t horizon, 
                              std::size_t subf_count, 
                              const T& ptsm_states)
    {
        const std::string name = "ptoc";
        o << "entity " << name << " is\n";
        o << "  Port (\n";
        
        foreach (long state_id, ptsm_states) {
            o << "    input" << state_id << " : in STD_LOGIC;\n";
        }

        for (std::size_t i = 0; i < subf_count; ++i) {
            o << "    input" << i << " : in unsigned(hor_range);\n";
        }
        
        o << "    output : out STD_LOGIC";
        o << "\n  );\n";
        o << "end " << name << ";\n\n";
        return o;
    }

    std::ostream& ptoc_architecture(std::ostream& o,
                                    const expr_t& out_expr)
    {
        // TODO connection between ptsm-state and ptoc
        //      How to connect state to circuit
        const std::string name = "ptoc";
        o << "architecture Behavioral of " << name << " is\n";
        signal_decl_dfs(o).run(out_expr);
        o << "\nbegin\n\n";
        build_logic_dfs(o).run(out_expr); 
        o << "\n";
        o << "    " << "output" << " <= " << signal_name(out_expr) << ";\n";
        o << "\nend Behavioral;\n";
        return o;
    }

    // create prefix transducer output circuit
    template<class T>
    std::ostream& ptoc(std::ostream& o,
                       std::size_t& horizon,
                       std::size_t& subf_count,
                       const T& ptsm_states,
                       const expr_t& out_expr)
    {
        create_header(o);
        ptoc_entity(o,horizon,subf_count,ptsm_states);
        ptoc_architecture(o,out_expr);
    }

    /* ********************************************************************** */
    /* ptsm */

    std::ostream& ptsm_entity(std::ostream& o, const size_t max_state)
    {
        using namespace boost::lambda;

        const std::string name = "ptsm";
        o << "entity " << name << " is\n";
        o << "  Port (CLOCK : in STD_LOGIC;\n";
        o << "        input : in unsigned(ptsm_range);\n";
        o << "        output: out unsigned(0 to " << max_state << "));\n";
        o << "end " << name << ";\n\n";
        return o;
    }

    /* Precondition: psm is universal
     */
    std::ostream& ptsm_architecture(std::ostream& o, const state_machine_t& sm)
    {
        const std::string name = "ptsm";

        o << "architecture Behavioral of " << name << " is\n";

        // signal declarations
        std::list<expr_t> sexprs;
        foreach(const state_t& s, sm.states) {
            sexprs.push_back(s->efac()->mkState(s));
        }
        signal_decl_dfs(o).run(sexprs.begin(),sexprs.end());

        o << "\nbegin\n\n";

        // generate input logic
        build_logic_dfs(o,true).run(sexprs.begin(),sexprs.end());

        // output
        size_t i = 0;
        foreach(const state_t s, sm.states) {
            o << "  output(" << i++ << ") <= " << signal_name(s) << ";\n";
        }

        o << "\nend Behavioral;\n";
        return o;
    }

    // state machine of prefix transducer
    std::ostream& ptsm(std::ostream& o,const state_machine_t& sm, std::size_t horizon)
    {
        // TODO build counter of bit-width
        // long max_bit = long(ceil(log2(h)));
        
        create_header(o);
        ptsm_entity(o,sm.states.size()-1);
        ptsm_architecture(o,sm);
        return o;
    }

    std::ostream& build_counter(std::ostream& o, const std::string& out_sig)
    {
        o << "  countdown : entity work.countdown\n";
        o << "    generic map(max => hor)\n";
        o << "    port map(CLOCK  => CLOCK,\n";
        o << "             output => " << out_sig << ");\n\n";
        return o;
    }

    // prefix transducer
    std::ostream& pt(std::ostream& o, const state_machine_t& sm, std::size_t subf_count)
    {
        create_header(o);

        o << "\n";

        o << "entity prefix_transducer is\n";
        o << "  Port( CLOCK : in STD_LOGIC;\n";

        for (std::size_t i = 0; i < subf_count; ++i) {
            o << "    input" << i << " : in unsigned (hor_range);\n";
        }

        o << "    output : out STD_LOGIC";

        o << "\n  );\n";
        o << "end prefix_transducer;\n";
     
        o << "\n";

        o << "architecture Behavioral of prefix_transducer is\n";

        o << "  signal states : unsigned(0 to " << sm.states.size()-1 << ");\n";
        o << "  signal counter_out : STD_LOGIC;\n";
        o << "  signal ptsm_in : unsigned(ptsm_range);\n";

        o << "\n";

        o << "begin\n";

        // add an instance of counter that triggers the init signal
        build_counter(o,"counter_out");

        // build input for ptsm
        std::string x;
        for(std::size_t i = 0; i < subf_count; ++i) {
             x += "input" + STRCAST(i) + "(0) & ";
        }
        x += " counter_out";
        o << "ptsm_in <= " << x << ";\n\n";

        // ptsm
        o << "ptsm : entity work.ptsm\n";
        o << "  port map(CLOCK => CLOCK,\n";
        o << "           input => ptsm_in,\n";
        o << "           output => states);\n";

        o << "\n";

        // ptoc
        o << " ptoc : entity work.ptoc \n";
        o << "   port map(\n";

        std::size_t i = 0;
        foreach (const state_t& s, sm.states) {
        o << "    input" << (long) sm.smap.find(s.uid())->second << " => states(" << i++ << "),\n";
        }

        i = 0;
        for (;i < subf_count-1; ++i) {
        o << "    input" << i << " => input" << i << ",\n"; }
        o << "    input" << i << " => input" << subf_count-1 << ",\n";
        o << "    output => output);\n";

        o << " end Behavioral;\n";

        o << "\n";

        return o;
    }

    template<class T>
    struct ptsm_states_dfs : public dfs
    {
        ptsm_states_dfs(std::back_insert_iterator<T> out) : out(out) {}
        std::back_insert_iterator<T> out;

        void state_hook(const expr_t& e)
        {
            out = signal(e.state());
        }
    };

    template<class T>
    ptsm_states_dfs<T> mk_ptsm_states_dfs(std::back_insert_iterator<T> x) { return ptsm_states_dfs<T>(x); }

    // monitor
    std::ostream& m(std::ostream& o, std::size_t subf_count)
    {
        create_header(o);

        o << "\n";

        // entity
        o << "entity monitor is\n";
        o << "  Port(CLOCK : in STD_LOGIC;\n";
        o << "       input : in unsigned(prop_range);\n";
        o << "       output : out STD_LOGIC);\n";
        o << "end monitor;\n";
     
        o << "\n";

        // architecture
        o << "architecture Behavioral of monitor is\n";

        // signals
        for(std::size_t i = 0; i < subf_count; ++i) {
            o << "  signal signal" << i << " : unsigned(hor_range);\n";
        }
        o << "  signal o : STD_LOGIC;\n";

        o << "\n";

        o << "begin\n";

        // suffix_transducer
        o << "suffix_transducer : entity work.suffix_transducer\n";
        o << "  port map(CLOCK => CLOCK,\n";
        for(std::size_t i = 0; i < subf_count; ++i) {
            o << "           output" << i << " => signal" << i << ",\n";
        }
        o << "           input => input);\n";

        o << "\n";

        // prefix_transducer
        o << " prefix_transducer: entity work.prefix_transducer \n";
        o << "   port map(CLOCK => CLOCK,\n";
        for(std::size_t i = 0; i < subf_count; ++i) {
            o << "           input" << i << " => signal" << i << ",\n";
        }
        o << "           output => o);\n";

        // throw output through a d-fip-flop
        o << " outdff: entity work.dff \n";
        o << " port map(CLOCK => CLOCK,\n";
        o << "          input => o,\n";
        o << "          output => output);\n";
        
        o << " end Behavioral;\n";

        o << "\n";

        return o;
    }
};

namespace vhdl {
    std::ostream& monitor(std::ostream& o,
                          std::size_t horizon,
                          std::size_t prop_count,
                          std::size_t subf_count,
                          const expr_t& global_out_expr,
                          const automaton_t& prefix_automaton,
                          const out_ports_field_t& out_ports)
{
    std::string stoc_name = "stoc";

    // general header
    general(o,prop_count,horizon,subf_count);

    o << "-- ----------------------------------------------------------------- --\n";

    // output circuit of suffix_transducer
    create_header(o);
    create_stoc_entity(o,prop_count,out_ports,stoc_name);
    create_stoc_architecture(o,out_ports,stoc_name);
    
    o << "-- ----------------------------------------------------------------- --\n";

    // suffix transducer
    st(o,prop_count,subf_count);

    o << "-- ----------------------------------------------------------------- --\n";
   
    // get states of universal prefix automaton
    std::list<long> ptsm_states;
    mk_ptsm_states_dfs(std::back_inserter(ptsm_states)).run(prefix_automaton.init->efac()->mkState(prefix_automaton.init));

    // output circuit of prefix transducer
    ptoc(o,horizon,subf_count,ptsm_states,global_out_expr);

    o << "-- ----------------------------------------------------------------- --\n";

    // get state machine
    state_machine_t sm(prefix_automaton);
    // add init_signal ??
    prop_t p;
    p.push_back(subf_count);
    expr_t init_signal = sm.init->efac()->mkProp(p);
    sm.init.set_suc(* sm.init || init_signal);

    // state machine of prefix transducer
    ptsm(o,sm,horizon);
    
    o << "-- ----------------------------------------------------------------- --\n";

    // prefix transducer
    pt(o,sm,subf_count);

    o << "-- ----------------------------------------------------------------- --\n";

    // monitor
    m(o,subf_count);

    return o;
}
};

