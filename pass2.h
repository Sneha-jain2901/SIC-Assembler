#include<string>
#include<fstream>

using namespace std;

void machineCode(ofstream &output){
    string size=decToHex(hexToDec(code[code.size()-2][0])-hexToDec(st_addrs));
    output<<"H "<<prog_name<<" "<<st_addrs<<" "<<size<<endl;

    int i=0,save=0,et_flag=0;
    string execute;

    while(1){
        int flag=0,len=0,codes=0;
        i=save;
        while(i<code.size()-1 && object_code[i]!="\t" && codes<30){
            len++;
            if(!flag && getOpcode(code[i][2])!="-1"){
                output<<"T ";
                flag=1;
                output<<code[i][0]<<" ";
                if(!et_flag){
                    execute=code[i][0];
                    et_flag=1;
                }
            }
            if(object_code[i]!="\t"){
                codes+=object_code[i].size()/2;
                if(codes>30){
                    codes-=object_code[i].size()/2;
                    break;
                }
            }
            i++;
        }
        if(!flag){
            save++;
            continue;
        }
        output<<decToHex(codes)<<" ";
        i=save;
        codes=0;
        for(;codes<30 && i<code.size();i++){
            if (object_code[i] != "\t") {
                codes += (object_code[i].size() / 2);
                if (codes > 30)
                    break;
                output << object_code[i] << " ";
            } else {
                i++;
                break;
            }
        }
        save = i;
        if (i >= code.size() - 1)
            break;
        output << endl;
    }
    output<<endl<<"E"<<execute<<endl;
}