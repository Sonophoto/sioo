## NAME

monitor - Manage attachment of scripts to Soar events 

## SYNOPSIS

**monitor** _action_

* * *

## DESCRIPTION

This command manages the attachment of scripts to Soar events. Soar events are important events that occur in Soar as an agent is being processed. Scripts can be attached to these Soar events so that they are invoked every time the Soar event occurs. These scripts can also be removed using the **monitor** command. The status of attachments can be listed and tested. If a new attachment is created, its identifier is returned as the result of this command. The identifier is used when deleting a script from an event.

The specified _action_ must have one of the following forms:

**-add** _soar-event script [id]_
    Add the _script_ to the list of scripts to be invoked when the _soar-event_ occurs in the Soar kernel. See below for a list of valid Soar event names. If the _id_ is given, it is used to name the attachment. If no _id_ is given, then a unique id is generated and returned.

**-delete** _soar-event [id]_
    Remove scripts attached to _soar-event_. The _soar-event_ must match a valid Soar event name (see below). If _id_ is given after a _soar-event_ , then only that particular attachment is removed. Otherwise, all attachments under _soar-event_ are removed.

-list
    List all attachments of scripts to Soar events

-test
    Test the script attachment process by attaching a print message to every Soar event.

-clear
    Remove _all_ attached scripts.

## EXAMPLES

This command creates an attachment which prints a message after every Soar decision cycle: 
    
    
    **monitor -add after-decision-cycle {puts "Finished DC!"}**
    

This example creates an attachment which calls the Tcl procedure "DoSomething" (presumably user-defined) after every production firing:
    
    
    **monitor -add firing DoSomething**
    

This example creates an attachment which processes pending X events after every decision cycle. This is needed in long-running Soar agents so that GUIs do not appear frozen:
    
    
    **monitor -add after-decision-cycle update**
    

This example adds an attachment which ticks a simulator after each agent has completed a cycle in the Soar scheduler. Note that this example command is executed in the global agent's interpreter so that the simulator is ticked only once per schedule cycle. The "TickSimulator" command would be defined by the user.
    
    
    **monitor -add after-schedule-cycle TickSimulator**
    

This example adds an attachment that could be used to inform the Soar development tool SDE that an agent has been selected for scheduling. SDE expects certain strings in Soar output and uses them to control some SDE operations.
    
    
    **monitor -add before-schedule-cycle {puts "\nSelecting agent $soar_agent_name"}**
    

This example removes the single attachment "m1" which associates a script with the Soar event after-init-agent:
    
    
    **monitor -delete after-init-agent m1**
    

This example removes all script attachments to the Soar event before-preference-phase-cycle:
    
    
    **monitor -delete before-preference-phase-cycle**
    

This example removes all script attachments that have been added with the monitor command:
    
    
    **monitor -clear**
    

## SOAR EVENTS

This command utilizes the Soar callback system which can associate TCL or C functions with Soar events. The permissable Soar event names are: 

system-termination
    The Soar system is exiting.

after-init-agent
    An agent has just been created and initialized.

before-init-soar
    The agent is about to be initialized. The **init-soar** command will generate this event.

after-init-soar
    The agent has just been initialized. The **init-soar** command will generate this event.

after-halt-soar
    The agent has just been halted.

before-schedule-cycle
    This event is triggered by the **schedule** command. In the global control agent, this event is triggered before each schedule cycle starts. In other agents, this event is triggered during the schedule cycle, just before the agent is run.

after-schedule-cycle
    This event is triggered by the **schedule** command. In the global control agent, this event is triggered just after each schedule cycle ends. In other agents, this event is triggered during the schedule cycle, just after the agent is run.

before-decision-cycle
    A decision cycle is just about to start.

after-decision-cycle
    A decision cycle has just ended.

before-input-phase
    An input phase is just about to start.

after-input-phase
    An input phase has just ended.

before-preference-phase-cycle
    A preference phase is just about to begin.

after-preference-phase-cycle
    A preference phase has just ended.

before-wm-phase-cycle
    A working memory phase is just about to begin.

after-wm-phase-cycle
    A working memory phase is just about to begin.

before-output-phase
    An output phase is just about to begin.

after-output-phase
    An output phase has just ended.

before-decision-phase-cycle
    A decision phase is about to begin.

after-decision-phase-cycle
    A decision phase has just ended.

wm-changes
    Changes to working memory have just completed.

create-new-context
    A new state has been created on the goal stack.

pop-context-stack
    A subgoal has finished.

create-new-attribute-impasse
    A new attribute impasse has occurred.

remove-attribute-impasse
    An attribute impasse has been removed.

production-just-added
    A productions has just been added to the agent.

production-just-about-to-be-excised
    A production is just about to be removed from the agent.

firing
    A production instantiation has just fired.

retraction
    A production instantiation is just about to retract.

system-parameter-changed
    A system parameter has just been changed.
