// #include<iostream>


// #include <bits/stdc++.h>

#include "non_forwarding_architecture.cpp"
#include<vector>
#include<tuple>
#include<iostream>

using namespace std;

int main(int argc, char* argv[])    // terminal input: forwarding status and no of cycles

{

    char* inputfile = argv[1];

    char* temp = argv[2];

    int cycles = stoi(temp);
    int cycles_start=0;
    int ask_inp = 0;
    if (argc>=4)
    {
        cycles_start = stoi(argv[3]);
    }
    if (argc>=5)
    {
        ask_inp = stoi(argv[4]);
    }

    vector<unsigned> instructions;

    vector<string> instr_statements;
    vector<tuple<string, int, int, int, int, int>> exec_table;
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
    int IF_PC = -1;
    int IF_InstrNum = 0;
    int ID_PC = -1;
    int ID_InstrNum = 0;
    bool ID_nop = 1;
    int Ex_PC = -1;
    int Ex_InstrNum = 0;
    bool Ex_nop = 1;
    int Mem_PC = -1;
    int Mem_InstrNum = 0;
    bool Mem_nop = 1;
    int WB_PC = -1;
    int WB_InstrNum = 0;
    bool WB_nop = 1;
    int cycle_num = 0;
    bool stall = false;
    while (cycle_num<cycles)
    {
        char x;
        if (ask_inp>0) cin>>x;
        (*rv32).DoCycle();
        
        for (int i = 0; i < 32; i++)
        {
            cout<<" x"<<i<<": "<<(*rv32).Register_Mem.Registers[i];
            if (i%4==3) cout<<endl;
        }

        unsigned curr = UINT_MAX-1;
        for (int i = 0; i < 10; i++)
        {
            cout<<(((*rv32).DataMemory_Unit.Memory[unsignedToBitsString(curr)] == "") ?    "00000000" : (*rv32).DataMemory_Unit.Memory[unsignedToBitsString(curr)]     )<<" ";
            cout<<(((*rv32).DataMemory_Unit.Memory[unsignedToBitsString(curr-1)] == "") ?  "00000000" : (*rv32).DataMemory_Unit.Memory[unsignedToBitsString(curr-1)]   )<<" ";
            cout<<(((*rv32).DataMemory_Unit.Memory[unsignedToBitsString(curr-2)] == "") ?  "00000000" : (*rv32).DataMemory_Unit.Memory[unsignedToBitsString(curr-2)]   )<<" ";
            cout<<(((*rv32).DataMemory_Unit.Memory[unsignedToBitsString(curr-3)] == "") ? "00000000" : (*rv32).DataMemory_Unit.Memory[unsignedToBitsString(curr-3)]  )<<" ";
            cout<<endl;
            curr-=4;
        }
        // for (auto x: (*rv32).DataMemory_Unit.Memory)
        // {
        //     cout<<x.first<<": "<<x.second<<endl;
        // }
        
        

        int last_IF_PC = IF_PC;
        IF_PC = bitsStringToUnsigned((*rv32).IF_ID_Reg.Data.substr(0, 32));
        stall = (IF_PC==last_IF_PC);
        cout<<"stall: "<<stall<<endl;
        if (stall)
        {
            ID_InstrNum=Ex_InstrNum;
        }
        
        ID_PC = bitsStringToUnsigned((*rv32).ID_EX_Reg.Data.substr(10, 32));
        if (IF_PC/4>=instr_statements.size())
        {
            cout<<"IF instruction: no instruction ar this address"<<endl;
        }
        else 
        {
            cout<<"IF instruction: "<<instr_statements[IF_PC/4]<<endl;
            if (!stall)
            {
                IF_InstrNum=exec_table.size();
                exec_table.push_back(make_tuple(instr_statements[IF_PC/4], cycle_num, -1, -1, -1 ,-1));
            }
            else get<1>(
                exec_table[exec_table.size()-1]
            ) = cycle_num;
        }
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
            else 
            {
                cout<<"ID instruction: "<<instr_statements[ID_PC/4]<<endl;
                get<2>(exec_table[ID_InstrNum]) = cycle_num;
            }
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
            else
            {
                cout<<"Ex instruction: "<<instr_statements[Ex_PC/4]<<endl;
                get<3>(exec_table[Ex_InstrNum]) = cycle_num;
            }
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
            else
            {
                cout<<"Mem instruction: "<<instr_statements[Mem_PC/4]<<endl;
                get<4>(exec_table[Mem_InstrNum]) = cycle_num;
            }
        }
        if (WB_nop)
        {
            cout<<"WB instruction: nop"<<endl;
        }
        else
        {
            if (WB_PC/4>=instr_statements.size())
            {
                cout<<"WB instruction: no instruction at this address"<<endl;
            }
            else
            {
                cout<<"WB instruction: "<<instr_statements[WB_PC/4]<<endl;
                get<5>(exec_table[WB_InstrNum]) = cycle_num;
            }
        }

        // cout<<IF_PC<<" "<<ID_PC<<" "<<Ex_PC<<" "<<mEm_PC<<endl;
        // cout<<"IF_ID_Register: "<<(*rv32).IF_ID_Reg.Data<<endl;
        // cout<<"IF_ID_Register PC: "<<(*rv32).IF_ID_Reg.Data.substr(0,32)<<endl;
        // cout<<"IF_ID_Register Instr: "<<(*rv32).IF_ID_Reg.Data.substr(32,32)<<endl;
        // cout<<"ID_EX_Register: "<<(*rv32).ID_EX_Reg.Data<<endl;
        // cout<<"ID_EX_Rd: "<<(*rv32).ID_EX_Reg.Data.substr(163-5,5)<<endl;
        // cout<<"ID_EX_Rs2: "<<(*rv32).ID_EX_Reg.Data.substr(163-5-5,5)<<endl;
        // cout<<"ID_EX_Rs1: "<<(*rv32).ID_EX_Reg.Data.substr(163-5-5-5,5)<<endl;
        // cout<<"EX_MEM_Register: "<<(*rv32).EX_MEM_Reg.Data<<endl;
        // cout<<"MEM_WB_Register: "<<(*rv32).MEM_WB_Reg.Data<<endl;

        WB_PC=Mem_PC;
        Mem_PC=Ex_PC;
        Ex_PC=ID_PC;
        // ID_PC=IF_PC;

        WB_InstrNum=Mem_InstrNum;
        Mem_InstrNum=Ex_InstrNum;
        Ex_InstrNum=ID_InstrNum;
        ID_InstrNum=IF_InstrNum;

        WB_nop=Mem_nop;
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

        cycle_num++;

        // for (int i = 0; i < exec_table.size(); i++)
        // {
        //     string temp = get<0>(exec_table[i]).substr(0, get<0>(exec_table[i]).size()-1);
        //     temp.resize(20, ' ');
        //     cout<<temp<<": ";
        //     for (int j = 0; j < cycles; j++)
        //     {
        //         if (get<1>(exec_table[i]) == j)
        //         {
        //             cout<<" IF ;";
        //         }
        //         else if (get<2>(exec_table[i]) == j)
        //         {
        //             cout<<" ID ;";
        //         }
        //         else if (get<3>(exec_table[i]) == j)
        //         {
        //             cout<<" EX ;";
        //         }
        //         else if (get<4>(exec_table[i]) == j)
        //         {
        //             cout<<" ME ;";
        //         }
        //         else if (get<5>(exec_table[i]) == j)
        //         {
        //             cout<<" WB ;";
        //         }
        //         else
        //         {
        //             cout<<"    ;";
        //         }
                
        //     }
        //     cout<<endl;
            
        // }
    }

    for (int i = 0; i < exec_table.size(); i++)
    {
        string temp = get<0>(exec_table[i]).substr(0, get<0>(exec_table[i]).size()-1);
        temp.resize(20, ' ');
        cout<<temp<<": ";
        for (int j = cycles_start; j < cycles; j++)
        {
            if (get<1>(exec_table[i]) == j)
            {
                cout<<" IF ;";
            }
            else if (get<2>(exec_table[i]) == j)
            {
                cout<<" ID ;";
            }
            else if (get<3>(exec_table[i]) == j)
            {
                cout<<" EX ;";
            }
            else if (get<4>(exec_table[i]) == j)
            {
                cout<<" ME ;";
            }
            else if (get<5>(exec_table[i]) == j)
            {
                cout<<" WB ;";
            }
            else
            {
                cout<<"    ;";
            }
            
        }
        cout<<endl;
        
    }
    
    delete rv32;
    return 0;

}

