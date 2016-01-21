/*************************************************************************
 *
 *  file:  backtrace.c
 *
 * =======================================================================
 *  
 *  Backtracing structures and routines.  See also explain.c
 *  
 *  
 * =======================================================================
 *
 * Copyright 1995-2004 Carnegie Mellon University,
 *										 University of Michigan,
 *										 University of Southern California/Information
 *										 Sciences Institute. All rights reserved.
 *										
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1.	Redistributions of source code must retain the above copyright notice,
 *		this list of conditions and the following disclaimer. 
 * 2.	Redistributions in binary form must reproduce the above copyright notice,
 *		this list of conditions and the following disclaimer in the documentation
 *		and/or other materials provided with the distribution. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE SOAR CONSORTIUM ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE SOAR CONSORTIUM  OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of Carnegie Mellon University, the
 * University of Michigan, the University of Southern California/Information
 * Sciences Institute, or the Soar consortium.
 * =======================================================================
 */

/* ====================================================================
                        Backtracing routines
   ==================================================================== */

#include "soarkernel.h"

#include <ctype.h>

#include "explain.h"

/* ====================================================================

                            Backtracing

   Four sets of conditions are maintained during backtracing:  locals,
   grounds, positive potentials, and negateds.  Negateds are really
   potentials, but we keep them separately throughout backtracing, and
   ground them at the very end.  Note that this means during backtracing,
   the grounds, positive potentials, and locals are all instantiated
   top-level positive conditions, so they all have a bt.wme on them.

   In order to avoid backtracing through the same instantiation twice,
   we mark each instantiation as we BT it, by setting
   inst->backtrace_number = backtrace_number (this is a global variable
   which gets incremented each time we build a chunk).

   Locals, grounds, and positive potentials are kept on lists (see the
   global variables below).  These are consed lists of the conditions
   (that is, the original instantiated conditions).  Furthermore,
   we mark the bt.wme's on each condition so we can quickly determine
   whether a given condition is already in a given set.  The "grounds_tc",
   "potentials_tc", "locals_tc", and "chunker_bt_pref" fields on wme's
   are used for this.  Wmes are marked as "in the grounds" by setting
   wme->grounds_tc = grounds_tc.  For potentials and locals, we also
   must set wme->chunker_bt_pref:  if the same wme was tested by two
   instantiations created at different times--times at which the wme
   was supported by two different preferences--then we really need to
   BT through *both* preferences.  Marking the wmes with just "locals_tc"
   or "potentials_tc" alone would prevent the second preference from
   being BT'd.

   The add_to_grounds(), add_to_potentials(), and add_to_locals()
   macros below are used to add conditions to these sets.  The negated
   conditions are maintained in the chunk_cond_set "negated_set."

   As we backtrace, each instantiation that has some Nots is added to
   the list instantiations_with_nots.  We have to go back afterwards
   and figure out which Nots are between identifiers that ended up in
   the grounds.
==================================================================== */

#define add_to_grounds(cond) { \
  if ((cond)->bt.wme->grounds_tc != current_agent(grounds_tc)) { \
    (cond)->bt.wme->grounds_tc = current_agent(grounds_tc); \
    push ((cond), current_agent(grounds)); } }

#define add_to_potentials(cond) { \
  if ((cond)->bt.wme->potentials_tc != current_agent(potentials_tc)) { \
    (cond)->bt.wme->potentials_tc = current_agent(potentials_tc); \
    (cond)->bt.wme->chunker_bt_pref = (cond)->bt.trace; \
    push ((cond), current_agent(positive_potentials)); \
  } else if ((cond)->bt.wme->chunker_bt_pref != (cond)->bt.trace) { \
    push ((cond), current_agent(positive_potentials)); } }

#define add_to_locals(cond) { \
  if ((cond)->bt.wme->locals_tc != current_agent(locals_tc)) { \
    (cond)->bt.wme->locals_tc = current_agent(locals_tc); \
    (cond)->bt.wme->chunker_bt_pref = (cond)->bt.trace; \
    push ((cond), current_agent(locals)); \
  } else if ((cond)->bt.wme->chunker_bt_pref != (cond)->bt.trace) { \
    push ((cond), current_agent(locals)); } }

