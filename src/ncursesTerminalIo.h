#include <ncurses.h>
#include <string>
#include <vector>
#include <regex>

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

public:
    NcursesTerminalIO()
    {
        this->mainWindow = initscr();
        raw(); // Line buffering disabled
        //noecho(); // Don't echo() while we do getch
        keypad(stdscr, TRUE); // Activate pseudo-character tokens outside ASCII range for some KEY-cases
    }
    void itemPrint(int selectorId, std::string text, bool isSelected = false)
    {
        char selector[4];
        sprintf(selector, "%d: ", selectorId);
        const int line = (selectorId)*2;
        attron(isSelected ? (A_STANDOUT | A_BOLD) : A_NORMAL);
        mvprintw(line, 1, selector);

        std::regex e("%"); // escape special printf char
        mvprintw(line, 4, std::regex_replace(text,e,"%%").c_str());
        attroff(isSelected ? (A_STANDOUT | A_BOLD) : A_NORMAL);
    }
    void vectorPrint(std::vector<std::string> stringVector, std::string highlightedField)
    {
        mvprintw(0, 1, "");
        for (auto stringValue : stringVector)
        {
            const bool isSelected = stringValue == highlightedField;
            attron(isSelected ? (A_STANDOUT | A_BOLD) : A_NORMAL);
            printw((' ' + stringValue + ' ').c_str());
            attroff(isSelected ? (A_STANDOUT | A_BOLD) : A_NORMAL);
        }
    }
    void clearScreen()
    {
        clear();
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
