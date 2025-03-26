#include <bits/stdc++.h>
#include "architecture.cpp"
using namespace std;
int main(int argc, char* argv[])    // terminal input: forwarding status and no of cycles
{
    string inputfile = argv[1];
    string temp = argv[2];
    int cycles = stoi(temp);
    vector<string> instructions;
    vector<string> instr_statements;
    FILE f = *fopen(inputfile.c_str(), "r");
    char line[256];
    while (fgets(line, sizeof(line), &f)) {
        int i = 0;
        while(line[i] != ' ' && line[i] != '\n' && line[i] != '\0') i++;
        while(line[i] == ' ') i++;
        string instruction = "";
        while(line[i] != ' ' && line[i] != '\0' && line[i] != '\n') {
            instruction += line[i];
            i++;
        }
        instructions.push_back(instruction);
        while(line[i] == ' ') i++;
        string instr = "";
        while(line[i] != '\0' && line[i] != '\n') {
            instr += line[i];
            i++;
        }
        instr_statements.push_back(instr);
    }
    fclose(&f);
    

    RV32Forwarding* rv32 = new RV32Forwarding();
    rv32->load_instructions(instructions);
    


}