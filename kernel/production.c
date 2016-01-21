/*************************************************************************
 *
 *  file:  production.c
 *
 * ====================================================================
 *                    Production Utilities for Soar 6
 *
 * This file contains various utility routines for manipulating 
 * productions and parts of productions:  tests, conditions, actions,
 * etc.  Also includes the reorderer and compile-time o-support calculations.
 * parser.c loads productions.
 * Init_production_utilities() should be called before anything else here.
 * =======================================================================
 */
#include "bsd-license.h"

#include "soarkernel.h"
#include <ctype.h>

/* comment out the following line to supress compile-time o-support
   calculations */
/* RCHONG: begin 10.11 */
/* #define DO_COMPILE_TIME_O_SUPPORT_CALCS */
/* RCHONG: end 10.11 */

/* uncomment the following line to get printouts of names of productions
   that can't be fully compile-time o-support evaluated */
/* #define LIST_COMPILE_TIME_O_SUPPORT_FAILURES */

void init_production_utilities(void)
{
    init_memory_pool(&current_agent(complex_test_pool), sizeof(complex_test), "complex test");
    init_memory_pool(&current_agent(condition_pool), sizeof(condition), "condition");
    init_memory_pool(&current_agent(production_pool), sizeof(production), "production");
    init_memory_pool(&current_agent(action_pool), sizeof(action), "action");
    init_memory_pool(&current_agent(not_pool), sizeof(not), "not");
    init_reorderer();
}

/* ********************************************************************

           Utility Routines for Various Parts of Productions

******************************************************************** */

/* ====================================================================

              Utilities for Symbols and Lists of Symbols

==================================================================== */

/* -----------------------------------------------------------------
                       First Letter From Symbol

   When creating dummy variables or identifiers, we try to give them
   names that start with a "reasonable" letter.  For example, ^foo <dummy>
   becomes ^foo <f*37>, where the variable starts with "f" because
   the attribute test starts with "f" also.  This routine looks at
   a symbol and tries to figure out a reasonable choice of starting
   letter for a variable or identifier to follow it.  If it can't
   find a reasonable choice, it returns '*'.
----------------------------------------------------------------- */

char first_letter_from_symbol(Symbol * sym)
{
    switch (sym->common.symbol_type) {
    case VARIABLE_SYMBOL_TYPE:
        return *(sym->var.name + 1);
    case IDENTIFIER_SYMBOL_TYPE:
        return sym->id.name_letter;
    case SYM_CONSTANT_SYMBOL_TYPE:
        return *(sym->sc.name);
    default:
        return '*';
    }
}

/* -----------------------------------------------------------------
   Find first letter of test, or '*' if nothing appropriate.
   (See comments on first_letter_from_symbol for more explanation.)
----------------------------------------------------------------- */

char first_letter_from_test(test t)
{
    complex_test *ct;
    cons *c;
    char ch;

    if (test_is_blank_test(t))
        return '*';
    if (test_is_blank_or_equality_test(t))
        return first_letter_from_symbol(referent_of_equality_test(t));

    ct = complex_test_from_test(t);
    switch (ct->type) {
    case GOAL_ID_TEST:
        return 's';
    case IMPASSE_ID_TEST:
        return 'i';
    case CONJUNCTIVE_TEST:
        for (c = ct->data.conjunct_list; c != NIL; c = c->rest) {
            ch = first_letter_from_test(c->first);
            if (ch != '*')
                return ch;
        }
        return '*';
    default:                   /* disjunction tests, and relational tests other than equality */
        return '*';
    }
}

/* ----------------------------------------------------------------
   Takes a list of symbols and returns a copy of the same list,
   incrementing the reference count on each symbol in the list.
---------------------------------------------------------------- */

list *copy_symbol_list_adding_references(list * sym_list)
{
    cons *c, *first, *prev;

    if (!sym_list)
        return NIL;
    allocate_cons(&first);
    first->first = sym_list->first;
    symbol_add_ref((Symbol *) (first->first));
    sym_list = sym_list->rest;
    prev = first;
    while (sym_list) {
        allocate_cons(&c);
        prev->rest = c;
        c->first = sym_list->first;
        symbol_add_ref((Symbol *) (c->first));
        sym_list = sym_list->rest;
        prev = c;
    }
    prev->rest = NIL;
    return first;
}

/* ----------------------------------------------------------------
   Frees a list of symbols, decrementing their reference counts.
---------------------------------------------------------------- */

void deallocate_symbol_list_removing_references(list * sym_list)
{
    cons *c;

    while (sym_list) {
        c = sym_list;
        sym_list = sym_list->rest;
        symbol_remove_ref((Symbol *) (c->first));
        free_cons(c);
    }
}

/* =================================================================

                      Utility Routines for Tests

================================================================= */

/* --- This just copies a consed list of tests. --- */
list *copy_test_list(cons * c)
{
    cons *new_c;

    if (!c)
        return NIL;
    allocate_cons(&new_c);
    new_c->first = copy_test(c->first);
    new_c->rest = copy_test_list(c->rest);
    return new_c;
}

/* ----------------------------------------------------------------
   Takes a test and returns a new copy of it.
---------------------------------------------------------------- */

test copy_test(test t)
{
    Symbol *referent;
    complex_test *ct, *new_ct;

    if (test_is_blank_test(t))
        return make_blank_test();

    if (test_is_blank_or_equality_test(t)) {
        referent = referent_of_equality_test(t);
        return make_equality_test(referent);
    }

    ct = complex_test_from_test(t);

    allocate_with_pool(&current_agent(complex_test_pool), &new_ct);
    new_ct->type = ct->type;
    switch (ct->type) {
    case GOAL_ID_TEST:
    case IMPASSE_ID_TEST:
        break;
    case DISJUNCTION_TEST:
        new_ct->data.disjunction_list = copy_symbol_list_adding_references(ct->data.disjunction_list);
        break;
    case CONJUNCTIVE_TEST:
        new_ct->data.conjunct_list = copy_test_list(ct->data.conjunct_list);
        break;
    default:                   /* relational tests other than equality */
        new_ct->data.referent = ct->data.referent;
        symbol_add_ref(ct->data.referent);
        break;
    }
    return make_test_from_complex_test(new_ct);
}

/* ----------------------------------------------------------------
   Same as copy_test(), only it doesn't include goal or impasse tests
   in the new copy.  The caller should initialize the two flags to FALSE
   before calling this routine; it sets them to TRUE if it finds a goal
   or impasse test.
---------------------------------------------------------------- */

