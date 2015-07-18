---
layout: page
title: "Projectmanagement"
category: doc
date: 2015-11-27 10:39:52
---

## Project management

For this project we are using a trimmed version of SCRUM. As toolset we are relying on the github internal capabilities like issues and milestones.

### Scrum Task Board
All of our tasks (represented as issues) are visible in our scrum task board consisting of the columns Backlog, Ready, In Progress and Done. The Backlog column makes all items of the sprint backlog visible. The next column contains all items which are ready to be worked on, followed by the In Progress column. This column shows other team members or stakeholders who is working on which task. The last column gives an overview of already done and finished tasks.

**Waffle.io** provides the possibility to show the github issues in the previously described scrum task board and adds scrum points and assigned members view.

The board is accessible via [https://waffle.io/flomath/fhv-os](https://waffle.io/flomath/fhv-os).

### Milestones
In order to get started we set ourself deadlines and tasks which needed to be done. After the first weeks we managed to work on seperate tasks independently and could ditch the detailed sprint planning.

#### Milestone 1 - Ideas, Brainstorming and Kickoff
The first sprint is to define the basic concept of our operating system and the possible application area. It also contains the very basic architecture thoughts which will be refined in the following sprints.

#### Milestone 2 - UART and Scheduler
In the second sprint the UART is implemented as generic and omap specific code. It is possible to send or receive data via UART to the RS232 interface. A generic driver allows the usage of UART by any process. A very basic scheduler foundation is available to enqueue processes or tasks and provide a interface to enable interrupt-based scheduling on the basis of a simple scheduling algorithm.

#### Milestone 3 - Timer, Interrupt, MMU
It is possible to trigger an interrupt via a timer by adding the timer to the generic interrupt handler. A simple task can be scheduled, for example every 10 seconds using the scheduler, interrupt and timer. The first steps towards an MMU is done by writing a guide and documentation on how to use and implement it.

#### Milestone 4 - IPC and Software Interrupts
It is possible for processes to communicate with each other using some IPC mechanism provided by the operating system. Also processes, which are running in the user mode, gain access to methods of the operating system via software interrupts.

#### Milestone 5 - Audio, Loader, SD Card
Processes can play audio signals. Also the SD card is accessible and a minimalistic file system, that can at least read data, is implemented. It must be possible to start processes that are not compiled into the operating system using a binary data format. Instead they are loaded from the SD card.

### Github
Github not only offers us the possibility to create issues and use them as scrum tasks but, of course version control. The public repository can be accessed here [https://github.com/flomath/fluxos/](https://github.com/flomath/fluxos/).

Every important module resp. bigger task was created using its own branch. This allowed us to independently work on different topics without interfering other team members with unfinished code or different configuration settings. In the end we created 12 different branches which of two are for documentation. All of the others are merged into the master branch using pull requests. 