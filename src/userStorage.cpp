#include "userStorage.h"
#include "todoNote.h"
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <set>
#include <pwd.h>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

UserStorage::UserStorage()
{
    this->tnmDir = constructPathToStorageDir(".tnm");
    this->contextName = ".";
    this->exists = fs::exists(this->tnmDir);
}

// TODO handle context of context situation
UserStorage UserStorage::context(std::string contextName)
{
    UserStorage subContextStorage = *this;
    subContextStorage.contextName = contextName;
    if (!subContextStorage.exists)
        subContextStorage.createEmptyStorage();
    return subContextStorage;
}

std::string UserStorage::getContextDir()
{
    return this->tnmDir + "/notes/" + this->contextName;
}

std::string UserStorage::getContextTrashDir()
{
    return this->tnmDir + "/trash/" + this->contextName;
}

std::string UserStorage::constructPathToStorageDir(std::string dirName)
{
    char *getenvHome = getenv("HOME");
    std::string homeDir = getenvHome != NULL ? getenvHome : getpwuid(getuid())->pw_dir;
    return homeDir + "/" + dirName;
}

void UserStorage::createEmptyStorage()
{
    fs::create_directories(this->getContextDir());
    fs::create_directories(this->getContextTrashDir());
}

std::vector<TodoNote> UserStorage::getNotes()
{
    std::set<TodoNote, decltype(TodoNote::sortByFileName) *> sortedNotes(TodoNote::sortByFileName);
    for (fs::path path : fs::directory_iterator(this->getContextDir()))
    {
        if (!fs::is_directory(path))
            sortedNotes.insert(TodoNote(this->getContextDir(), path.filename()));
    }
    std::vector<TodoNote> notes(sortedNotes.begin(), sortedNotes.end());
    return notes;
}

int UserStorage::countNotes()
{
    return this->getNotes().size();
}

TodoNote UserStorage::createNote(std::string title)
{
    TodoNote note = TodoNote(this->getContextDir(), this->currentSortableDateTime() + "_" + title);
    note.setTitle(title);
    return note;
}

std::string UserStorage::currentSortableDateTime()
{
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H%M%S%z", timeinfo);
    std::string str(buffer);

    return str;
}

void UserStorage::trashNote(TodoNote note)
{
    note.moveNote(this->getContextTrashDir());
}