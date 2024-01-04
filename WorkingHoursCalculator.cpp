#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <cstring> // For strcpy
#include <windows.h> // For Windows API functions

using namespace std;

// Function to check if a given year is a leap year
bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// Function to calculate the number of days in a given month and year
int daysInMonth(int month, int year) {
    if (month == 1 || month == 3 || month == 5 || month == 7 ||
        month == 8 || month == 10 || month == 12) {
        return 31;
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30;
    } else {
        // February
        return isLeapYear(year) ? 29 : 28;
    }
}

// Function to calculate the day of the week for a given date (returns 0 for Thursday)
int calculateDayOfWeek(int day, int month, int year) {
    static int daysInMonthLookup[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    year -= month <= 2;
    return (year + year / 4 - year / 100 + year / 400 + daysInMonthLookup[month - 1] + day) % 7;
}

// Function to calculate the working days in a given month and year
int calculateWorkingDays(int month, int year) {
    int totalDays = daysInMonth(month, year);
    int workingDays = 0;

    for (int day = 1; day <= totalDays; ++day) {
        int dayOfWeek = calculateDayOfWeek(day, month, year);
        if (dayOfWeek != 5 && dayOfWeek != 6) {
            // Not Saturday or Sunday
            ++workingDays;
        }
    }

    return workingDays;
}

// Function to copy a string to the clipboard (Windows only)
void copyToClipboard(const string& str) {
    const char* text = str.c_str();

    if (OpenClipboard(nullptr)) {
        EmptyClipboard();
        HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, strlen(text) + 1);
        if (hg != nullptr) {
            char* dest = static_cast<char*>(GlobalLock(hg));
            strcpy(dest, text);
            GlobalUnlock(hg);
            SetClipboardData(CF_TEXT, hg);
        }
        CloseClipboard();
    }
}

int main() {
    map<int, string> monthMap = {
        {1, "January"}, {2, "February"}, {3, "March"}, {4, "April"},
        {5, "May"}, {6, "June"}, {7, "July"}, {8, "August"},
        {9, "September"}, {10, "October"}, {11, "November"}, {12, "December"}
    };

    // Display available month options
    cout << "Available months:\n";
    for (const auto& entry : monthMap) {
        cout << entry.first << " - " << entry.second << '\n';
    }

    int month;
    int year;
    char confirmation;

    // Input month and year
    do {
        cout << "Enter the month (e.g., 1 for January): ";
        cin >> month;

        if (month < 1 || month > 12) {
            cout << "Invalid month. Please enter a number between 1 and 12.\n";
            continue;
        }

        cout << "Enter the year: ";
        cin >> year;

        // Confirm the input
        cout << "You entered: " << monthMap[month] << " " << year << ". Press Y or y to confirm, or N or n to enter again: ";
        cin >> confirmation;

        if (confirmation == 'n' || confirmation == 'N') {
            cout << "Please enter the month and year again.\n";
        } else if (confirmation != 'y' && confirmation != 'Y') {
            cout << "Invalid input. Operation aborted by user." << endl;
            return 0;
        }
    } while (!(confirmation == 'y' || confirmation == 'Y'));

    // Calculate and display working days
    int workingDays = calculateWorkingDays(month, year);
    cout << "Working days in " << monthMap[month] << " " << year << ": " << workingDays << endl;
    cout << "Your total Working hours were: " << workingDays * 8 << endl;

    // Copy result to clipboard
    copyToClipboard(to_string(workingDays));
    copyToClipboard(to_string(workingDays*8));
    // Pause the console window (for Windows)
    system("pause");

    return 0;
}
