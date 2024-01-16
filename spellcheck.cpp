# include <hash.h>
# include <iostream>
# include <sstream>
# include <string>
# include <vector>
# include <fstream>
# include <ctime>
# include <cctype>

using namespace std;

// Spell Check Program
// By: Jeffrey Wong
/* This program uses a custom hashtable implementation to process and store a 
"dictionary" (really a collection) of words as keys. It then allows the user to "check" 
a file for words that are not in the dictionary or are too long, line by line. */

void loadDictionary(hashTable& dict){
    string inputFile;
    cout << "Enter name of dictionary: " << "\n";
    cin >> inputFile;
    ifstream input(inputFile);
    time_t t1 = clock();
    string line;
    while(getline(input, line)){
        // Turn all letters lowercase
        string line_lower;
        for(char c:line){line_lower += tolower((unsigned char)c);}
        dict.insert(line_lower);
    }
    time_t t2 = clock();
    double time_diff = ((double) (t2-t1))/CLOCKS_PER_SEC;
    cout << "Total time (in seconds) to load dictionary: " << time_diff << "\n";
    input.close();
}

void spellCheck(hashTable& dict){
    string inputFile;
    cout << "Enter name of input file: " << "\n";
    cin >> inputFile;
    ifstream input(inputFile);
    string outputFile;
    cout << "Enter name of output file: " << "\n";
    cin >> outputFile;
    ofstream output(outputFile);
    time_t t1 = clock();
    string line;
    int linenum = 1;
    while(getline(input, line)){
        // Process input file: TODO
        string curWord;
        bool containsDigit = false;
        for (char c:line){
            // Note: isalnum requires an unsigned char. 
            if(isalnum((unsigned char)c) || c == '-' || c == '\''){
                curWord += tolower((unsigned char)c);
                if(isdigit((unsigned char)c)){containsDigit = true;}
            }
            else{
                if(curWord.length() == 0 || containsDigit){containsDigit = false;}
                else if(curWord.length() > 20){
                    output << "Long word at line " << linenum << ", starts: " << curWord.substr(0,20) << "\n";
                }
                else if(!(dict.contains(curWord))){
                    output << "Unknown word at line " << linenum << ": " << curWord << "\n";
                }
                curWord = ""; // Resets curWord after each word finished processing
            }
        }
        // This is a temporary workaround! Last word does not parse if there are no intervening characters between last word and newline!
        if(curWord.length() == 0 || containsDigit){;}
        else if(curWord.length() > 20){
            output << "Long word at line " << linenum << ", starts: " << curWord.substr(0,20) << "\n";
        }
        else if(!(dict.contains(curWord))){
            output << "Unknown word at line " << linenum << ": " << curWord << "\n";
        }
        linenum++;
    }
    time_t t2 = clock();
    double time_diff = ((double) (t2-t1))/CLOCKS_PER_SEC;
    cout << "Total time (in seconds) to check document: " << time_diff << "\n";
    input.close();
    output.close();
}

int main(){
    hashTable dict(50000);
    loadDictionary(dict);
    spellCheck(dict);
    return 0;
}