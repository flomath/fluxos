---
layout: page
title: "Projectmanagement"
category: doc
date: 2015-11-27 10:39:52
---

For this project we are using a trimmed version of SCRUM. As toolset we are relying on the github internal capabilities like issues and milestones.

### Scrum Task Board
All of our tasks (represented as issues) are visible in our scrum task board consisting of the columns Backlog, Ready, In Progress and Done. The Backlog column makes all items of the sprint backlog visible. The next column contains all items which are ready to be worked on, followed by the In Progress column. This column shows other team members or stakeholders who is working on which task. The last column gives an overview of already done and finished tasks. 

**Waffle.io** provides the possibility to show the github issues in the previously described scrum task board and adds scrum points and assigned members view.

The board is accessible via [https://waffle.io/flomath/fhv-os](https://waffle.io/flomath/fhv-os).

### Milestones

#### Sprint 1 - Ideas, Brainstorming and Kickoff
The first sprint is to define the basic concept of our operating system and the possible application area. It also contains the very basic architecture thoughts which will be refined in the following sprints.

#### Sprint 2 - UART and Scheduler
In the second sprint the UART is implemented as generic and omap specific code. It is possible to send or receive data via UART to the RS232 interface. A generic driver allows the usage of UART by any process. A very basic scheduler foundation is available to enqueue processes or tasks and provide a interface to enable interrupt-based scheduling on the basis of a simple scheduling algorithm. 

#### Sprint 3 - Timer, Interrupt, MMU
It is possible to trigger an interrupt via a timer by adding the timer to the generic interrupt handler. A simple task can be scheduled, for example every 10 seconds using the scheduler, interrupt and timer. The first steps towards an MMU is done by writing a guide and documentation on how to use and implement it. 