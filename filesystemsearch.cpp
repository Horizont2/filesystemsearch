#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>

using namespace std;
namespace fs = std::filesystem;

vector<int> findWordInFile(const string& filePath, const string& word) {
    ifstream file(filePath);
    string line;
    vector<int> wordPositions;
    int lineNumber = 0;
    
    while (getline(file, line)) {
        lineNumber++;
        size_t pos = line.find(word);
        if (pos != string::npos) {
            wordPositions.push_back(lineNumber);
        }
    }
    
    return wordPositions;
}
void replaceOrRemoveWordInFile(const string& filePath, const string& word, const string& replacement = "") {
    ifstream file(filePath);
    string content;
    string line;
    
    while (getline(file, line)) {
        size_t pos = line.find(word);
        while (pos != string::npos) {
            line.replace(pos, word.length(), replacement);
            pos = line.find(word, pos + replacement.length());
        }
        content += line + "\n";
    }
    
    file.close();
    
    ofstream outFile(filePath);
    outFile << content;
    outFile.close();
}
void searchAndReplaceInFolder(const string& folderPath, const string& word, const string& replacement = "") {
    for (const auto& entry : fs::recursive_directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            const string filePath = entry.path().string();
            vector<int> positions = findWordInFile(filePath, word);
            
            if (!positions.empty()) {
                cout << "Word found in file: " << filePath << " on lines: ";
                for (const auto& line : positions) {
                    cout << line << " ";
                }
                cout << endl;
                
                if (!replacement.empty() || replacement == "") {
                    replaceOrRemoveWordInFile(filePath, word, replacement);
                    cout << "Word replaced/removed in file: " << filePath << endl;
                }
            }
        }
    }
}
int main() {
    string folderPath;
    string word;
    string replacement;
    
    cout << "Enter the folder path: ";
    getline(cin, folderPath);
    
    cout << "Enter the word to search: ";
    getline(cin, word);
    
    cout << "Enter the replacement word (or press Enter to delete the word): ";
    getline(cin, replacement);
    
    searchAndReplaceInFolder(folderPath, word, replacement);
    
    return 0;
}
