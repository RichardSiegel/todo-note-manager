# TNM - todo-note-manager (in Development)
is a small CLI program, designed to manage todos and notes very quickly. It was developed for the Linux-Terminal and is likely to run in other Unix Terminals too.
the interface and input commands have been inspired by VIM

## Features and CLI
* press `[q]` to exit
* Create new notes only by pressing `[i]`, typing and pressing `[Enter]`
* Select from up to 9 notes with the number keys (list more notes with 0)
* Edit selected note by pressing `[a]`, typing and pressing `[Enter]`
* Delete selected note by pressing `[x]`
* Delete context group by pressing `[d]`
* Create context group by pressing `[c]` or `[m]` when no note is selected
* Move a note from one context group to another by pressing `[m]` and entering the new context group name
* press `[space]` to toggle context group names
* press `[e]` to edit selected note in vim (first line of document is the title listed in tnm)
* press `[r]` run selected notes title in terminal

## Planned Features
* undo delete (nots are currently not deleted but trashed, however the process can only be undone within the file system and not the program)
* scheduling of todos and notes to appear or be trashed in the future
* and more ...

## Setup Info
This is a C++ Project created in visual studio code using the g++ compiler. To compile this code without the IDE run:
`g++ src/*.cpp -std=c++17 -lstdc++fs -lncurses`
