#include <iostream>
#include <string>
#include <vector>
#include "userStorage.h"
#include "todoNote.h"
#include "ncursesTerminalIo.h"

/* This file ties together the data-storage and manipulation implementation classes and Terminal input/output.
 * If at any point other user interfaces should be implemented this is the only code that must be altered.
 * 
 * Author: Richard Siegel
 */

int main(int argc, char *argv[])
{
    UserStorage storage;

    // TODO implement context change
    // storage = storage.context("development-notes");

    // TODO output info:
    // std::cout << "Welcome to TodoNoteManager!" << std::endl << std::endl;
    // std::cout << "a hidden directory \"~/.tnm\" has been created to store your notes." << std::endl;

    NcursesTerminalIO cursesTerm;
    int notesToSkip = 0;     // number of itemes, which is 'scrolled' out of view at the top.
    char selectedIndex = -1; // the number used to select an item (-1 means no selection)
    int noteIndex = -1;      // index of the selected note
    char keyInput = -1;      // the last key entered
    bool runningMainLoop = true;
    TodoNote selectedNote;
    while (runningMainLoop)
    {
        std::vector<TodoNote> notes = storage.getNotes();
        noteIndex = selectedIndex + notesToSkip - 1;
        selectedNote = (noteIndex > -1 && noteIndex < notes.size())? notes[noteIndex] : TodoNote();
        cursesTerm.displayNotes(notes, notesToSkip, selectedIndex);
        keyInput = cursesTerm.awaitKeyPress();
        switch (keyInput)
        {
        case 'q': // close TodoNoteManager
            runningMainLoop = false;
            break;
        case 'x': // delete selected note
            storage.trashNote(selectedNote);
            break;
        case 'i': // input new note // TODO select and insure display of new item
            selectedIndex = 0;
            storage.createNote(cursesTerm.awaitInputEnter());
            break;
        case 'a': // add to title of existing note
            selectedNote.setTitle(cursesTerm.awaitInputEnter(selectedNote.getTitle()));
            break;
        case '0': // show more notes
            if (storage.countNotes() == ++notesToSkip)
                notesToSkip = 0;
            selectedIndex--;
            break;
        default:
            if (keyInput >= '1' && keyInput <= '9') // select item if key was a number
                selectedIndex = keyInput - '0';
            // else TODO implement default case
            break;
        }
    }

    return 0;
}