test copy_test_removing_goal_impasse_tests(test t, bool * removed_goal, bool * removed_impasse)
{
    complex_test *ct, *new_ct;
    cons *c;
    test new_t, temp;

    if (test_is_blank_or_equality_test(t))
        return copy_test(t);

    ct = complex_test_from_test(t);

    switch (ct->type) {
    case GOAL_ID_TEST:
        *removed_goal = TRUE;
        return make_blank_test();
    case IMPASSE_ID_TEST:
        *removed_impasse = TRUE;
        return make_blank_test();

    case CONJUNCTIVE_TEST:
        new_t = make_blank_test();
        for (c = ct->data.conjunct_list; c != NIL; c = c->rest) {
            temp = copy_test_removing_goal_impasse_tests(c->first, removed_goal, removed_impasse);
            if (!test_is_blank_test(temp))
                add_new_test_to_test(&new_t, temp);
        }
        if (test_is_complex_test(new_t)) {
            new_ct = complex_test_from_test(new_t);
            if (new_ct->type == CONJUNCTIVE_TEST)
                new_ct->data.conjunct_list = destructively_reverse_list(new_ct->data.conjunct_list);
        }
        return new_t;

    default:                   /* relational tests other than equality */
        return copy_test(t);
    }
}

/* ----------------------------------------------------------------
   Deallocates a test.
---------------------------------------------------------------- */

void deallocate_test(test t)
{
    cons *c, *next_c;
    complex_test *ct;

    if (test_is_blank_test(t))
        return;
    if (test_is_blank_or_equality_test(t)) {
        symbol_remove_ref(referent_of_equality_test(t));
        return;
    }

    ct = complex_test_from_test(t);

    switch (ct->type) {
    case GOAL_ID_TEST:
    case IMPASSE_ID_TEST:
        break;
    case DISJUNCTION_TEST:
        deallocate_symbol_list_removing_references(ct->data.disjunction_list);
        break;
    case CONJUNCTIVE_TEST:
        c = ct->data.conjunct_list;
        while (c) {
            next_c = c->rest;
            deallocate_test(c->first);
            free_cons(c);
            c = next_c;
        }
        break;
    default:                   /* relational tests other than equality */
        symbol_remove_ref(ct->data.referent);
        break;
    }
    free_with_pool(&current_agent(complex_test_pool), ct);
}

/* --- Macro for doing this (usually) without procedure call overhead. --- */
#define quickly_deallocate_test(t) { \
  if (! test_is_blank_test(t)) { \
    if (test_is_blank_or_equality_test(t)) { \
      symbol_remove_ref (referent_of_equality_test(t)); \
    } else { \
      deallocate_test (t); } } }

/* ----------------------------------------------------------------
   Destructively modifies the first test (t) by adding the second
   one (add_me) to it (usually as a new conjunct).  The first test
   need not be a conjunctive test.
---------------------------------------------------------------- */

void add_new_test_to_test(test * t, test add_me)
{
    complex_test *ct = NULL;
    cons *c;
    bool already_a_conjunctive_test;

    if (test_is_blank_test(add_me))
        return;

    if (test_is_blank_test(*t)) {
        *t = add_me;
        return;
    }

    /* --- if *t isn't already a conjunctive test, make it into one --- */
    already_a_conjunctive_test = FALSE;
    if (test_is_complex_test(*t)) {
        ct = complex_test_from_test(*t);
        if (ct->type == CONJUNCTIVE_TEST)
            already_a_conjunctive_test = TRUE;
    }

    if (!already_a_conjunctive_test) {
        allocate_with_pool(&current_agent(complex_test_pool), &ct);
        ct->type = CONJUNCTIVE_TEST;
        allocate_cons(&c);
        ct->data.conjunct_list = c;
        c->first = *t;
        c->rest = NIL;
        *t = make_test_from_complex_test(ct);
    }
    /* --- at this point, ct points to the complex test structure for *t --- */

    /* --- now add add_me to the conjunct list --- */
    allocate_cons(&c);
    c->first = add_me;
    c->rest = ct->data.conjunct_list;
    ct->data.conjunct_list = c;
}

/* ----------------------------------------------------------------
   Same as add_new_test_to_test(), only has no effect if the second
   test is already included in the first one.
---------------------------------------------------------------- */

void add_new_test_to_test_if_not_already_there(test * t, test add_me)
{
    complex_test *ct;
    cons *c;

    if (tests_are_equal(*t, add_me)) {
        deallocate_test(add_me);
        return;
    }

    if (test_is_complex_test(*t)) {
        ct = complex_test_from_test(*t);
        if (ct->type == CONJUNCTIVE_TEST)
            for (c = ct->data.conjunct_list; c != NIL; c = c->rest)
                if (tests_are_equal(c->first, add_me)) {
                    deallocate_test(add_me);
                    return;
                }
    }

    add_new_test_to_test(t, add_me);
}

/* ----------------------------------------------------------------
   Returns TRUE iff the two tests are identical.
---------------------------------------------------------------- */

bool tests_are_equal(test t1, test t2)
{
    cons *c1, *c2;
    complex_test *ct1, *ct2;

    if (test_is_blank_or_equality_test(t1))
        return (bool) (t1 == t2);       /* Warning: this relies on the representation of tests */

    ct1 = complex_test_from_test(t1);
    ct2 = complex_test_from_test(t2);

    if (ct1->type != ct2->type)
        return FALSE;

    switch (ct1->type) {
    case GOAL_ID_TEST:
        return TRUE;
    case IMPASSE_ID_TEST:
        return TRUE;

    case DISJUNCTION_TEST:
        for (c1 = ct1->data.disjunction_list, c2 = ct2->data.disjunction_list;
             ((c1 != NIL) && (c2 != NIL)); c1 = c1->rest, c2 = c2->rest)
            if (c1->first != c2->first)
                return FALSE;
        if (c1 == c2)
            return TRUE;        /* make sure they both hit end-of-list */
        return FALSE;

    case CONJUNCTIVE_TEST:
        for (c1 = ct1->data.conjunct_list, c2 = ct2->data.conjunct_list;
             ((c1 != NIL) && (c2 != NIL)); c1 = c1->rest, c2 = c2->rest)
            if (!tests_are_equal(c1->first, c2->first))
                return FALSE;
        if (c1 == c2)
            return TRUE;        /* make sure they both hit end-of-list */
        return FALSE;

    default:                   /* relational tests other than equality */
        if (ct1->data.referent == ct2->data.referent)
            return TRUE;
        return FALSE;
    }
}

/* ----------------------------------------------------------------
   Returns a hash value for the given test.
---------------------------------------------------------------- */

