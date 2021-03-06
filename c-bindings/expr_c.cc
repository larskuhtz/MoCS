/* *** MOCS-COPYRIGHT-NOTICE-BEGIN ***
 *
 * This copyright notice is auto-generated by ./add-copyright-notice.
 * Additional copyright notices must be added below the last line of this notice.
 *
 * MoCS (https://lewis.cs.uni-saarland.de/tools/mocs/): "c-bindings/expr_c.cc".
 * The content of this file is copyright of Saarland University -
 * Copyright (C) 2009 Saarland University, Reactive Systems Group, Lars Kuhtz.
 *
 * This file is part of MoCS (https://lewis.cs.uni-saarland.de/tools/mocs/).
 *
 * License: three-clause BSD style license.
 * The license text can be found in the file LICENSE.
 *
 * *** MOCS-COPYRIGHT-NOTICE-END *** */

#include "expr.h"

#include "expr.hh"

#include <sstream>
#include <fstream>


// constructors/ destructors
// expr_ptr expr_new();
void expr_free(expr_ptr e)
{
    delete e;
}

// printing
void expr_expr2dot(const char* file_name, const expr_ptr e, const char* name)
{
    std::ofstream file(file_name);
    if (file) {
        expr2dot(file,*e,name);
    } else {
        std::cerr << "Error: Failed to open file for writing." << std::endl;
    }
}

char* expr_exprs2dot(const char* file_name, const expr_ptr* e,int dim, const char* name)
{
    std::ofstream file(file_name);
    if (file) {
        std::list<expr_t> l;
        const expr_ptr* ptr = e;
        for(int i = 0; i++; i < dim) {
            l.push_back(**e++);
        }
        exprs2dot(file,l,name);
    } else {
        std::cerr << "Error: Failed to open file for writing." << std::endl;
    }
}

char* expr_exprs2dot_(const char* file_name, const expr_ptr* e, const char* name) // null-terminated
{
    std::ofstream file(file_name);
    if (file) {
        std::list<expr_t> l;
        const expr_ptr* ptr = e;
        while (*e) {
            l.push_back(**e++);
        }
        exprs2dot(file,l,name);
    } else {
        std::cerr << "Error: Failed to open file for writing." << std::endl;
    }
}

char* expr_to_string(const expr_ptr e)
{
    std::ostringstream s;
    s << *e;
    char* ret = (char*) malloc(sizeof(char) * (s.str().size() + 1));
    return strcpy(ret,s.str().c_str());
}

// copy
expr_ptr copy(const expr_ptr e, context_ptr target_context)
{
    return new expr_t(target_context->impl->copy(*e));
}

// comparing
int expr_eq(const expr_ptr e0, const expr_ptr e1) { return *e0 == *e1; }

// querying
int expr_is_false(const expr_ptr e) { return e->is_false(); }
int expr_is_true(const expr_ptr e) { return e->is_true(); }

context_ptr expr_get_context(expr_ptr e)
{
    return new c::context::context_t(e->efac());
}

//expr_ptr* expr_get_childreen(const expr_ptr); // returns null-terminated array
// TODO query type and content

int expr_size(const expr_ptr e)
{
    return e->size();
}

int exprs_size(const expr_ptr* e) // null-terminated
{
    std::list<expr_t> l;
    const expr_ptr* ptr = e;
    while (e) {
        l.push_back(**e++);
    }
    return size(l);
}

// manipulation
expr_ptr expr_and(const expr_ptr e0,const expr_ptr e1)
{
    return new expr_t(*e0 && *e1);
}

expr_ptr expr_or(const expr_ptr e0,const expr_ptr e1)
{
    return new expr_t(*e0 || *e1);
}

// TODO missing generators (from efac)
