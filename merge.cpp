# include <iostream>
# include <sstream>
# include <string>
# include <vector>
# include <fstream>

using namespace std;

int validMerges[1001][1001]; // Each entry corresponds to substrings of length equal to index
// -1 means unknown, 0 means we are stuck, 1 means we take from the first string, 2 means we take from the second string-
// Differentiating between 1 and 2 is important for forming the final merge

// Recursive method for peforming merge
int testmerge(string string1, string string2, string candidate){
    // At any given time a merge is viable only if we have the first letter of the candidate matching the first letter of one of the strings
    if(string1.length() > 0 && string1[0] == candidate[0] && validMerges[string1.length()-1][string2.length()] < 0){
        testmerge(string1.substr(1), string2, candidate.substr(1));
    }
    if(string2.length() > 0 && string2[0] == candidate[0] && validMerges[string1.length()][string2.length()-1] < 0){
        testmerge(string1, string2.substr(1), candidate.substr(1));
    }
    // Base cases occur when one string has been exhausted and the other has one char left
    if(string2.length() == 0 && string1.length() == 1 && string1 == candidate){
        validMerges[string1.length()][string2.length()] = 1;
        return 1;
    }
    else if(string1.length() == 0 && string2.length() == 1 && string2 == candidate){
        validMerges[string1.length()][string2.length()] = 2;
        return 2;
    }
    // Prioritize taking from string 1 first, if possible
    else if(string1.length() > 0 && string1[0] == candidate[0] && validMerges[string1.length()-1][string2.length()] > 0){
        validMerges[string1.length()][string2.length()] = 1;
    }
    // Then try taking from string 2
    else if(string2.length() > 0 && string2[0] == candidate[0] && validMerges[string1.length()][string2.length()-1] > 0){
        validMerges[string1.length()][string2.length()] = 2;
    }
    // If neither work, no valid merge from this path
    else{validMerges[string1.length()][string2.length()] = 0;}
    return validMerges[string1.length()][string2.length()];
}

// Outputs merged string based on validMerges array
string traverseMerge(string string1, string string2){
    int i = string1.length(), j = string2.length();
    string result = "";

    // This should run until exactly i = 0 and j = 0
    while(i > 0 || j > 0){
        if(validMerges[i][j] == 1){
            result += toupper(string1[string1.length()-i]); // Converts char to uppercase
            i--;
        }
        else if(validMerges[i][j] == 2){
            result += (string2[string2.length()-j]);
            j--;
        }
        else{
            cerr << "Error: No valid path found, should not happen if we have reached here\n";
            exit(-1);
        }
    }
    return result;
}

int main(){
    string inputFile, outputFile;
    cout << "Enter the name of the input file: ";
    cin >> inputFile;
    ifstream input(inputFile);
    cout << "Enter the name of the output file: ";
    cin >> outputFile;
    ofstream output(outputFile);

    string line, string1, string2, candidate, result;
    int linenum = 1;
    while(getline(input, line)){
        switch(linenum % 3){
            case 1:
                string1 = line;
                break;
            case 2:
                string2 = line;
                break;
            default:
                for(int i = 0; i < 1001; i++){
                    for(int j = 0; j < 1001; j++){
                        validMerges[i][j] = -1; // Reset array before doing merge
                    }
                }
                candidate = line;
                if(testmerge(string1, string2, candidate) > 0){
                    result = traverseMerge(string1, string2);
                    output << result << "\n";
                }
                else{
                    output << "*** NOT A MERGE ***\n";
                }
                break;
        }
        linenum++;
    }
    return 0;
}