unsigned long hash_test(test t)
{
    complex_test *ct;
    cons *c;
    unsigned long result;

    if (test_is_blank_test(t))
        return 0;

    if (test_is_blank_or_equality_test(t))
        return (referent_of_equality_test(t))->common.hash_id;

    ct = complex_test_from_test(t);

    switch (ct->type) {
    case GOAL_ID_TEST:
        return 34894895;        /* just use some unusual number */
    case IMPASSE_ID_TEST:
        return 2089521;
    case DISJUNCTION_TEST:
        result = 7245;
        for (c = ct->data.conjunct_list; c != NIL; c = c->rest)
            result = result + ((Symbol *) (c->first))->common.hash_id;
        return result;
    case CONJUNCTIVE_TEST:
        result = 100276;
        for (c = ct->data.disjunction_list; c != NIL; c = c->rest)
            result = result + hash_test(c->first);
        return result;
    case NOT_EQUAL_TEST:
    case LESS_TEST:
    case GREATER_TEST:
    case LESS_OR_EQUAL_TEST:
    case GREATER_OR_EQUAL_TEST:
    case SAME_TYPE_TEST:
        return (ct->type << 24) + ct->data.referent->common.hash_id;
    default:
        {
            char msg[MESSAGE_SIZE];
            strncpy(msg, "production.c: Error: bad test type in hash_test\n", MESSAGE_SIZE);
            msg[MESSAGE_SIZE - 1] = 0;
            abort_with_fatal_error(msg);
        }
    }
    return 0;                   /* unreachable, but without it, gcc -Wall warns here */
}

/****************************/
  /* ----------------------------------------------------------------
     This returns a boolean that indicates that one condition is
     greater than another in some ordering of the conditions. The ordering
     is dependent upon the hash-value of each of the tests in the
     condition.
     ------------------------------------------------------------------ */

#define NON_EQUAL_TEST_RETURN_VAL 0     /* some unusual number */

unsigned long canonical_test(test t)
{
    Symbol *sym;

    if (test_is_blank_test(t))
        return NON_EQUAL_TEST_RETURN_VAL;

    if (test_is_blank_or_equality_test(t)) {
        sym = referent_of_equality_test(t);
        if (sym->common.symbol_type == SYM_CONSTANT_SYMBOL_TYPE ||
            sym->common.symbol_type == INT_CONSTANT_SYMBOL_TYPE ||
            sym->common.symbol_type == FLOAT_CONSTANT_SYMBOL_TYPE) {
            return (sym->common.hash_id);
        } else
            return NON_EQUAL_TEST_RETURN_VAL;
    }
    return NON_EQUAL_TEST_RETURN_VAL;
}

#define CANONICAL_TEST_ORDER canonical_test

/*
#define CANONICAL_TEST_ORDER hash_test
*/

bool canonical_cond_greater(condition * c1, condition * c2)
/*

 Original:  676,362 total rete nodes (1 dummy + 560045 positive + 4
            unhashed positive + 2374 negative + 113938 p_nodes)

The following notation describes the order of tests and the relation
of the hash_test that was used. IAV< means test the (I)d field first
then the (A)ttribute field, then the (V)alue field. and use less than
as the ordering constraint. The actual ordering constraint should not
make any difference.

 IAV<:  737,605 total rete nodes (1 dummy + 617394 positive + 3
            unhashed positive + 6269 negative + 113938 p_nodes)

Realized that the identifier will always be a variable and thus
shouldn't be part of the ordering.

Changed to put all negative tests in front of cost 1 tests list.
   That is always break ties of cost 1 with a negative test if
   it exists.

Changed so that canonical_test_order returns a negative -1 when
comparing anything but constants. Also fixed a bug.

Consistency checks:

 Original:  676,362 total rete nodes (1 dummy + 560045 positive + 4
            unhashed positive + 2374 negative + 113938 p_nodes)
    Still holds with 1 optimization in and always returning False

 Remove 1:  720,126 total rete nodes (1 dummy + 605760 positive + 4
            unhashed positive +  423 negative + 113938 p_nodes)
    Always returning False causes the first item in the tie list to
       be picked.

 Surprise:  637,482 total rete nodes (1 dummy + 523251 positive + 3
            unhashed positive +  289 negative + 113938 p_nodes)
    Without 1 optimization and always returning True. Causes the
      last item in 1-tie list to be picked.

 In the following tests ht means hash test provided the canonical
 value. ct means that the routine constant test provided the canonical
 value. ct provides a value for non constant equality tests. I tried
 both 0 and a big number (B)  with no difference noted.

  ht_AV<:   714,427 total rete nodes (1 dummy + 600197 positive + 2
            unhashed positive +  289 negative + 113938 p_nodes)

  ht_AV>:   709,637 total rete nodes (1 dummy + 595305 positive + 3
            unhashed positive +  390 negative + 113938 p_nodes)

  ct0_AV>:  709,960 total rete nodes (1 dummy + 595628 positive + 3
            unhashed positive +  390 negative + 113938 p_nodes)

  ct0_AV<:  714,162 total rete nodes (1 dummy + 599932 positive + 2
            unhashed positive +  289 negative + 113938 p_nodes)

  ctB_AV>:  709,960 total rete nodes (1 dummy + 595628 positive + 3
            unhashed positive +  390 negative + 113938 p_nodes)

  ctB_AV<:  714,162 total rete nodes (1 dummy + 599932 positive + 2
            unhashed positive +  289 negative + 113938 p_nodes)

  ctB_VA>:  691,193 total rete nodes (1 dummy + 576861 positive + 3
            unhashed positive +  390 negative + 113938 p_nodes)

  ctB_VA<:  704,539 total rete nodes (1 dummy + 590309 positive + 2
            unhashed positive +  289 negative + 113938 p_nodes)

  ct0_VA<:  744,604 total rete nodes (1 dummy + 630374 positive + 2
            unhashed positive +  289 negative + 113938 p_nodes)

  ct0_VA>:  672,367 total rete nodes (1 dummy + 558035 positive + 3
            unhashed positive +  390 negative + 113938 p_nodes)

   ht_VA>:  727,742 total rete nodes (1 dummy + 613517 positive + 3
            unhashed positive +  283 negative + 113938 p_nodes)

   ht_VA<:  582,559 total rete nodes (1 dummy + 468328 positive + 3
            unhashed positive +  289 negative + 113938 p_nodes)

Changed  < to > 10/5/92*/
{
    unsigned long test_order_1, test_order_2;

    if ((test_order_1 = CANONICAL_TEST_ORDER(c1->data.tests.attr_test)) <
        (test_order_2 = CANONICAL_TEST_ORDER(c2->data.tests.attr_test))) {
        return TRUE;
    } else if (test_order_1 == test_order_2 &&
               CANONICAL_TEST_ORDER(c1->data.tests.value_test) < CANONICAL_TEST_ORDER(c2->data.tests.value_test)) {
        return TRUE;
    }
    return FALSE;
}

/* ----------------------------------------------------------------
   Returns TRUE iff the test contains an equality test for the given
   symbol.  If sym==NIL, returns TRUE iff the test contains any
   equality test.
---------------------------------------------------------------- */

