## NAME

sp - Define a Soar production 

## SYNOPSIS

**sp _rule_**

* * *

## DESCRIPTION

This command defines a new Soar production. _rule_ is a single argument parsed by the Soar kernel, so it should be enclosed in curly braces to avoid being parsed by Tcl. The overall syntax of a _rule_ is as follows:
    
    
       _name_ 
           [**"**_documentation-string_**"**] 
           [_FLAG_ *]
           _LHS_
           **-- >**
           _RHS_
    

The first element of a _rule_ is its _name_. Conventions for _names_ are given in the Soar Users Manual. If given, the _documentation-string_ must be enclosed in double quotes. Optional flags define the type of rule and the form of support its right-hand side assertions will receive. The specific flags are listed in a separate section below. The _LHS_ defines the left-hand side of the production and specifies the conditions under which the rule can be fired. Its syntax is given in detail in a subsequent section. The **\-- >** symbol serves to separate the _LHS_ and _RHS_ portions. The _RHS_ defines the right-hand side of the production and specifies the assertions to be made and the actions to be performed when the rule fires. The syntax of the allowable right-hand side actions are given in a later section. The Soar Users Manual gives an elaborate discussion of the design and coding of productions. Please see that reference for tutorial information about productions.

More complex productions can be formed by surrounding the _rule_ with double quotes instead of curly braces. This enables variable and command result substitutions in productions. If another production with the same name already exists, it is excised, and the new production is loaded.

## RULE FLAGS

The optional _FLAG_ s are given below. Note that these switches are preceeded by a colon instead of a dash -- this is a Soar parser convention.

:o-support
    specifies that all the RHS actions are to be given o-support when the production fires

:no-support
    specifies that all the RHS actions are only to be given i-support when the production fires

:default
    specifies that this production is a default production (this matters for **excise -task** and **watch task**)

:chunk
    specifies that this production is a chunk (this matters for **learn trace**)

## LHS GRAMMAR

Grammar for left hand sides of productions: 
    
    
       _lhs_ ::= _cond_ +
       _cond_ ::= _positive_cond_  | - _positive_cond_
       _positive_cond_ ::= _conds_for_one_id_  | { _cond_ + }
       _conds_for_one_id_ ::= ( [state |impasse] [_id_test_] _attr_value_tests_ * )
       _id_test_ ::= _test_
       _attr_value_tests_ ::= [-] ^ _attr_test_ [._attr_test_]* _value_test_ *
       _attr_test_ ::= _test_
       _value_test_ ::= _test_ [+]  | _conds_for_one_id_ [+]
    
       _test_ ::= _conjunctive_test_  | _simple_test_
       _conjunctive_test_ ::= { _simple_test_ + }
       _simple_test_ ::= _disjunction_test_  | _relational_test_
       _disjunction_test_ ::= << _constant_ * >>
       _relational_test_ ::= [_relation_] _single_test_
       _relation_ ::= <>  | <  | >  | <=  | >=  | =  | <=>
       _single_test_ ::= variable  | _constant_
       _constant_ ::= sym_constant  | int_constant  | float_constant
    

## RHS GRAMMAR

Grammar for right hand sides of productions: 
    
    
       _rhs_ ::= _rhs_action_ *
       _rhs_action_ ::= ( variable _attr_value_make_ + )  | _function_call_
       _function_call_ ::= ( _function_name_ _rhs_value_ * )
       _function_name_ ::= sym_constant  | +  | -
       _rhs_value_ ::= _constant_  | _function_call_  | variable
       _constant_ ::= sym_constant  | int_constant  | float_constant
       _attr_value_make_ ::= ^ _rhs_value_ [._rhs_value_]* _value_make_ +
       _value_make_ ::= _rhs_value_ _preferences_
    
       _preferences_ ::= [,] | _preference-specifier_ +
       _preference-specifier_ ::= _naturally-unary-preference_ [,]
                                 | _forced-unary-preference_
                                 | _binary-preference_ _rhs_value_ [,]
       _naturally-unary-preference_ ::= +  | -  | !  | ~  | @
       _binary-preference_ ::= >  | =  | <  | &
       _forced-unary-preference_ ::= _binary-preference_ {,  | )  | ^}
    

## EXAMPLES

There are many examples in the Soar Users Manual and the demos subdirectory. Here is a simple production to create a problem space. It comes from the critter-world demo (see the file critter.tcl): 
    
    
    **sp {critter*create*space*critter
       "Formulate the initial problem space"
      (state <s> ^superstate nil)
      -->
      (<s> ^name move-around ^problem-space <p>)
      (<p> ^name critter)}
    **
    

The production above has the _name_ **critter*create*space*critter**. It has a documentation string that is surrounded by double quotes. The _LHS_ is **(state <s> ^superstate nil)** and indicates that this rule will match whenever there is a state object that has the attribute-value pair **^superstate nil**. The **\-- >** arrow separates the left and right-hand sides. The _RHS_ consists of two lines. The first asserts that the state object is to be augmented with the name **move-around** and a problem space should be created. The second line of the _RHS_ indicates that this problem space should be named **critter**.

New for Soar 8, is right-hand-side dot notation. So this production could also be written:
    
    
    **sp {critter*create*space*critter
       "Formulate the initial problem space"
      (state <s> ^superstate nil)
      -->
      (<s> ^name move-around ^problem-space.name critter)}
    **
    

Here is a variant of the above example using double quotes instead of curly braces. Double quotes are needed in order to imbed the value of the Tcl variable **soar_agent_name** in the production. The value of this variable is used to name the problem-space created.
    
    
    **sp "critter*create*space*critter
       (state <s> ^superstate nil)
      -->
      (<s> ^name move-around ^problem-space <p>)
      (<p> ^name $soar_agent_name)"
    **
    

The primary change in the rule is the last clause of the _RHS_. In that clause, the Tcl variable **soar_agent_name** is expanded. If this rule is given in an interpreter which has the variable **soar_agent_name** set to **fred** , then the _RHS_ would expand to the following before being sent to the Soar kernel to be parsed:
    
    
    **( <p> ^name fred)
    **
    

Please be aware that when using double quotes, both the dollar sign (variable expansion) and square brackets (command result substitution) will be interpreted by Tcl. If these characters (**$** , **[** , and **]**) are to be passed to the Soar production parser, they must be escaped (using a backslash) to avoid interpretation by Tcl.

## WARNINGS

The last production above does not contain a documentation string. If one were added, double quotes would need to be escaped to avoid premature termination of the rule:
    
    
    **sp "critter*create*space*critter
       \"Formulate the initial problem space\"
       (state ...**
    

## SEE ALSO

**excise** , **learn** , **watch**
