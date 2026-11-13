/////////////////////////////////////////////////////////////////////
// Your Name: Ashley Villela
// Date: 10/10/26
// Class: CSCI 1470.0X
// Semester: Fall 2026
// CSCI 1470 Instructor: Dr. Jonatan Reyes
// Program Description: Spell Checker (Phase 2)
/////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace std::chrono;

void initArr(string dictionaryWords[]);
void loadData(string dictionaryWords[], bool &flag);
bool spellCheck(string dictionaryWords[], string wordToMatch, string typeOfSearch);




const int MAX_WORDS = 466551;
const string LINEAR_SEARCH_UNSORTED = "LINEAR_UNSORTED";
const string LINEAR_SEARCH_SORTED = "LINEAR_SORTED";
const string BINARY_SEARCH = "BINARY";

int MAX_MISTAKES = 0;
int numberOfWords = 0;

const bool DEBUG = false;

int main()
{
    ifstream iFile;
    bool flag = true;
    string wordToMatch, typeOfSearch;

    iFile.open("dictionary.txt");
    string* dictionaryWords = new string[MAX_WORDS];

    auto start = high_resolution_clock::now();

    cout << setfill('*') << setw(39) << "*" << endl;
    cout << "    Welcome To My Spell Check Game!" << endl;
    cout << setfill('*') << setw(39) << "*" << endl;

    initArr(dictionaryWords);    
    if(DEBUG)
    {
        auto end = high_resolution_clock::now();
        duration<double> elapsed = (end - start) * 1000000000;
        cout << endl << "DEBUG: Initialization took " << elapsed.count() << " microseconds." << endl;
    }

    start = high_resolution_clock::now();
    loadData(dictionaryWords, flag);

    if(!flag)
    {
        return 1;
    }

    if(DEBUG)
    {
        auto end = high_resolution_clock::now();
        duration<double> elapsed = (end - start) * 1000000000;        
        cout << "DEBUG: Loading took " << elapsed.count() << " microseconds." << endl;
    }

    string settingsFiles[] =
    {
        "settings_1.txt",
        "settings_2.txt",
        "settings_3.txt"
    };

    for(int i = 0; i < 3 && i < MAX_WORDS; i++)
    {
        ifstream settingsFile(settingsFiles[i]);
        
        settingsFile >> MAX_MISTAKES >> typeOfSearch;
        settingsFile.close();

        if(typeOfSearch == LINEAR_SEARCH_SORTED || typeOfSearch == BINARY_SEARCH)
        {
            sort(dictionaryWords, dictionaryWords + MAX_MISTAKES);
        }
        
        int mistakes = 0;
        char sentinel = 'Y';

        
        while( sentinel == 'Y' || sentinel == 'y' && mistakes < MAX_MISTAKES)
        {
            cout << "Please Enter a Word: ";
            cin >> wordToMatch;
            
            start = high_resolution_clock::now();
            if(spellCheck(dictionaryWords, wordToMatch, typeOfSearch))
            {
                cout << "CONGRATULATIONS YOU GOT IT RIGHT!!" << endl;
                cout << "The word '" << wordToMatch << "' is spelled correctly! Great Job! :)" << endl;
            }
            else
            {
                cout << "OOPS! It looks like '" << wordToMatch << "' is not correct :(" << endl;
                cout << "But don't worry! You are doing AMAZING! Keep trying! :)" << endl;
                mistakes++;
            }
            if(DEBUG)
            {
                auto end = high_resolution_clock::now();
                duration<double> elapsed = (end - start) * 1000000000;        
                cout << "DEBUG: Loading took " << elapsed.count() << " microseconds." << endl;
            }

            if(mistakes == MAX_MISTAKES)
            {
                cout << "You have reached the maximum number (" << MAX_MISTAKES << ") of misspelled words allowed:(" << endl;
                break;
            }

            cout << "Do you want to continue? (Y/N): ";
            cin >> sentinel;

            if(sentinel == 'n' || sentinel == 'N')
            {
                break;
            }

        }
    }
    cout << "Thank You for Playing! :)" << endl;
    
    return 0;
}

void initArr(string dictionaryWords[])
{
    for(int i = 0; i < MAX_WORDS; i++)
    {
        dictionaryWords[i] = "";
    }
}

void loadData(string dictionaryWords[], bool &flag)
{
    ifstream iFile;
    int count = 0;
    
    iFile.open("dictionary.txt");
    
    if(!iFile)
    {
        flag = true;
        return;
    }
    flag = false;
    
    while(count < MAX_WORDS && iFile >> dictionaryWords[count])
    {
        count++;
    }
    iFile.close();
}

bool spellCheck(string dictionaryWords[], string wordToMatch, string typeOfSearch)
{
    if(typeOfSearch == LINEAR_SEARCH_UNSORTED)
    {
        for(int i = 0; i < MAX_MISTAKES; i++)
        {
            if(dictionaryWords[i] == wordToMatch)
            {
                return true;
            }
        }
    }
    else if(typeOfSearch == LINEAR_SEARCH_SORTED)
    {
        for(int i = 0; i < MAX_MISTAKES; i++)
        {
            if(dictionaryWords[i] == wordToMatch)
            {
                return true;
            }
            else if(dictionaryWords[i] > wordToMatch)
            {
                break;
            }
        }
    }
    else if(typeOfSearch == BINARY_SEARCH)
    {
        int left = 0;
        int right = MAX_MISTAKES - 1;

        while(left <= right)
        {
            int mid = left + (right - left) / 2;
            
            if(dictionaryWords[mid] == wordToMatch)
            {
                return true;
            }
            else if(dictionaryWords[mid] < wordToMatch)
            {
                left = mid + 1;
            }
            else
            {
                right = mid - 1;
            }
        }
    }
    
    return false;
}