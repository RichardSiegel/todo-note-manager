#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "userStorage.h"
#include "todoNote.h"
#include "ncursesTerminalIo.h"

/* This file ties together the data-storage and manipulation implementation classes and Terminal input/output.
 * If at any point other user interfaces should be implemented this is the only code that must be altered.
 * 
 * Author: Richard Siegel
 */

UserStorage storage;
NcursesTerminalIO cursesTerm;

void displayInTerminal(std::vector<TodoNote> notes, int skip, int selectedItem)
{
    cursesTerm.clearScreen();
    cursesTerm.vectorPrint(storage.getAvailableContexts(), storage.getContextName());
    const int lastAvailable = notes.size() - skip;
    const int lastVisible = 9;
    for (int i = 0; i < lastAvailable && i < lastVisible; i++)
    {
        bool isSelected = i + 1 == selectedItem;
        cursesTerm.itemPrint(i + 1, notes[i + skip].getTitle(), isSelected);
    }
}

int main(int argc, char *argv[])
{
    // TODO output info:
    // std::cout << "Welcome to TodoNoteManager!" << std::endl << std::endl;
    // std::cout << "a hidden directory \"~/.tnm\" has been created to store your notes." << std::endl;

    int contextIndex = 0;
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
        selectedNote = (noteIndex > -1 && noteIndex < notes.size()) ? notes[noteIndex] : TodoNote();
        displayInTerminal(notes, notesToSkip, selectedIndex);
        keyInput = cursesTerm.awaitKeyPress();
        switch (keyInput)
        {
        case 'q': // close TodoNoteManager
            runningMainLoop = false;
            break;
        case 'e': // edit selected note
            system(("vim '" + selectedNote.getFilePath() + "'").c_str());
            break;
        case 'r': // run selected notes title in terminal
            system("reset"); // end special new line formating of NcursesTerminalIO
            system("clear");
            system(selectedNote.getTitle().c_str());
            cursesTerm.awaitKeyPress();
            cursesTerm = NcursesTerminalIO(); // restoring special new line formating of NcursesTerminalIO
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
        case ' ': // view next context
            contextIndex = ++contextIndex % storage.getAvailableContexts().size();
            storage = UserStorage(storage.getAvailableContexts()[contextIndex]);
            selectedIndex = -1;
            break;
        case 'c': // create context // TODO consider removing this in favor of 'm'
            storage = UserStorage(cursesTerm.awaitInputEnter());
            contextIndex = storage.getIndexWithinAvailableContexts();
            break;
        case 'm': // move note into other context OR create context
            storage = UserStorage(cursesTerm.awaitInputEnter());
            storage.moveNoteIntoContext(selectedNote);
            selectedIndex = -1;
            contextIndex = storage.getIndexWithinAvailableContexts();
            break;
        case 'd': // trash all notes in context and delete context
            storage.trashContext();
            storage = UserStorage();
            selectedIndex = -1;
            contextIndex = storage.getIndexWithinAvailableContexts();
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