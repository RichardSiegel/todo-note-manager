#pragma once
#include <string>
#include <fstream>
#include <ctime>

/* Instances of this class represent a single note or todo.
 * Only empty notes don't have a file representation.
 * Files, which represent the notes are created/updated anytime the note changes.
 * If a note file already exists when an instance is created, its contents will be loaded.
 * 
 * Author: Richard Siegel
 */

class TodoNote
{
private:
    bool exists;
    std::string path;
    std::string dirPath;
    std::string fileName;
    std::string title;
    std::string content;
    void saveToFile()
    {
        std::ofstream f(this->path);
        f << this->content;
        f.close();
    }

public:
    static bool sortByFileName(TodoNote a, TodoNote b) { return a.fileName < b.fileName; };

    TodoNote()
    {
        this->exists = false;
    }

    TodoNote(std::string dirPath, std::string fileName)
    {
        this->exists = true;
        this->dirPath = dirPath;
        this->fileName = fileName;
        this->path = dirPath + "/" + this->fileName;

        std::ifstream f(this->path);
        if (f.is_open())
        {
            std::string line;
            while (getline(f, line))
            {
                if (this->title == "")
                    this->title = line;
                this->content += line + "\n";
            }
            f.close();
        }
        else
        {
            this->title = "[unsaved empty note]";
            this->content = "";
        }
    };
    void moveNote(std::string dirPath)
    {
        if (this->exists)
        {
            std::rename(this->path.c_str(), (dirPath + "/" + this->fileName).c_str());
            this->path = this->dirPath;
            this->title = "[moved] " + this->title;
        }
    }
    void setTitle(std::string value)
    {
        if (this->exists)
        {
            if (this->title != "[unsaved empty note]")
                this->content = value + "\nOld Title: " + this->content;
            else
                this->content = value;
            this->title = value;
            this->saveToFile();
        }
    }
    std::string getTitle()
    {
        return this->exists ? this->title : "[undefined note]";
    }
};