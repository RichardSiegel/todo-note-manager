#include "userStorage.h"
#include "todoNote.h"
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <set>
#include <pwd.h>
#include <experimental/filesystem>
#include <algorithm>
namespace fs = std::experimental::filesystem;

UserStorage::UserStorage(std::string contextName)
{
    this->contextName = contextName;
    this->tnmDir = constructPathToStorageDir(".tnm");
    this->exists = fs::exists(this->getContextDir()) && fs::exists(this->getContextTrashDir());
    if (!this->exists)
        this->createEmptyStorage();
}

std::string UserStorage::getContextName()
{
    return this->contextName;
}

std::string UserStorage::getDirWithContexts()
{
    return this->tnmDir + "/notes/";
}

std::string UserStorage::getContextDir()
{
    return this->getDirWithContexts() + this->contextName;
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

void UserStorage::moveNoteIntoContext(TodoNote note)
{
    note.moveNote(this->getContextDir());
}

std::vector<std::string> UserStorage::getAvailableContexts()
{
    std::set<std::string> sortedContexts;
    sortedContexts.insert(".");
    for (fs::path path : fs::directory_iterator(this->getDirWithContexts()))
    {
        if (fs::is_directory(path))
            sortedContexts.insert(path.filename());
    }
    std::vector<std::string> contexts(sortedContexts.begin(), sortedContexts.end());
    return contexts;
}

template<typename T>
int findVectorIndex(std::vector<T> v, T item) {
    auto itr = std::find(v.begin(), v.end(), item);
    if (itr == v.cend())
        throw "Element not found in Vector";
    return std::distance(v.begin(), itr);
}

int UserStorage::getIndexWithinAvailableContexts()
{
    return findVectorIndex<std::string>(this->getAvailableContexts(), this->getContextName()); 
}

void UserStorage::trashContext()
{
    for (auto note : this->getNotes())
    {
        this->trashNote(note);
    }
    fs::remove(this->getContextDir());
    this->exists = false;
}