bool test_includes_equality_test_for_symbol(test t, Symbol * sym)
{
    cons *c;
    complex_test *ct;

    if (test_is_blank_test(t))
        return FALSE;

    if (test_is_blank_or_equality_test(t)) {
        if (sym)
            return (bool) (referent_of_equality_test(t) == sym);
        return TRUE;
    }

    ct = complex_test_from_test(t);

    if (ct->type == CONJUNCTIVE_TEST) {
        for (c = ct->data.conjunct_list; c != NIL; c = c->rest)
            if (test_includes_equality_test_for_symbol(c->first, sym))
                return TRUE;
    }
    return FALSE;
}

/* ----------------------------------------------------------------
   Looks for goal or impasse tests (as directed by the two flag
   parameters) in the given test, and returns TRUE if one is found.
---------------------------------------------------------------- */

bool test_includes_goal_or_impasse_id_test(test t, bool look_for_goal, bool look_for_impasse)
{
    complex_test *ct;
    cons *c;

    if (test_is_blank_or_equality_test(t))
        return FALSE;
    ct = complex_test_from_test(t);
    if (look_for_goal && (ct->type == GOAL_ID_TEST))
        return TRUE;
    if (look_for_impasse && (ct->type == IMPASSE_ID_TEST))
        return TRUE;
    if (ct->type == CONJUNCTIVE_TEST) {
        for (c = ct->data.conjunct_list; c != NIL; c = c->rest)
            if (test_includes_goal_or_impasse_id_test(c->first, look_for_goal, look_for_impasse))
                return TRUE;
        return FALSE;
    }
    return FALSE;
}

/* ----------------------------------------------------------------
   Looks through a test, and returns a new copy of the first equality
   test it finds.  Signals an error if there is no equality test in
   the given test.
---------------------------------------------------------------- */

test copy_of_equality_test_found_in_test(test t)
{
    complex_test *ct;
    cons *c;
    char msg[MESSAGE_SIZE];

    if (test_is_blank_test(t)) {
        strncpy(msg, "Internal error: can't find equality test in test\n", MESSAGE_SIZE);
        msg[MESSAGE_SIZE - 1] = 0;
        abort_with_fatal_error(msg);
    }
    if (test_is_blank_or_equality_test(t))
        return copy_test(t);
    ct = complex_test_from_test(t);
    if (ct->type == CONJUNCTIVE_TEST) {
        for (c = ct->data.conjunct_list; c != NIL; c = c->rest)
            if ((!test_is_blank_test((test) (c->first))) && (test_is_blank_or_equality_test((test) (c->first))))
                return copy_test(c->first);
    }
    strncpy(msg, "Internal error: can't find equality test in test\n", MESSAGE_SIZE);
    msg[MESSAGE_SIZE - 1] = 0;
    abort_with_fatal_error(msg);
    return 0;                   /* unreachable, but without it, gcc -Wall warns here */
}

/* =================================================================

                  Utility Routines for Conditions

================================================================= */

/* ----------------------------------------------------------------
   Deallocates a condition list (including any NCC's and tests in it).
---------------------------------------------------------------- */

void deallocate_condition_list(condition * cond_list)
{
    condition *c;

    while (cond_list) {
        c = cond_list;

        cond_list = cond_list->next;
        if (c->type == CONJUNCTIVE_NEGATION_CONDITION) {
            deallocate_condition_list(c->data.ncc.top);
        } else {                /* positive and negative conditions */
            quickly_deallocate_test(c->data.tests.id_test);
            quickly_deallocate_test(c->data.tests.attr_test);
            quickly_deallocate_test(c->data.tests.value_test);
        }
        free_with_pool(&current_agent(condition_pool), c);
    }
}

/* ----------------------------------------------------------------
   Returns a new copy of the given condition.
---------------------------------------------------------------- */

condition *copy_condition(condition * cond)
{
    condition *new;

    if (!cond)
        return NIL;
    allocate_with_pool(&current_agent(condition_pool), &new);
    new->type = cond->type;

    switch (cond->type) {
    case POSITIVE_CONDITION:
        new->bt = cond->bt;
        /* ... and fall through to next case */
    case NEGATIVE_CONDITION:
        new->data.tests.id_test = copy_test(cond->data.tests.id_test);
        new->data.tests.attr_test = copy_test(cond->data.tests.attr_test);
        new->data.tests.value_test = copy_test(cond->data.tests.value_test);
        new->test_for_acceptable_preference = cond->test_for_acceptable_preference;
        break;
    case CONJUNCTIVE_NEGATION_CONDITION:
        copy_condition_list(cond->data.ncc.top, &(new->data.ncc.top), &(new->data.ncc.bottom));
        break;
    }
    return new;
}

/* ----------------------------------------------------------------
   Copies the given condition list, returning pointers to the
   top-most and bottom-most conditions in the new copy.
---------------------------------------------------------------- */

void copy_condition_list(condition * top_cond, condition ** dest_top, condition ** dest_bottom)
{
    condition *new, *prev;

    prev = NIL;
    while (top_cond) {
        new = copy_condition(top_cond);
        if (prev)
            prev->next = new;
        else
            *dest_top = new;
        new->prev = prev;
        prev = new;
        top_cond = top_cond->next;
    }
    if (prev)
        prev->next = NIL;
    else
        *dest_top = NIL;
    *dest_bottom = prev;
}

/* ----------------------------------------------------------------
   Returns TRUE iff the two conditions are identical.
---------------------------------------------------------------- */

bool conditions_are_equal(condition * c1, condition * c2)
{
    if (c1->type != c2->type)
        return FALSE;
    switch (c1->type) {
    case POSITIVE_CONDITION:
    case NEGATIVE_CONDITION:
        if (!tests_are_equal(c1->data.tests.id_test, c2->data.tests.id_test))
            return FALSE;
        if (!tests_are_equal(c1->data.tests.attr_test, c2->data.tests.attr_test))
            return FALSE;
        if (!tests_are_equal(c1->data.tests.value_test, c2->data.tests.value_test))
            return FALSE;
        if (c1->test_for_acceptable_preference != c2->test_for_acceptable_preference)
            return FALSE;
        return TRUE;

    case CONJUNCTIVE_NEGATION_CONDITION:
        for (c1 = c1->data.ncc.top, c2 = c2->data.ncc.top; ((c1 != NIL) && (c2 != NIL)); c1 = c1->next, c2 = c2->next)
            if (!conditions_are_equal(c1, c2))
                return FALSE;
        if (c1 == c2)
            return TRUE;        /* make sure they both hit end-of-list */
        return FALSE;
    }
    return FALSE;               /* unreachable, but without it, gcc -Wall warns here */
}

/* ----------------------------------------------------------------
   Returns a hash value for the given condition.
---------------------------------------------------------------- */

