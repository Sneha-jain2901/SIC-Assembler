#include<iostream>
#include<fstream>
#include "optab.h"
#include"pass1.h"
#include"pass2.h"

using namespace std;
int main(){
    ifstream inputFile("input.txt");
    ofstream outputFile("output.txt");

    if(!inputFile.is_open() || !outputFile.is_open()){
        cout<<"Error: Could not open file.\n";
        return 1;
    }
    inputFile>>prog_name>>str>>st_addrs;
    inputFile.ignore();
    input(inputFile);
    cout<<"Input Read"<<endl;
    printCode();
    addressing();
    cout<<"Addressing done"<<endl;
    printCode();
    printSymtab();
    objectCode();
    cout<<"Object Code done";
    printObjectCode();
    machineCode(outputFile);
}
