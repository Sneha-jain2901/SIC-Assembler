#ifndef OPTAB_H
#define OPTAB_H

#include <unordered_map>
#include <string>

using namespace std;

// Function to get the singleton optab
unordered_map<string, string>& getOptab() {
    static unordered_map<string, string> optab = {
        {"LDA", "00"}, {"AND", "40"}, {"DIV", "24"}, {"SUB", "1C"},
        {"ADD", "18"}, {"LDL", "08"}, {"RD", "D8"}, {"WD", "DC"},
        {"LDCH", "50"}, {"STX", "10"}, {"JLT", "38"}, {"TIX", "2C"},
        {"TD", "E0"}, {"STCH", "54"}, {"STL", "14"}, {"LDX", "04"},
        {"RSUB", "4C"}, {"STA", "0C"}, {"J", "3C"}, {"JEQ", "30"},
        {"COMP", "28"}, {"JSUB", "48"}, {"JGT", "34"}, {"MUL", "20"},
        {"OR", "44"}, {"STSW", "E8"}
    };
    return optab;
}

string getOpcode(string& mnenomic){
    const auto& optab =getOptab();
    auto it=optab.find(mnenomic);
    return it!=optab.end()?it->second:"-1";
}

#endif