/* -------------------------------------------------------------------
                     Backtrace Through Instantiation

   This routine BT's through a given instantiation.  The general method
   is as follows:

     1. If we've already BT'd this instantiation, then skip it.
     2. Mark the TC (in the instantiated conditions) of all higher goal
        ids tested in top-level positive conditions
     3. Scan through the instantiated conditions; add each one to the
        appropriate set (locals, positive_potentials, grounds, negated_set).
     4. If the instantiation has any Nots, add this instantiation to
        the list of instantiations_with_nots.
------------------------------------------------------------------- */

/* mvp 5-17-94 */
void print_consed_list_of_conditions(list * c, int indent)
{
    for (; c != NIL; c = c->rest) {
        if (get_printer_output_column() >= COLUMNS_PER_LINE - 20)
            print("\n      ");

        /* mvp 5-17-94 */
        print_spaces(indent);
        print_condition(c->first);
    }
}

/* mvp 5-17-94 */
void print_consed_list_of_condition_wmes(list * c, int indent)
{
    for (; c != NIL; c = c->rest) {
        if (get_printer_output_column() >= COLUMNS_PER_LINE - 20)
            print("\n      ");

        /* mvp 5-17-94 */
        print_spaces(indent);
        print("     ");
        print_wme(((condition *) (c->first))->bt.wme);
    }
}

/* This is the wme which is causing this production to be backtraced through. 
   It is NULL when backtracing for a result preference.                   */

