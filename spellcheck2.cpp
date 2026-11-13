
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

const bool DEBUG = true;

int main()
{
    bool flag = false;
    bool stopProgram = false;

    string wordToMatch, typeOfSearch;

    string* dictionaryWords = new string[MAX_WORDS];

    auto start = high_resolution_clock::now();

    cout << setfill('*') << setw(39) << "*" << endl;
    cout << "    Welcome To My Spell Check Game!" << endl;
    cout << setfill('*') << setw(39) << "*" << endl;
    cout << endl;
    cout << endl;

    initArr(dictionaryWords);

    if(DEBUG)
    {
        auto end = high_resolution_clock::now();
        duration<double, micro> elapsed = end - start;

        cout << endl << "DEBUG: Initialization took "
             << elapsed.count() << " microseconds." << endl;
    }

    start = high_resolution_clock::now();

    loadData(dictionaryWords, flag);

    if(flag)
    {
        cout << "Error opening dictionary.txt!" << endl;
        delete[] dictionaryWords;
        return 1;
    }

    if(DEBUG)
    {
        auto end = high_resolution_clock::now();
        duration<double, micro> elapsed = end - start;

        cout << "DEBUG: Loading took "
             << elapsed.count() << " microseconds." << endl << endl;
    }

    string settingsFiles[] =
    {
        "settings.txt",
        "settings_2.txt",
        "settings_3.txt"
    };

    for(int i = 0; i < 3 && !stopProgram; i++)
    {
        ifstream settingsFile(settingsFiles[i]);

        if(!settingsFile)
        {
            cout << "Error opening " << settingsFiles[i] << "!" << endl;
            continue;
        }

        if(!(settingsFile >> MAX_MISTAKES >> typeOfSearch))
        {
            cout << "Error reading " << settingsFiles[i] << "!" << endl;
            settingsFile.close();
            continue;
        }

        settingsFile.close();

        if(typeOfSearch == LINEAR_SEARCH_SORTED ||
           typeOfSearch == BINARY_SEARCH)
        {
            sort(dictionaryWords, dictionaryWords + numberOfWords);
        }

        int mistakes = 1;
        char sentinel;

        cout << "Please Enter a Word: ";
        cin >> wordToMatch;

        while((sentinel == 'Y' || sentinel == 'y') || (mistakes < MAX_MISTAKES))
        {

            start = high_resolution_clock::now();

            if(spellCheck(dictionaryWords, wordToMatch, typeOfSearch))
            {
                cin.ignore();
                system("clear");
                cout << endl;
                cout << setfill('*') << setw(39) << "*" << endl;
                cout << "CONGRATULATIONS YOU GOT IT RIGHT!!" << endl;
                cout << setfill('*') << setw(39) << "*" << endl;
                cout << endl << "The word '" << wordToMatch << "' is spelled correctly! Great Job! :)" << endl;

                cout << endl << "Do you want to continue? (Y/N): ";
                cin >> sentinel;
            
                if(sentinel == 'n' || sentinel == 'N')
                {
                    stopProgram = true;
                    break;
                }
            
                else if(sentinel == 'Y' || sentinel == 'y')
                {
                    cin.ignore();
                    system("clear");
                    cout << "Please Enter a Word: ";
                    cin >> wordToMatch;
                }
                else if(sentinel != 'y' || sentinel != 'Y' || sentinel != 'n' || sentinel != 'N')
                {
                    cout << "Error! Please Enter a Valid Response (Y/N): ";
                    cin >> sentinel;
                }
            }
            else 
            {
                mistakes++;
                cout << endl << "OOPS! It looks like '" << wordToMatch << "' is not correct :(" << endl;
                cout << "But don't worry! You are doing AMAZING!" << " Keep trying! :)" << endl << endl;

                cout << "Try Again :) : ";
                cin >> wordToMatch;

                if(spellCheck(dictionaryWords, wordToMatch, typeOfSearch))
                {
                    stopProgram = true;
                }   
            }
            if(mistakes == MAX_MISTAKES)
            {
                cout << "Sorry, you have reached the maximum number of mistakes (" << MAX_MISTAKES << ")." << endl;
                stopProgram = true;
                break;
            }

        }
    }
    cin.get();
    cin.ignore();
    system("clear");
    cout << "Thank You for Playing! :)" << endl;
    cout << endl;

    delete[] dictionaryWords;

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

    numberOfWords = 0;

    iFile.open("dictionary.txt");

    if(!iFile)
    {
        flag = true;
        return;
    }

    flag = false;

    while(numberOfWords < MAX_WORDS &&
          iFile >> dictionaryWords[numberOfWords])
    {
        numberOfWords++;
    }

    iFile.close();
}

bool spellCheck(string dictionaryWords[], string wordToMatch, string typeOfSearch)
{
    auto start = high_resolution_clock::now();
    if(typeOfSearch == LINEAR_SEARCH_UNSORTED)
    {
        for(int i = 0; i < numberOfWords; i++)
        {
            if(dictionaryWords[i] == wordToMatch)
            {
                return true;
            }
        }
    }
        if(DEBUG)
        {
            auto end = high_resolution_clock::now();
            duration<double, micro> elapsed = end - start;
            cout << "DEBUG: Linear Search Unsorted took " << elapsed.count() << " microseconds." << endl << endl;
        }
    

    start = high_resolution_clock::now();
    if(typeOfSearch == LINEAR_SEARCH_SORTED)
    {
        for(int i = 0; i < numberOfWords; i++)
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
        if(DEBUG)
        {
            auto end = high_resolution_clock::now();
            duration<double, micro> elapsed = end - start;
            cout << "DEBUG: Linear Search sorted took " << elapsed.count() << " microseconds." << endl << endl;
        }

    start = high_resolution_clock::now();

    if(typeOfSearch == BINARY_SEARCH)
    {
        int left = 0;
        int right = numberOfWords - 1;

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
    if(DEBUG)
        {
            auto end = high_resolution_clock::now();
            duration<double, micro> elapsed = end - start;
            cout << "DEBUG: Binary search took " << elapsed.count() << " microseconds." << endl << endl;
        }
    

    return false;
}