unsigned long hash_condition(condition * cond)
{
    unsigned long result;
    condition *c;

    switch (cond->type) {
    case POSITIVE_CONDITION:
        result = hash_test(cond->data.tests.id_test);
        result = (result << 24) | (result >> 8);
        result ^= hash_test(cond->data.tests.attr_test);
        result = (result << 24) | (result >> 8);
        result ^= hash_test(cond->data.tests.value_test);
        if (cond->test_for_acceptable_preference)
            result++;
        break;
    case NEGATIVE_CONDITION:
        result = 1267818;
        result ^= hash_test(cond->data.tests.id_test);
        result = (result << 24) | (result >> 8);
        result ^= hash_test(cond->data.tests.attr_test);
        result = (result << 24) | (result >> 8);
        result ^= hash_test(cond->data.tests.value_test);
        if (cond->test_for_acceptable_preference)
            result++;
        break;
    case CONJUNCTIVE_NEGATION_CONDITION:
        result = 82348149;
        for (c = cond->data.ncc.top; c != NIL; c = c->next) {
            result ^= hash_condition(c);
            result = (result << 24) | (result >> 8);
        }
        break;
    default:
        {
            char msg[MESSAGE_SIZE];
            strncpy(msg, "Internal error: bad cond type in hash_condition\n", MESSAGE_SIZE);
            msg[MESSAGE_SIZE - 1] = 0;
            abort_with_fatal_error(msg);
        }
        result = 0;             /* unreachable, but gcc -Wall warns without it */
    }
    return result;
}

/* =================================================================

              Utility Routines for Actions and RHS Values

================================================================= */

/* ----------------------------------------------------------------
   Deallocates the given rhs_value.
---------------------------------------------------------------- */

void deallocate_rhs_value(rhs_value rv)
{
    cons *c;
    list *fl;

    if (rhs_value_is_reteloc(rv))
        return;
    if (rhs_value_is_unboundvar(rv))
        return;
    if (rhs_value_is_funcall(rv)) {
        fl = rhs_value_to_funcall_list(rv);
        for (c = fl->rest; c != NIL; c = c->rest)
            deallocate_rhs_value(c->first);
        free_list(fl);
    } else {
        symbol_remove_ref(rhs_value_to_symbol(rv));
    }
}

/* ----------------------------------------------------------------
   Returns a new copy of the given rhs_value.
---------------------------------------------------------------- */

rhs_value copy_rhs_value(rhs_value rv)
{
    cons *c, *new_c, *prev_new_c;
    list *fl, *new_fl;

    if (rhs_value_is_reteloc(rv))
        return rv;
    if (rhs_value_is_unboundvar(rv))
        return rv;
    if (rhs_value_is_funcall(rv)) {
        fl = rhs_value_to_funcall_list(rv);
        allocate_cons(&new_fl);
        new_fl->first = fl->first;
        prev_new_c = new_fl;
        for (c = fl->rest; c != NIL; c = c->rest) {
            allocate_cons(&new_c);
            new_c->first = copy_rhs_value(c->first);
            prev_new_c->rest = new_c;
            prev_new_c = new_c;
        }
        prev_new_c->rest = NIL;
        return funcall_list_to_rhs_value(new_fl);
    } else {
        symbol_add_ref(rhs_value_to_symbol(rv));
        return rv;
    }
}

/* ----------------------------------------------------------------
   Deallocates the given action (singly-linked) list.
---------------------------------------------------------------- */

void deallocate_action_list(action * actions)
{
    action *a;

    while (actions) {
        a = actions;
        actions = actions->next;
        if (a->type == FUNCALL_ACTION) {
            deallocate_rhs_value(a->value);
        } else {
            /* --- make actions --- */
            deallocate_rhs_value(a->id);
            deallocate_rhs_value(a->attr);
            deallocate_rhs_value(a->value);
            if (preference_is_binary(a->preference_type))
                deallocate_rhs_value(a->referent);
        }
        free_with_pool(&current_agent(action_pool), a);
    }
}

/* -----------------------------------------------------------------
   Find first letter of rhs_value, or '*' if nothing appropriate.
   (See comments on first_letter_from_symbol for more explanation.)
----------------------------------------------------------------- */

char first_letter_from_rhs_value(rhs_value rv)
{
    if (rhs_value_is_symbol(rv))
        return first_letter_from_symbol(rhs_value_to_symbol(rv));
    return '*';                 /* function calls, reteloc's, unbound variables */
}

/* =================================================================

                    Utility Routines for Nots

================================================================= */

/* ----------------------------------------------------------------
   Deallocates the given (singly-linked) list of Nots.
---------------------------------------------------------------- */

void deallocate_list_of_nots(not * nots)
{
    not *temp;

    while (nots) {
        temp = nots;
        nots = nots->next;
        symbol_remove_ref(temp->s1);
        symbol_remove_ref(temp->s2);
        free_with_pool(&current_agent(not_pool), temp);
    }
}

/* *********************************************************************

                    Transitive Closure Utilities

********************************************************************* */

/* =====================================================================

              Increment TC Counter and Return New TC Number

   Get_new_tc_number() is called from lots of places.  Any time we need
   to mark a set of identifiers and/or variables, we get a new tc_number
   by calling this routine, then proceed to mark various ids or vars
   by setting the sym->id.tc_num or sym->var.tc_num fields.

   A global tc number counter is maintained and incremented by this
   routine in order to generate a different tc_number each time.  If
   the counter ever wraps around back to 0, we bump it up to 1 and
   reset the the tc_num fields on all existing identifiers and variables
   to 0.
===================================================================== */

tc_number current_tc_number = 0;

tc_number get_new_tc_number(void)
{
    current_tc_number++;
    if (current_tc_number == 0) {
        reset_id_and_variable_tc_numbers();
        current_tc_number = 1;
    }
    return current_tc_number;
}

/* =====================================================================

               Marking, Unmarking, and Collecting Symbols

   Sometimes in addition to marking symbols using their tc_num fields,
   we also want to build up a list of the symbols we've marked.  So,
   many routines in this file take an "id_list" or "var_list" argument.
   This argument should be NIL if no such list is desired.  If non-NIL,
   it should point to the header of the linked list being built.

   Mark_identifier_if_unmarked() and mark_variable_if_unmarked() are
   macros for adding id's and var's to the set of symbols.

   Unmark_identifiers_and_free_list() unmarks all the id's in the given
   list, and deallocates the list.  Unmark_variables_and_free_list()
   is similar, only the list should be a list of variables rather than
   identifiers.

   Symbol_is_constant_or_marked_variable() tests whether the given symbol
   is either a constant (non-variable) or a variable marked with the
   given tc number.
===================================================================== */