/* mvp 5-17-94 */
void backtrace_through_instantiation(instantiation * inst,
                                     goal_stack_level grounds_level, condition * trace_cond, int indent)
{

    tc_number tc;               /* use this to mark ids in the ground set */
    tc_number tc2;              /* use this to mark other ids we see */
    condition *c;
    list *grounds_to_print, *pots_to_print, *locals_to_print, *negateds_to_print;
    bool need_another_pass;
    backtrace_str temp_explain_backtrace;

#ifndef TRACE_CONTEXT_DECISIONS_ONLY

    if (current_agent(sysparams)[TRACE_BACKTRACING_SYSPARAM]) {

        /* mvp 5-17-94 */
        print_spaces(indent);
        print("... BT through instantiation of ");
        if (inst->prod)
            print_with_symbols("%y\n", inst->prod->name);
        else
            print_string("[dummy production]\n");
    }
#endif

    /* --- if the instantiation has already been BT'd, don't repeat it --- */
    if (inst->backtrace_number == current_agent(backtrace_number)) {

#ifndef TRACE_CONTEXT_DECISIONS_ONLY
        if (current_agent(sysparams)[TRACE_BACKTRACING_SYSPARAM]) {

            /* mvp 5-17-94 */
            print_spaces(indent);
            print_string("(We already backtraced through this instantiation.)\n");
        }
#endif

        return;
    }
    inst->backtrace_number = current_agent(backtrace_number);

    /* Record information on the production being backtraced through */
    /* if (current_agent(explain_flag)) { */
    if (current_agent(sysparams)[EXPLAIN_SYSPARAM]) {
        temp_explain_backtrace.trace_cond = trace_cond; /* Not copied yet */
        if (trace_cond == NULL) /* Backtracing for a result */
            temp_explain_backtrace.result = TRUE;
        else
            temp_explain_backtrace.result = FALSE;

        temp_explain_backtrace.grounds = NIL;
        temp_explain_backtrace.potentials = NIL;
        temp_explain_backtrace.locals = NIL;
        temp_explain_backtrace.negated = NIL;

        if (inst->prod) {
            strncpy(temp_explain_backtrace.prod_name, inst->prod->name->sc.name, PROD_NAME_SIZE);
        } else {
            strncpy(temp_explain_backtrace.prod_name, "Dummy production", PROD_NAME_SIZE);
        }
        temp_explain_backtrace.prod_name[PROD_NAME_SIZE - 1] = 0;       /* in case the strncpy truncated the string */

        temp_explain_backtrace.next_backtrace = NULL;
    }

    /* --- check okay_to_variablize flag --- */
    if (!inst->okay_to_variablize)
        current_agent(variablize_this_chunk) = FALSE;

    /* --- mark transitive closure of each higher goal id that was tested in
       the id field of a top-level positive condition --- */
    tc = get_new_tc_number();
    tc2 = get_new_tc_number();
    need_another_pass = FALSE;

    for (c = inst->top_of_instantiated_conditions; c != NIL; c = c->next) {
        Symbol *id, *value;

        if (c->type != POSITIVE_CONDITION)
            continue;
        id = referent_of_equality_test(c->data.tests.id_test);

        if (id->id.tc_num == tc) {
            /* --- id is already in the TC, so add in the value --- */
            value = referent_of_equality_test(c->data.tests.value_test);
            if (value->common.symbol_type == IDENTIFIER_SYMBOL_TYPE) {
                /* --- if we already saw it before, we're going to have to go back
                   and make another pass to get the complete TC --- */
                if (value->id.tc_num == tc2)
                    need_another_pass = TRUE;
                value->id.tc_num = tc;
            }
        } else if ((id->id.isa_goal) && (c->bt.level <= grounds_level)) {
            /* --- id is a higher goal id that was tested: so add id to the TC --- */
            id->id.tc_num = tc;
            value = referent_of_equality_test(c->data.tests.value_test);
            if (value->common.symbol_type == IDENTIFIER_SYMBOL_TYPE) {
                /* --- if we already saw it before, we're going to have to go back
                   and make another pass to get the complete TC --- */
                if (value->id.tc_num == tc2)
                    need_another_pass = TRUE;
                value->id.tc_num = tc;
            }
        } else {
            /* --- as far as we know so far, id shouldn't be in the tc: so mark it
               with number "tc2" to indicate that it's been seen already --- */
            id->id.tc_num = tc2;
        }
    }

    /* --- if necessary, make more passes to get the complete TC through the
       top-level positive conditions (recall that they're all super-simple
       wme tests--all three fields are equality tests --- */
    while (need_another_pass) {
        Symbol *value;

        need_another_pass = FALSE;
        for (c = inst->top_of_instantiated_conditions; c != NIL; c = c->next) {
            if (c->type != POSITIVE_CONDITION)
                continue;
            if (referent_of_equality_test(c->data.tests.id_test)->id.tc_num != tc)
                continue;
            value = referent_of_equality_test(c->data.tests.value_test);
            if (value->common.symbol_type == IDENTIFIER_SYMBOL_TYPE)
                if (value->id.tc_num != tc) {
                    value->id.tc_num = tc;
                    need_another_pass = TRUE;
                }
        }                       /* end of for loop */
    }                           /* end of while loop */

    /* --- scan through conditions, collect grounds, potentials, & locals --- */
    grounds_to_print = NIL;
    pots_to_print = NIL;
    locals_to_print = NIL;
    negateds_to_print = NIL;

    /* Record the conds in the print_lists even if not going to be printed */

    for (c = inst->top_of_instantiated_conditions; c != NIL; c = c->next) {
        if (c->type == POSITIVE_CONDITION) {

            /* REW: begin 11.22.97 */
            /* print ("\n Checking...");print_wme(c->bt.wme);
               if (c->bt.trace) print ("c->bt.trace exists..."); else print("\n    no c->bt.trace...");
               if (c->bt.wme) { 
               print ("c->bt.wme....");
               if  (c->bt.wme->preference)
               print("c->bt.wme->preference");
               else 
               print("\n no c->bt.wme->preference");
               }      else
               print ("\nNo WME No Preference!!!!!!");
               print("\n"); 
               if ((c->bt.trace) && (c->bt.wme->preference)){
               if (c->bt.trace != c->bt.wme->preference) {
               print("\n bt.trace and WME preferences not equal:\n");
               print("\nWME:"); print_wme(c->bt.wme);
               print("\n bt.trace:"); 
               if (c->bt.trace) print_preference(c->bt.trace); else print(" NIL\n");
               print("\n bt.wme->preference:"); 
               if (c->bt.wme->preference) print_preference(c->bt.wme->preference);
               else print(" NIL\n");
               c->bt.trace = c->bt.wme->preference;
               c->bt.level = c->bt.wme->id->id.level;
               }
               } */
            /* REW: end   11.22.97 */
            /* --- positive cond's are grounds, potentials, or locals --- */
            if (referent_of_equality_test(c->data.tests.id_test)->id.tc_num == tc) {
                add_to_grounds(c);

#ifndef TRACE_CONTEXT_DECISIONS_ONLY
                if (current_agent(sysparams)[TRACE_BACKTRACING_SYSPARAM] || current_agent(sysparams)[EXPLAIN_SYSPARAM])
#else
                if (current_agent(sysparams)[EXPLAIN_SYSPARAM])
#endif
                    push(c, grounds_to_print);
            } else if (c->bt.level <= grounds_level) {
                add_to_potentials(c);
                if (current_agent(sysparams)[TRACE_BACKTRACING_SYSPARAM] || current_agent(sysparams)[EXPLAIN_SYSPARAM])
                    push(c, pots_to_print);
            } else {
                add_to_locals(c);
#ifndef TRACE_CONTEXT_DECISIONS_ONLY
                if (current_agent(sysparams)[TRACE_BACKTRACING_SYSPARAM] || current_agent(sysparams)[EXPLAIN_SYSPARAM])
#else
                if (current_agent(sysparams)[EXPLAIN_SYSPARAM])
#endif
                    push(c, locals_to_print);
            }
        } else {
            /* --- negative or nc cond's are either grounds or potentials --- */
            add_to_chunk_cond_set(&current_agent(negated_set), make_chunk_cond_for_condition(c));
#ifndef TRACE_CONTEXT_DECISIONS_ONLY
            if (current_agent(sysparams)[TRACE_BACKTRACING_SYSPARAM] || current_agent(sysparams)[EXPLAIN_SYSPARAM])
#else
            if (current_agent(sysparams)[EXPLAIN_SYSPARAM])
#endif
                push(c, negateds_to_print);
        }
    }                           /* end of for loop */

    /* --- add new nots to the not set --- */
    if (inst->nots)
        push(inst, current_agent(instantiations_with_nots));

    /* Now record the sets of conditions.  Note that these are not necessarily */
    /* the final resting place for these wmes.  In particular potentials may   */
    /* move over to become grounds, but since all we really need for explain is */
    /* the list of wmes, this will do as a place to record them.               */

    if (current_agent(sysparams)[EXPLAIN_SYSPARAM])
        explain_add_temp_to_backtrace_list(&temp_explain_backtrace, grounds_to_print,
                                           pots_to_print, locals_to_print, negateds_to_print);

#ifndef TRACE_CONTEXT_DECISIONS_ONLY

    /* --- if tracing BT, print the resulting conditions, etc. --- */
    if (current_agent(sysparams)[TRACE_BACKTRACING_SYSPARAM]) {
        not *not1;

        /* mvp 5-17-94 */
        print_spaces(indent);
        print_string("  -->Grounds:\n");
        print_consed_list_of_condition_wmes(grounds_to_print, indent);
        print("\n");
        print_spaces(indent);
        print_string("\n  -->Potentials:\n");
        print_consed_list_of_condition_wmes(pots_to_print, indent);
        print("\n");
        print_spaces(indent);
        print_string("  -->Locals:\n");
        print_consed_list_of_condition_wmes(locals_to_print, indent);
        print("\n");
        print_spaces(indent);
        print_string("  -->Negated:\n");
        print_consed_list_of_conditions(negateds_to_print, indent);
        print("\n");
        print_spaces(indent);
        print_string("  -->Nots:\n");
        /* mvp done */

        for (not1 = inst->nots; not1 != NIL; not1 = not1->next)
            print_with_symbols("    %y <> %y\n", not1->s1, not1->s2);
    }
#endif

    /* Moved these free's down to here, to ensure they are cleared even if we're 
       not printing these lists     */

    free_list(grounds_to_print);
    free_list(pots_to_print);
    free_list(locals_to_print);
    free_list(negateds_to_print);
}

