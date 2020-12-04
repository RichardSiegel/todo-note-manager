#include <ncurses.h>
#include <string>
#include <vector>
#include "todoNote.h"

/* This class handles input and output via the Terminal.
 * it uses ncurses, which is well documented:
 * see: https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/init.html
 * 
 * Author: Richard Siegel
 */

class NcursesTerminalIO
{
private:
    WINDOW *mainWindow;
    void itemPrint(int selectorId, std::string text)
    {
        char selector[4];
        sprintf(selector, "%d: ", selectorId);
        const int line = (selectorId - 1) * 2;
        mvprintw(line, 1, selector);
        mvprintw(line, 4, text.c_str());
    }

public:
    NcursesTerminalIO()
    {
        this->mainWindow = initscr();
        raw(); // Line buffering disabled
        //noecho(); // Don't echo() while we do getch
    }
    void displayNotes(std::vector<TodoNote> notes, int skip, int selectedItem)
    {
        clear();
        const int lastAvailable = notes.size() - skip;
        const int lastVisible = 9;
        for (int i = 0; i < lastAvailable && i < lastVisible; i++)
        {
            if (i + 1 == selectedItem)
                attron(A_STANDOUT | A_BOLD);
            itemPrint(i + 1, notes[i + skip].getTitle());
            if (i + 1 == selectedItem)
                attroff(A_STANDOUT | A_BOLD);
        }
    }
    char awaitKeyPress()
    {
        const int lastLine = getmaxy(this->mainWindow) - 1;
        mvprintw(lastLine, 1, ":");
        refresh(); // Print it on to the real screen
        return getch();
    }
    std::string awaitInputEnter(std::string inputText = "")
    {
        int index = inputText.length();
        keypad(stdscr, TRUE); // Activate pseudo-character tokens outside ASCII range for some KEY-cases
        const int lastLine = getmaxy(this->mainWindow) - 1;
        const int maxLength = getmaxx(this->mainWindow);
        int ch = 0; // null char can be ignored
        while (ch != '\n')
        {
            switch (ch)
            {
            case 0:
                break;
            case KEY_BACKSPACE:
                if (index)
                    inputText.erase(--index, 1);
                break;
            case KEY_LEFT:
                if (index)
                    --index;
                break;
            case KEY_RIGHT:
                if (index < inputText.length())
                    ++index;
                break;
            case KEY_UP:
                index = 0;
                break;
            case KEY_DOWN:
                index = inputText.length();
                break;
            default:
                inputText.insert(index++, std::string(1, ch));
                break;
            }
            mvprintw(lastLine, 1, std::string(maxLength, '_').c_str());
            mvprintw(lastLine, 1, ("> " + inputText.substr(inputText.length() > maxLength - 4 ? -1 * (maxLength - inputText.length() - 4) : 0)).c_str());
            mvprintw(lastLine, index + 3, ""); // position curser
            refresh();                         // Print it on to the real screen
            ch = getch();
        }
        raw(); // stop waiting for enter
        return inputText;
    }
    ~NcursesTerminalIO()
    {
        endwin();
    }
};
