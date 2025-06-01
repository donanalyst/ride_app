#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <cstdlib>
#include <conio.h> // For _getch() on Windows, may need to change for other platforms
#include "calendar_picker.h"


// Function to check if a year is a leap year
bool isLeapYear(int year) {
    if (year % 4 != 0) return false;
    if (year % 100 != 0) return true;
    return (year % 400 == 0);
}

// Function to get the number of days in a month
int daysInMonth(int month, int year) {
    switch (month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            return 31;
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            return isLeapYear(year) ? 29 : 28;
        default:
            return 0;
    }
}

// Function to get the day of the week for the first day of the month (0=Sunday)
int getFirstDayOfMonth(int month, int year) {
    // Using Zeller's Congruence algorithm
    if (month < 3) {
        month += 12;
        year--;
    }
    int k = year % 100;
    int j = year / 100;
    int day = 1;
    
    int h = (day + 13*(month + 1)/5 + k + k/4 + j/4 + 5*j) % 7;
    return (h + 5) % 7; // Convert to 0=Sunday format
}

// Function to display the calendar for a given month and year
void displayCalendar(int month, int year, int selectedDay = 0) {
    std::string monthNames[] = {"January", "February", "March", "April", "May", "June",
                          "July", "August", "September", "October", "November", "December"};
    
    // Clear screen (platform dependent)
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    
    std::cout << "\n  " << monthNames[month - 1] << " " << year << "\n";
    std::cout << "  Su Mo Tu We Th Fr Sa\n";
    
    int firstDay = getFirstDayOfMonth(month, year);
    int daysInCurrentMonth = daysInMonth(month, year);
    
    // Print leading spaces
    for (int i = 0; i < firstDay; i++) {
        std::cout << "   ";
    }
    
    // Print days of the month
    for (int day = 1; day <= daysInCurrentMonth; day++) {
        if (day == selectedDay) {
            std::cout << " [" << std::setw(2) << day << "]";
        } else {
            std::cout << " " << std::setw(2) << day;
        }
        
        if ((day + firstDay) % 7 == 0 || day == daysInCurrentMonth) {
            std::cout << "\n";
        }
    }
}

// Function to get user input for date selection
void selectDate(int &month, int &year, int &day) {
    char key;
    bool dateSelected = false;
    
    // Get current date as default
    time_t now = time(0);
    tm *ltm = localtime(&now);
    year = 1900 + ltm->tm_year;
    month = 1 + ltm->tm_mon;
    day = ltm->tm_mday;
    
    while (!dateSelected) {
        displayCalendar(month, year, day);
        
        std::cout << "\nUse arrow keys to navigate, Enter to select, Q to quit";
        std::cout << "\nSelected date: " << year << "-" << month << "-" << day << "\n";
        
        key = _getch();
        
        // Handle arrow keys (Windows specific, may need adjustment for other platforms)
        if (key == 0 || key == -32) { // Arrow key prefix
            key = _getch();
            switch (key) {
                case 72: // Up arrow
                    day -= 7;
                    if (day < 1) {
                        month--;
                        if (month < 1) {
                            month = 12;
                            year--;
                        }
                        day = daysInMonth(month, year) + day;
                    }
                    break;
                case 80: // Down arrow
                    day += 7;
                    if (day > daysInMonth(month, year)) {
                        day -= daysInMonth(month, year);
                        month++;
                        if (month > 12) {
                            month = 1;
                            year++;
                        }
                    }
                    break;
                case 75: // Left arrow
                    day--;
                    if (day < 1) {
                        month--;
                        if (month < 1) {
                            month = 12;
                            year--;
                        }
                        day = daysInMonth(month, year);
                    }
                    break;
                case 77: // Right arrow
                    day++;
                    if (day > daysInMonth(month, year)) {
                        day = 1;
                        month++;
                        if (month > 12) {
                            month = 1;
                            year++;
                        }
                    }
                    break;
            }
        } else if (key == 13) { // Enter key
            dateSelected = true;
        } else if (key == 'q' || key == 'Q') {
            exit(0);
        } else if (key == 'n' || key == 'N') { // Next month
            month++;
            if (month > 12) {
                month = 1;
                year++;
            }
            day = std::min(day, daysInMonth(month, year));
        } else if (key == 'p' || key == 'P') { // Previous month
            month--;
            if (month < 1) {
                month = 12;
                year--;
            }
            day = std::min(day, daysInMonth(month, year));
        }
    }
}