/* ---------------------------------------------------------------
                             Trace Locals

   This routine backtraces through locals, and keeps doing so until
   there are no more locals to BT.
--------------------------------------------------------------- */

void trace_locals(goal_stack_level grounds_level)
{

    /* mvp 5-17-94 */
    cons *c, *prohibits;
    condition *cond;
    preference *bt_pref, *p;

#ifndef TRACE_CONTEXT_DECISIONS_ONLY
    if (current_agent(sysparams)[TRACE_BACKTRACING_SYSPARAM])
        print_string("\n\n*** Tracing Locals ***\n");
#endif

    while (current_agent(locals)) {
        c = current_agent(locals);
        current_agent(locals) = current_agent(locals)->rest;
        cond = c->first;
        free_cons(c);

#ifndef TRACE_CONTEXT_DECISIONS_ONLY

        if (current_agent(sysparams)[TRACE_BACKTRACING_SYSPARAM]) {
            print_string("\nFor local ");
            print_wme(cond->bt.wme);
            print_string(" ");
        }
#endif

        bt_pref = find_clone_for_level(cond->bt.trace, (goal_stack_level) (grounds_level + 1));
        /* --- if it has a trace at this level, backtrace through it --- */
        if (bt_pref) {

#ifdef NO_TOP_JUST
            if (bt_pref->inst) {

                /* mvp 5-17-94 */
                backtrace_through_instantiation(bt_pref->inst, grounds_level, cond, 0);

                /* check if any prohibit preferences */
                if (cond->bt.prohibits) {
                    for (prohibits = cond->bt.prohibits; prohibits != NIL; prohibits = prohibits->rest) {
                        p = prohibits->first;

#ifndef TRACE_CONTEXT_DECISIONS_ONLY
                        if (current_agent(sysparams)[TRACE_BACKTRACING_SYSPARAM]) {
                            print_string("     For prohibit preference: ");
                            print_preference(p);
                        }
#endif
                        if (p->inst)
                            backtrace_through_instantiation(p->inst, grounds_level, cond, 6);
                    }
                }
                /* mvp done */
            }
#else

            /* mvp 5-17-94 */
            backtrace_through_instantiation(bt_pref->inst, grounds_level, cond, 0);

            /* check if any prohibit preferences */
            if (cond->bt.prohibits) {
                for (prohibits = cond->bt.prohibits; prohibits != NIL; prohibits = prohibits->rest) {
                    p = prohibits->first;

#ifndef TRACE_CONTEXT_DECISIONS_ONLY
                    if (current_agent(sysparams)[TRACE_BACKTRACING_SYSPARAM]) {
                        print_string("     For prohibit preference: ");
                        print_preference(p);
                    }
#endif
                    backtrace_through_instantiation(p->inst, grounds_level, cond, 6);
                }
            }
            /* mvp done */

#endif
            continue;
        }
#ifndef TRACE_CONTEXT_DECISIONS_ONLY
        if (current_agent(sysparams)[TRACE_BACKTRACING_SYSPARAM])
            print_string("...no trace, can't BT");
#endif

        /* --- for augmentations of the local goal id, either handle the
           "^quiescence t" test or discard it --- */
        if (referent_of_equality_test(cond->data.tests.id_test)->id.isa_goal) {
            if ((referent_of_equality_test(cond->data.tests.attr_test) ==
                 current_agent(quiescence_symbol)) &&
                (referent_of_equality_test(cond->data.tests.value_test) ==
                 current_agent(t_symbol)) && (!cond->test_for_acceptable_preference)) {
                current_agent(variablize_this_chunk) = FALSE;
                current_agent(quiescence_t_flag) = TRUE;
            }
            continue;
        }

        /* --- otherwise add it to the potential set --- */

#ifndef TRACE_CONTEXT_DECISIONS_ONLY
        if (current_agent(sysparams)[TRACE_BACKTRACING_SYSPARAM])
            print_string(" --> make it a potential.");
#endif
        add_to_potentials(cond);

    }                           /* end of while locals loop */
}

