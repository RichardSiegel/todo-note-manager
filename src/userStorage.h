#pragma once
#include <string>
#include <vector>
#include "todoNote.h"

/* This class manages directories and files in the users home directory.
 * To the outside this class presents only lists of notes (within different contexts).
 * The directory structure includes:
 *  - a notes directory where notes are stored
 *      - Context sub-directories which are used to group the notes contained
 *  - a trash directory where delete notes go
 *      - Context sub-directories to preserve the context source of deleted notes
 * 
 * Author: Richard Siegel
 */

class UserStorage // TODO through Errors where needed
{
private:
    std::string tnmDir;
    std::string constructPathToStorageDir(std::string dirName);
    std::string contextName;
    std::string getDirWithContexts();
    std::string getContextDir();
    std::string getContextTrashDir();
    void createEmptyStorage(); 
    std::string currentSortableDateTime();

public:
    bool exists;
    UserStorage(std::string contextName = ".");
    std::string getContextName();
    std::vector<TodoNote> getNotes();
    int countNotes();
    TodoNote createNote(std::string title);
    void trashNote(TodoNote note);
    void moveNoteIntoContext(TodoNote note);
    std::vector<std::string> getAvailableContexts();
    int getIndexWithinAvailableContexts();
    void trashContext(); // current context is trashed and removed if not main/the fallback
};