#define mark_identifier_if_unmarked(ident,tc,id_list) { \
  if ((ident)->id.tc_num != (tc)) { \
    (ident)->id.tc_num = (tc); \
    if (id_list) push ((ident),(*(id_list))); } }

#define mark_variable_if_unmarked(v,tc,var_list) { \
  if ((v)->var.tc_num != (tc)) { \
    (v)->var.tc_num = (tc); \
    if (var_list) push ((v),(*(var_list))); } }

void unmark_identifiers_and_free_list(list * id_list)
{
    cons *next;
    Symbol *sym;

    while (id_list) {
        sym = id_list->first;
        next = id_list->rest;
        free_cons(id_list);
        sym->id.tc_num = 0;
        id_list = next;
    }
}

void unmark_variables_and_free_list(list * var_list)
{
    cons *next;
    Symbol *sym;

    while (var_list) {
        sym = var_list->first;
        next = var_list->rest;
        free_cons(var_list);
        sym->var.tc_num = 0;
        var_list = next;
    }
}

/* =====================================================================

   Finding the variables bound in tests, conditions, and condition lists

   These routines collect the variables that are bound in tests, etc.  Their
   "var_list" arguments should either be NIL or else should point to
   the header of the list of marked variables being constructed.
===================================================================== */

void add_bound_variables_in_test(test t, tc_number tc, list ** var_list)
{
    cons *c;
    Symbol *referent;
    complex_test *ct;

    if (test_is_blank_test(t))
        return;

    if (test_is_blank_or_equality_test(t)) {
        referent = referent_of_equality_test(t);
        if (referent->common.symbol_type == VARIABLE_SYMBOL_TYPE)
            mark_variable_if_unmarked(referent, tc, var_list);
        return;
    }

    ct = complex_test_from_test(t);
    if (ct->type == CONJUNCTIVE_TEST) {
        for (c = ct->data.conjunct_list; c != NIL; c = c->rest)
            add_bound_variables_in_test(c->first, tc, var_list);
    }
}

void add_bound_variables_in_condition(condition * c, tc_number tc, list ** var_list)
{
    if (c->type != POSITIVE_CONDITION)
        return;
    add_bound_variables_in_test(c->data.tests.id_test, tc, var_list);
    add_bound_variables_in_test(c->data.tests.attr_test, tc, var_list);
    add_bound_variables_in_test(c->data.tests.value_test, tc, var_list);
}

void add_bound_variables_in_condition_list(condition * cond_list, tc_number tc, list ** var_list)
{
    condition *c;

    for (c = cond_list; c != NIL; c = c->next)
        add_bound_variables_in_condition(c, tc, var_list);
}

/* =====================================================================

   Finding all variables from tests, conditions, and condition lists

   These routines collect all the variables in tests, etc.  Their
   "var_list" arguments should either be NIL or else should point to
   the header of the list of marked variables being constructed.
===================================================================== */

void add_all_variables_in_test(test t, tc_number tc, list ** var_list)
{
    cons *c;
    Symbol *referent;
    complex_test *ct;

    if (test_is_blank_test(t))
        return;

    if (test_is_blank_or_equality_test(t)) {
        referent = referent_of_equality_test(t);
        if (referent->common.symbol_type == VARIABLE_SYMBOL_TYPE)
            mark_variable_if_unmarked(referent, tc, var_list);
        return;
    }

    ct = complex_test_from_test(t);

    switch (ct->type) {
    case GOAL_ID_TEST:
    case IMPASSE_ID_TEST:
    case DISJUNCTION_TEST:
        break;

    case CONJUNCTIVE_TEST:
        for (c = ct->data.conjunct_list; c != NIL; c = c->rest)
            add_all_variables_in_test(c->first, tc, var_list);
        break;

    default:
        /* --- relational tests other than equality --- */
        referent = ct->data.referent;
        if (referent->common.symbol_type == VARIABLE_SYMBOL_TYPE)
            mark_variable_if_unmarked(referent, tc, var_list);
        break;
    }
}

void add_all_variables_in_condition_list(condition * cond_list, tc_number tc, list ** var_list);

void add_all_variables_in_condition(condition * c, tc_number tc, list ** var_list)
{
    if (c->type == CONJUNCTIVE_NEGATION_CONDITION) {
        add_all_variables_in_condition_list(c->data.ncc.top, tc, var_list);
    } else {
        add_all_variables_in_test(c->data.tests.id_test, tc, var_list);
        add_all_variables_in_test(c->data.tests.attr_test, tc, var_list);
        add_all_variables_in_test(c->data.tests.value_test, tc, var_list);
    }
}

void add_all_variables_in_condition_list(condition * cond_list, tc_number tc, list ** var_list)
{
    condition *c;

    for (c = cond_list; c != NIL; c = c->next)
        add_all_variables_in_condition(c, tc, var_list);
}

/* =====================================================================

   Finding all variables from rhs_value's, actions, and action lists

   These routines collect all the variables in rhs_value's, etc.  Their
   "var_list" arguments should either be NIL or else should point to
   the header of the list of marked variables being constructed.

   Warning: These are part of the reorderer and handle only productions
   in non-reteloc, etc. format.  They don't handle reteloc's or
   RHS unbound variables.
===================================================================== */

void add_all_variables_in_rhs_value(rhs_value rv, tc_number tc, list ** var_list)
{
    list *fl;
    cons *c;
    Symbol *sym;

    if (rhs_value_is_symbol(rv)) {
        /* --- ordinary values (i.e., symbols) --- */
        sym = rhs_value_to_symbol(rv);
        if (sym->common.symbol_type == VARIABLE_SYMBOL_TYPE)
            mark_variable_if_unmarked(sym, tc, var_list);
    } else {
        /* --- function calls --- */
        fl = rhs_value_to_funcall_list(rv);
        for (c = fl->rest; c != NIL; c = c->rest)
            add_all_variables_in_rhs_value(c->first, tc, var_list);
    }
}

void add_all_variables_in_action(action * a, tc_number tc, list ** var_list)
{
    Symbol *id;

    if (a->type == MAKE_ACTION) {
        /* --- ordinary make actions --- */
        id = rhs_value_to_symbol(a->id);
        if (id->common.symbol_type == VARIABLE_SYMBOL_TYPE)
            mark_variable_if_unmarked(id, tc, var_list);
        add_all_variables_in_rhs_value(a->attr, tc, var_list);
        add_all_variables_in_rhs_value(a->value, tc, var_list);
        if (preference_is_binary(a->preference_type))
            add_all_variables_in_rhs_value(a->referent, tc, var_list);
    } else {
        /* --- function call actions --- */
        add_all_variables_in_rhs_value(a->value, tc, var_list);
    }
}

void add_all_variables_in_action_list(action * actions, tc_number tc, list ** var_list)
{
    action *a;

    for (a = actions; a != NIL; a = a->next)
        add_all_variables_in_action(a, tc, var_list);
}

