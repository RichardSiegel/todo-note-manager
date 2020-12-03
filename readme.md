# TNM - todo-note-manager (in Development)
is a small CLI program, designed to manage todos and notes very quickly. It was developed for the Linux-Terminal and is likely to run in other Unix Terminals too.
the interface and input commands have been inspired by VIM

## Features and CLI
* press `[q]` to exit
* Create new notes only by pressing `[i]`, typing and pressing `[Enter]`
* Select from up to 9 notes with the number keys (list more notes with 0)
* Edit selected note by pressing `[a]`, typing and pressing `[Enter]`
* Delete selected note by pressing `[x]`

## Planned Features
* different custom contexts for notes
* undo delete
* open note in editor (first line of document is the title listed in tnm)
* and more ...

## Setup Info
This is a C++ Project created in visual studio code using the g++ compiler. To compile this code without the IDE run:
`g++ src/*.cpp -std=c++17 -lstdc++fs -lncurses`