/* ---------------------------------------------------------------
                       Trace Grounded Potentials

   This routine looks for positive potentials that are in the TC
   of the ground set, and moves them over to the ground set.  This
   process is repeated until no more positive potentials are in
   the TC of the grounds.
--------------------------------------------------------------- */

void trace_grounded_potentials(void)
{
    tc_number tc;
    cons *c, *next_c, *prev_c;
    condition *pot;
    bool need_another_pass;

#ifndef TRACE_CONTEXT_DECISIONS_ONLY

    if (current_agent(sysparams)[TRACE_BACKTRACING_SYSPARAM])
        print_string("\n\n*** Tracing Grounded Potentials ***\n");
#endif

    /* --- setup the tc of the ground set --- */
    tc = get_new_tc_number();
    for (c = current_agent(grounds); c != NIL; c = c->rest)
        add_cond_to_tc(c->first, tc, NIL, NIL);

    need_another_pass = TRUE;
    while (need_another_pass) {
        need_another_pass = FALSE;
        /* --- look for any potentials that are in the tc now --- */
        prev_c = NIL;
        for (c = current_agent(positive_potentials); c != NIL; c = next_c) {
            next_c = c->rest;
            pot = c->first;
            if (cond_is_in_tc(pot, tc)) {
                /* --- pot is a grounded potential, move it over to ground set --- */

#ifndef TRACE_CONTEXT_DECISIONS_ONLY
                if (current_agent(sysparams)[TRACE_BACKTRACING_SYSPARAM]) {
                    print_string("\n-->Moving to grounds: ");
                    print_wme(pot->bt.wme);
                }
#endif
                if (prev_c)
                    prev_c->rest = next_c;
                else
                    current_agent(positive_potentials) = next_c;
                if (pot->bt.wme->grounds_tc != current_agent(grounds_tc)) {     /* add pot to grounds */
                    pot->bt.wme->grounds_tc = current_agent(grounds_tc);
                    c->rest = current_agent(grounds);
                    current_agent(grounds) = c;
                    add_cond_to_tc(pot, tc, NIL, NIL);
                    need_another_pass = TRUE;
                } else {        /* pot was already in the grounds, do don't add it */
                    free_cons(c);
                }
            } else {
                prev_c = c;
            }
        }                       /* end of for c */
    }                           /* end of while need_another_pass */
}