/* ====================================================================

              Transitive Closure for Conditions and Actions

   These routines do transitive closure calculations for tests,
   conditions, actions, etc.

   Usage: 
     1. Set my_tc = get_new_tc_number() to start a new TC
     2. (optional) If you want linked lists of symbols in the TC, initialize
        id_list=NIL and var_list=NIL.
        If you're not using id_list and/or var_list, give NIL for "&id_list"
        and/or "&var_list" in the function calls below.
     3. (optional) setup any id's or var's that you want to include in the
        initial TC, by calling 
           add_symbol_to_tc (sym, my_tc, &id_list, &var_list)
        (If not using id_list or var_list, you can just mark
         sym->{id,var}.tc_num = my_tc instead.)
     4. To do the work you want, use any of the following any number of times:
            add_cond_to_tc (cond, my_tc, &id_list, &var_list);
            add_action_to_tc (cond, my_tc, &id_list, &var_list);
            result = cond_is_in_tc (cond, my_tc);
            result = action_is_in_tc (action, my_tc);
     5. When finished, free the cons cells in id_list and var_list (but
        don't call symbol_remove_ref() on the symbols in them).

  Warning:  actions must not contain reteloc's or rhs unbound variables here.
==================================================================== */

void add_symbol_to_tc(Symbol * sym, tc_number tc, list ** id_list, list ** var_list)
{
    if (sym->common.symbol_type == VARIABLE_SYMBOL_TYPE) {
        mark_variable_if_unmarked(sym, tc, var_list);
    } else if (sym->common.symbol_type == IDENTIFIER_SYMBOL_TYPE) {
        mark_identifier_if_unmarked(sym, tc, id_list);
    }
}

void add_test_to_tc(test t, tc_number tc, list ** id_list, list ** var_list)
{
    cons *c;
    complex_test *ct;

    if (test_is_blank_test(t))
        return;

    if (test_is_blank_or_equality_test(t)) {
        add_symbol_to_tc(referent_of_equality_test(t), tc, id_list, var_list);
        return;
    }

    ct = complex_test_from_test(t);
    if (ct->type == CONJUNCTIVE_TEST) {
        for (c = ct->data.conjunct_list; c != NIL; c = c->rest)
            add_test_to_tc(c->first, tc, id_list, var_list);
    }
}

void add_cond_to_tc(condition * c, tc_number tc, list ** id_list, list ** var_list)
{
    if (c->type == POSITIVE_CONDITION) {
        add_test_to_tc(c->data.tests.id_test, tc, id_list, var_list);
        add_test_to_tc(c->data.tests.value_test, tc, id_list, var_list);
    }
}

void add_action_to_tc(action * a, tc_number tc, list ** id_list, list ** var_list)
{
    if (a->type != MAKE_ACTION)
        return;
    add_symbol_to_tc(rhs_value_to_symbol(a->id), tc, id_list, var_list);
    if (rhs_value_is_symbol(a->value))
        add_symbol_to_tc(rhs_value_to_symbol(a->value), tc, id_list, var_list);
    if (preference_is_binary(a->preference_type))
        if (rhs_value_is_symbol(a->referent))
            add_symbol_to_tc(rhs_value_to_symbol(a->referent), tc, id_list, var_list);
}

bool symbol_is_in_tc(Symbol * sym, tc_number tc)
{
    if (sym->common.symbol_type == VARIABLE_SYMBOL_TYPE)
        return (bool) (sym->var.tc_num == tc);
    if (sym->common.symbol_type == IDENTIFIER_SYMBOL_TYPE)
        return (bool) (sym->id.tc_num == tc);
    return FALSE;
}

bool test_is_in_tc(test t, tc_number tc)
{
    cons *c;
    complex_test *ct;

    if (test_is_blank_test(t))
        return FALSE;
    if (test_is_blank_or_equality_test(t)) {
        return symbol_is_in_tc(referent_of_equality_test(t), tc);
    }

    ct = complex_test_from_test(t);
    if (ct->type == CONJUNCTIVE_TEST) {
        for (c = ct->data.conjunct_list; c != NIL; c = c->rest)
            if (test_is_in_tc(c->first, tc))
                return TRUE;
        return FALSE;
    }
    return FALSE;
}

bool cond_is_in_tc(condition * cond, tc_number tc)
{
    condition *c;
    bool anything_changed;
    bool result;
    list *new_ids, *new_vars;

    if (cond->type != CONJUNCTIVE_NEGATION_CONDITION)
        return test_is_in_tc(cond->data.tests.id_test, tc);

    /* --- conjunctive negations:  keep trying to add stuff to the TC --- */
    new_ids = NIL;
    new_vars = NIL;
    for (c = cond->data.ncc.top; c != NIL; c = c->next)
        c->already_in_tc = FALSE;
    for (;;) {
        anything_changed = FALSE;
        for (c = cond->data.ncc.top; c != NIL; c = c->next)
            if (!c->already_in_tc)
                if (cond_is_in_tc(c, tc)) {
                    add_cond_to_tc(c, tc, &new_ids, &new_vars);
                    c->already_in_tc = TRUE;
                    anything_changed = TRUE;
                }
        if (!anything_changed)
            break;
    }

    /* --- complete TC found, look for anything that didn't get hit --- */
    result = TRUE;
    for (c = cond->data.ncc.top; c != NIL; c = c->next)
        if (!c->already_in_tc)
            result = FALSE;

    /* --- unmark identifiers and variables that we just marked --- */
    unmark_identifiers_and_free_list(new_ids);
    unmark_variables_and_free_list(new_vars);

    return result;
}

bool action_is_in_tc(action * a, tc_number tc)
{
    if (a->type != MAKE_ACTION)
        return FALSE;
    return symbol_is_in_tc(rhs_value_to_symbol(a->id), tc);
}

/* *********************************************************************

                         Variable Generator

   These routines are used for generating new variables.  The variables
   aren't necessarily "completely" new--they might occur in some existing
   production.  But we usually need to make sure the new variables don't
   overlap with those already used in a *certain* production--for instance,
   when variablizing a chunk, we don't want to introduce a new variable that
   conincides with the name of a variable already in an NCC in the chunk.
   
   To use these routines, first call reset_variable_generator(), giving
   it lists of conditions and actions whose variables should not be
   used.  Then call generate_new_variable() any number of times; each
   time, you give it a string to use as the prefix for the new variable's
   name.  The prefix string should not include the opening "<".
********************************************************************* */

