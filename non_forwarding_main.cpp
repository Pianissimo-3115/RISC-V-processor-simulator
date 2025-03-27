// #include<iostream>


#include <bits/stdc++.h>

#include "non_forwarding_architecture.cpp"

using namespace std;

int main(int argc, char* argv[])    // terminal input: forwarding status and no of cycles

{

    char* inputfile = argv[1];

    char* temp = argv[2];

    int cycles = stoi(temp);

    vector<unsigned> instructions;

    vector<string> instr_statements;
    cout<<inputfile<<"Hello";
    FILE* f = fopen(inputfile, "r");

    char line[256];

    while (fgets(line, sizeof(line), f)) {

        int i = 0;

        while(line[i] != ' ' && line[i] != '\n' && line[i] != '\0') i++;

        while(line[i] == ' ') i++;

        string instruction = "";

        while(line[i] != ' ' && line[i] != '\0' && line[i] != '\n') {

            instruction += line[i];

            i++;

        }

        instructions.push_back((unsigned)strtoul(instruction.c_str(), NULL, 16));

        while(line[i] == ' ') i++;

        string instr = "";

        while(line[i] != '\0' && line[i] != '\n') {

            instr += line[i];

            i++;

        }

        instr_statements.push_back(instr);

    }

    fclose(f);

    



    RV32NonForwarding* rv32 = new RV32NonForwarding();

    rv32->LoadInstructions(instructions);

    for (auto i: instructions)
    {
        cout<<hex<<i<<dec<<endl;
    }
    
    rv32->MakeConnections();
    int IF_PC = 0;
    int ID_PC = 0;
    bool will_bop_ID = 1;
    bool ID_nop = 1;
    int Ex_PC = 0;
    bool Ex_nop = 1;
    int Mem_PC = 0;
    bool Mem_nop = 1;
    while (1)
    {
        char x;
        cin>>x;
        (*rv32).DoCycle();
        for (int i = 0; i < 32; i++)
        {
            cout<<"x"<<i<<": "<<(*rv32).Register_Mem.Registers[i]<<endl;
        }
        
        IF_PC = bitsStringToUnsigned((*rv32).IF_ID_Reg.Data.substr(0, 32));
        ID_PC = bitsStringToUnsigned((*rv32).ID_EX_Reg.Data.substr(10, 32));
        if (IF_PC/4>=instr_statements.size())
        {
            cout<<"IF instruction: no instruction ar this address"<<endl;
        }
        else cout<<"IF instruction: "<<instr_statements[IF_PC/4]<<endl;
        
        if (ID_nop)
        {
            cout<<"ID instruction: nop"<<endl;
        }
        else
        {
            if (ID_PC/4>=instr_statements.size())
            {
                cout<<"ID instruction: no instruction at this address"<<endl;
            }
            else cout<<"ID instruction: "<<instr_statements[ID_PC/4]<<endl;
        }
        
        if (Ex_nop)
        {
            cout<<"Ex instruction: nop"<<endl;
        }
        else
        {
            if (Ex_PC/4>=instr_statements.size())
            {
                cout<<"Ex instruction: no instruction at this address"<<endl;
            }
            else cout<<"Ex instruction: "<<instr_statements[Ex_PC/4]<<endl;
        }
        if (Mem_nop)
        {
            cout<<"Mem instruction: nop"<<endl;
        }
        else
        {
            if (Mem_PC/4>=instr_statements.size())
            {
                cout<<"Mem instruction: no instruction at this address"<<endl;
            }
            else cout<<"Mem instruction: "<<instr_statements[Mem_PC/4]<<endl;
        }

        // cout<<IF_PC<<" "<<ID_PC<<" "<<Ex_PC<<" "<<mEm_PC<<endl;
        cout<<"IF_ID_Register: "<<(*rv32).IF_ID_Reg.Data<<endl;
        cout<<"IF_ID_Register PC: "<<(*rv32).IF_ID_Reg.Data.substr(0,32)<<endl;
        cout<<"IF_ID_Register Instr: "<<(*rv32).IF_ID_Reg.Data.substr(32,32)<<endl;
        cout<<"ID_EX_Register: "<<(*rv32).ID_EX_Reg.Data<<endl;
        cout<<"ID_EX_Rd: "<<(*rv32).ID_EX_Reg.Data.substr(163-5,5)<<endl;
        cout<<"ID_EX_Rs2: "<<(*rv32).ID_EX_Reg.Data.substr(163-5-5,5)<<endl;
        cout<<"ID_EX_Rs1: "<<(*rv32).ID_EX_Reg.Data.substr(163-5-5-5,5)<<endl;
        cout<<"EX_MEM_Register: "<<(*rv32).EX_MEM_Reg.Data<<endl;
        cout<<"MEM_WB_Register: "<<(*rv32).MEM_WB_Reg.Data<<endl;
        Mem_PC=Ex_PC;
        Ex_PC=ID_PC;
        ID_PC=IF_PC;
        Mem_nop=Ex_nop;
        if (ID_nop)
        {
            Ex_nop=true;
        }
        else if ((*rv32).ID_EX_Reg.Data.substr(0,10) == "0000000000" || rv32->ID_EX_Reg.Data.substr(0,10) == "0000xx0000")
        {
            Ex_nop=true;
        }
        else
        {
            Ex_nop=false;
        }
        
        
        
        ID_nop=((*rv32).IF_ID_Reg.FlushBit == "1" && (*rv32).IF_ID_Reg.StallBit == "0")  ? true : false;
        cout<<"IF_ID_Reg.FLushbit: "<<((*rv32).IF_ID_Reg.FlushBit == "1")<<endl;
    }
    

}