/* ---------------------------------------------------------------
                     Trace Ungrounded Potentials

   This routine backtraces through ungrounded potentials.  At entry,
   all potentials must be ungrounded.  This BT's through each
   potential that has some trace (at the right level) that we can
   BT through.  Other potentials are left alone.  TRUE is returned
   if anything was BT'd; FALSE if nothing changed.
--------------------------------------------------------------- */

bool trace_ungrounded_potentials(goal_stack_level grounds_level)
{

    /* mvp 5-17-94 */
    cons *c, *next_c, *prev_c, *prohibits;
    cons *pots_to_bt;
    condition *potential;
    preference *bt_pref, *p;

#ifndef TRACE_CONTEXT_DECISIONS_ONLY
    if (current_agent(sysparams)[TRACE_BACKTRACING_SYSPARAM])
        print_string("\n\n*** Tracing Ungrounded Potentials ***\n");
#endif

    /* --- scan through positive potentials, pick out the ones that have
       a preference we can backtrace through --- */
    pots_to_bt = NIL;
    prev_c = NIL;
    for (c = current_agent(positive_potentials); c != NIL; c = next_c) {
        next_c = c->rest;
        potential = c->first;
        bt_pref = find_clone_for_level(potential->bt.trace, (goal_stack_level) (grounds_level + 1));
        if (bt_pref) {
            if (prev_c)
                prev_c->rest = next_c;
            else
                current_agent(positive_potentials) = next_c;
            c->rest = pots_to_bt;
            pots_to_bt = c;
        } else {
            prev_c = c;
        }
    }

    /* --- if none to BT, exit --- */
    if (!pots_to_bt)
        return FALSE;

    /* --- backtrace through each one --- */
    while (pots_to_bt) {
        c = pots_to_bt;
        pots_to_bt = pots_to_bt->rest;
        potential = c->first;
        free_cons(c);

#ifndef TRACE_CONTEXT_DECISIONS_ONLY
        if (current_agent(sysparams)[TRACE_BACKTRACING_SYSPARAM]) {
            print_string("\nFor ungrounded potential ");
            print_wme(potential->bt.wme);
            print_string(" ");
        }
#endif

        bt_pref = find_clone_for_level(potential->bt.trace, (goal_stack_level) (grounds_level + 1));

#ifdef NO_TOP_JUST
        if (bt_pref->inst)
            backtrace_through_instantiation(bt_pref->inst, grounds_level, potential, 0);
#else

        /* mvp 5-17-94 */
        backtrace_through_instantiation(bt_pref->inst, grounds_level, potential, 0);
#endif

        if (potential->bt.prohibits) {
            for (prohibits = potential->bt.prohibits; prohibits != NIL; prohibits = prohibits->rest) {
                p = prohibits->first;

#ifndef TRACE_CONTEXT_DECISIONS_ONLY
                if (current_agent(sysparams)[TRACE_BACKTRACING_SYSPARAM]) {
                    print_string("     For prohibit preference: ");
                    print_preference(p);
                }
#endif

#ifdef NO_TOP_JUST
                if (p->inst)
                    backtrace_through_instantiation(p->inst, grounds_level, potential, 6);
#else
                backtrace_through_instantiation(p->inst, grounds_level, potential, 6);
#endif

            }
        }
        /* mvp done */
    }
    return TRUE;
}
