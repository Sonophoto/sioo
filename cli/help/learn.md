## NAME

learn - Control the learning of productions 

## SYNOPSIS

**learn** [_-on_ | _-off_ | _-except_ | _-only_ | _-list_ | _-all-levels_ | _-bottom-up_] 

* * *

## DESCRIPTION

This command is used to control the learning (or "chunking") of productions. With no arguments, this command prints out the current learning environment status. The **watch** command can be used to provide various levels of detail when productions are learned. Learning is on by default.

## OPTIONS

Any _option_ s given must match one of the following keywords: 

-on
    turn learning on

-off
    turn all learning off

-except
    learning is on, except as specified by RHS **dont-learn** actions

-only
    learning is off, except as specified by RHS **force-learn** actions

-list
    lists the force-learn and dont-learn states registered by the RHS actions

-all-levels
    when learning is on, this allows learning whenever a subgoal returns a result (in contrast to bottom-up learning)

-bottom-up
    when learning is on, this allows learning at only the lowest level; i.e., build chunks only for subgoals that have not yet had any subgoals with chunks built.

## SEE ALSO

**watch** , **explain-backtraces** , **save_backtraces**
