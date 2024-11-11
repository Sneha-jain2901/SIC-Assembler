#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
// #include "optab.h"
using namespace std;

vector<vector<string>> code;
string prog_name,str,st_addrs;
map<string ,string> symtab;
vector<string> object_code;

void input(ifstream& file) {
    string str, temp;
    while (getline(file, str)) {
        vector<string> line(4, "\t"); // Initialize a line with 4 empty strings
        int k = 0, flag = 0, op_found = 0;
        temp.clear();
        
        for (int i = 0; i < str.size(); i++) {
            while (i < str.size() && str[i] != ' ' && str[i] != '\t' && str[i] != '\n') {
                temp.push_back(str[i++]);
                flag = 1;
            }
            
            if (flag) {
                if (getOpcode(temp) == "-1" && (temp != "BYTE" && temp != "WORD" && temp != "RESW" && temp != "RESB" && temp != "END")) {
                    // If it's not an opcode, check if we've already found the opcode
                    if (!op_found)
                        line[1] = temp; // Store as Label
                    else
                        line[3] = temp; // Store as Operand1 or Operand2
                } else {
                    line[2] = temp; // Store as Opcode
                    op_found = 1;
                }
                temp.clear();
                flag = 0;
            }
        }
        code.push_back(line); // Add line to code vector
    }
}

int hexToDec(string str)
{
    int y;
    stringstream stream;
    stream << str;
    stream >> hex >> y;
    return y;
}

    string decToHex(int num){
        stringstream stream;
        stream<<hex<<num;
        return stream.str();
    }

    string add(string last,string val,int flag){
        //adding hex and hex
        if(flag){
            int n1=hexToDec(last);
            int n2=hexToDec(val);
            return decToHex(n1+n2);
        }else{
            // adding dec and hex
            int n1=hexToDec(last);
            int n2=atoi(val.c_str());
            return decToHex(n1+n2);
        }
    }

    void addressing() {
    code[0][0] = st_addrs;

    if (code[0][1].size() > 0) {
        symtab[code[0][1]] = code[0][0];
    }

    for (int i = 1; i < code.size()-1; i++) {
        string mnemonic = code[i - 1][2];
        string last = code[i - 1][0];

        if (mnemonic != "BYTE" && mnemonic != "RESW" && mnemonic != "RESB") {
            code[i][0] = add(last, "3", 0); 
        } else {
            int bytes = 0;
            if (mnemonic == "BYTE") {
                string op_val = code[i-1][3];
                char ch = op_val[0];
                
                if (ch == 'C') {
                    bytes = op_val.size() - 3;
                } else if (ch == 'X') {
                    bytes = (op_val.size()+1 - 3) / 2;
                }
                code[i][0] = add(last, to_string(bytes), 1);
            } else if (mnemonic == "RESB") {
                int val = atoi(code[i-1][3].c_str());
                bytes = val;
                code[i][0] = add(last, decToHex(bytes), 1);
            } else if (mnemonic == "RESW") {
                int val = atoi(code[i-1][3].c_str());
                bytes = 3 * val;
                code[i][0] = add(last, decToHex(bytes), 1);
            }
        }
        if (code[i][1].size() > 0) {
            symtab[code[i][1]] = code[i][0];
        }
    }
}


    void objectCode(){
        string  objCode="",mnemonic,operand,label_addrs;
        for(int i=0;i<code.size();i++){
            int flag=0;
            objCode.clear();
            mnemonic=code[i][2];
            if(mnemonic=="RESW" || mnemonic=="RESB" || mnemonic=="END"){
                object_code.push_back("\t");
                continue;
            }
            operand.clear();
            for(int j=0;j<code[i][3].size();j++){
                if(code[i][3][j]==','){
                    flag=1;
                    break;
                }
                operand+=code[i][3][j];
            }
            if(mnemonic=="BYTE"){
                if(operand[0]=='C'){
                    for(int j=2;j<operand.size()-1;j++){
                           objCode+=(decToHex((int)operand[j]));
                    }
                }else{
                    for(int j=2;j<operand.size()-1;j++){
                           objCode+=operand[j];
                    }
                }
                if(objCode.size()<6){
                       objCode.insert(0,6-objCode.size(),'0');
                }
                object_code.push_back(objCode);
                continue;
            }else if(mnemonic=="WORD"){
                   objCode+=decToHex(atoi(operand.c_str()));
                if(objCode.size()<6){
                    objCode.insert(0,6-objCode.size(),'0');
                }
                object_code.push_back(objCode);
                continue;
            }
               objCode+=getOpcode(mnemonic);
            if(operand=="\t"){
                   objCode+="0000";
                object_code.push_back(objCode);
                continue;
            }
            label_addrs=symtab[operand];
            if(label_addrs[0]>'7'){
                if(label_addrs[0]>='A')
                    label_addrs[0]-=15;
                else
                    label_addrs[0]-=8;
            }
               objCode+=label_addrs;
            if(flag){
                   objCode=add(objCode,"8000",1);
            }
            if(objCode.size()<6){
                    objCode.insert(0,6-objCode.size(),'0');
                }
                object_code.push_back(objCode);
        }
    }
    void printSymtab(){
        cout<<"Symtab"<<endl;
            for (const auto& pair : symtab) {
                cout << "Key: " << pair.first << ", Value: " << pair.second << endl;
            }
        }
        void printObjectCode(){
            cout<<"ObjectCode"<<endl;
            for(auto &s:object_code){
                cout<<s<<endl;
            }
        }
void printCode() {
    for (const auto& row : code) {
        for (const auto& col : row) {
            cout << col << " ";
        }
        cout << endl;
    }
}