void reset_variable_generator(condition * conds_with_vars_to_avoid, action * actions_with_vars_to_avoid)
{
    tc_number tc;
    list *var_list;
    cons *c;
    int i;

    /* --- reset counts, and increment the gensym number --- */
    for (i = 0; i < 26; i++)
        current_agent(gensymed_variable_count)[i] = 1;
    current_agent(current_variable_gensym_number)++;
    if (current_agent(current_variable_gensym_number) == 0) {
        reset_variable_gensym_numbers();
        current_agent(current_variable_gensym_number) = 1;
    }

    /* --- mark all variables in the given conds and actions --- */
    tc = get_new_tc_number();
    var_list = NIL;
    add_all_variables_in_condition_list(conds_with_vars_to_avoid, tc, &var_list);
    add_all_variables_in_action_list(actions_with_vars_to_avoid, tc, &var_list);
    for (c = var_list; c != NIL; c = c->rest)
        ((Symbol *) (c->first))->var.gensym_number = current_agent(current_variable_gensym_number);
    free_list(var_list);
}

#define NAME_BUF_SIZE 200
Symbol *generate_new_variable(char *prefix)
{
    char name[NAME_BUF_SIZE];   /* that ought to be long enough! */
    Symbol *new;
    char first_letter;

    first_letter = *prefix;
    if (isalpha(first_letter)) {
        if (isupper(first_letter))
            first_letter = (char) tolower(first_letter);
    } else {
        first_letter = 'v';
    }

    for (;;) {
        snprintf(name, NAME_BUF_SIZE, "<%s%lu>", prefix, current_agent(gensymed_variable_count)[first_letter - 'a']++);
        name[NAME_BUF_SIZE - 1] = 0;    /* snprintf doesn't set last char to null if output is truncated */
        new = make_variable(name);
        if (new->var.gensym_number != current_agent(current_variable_gensym_number))
            break;
        symbol_remove_ref(new);
    }

    new->var.current_binding_value = NIL;
    new->var.gensym_number = current_agent(current_variable_gensym_number);
    return new;
}

/* *********************************************************************

                         Production Management

    Make_production() does reordering, compile-time o-support calc's,
    and builds and returns a production structure for a new production.
    It does not enter the production into the Rete net, however.
    The "type" argument should be one of USER_PRODUCTION_TYPE, etc.
    The flag "reorder_nccs" tells whether to recursively reorder
    the subconditions of NCC's--this is not necessary for newly
    built chunks, as their NCC's are copies of other NCC's in SP's that
    have already been reordered.  If any error occurs, make_production()
    returns NIL.

    Deallocate_production() and excise_production() do just what they
    say.  Normally deallocate_production() should be invoked only via
    the production_remove_ref() macro.
********************************************************************* */

extern char *name_of_production_being_reordered;

production *make_production(byte type,
                            Symbol * name,
                            condition ** lhs_top, condition ** lhs_bottom, action ** rhs_top, bool reorder_nccs)
{
    production *p;
    tc_number tc;
    action *a;

    name_of_production_being_reordered = name->sc.name;

    if (type != JUSTIFICATION_PRODUCTION_TYPE) {
        reset_variable_generator(*lhs_top, *rhs_top);
        tc = get_new_tc_number();
        add_bound_variables_in_condition_list(*lhs_top, tc, NIL);
        if (!reorder_action_list(rhs_top, tc))
            return NIL;
        if (!reorder_lhs(lhs_top, lhs_bottom, reorder_nccs))
            return NIL;

#ifdef DO_COMPILE_TIME_O_SUPPORT_CALCS
        calculate_compile_time_o_support(*lhs_top, *rhs_top);
#ifdef LIST_COMPILE_TIME_O_SUPPORT_FAILURES
        for (a = *rhs_top; a != NIL; a = a->next)
            if ((a->type == MAKE_ACTION) && (a->support == UNKNOWN_SUPPORT))
                break;
        if (a)
            print_with_symbols("\nCan't classify %y\n", name);
#endif
#else
        for (a = *rhs_top; a != NIL; a = a->next)
            a->support = UNKNOWN_SUPPORT;
#endif
    } else {
        /* --- for justifications --- */
        /* force run-time o-support (it'll only be done once) */

        for (a = *rhs_top; a != NIL; a = a->next)
            a->support = UNKNOWN_SUPPORT;
    }

    allocate_with_pool(&current_agent(production_pool), &p);
    p->name = name;
    if (name->sc.production) {
        print("Internal error: make_production called with name %s\n", name_of_production_being_reordered);
        print("for which a production already exists\n");
    }
    name->sc.production = p;
    p->documentation = NIL;
    p->filename = NIL;
    p->firing_count = 0;
    p->reference_count = 1;
    insert_at_head_of_dll(current_agent(all_productions_of_type)[type], p, next, prev);
    current_agent(num_productions_of_type)[type]++;
    p->type = type;
    p->declared_support = UNDECLARED_SUPPORT;

#ifndef TRACE_CONTEXT_DECISIONS_ONLY
    p->trace_firings = FALSE;
#endif
    p->p_node = NIL;            /* it's not in the Rete yet */
    p->action_list = *rhs_top;
    p->rhs_unbound_variables = NIL;     /* the Rete fills this in */
    p->instantiations = NIL;
    p->interrupt = FALSE;

    return p;
}

void deallocate_production(production * prod)
{
    if (prod->instantiations) {
        char msg[MESSAGE_SIZE];
        strncpy(msg, "Internal error: deallocating prod. that still has inst's\n", MESSAGE_SIZE);
        msg[MESSAGE_SIZE - 1] = 0;
        abort_with_fatal_error(msg);
    }
#ifdef WATCH_PRODUCTIONS
    print_with_symbols("DEALLOCATING PRODUCTION: %y\n", prod->name);
#endif
    deallocate_action_list(prod->action_list);
    /* RBD 3/28/95 the following line used to use free_list(), leaked memory */
    deallocate_symbol_list_removing_references(prod->rhs_unbound_variables);
    symbol_remove_ref(prod->name);
    if (prod->documentation)
        free_memory_block_for_string(prod->documentation);
    /* next line, kjh CUSP(B11) */
    if (prod->filename)
        free_memory_block_for_string(prod->filename);
    free_with_pool(&current_agent(production_pool), prod);
}

void excise_production(production * prod, bool print_sharp_sign)
{

#ifndef TRACE_CONTEXT_DECISIONS_ONLY
    if (prod->trace_firings)
        remove_pwatch(prod);
#endif
#ifdef WATCH_PRODUCTIONS
    print_with_symbols("EXCISING PRODUCTION: %y\n", prod->name);
    print("Productions has reference count %d\n", prod->reference_count);
#endif

    remove_from_dll(current_agent(all_productions_of_type)[prod->type], prod, next, prev);
    current_agent(num_productions_of_type)[prod->type]--;
    if (print_sharp_sign)
        print("#");
    if (prod->p_node)
        excise_production_from_rete(prod);
    prod->name->sc.production = NIL;
    production_remove_ref(prod);
}
