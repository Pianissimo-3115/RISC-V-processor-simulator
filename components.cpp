#include<bits/stdc++.h>
#include<limits>
// #include<string>
// #include<bitset>
// #include<unordered_set>
// #include<set>
// #include<unordered_map>
// #include<assert.h>
// #include<exception>
// #include<tuple>
// #include<iostream>

using namespace std;

string unsignedToBitsString(unsigned num) 
{
    string bits = bitset<32>(num).to_string();
    return bits;
}

string intToBitsString(int num)
{
    string bits = bitset<32>(num).to_string();
    return bits;
}

unsigned bitsStringToUnsigned(string bits) 
{
    unsigned num = bitset<32>(bits).to_ulong();
    return num;
}

int bitsStringToInt(string bits)
{
    int num = bitset<32>(bits).to_ulong();
    return num;
}

class MUX2x1
{
    private:
        unordered_set<char*> OutputPorts;
    public:
        string Input1;
        string Input2;
        string InputSwitch;
        size_t dataSizeBits;

        MUX2x1(size_t dataSizeBitsInput) 
        {
            dataSizeBits = dataSizeBitsInput;
            Input1 = string(dataSizeBits, '0');
            Input2 = string(dataSizeBits, '0');
            InputSwitch = "0";
        }

        void Reset()
        {
            Input1 = string(dataSizeBits, '0');
            Input2 = string(dataSizeBits, '0');
            InputSwitch = "0";
        }

        void ConnectOutput(char* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            // assert(connection->size() == dataSizeBits); // Ensure connection is the correct size
            OutputPorts.insert(connection);
        }

        void Step()
        {
            for(auto port : OutputPorts)
            {
                assert(port != nullptr); // Ensure port is not null
                if (InputSwitch == "0")
                {
                    for (size_t i = 0; i < dataSizeBits; i++)
                    {
                        port[i] = Input1[i];
                    }
                }
                else
                {
                    for (size_t i = 0; i < dataSizeBits; i++)
                    {
                        port[i] = Input2[i];
                    }
                }
            }
        }

        ~MUX2x1()
        {
            OutputPorts.clear();
        }

    };

class MUX4x1 
{
    private:
        unordered_set<char*> OutputPorts;
    public:
        string Input1;
        string Input2;
        string Input3;
        string Input4;
        string InputSwitch;
        size_t dataSizeBits;

        MUX4x1(size_t dataSizeBitsInput) 
        {
            dataSizeBits = dataSizeBitsInput;
            Input1 = string(dataSizeBits, '0');
            Input2 = string(dataSizeBits, '0');
            Input3 = string(dataSizeBits, '0');
            Input4 = string(dataSizeBits, '0');
            InputSwitch = "00";
        }

        void Reset()
        {
            Input1 = string(dataSizeBits, '0');
            Input2 = string(dataSizeBits, '0');
            Input3 = string(dataSizeBits, '0');
            Input4 = string(dataSizeBits, '0');
            InputSwitch = "00";
        }

        void ConnectOutput(char* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            // assert(connection->size() == dataSizeBits); // Ensure connection is the correct size
            OutputPorts.insert(connection);
        }

        void Step()
        {
            // assert(InputSwitch.size() == 2); // Ensure InputSwitch is 2 bits
            string chosen;
            if (InputSwitch == "00") chosen = Input1;
            else if (InputSwitch == "01") chosen = Input2;
            else if (InputSwitch == "10") chosen = Input3;
            else if (InputSwitch == "11") chosen = Input4;
            else throw invalid_argument("Invalid InputSwitch value");
            for(auto port : OutputPorts)
            {
                assert(port != nullptr); // Ensure port is not null
                for (size_t i = 0; i < dataSizeBits; i++)
                {
                    port[i] = chosen[i];
                }
            }
        }

        ~MUX4x1()
        {
            OutputPorts.clear();
        }
};

class Register
{
    private:
        set<tuple<int, int, char*>> OutputPorts;
        size_t Reg_size;
    public:
        string Data;
        string FlushBit;
        string StallBit;

        Register(size_t reg_size)
        {
            Reg_size = reg_size;
            Data = string(Reg_size, '0');
            FlushBit = "0"; 
            StallBit = "0"; 
        }

    void Reset()
    {
        Data = string(Reg_size, '0');
        FlushBit = "0"; 
        StallBit = "0"; 
    }

    void ConnectOutput(unsigned start, unsigned end, char* connection)
    {
        if (start<0 || end<start || end>=Reg_size)
        {
            throw invalid_argument("Invalid register start and/or end values");
        }
        assert(connection != nullptr);
        OutputPorts.insert( tuple(start, end, connection));
    }

    void Step()
    {
        if (FlushBit == "1" && StallBit == "0")
        {
            //Data = string(Reg_size, '0');
            for (auto port: OutputPorts)
            {
                char* out_pointer = get<2>(port);
                int start = get<0>(port);
                int end = get<1>(port);

                for (int i = 0; i <= end-start; i++)
                {
                    out_pointer[i]= '0';
                }
                
            }
            return;
        }

        if (StallBit == "0")
        {
            for (auto port: OutputPorts)
            {
                char* out_pointer = get<2>(port);
                int start = get<0>(port);
                int end = get<1>(port);

                for (int i = start; i <= end; i++)
                {
                    out_pointer[i-start] = Data[i];
                }
                
            }
        }
        
    }
};

class HazardDetectionUnitNoFwd
{
    private:
        unordered_set<char*> OutputPorts;
    public:
        string ID_EX_RegWrite;
        string EX_MEM_RegWrite;
        string ID_EX_RegisterRd;
        string IF_ID_RegisterRs1;
        string IF_ID_RegisterRs2;
        string EX_MEM_RegisterRd;

        HazardDetectionUnitNoFwd()
        {
            ID_EX_RegWrite = "0";
            EX_MEM_RegWrite = "0";
            ID_EX_RegisterRd = string(5, '0');
            IF_ID_RegisterRs1 = string(5, '0');
            IF_ID_RegisterRs2 = string(5, '0');
            EX_MEM_RegisterRd = string(5, '0');
        }

        void Reset()
        {
            ID_EX_RegWrite = "0";
            EX_MEM_RegWrite = "0";
            ID_EX_RegisterRd = string(5, '0');
            IF_ID_RegisterRs1 = string(5, '0');
            IF_ID_RegisterRs2 = string(5, '0');
            EX_MEM_RegisterRd = string(5, '0');
        }

        void ConnectOutput(char* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            OutputPorts.insert(connection);
        }

        void Step()
        {
            for (auto port : OutputPorts)
            {
                assert(port != nullptr); // Ensure port is not null
                port[0] = '0';
            }
            if
            (
                (
                    ID_EX_RegWrite == "1"
                    && 
                    (
                        ID_EX_RegisterRd == IF_ID_RegisterRs1 
                        || ID_EX_RegisterRd == IF_ID_RegisterRs2
                    )
                )
                ||
                (
                    EX_MEM_RegWrite == "1"
                    &&
                    (
                        EX_MEM_RegisterRd == IF_ID_RegisterRs1
                        || EX_MEM_RegisterRd == IF_ID_RegisterRs2
                    )
                )               // EK BAAR RECHECK CONDITIONS OF THE if statement
            )
            {
                for(auto port : OutputPorts)
                {
                    assert(port != nullptr); // Ensure port is not null
                    port[0] = '1';
                }
            }
        }

        ~HazardDetectionUnitNoFwd()
        {
            OutputPorts.clear();
        }
};

class HazardDetectionUnit
{
    private:
        unordered_set<char*> OutputPorts;
    public:
        string ID_EX_Memread;
        string EX_MEM_Memread;
        string ID_EX_RegWrite;
        string ID_EX_RegisterRd;
        string IF_ID_RegisterRs1;
        string IF_ID_RegisterRs2;
        string EX_MEM_RegisterRd;
        string opcode;

        HazardDetectionUnit()
        {
            ID_EX_Memread = "0";
            EX_MEM_Memread = "0";
            ID_EX_RegisterRd = string(5, '0');
            IF_ID_RegisterRs1 = string(5, '0');
            IF_ID_RegisterRs2 = string(5, '0');
            EX_MEM_RegisterRd = string(5, '0');
            ID_EX_RegWrite = "0";
            opcode = "0000000";
        }

        void Reset()
        {
            ID_EX_Memread = "0";
            EX_MEM_Memread = "0";
            ID_EX_RegisterRd = string(5, '0');
            IF_ID_RegisterRs1 = string(5, '0');
            IF_ID_RegisterRs2 = string(5, '0');
            EX_MEM_RegisterRd = string(5, '0');
            ID_EX_RegWrite = "0";
            opcode = "0000000";
        }

        void ConnectOutput(char* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            OutputPorts.insert(connection);
        }

        void Step()
        {
            unsigned OPCODE = bitsStringToUnsigned(opcode);
            for (auto port : OutputPorts)
            {
                assert(port != nullptr); // Ensure port is not null
                port[0] = '0';
            }
            if
            (
                (
                    ID_EX_Memread == "1"            // R type instructions
                    && 
                    (
                        ID_EX_RegisterRd == IF_ID_RegisterRs1 
                        || ID_EX_RegisterRd == IF_ID_RegisterRs2
                    )
                )
                ||
                (
                    EX_MEM_Memread == "1"
                    &&
                    (
                        EX_MEM_RegisterRd == IF_ID_RegisterRs1
                        || EX_MEM_RegisterRd == IF_ID_RegisterRs2
                    )
                    && OPCODE == 0b1100011          // BRANCH instruction with load instruction 2 lines above it
                )
                ||
                (
                    ID_EX_RegWrite == "1"
                    &&
                    (
                        ID_EX_RegisterRd == IF_ID_RegisterRs1
                        || ID_EX_RegisterRd == IF_ID_RegisterRs2
                    )
                    && OPCODE == 0b1100011          // BRANCH instruction with R type instruction above it
                )
            )
            {
                for(auto port : OutputPorts)
                {
                    assert(port != nullptr); // Ensure port is not null
                    port[0] = '1';
                }
            }
        }

        ~HazardDetectionUnit()
        {
            OutputPorts.clear();
        }

};

class ALUx32
{
    private:
        unordered_set<char*> OutputPorts;
        // unordered_set<char*> ZeroPorts;
    public:
        string Input1;
        string Input2;
        string ALUControl;

        ALUx32() 
        {
            Input1 = string(32, '0');
            Input2 = string(32, '0');
            ALUControl = "00000";
        }

        void Reset()
        {
            Input1 = string(32, '0');
            Input2 = string(32, '0');
            ALUControl = "00000";
        }

        void ConnectResult(char* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            // assert(connection->size() == 32); // Ensure connection is the correct size
            OutputPorts.insert(connection);
        }

        // void ConnectZero(char* connection)
        // {
        //     assert(connection != nullptr); // Ensure connection is not null
        //     ZeroPorts.insert(connection);
        // }

        void Step()
        {
            string result;
            if (ALUControl == "xxxxx")
            {
                result = unsignedToBitsString(UINT_MAX);
                for(auto port : OutputPorts)
                {
                    assert(port != nullptr); // Ensure port is not null
                    for (size_t i = 0; i < 32; i++)
                    {
                        port[i] = result[i];
                    }
                }
                return;
            }
            
            unsigned inp1 = bitsStringToUnsigned(Input1);
            unsigned inp2 = bitsStringToUnsigned(Input2);
            unsigned ALUC = bitsStringToUnsigned(ALUControl);
            string zero;
            switch(ALUC)
            {
                case 0:         // AND
                {
                    unsigned res = inp1 & inp2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? "1" : "0";
                    break;
                }
                case 1:         // OR
                {
                    unsigned res = inp1 | inp2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? "1" : "0";
                    break;
                }
                case 2:         // add
                {
                    unsigned res = inp1 + inp2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? "1" : "0";
                    break;
                }
                case 3:         // XOR
                {
                    unsigned res = inp1 ^ inp2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? "1" : "0";
                    break;
                }
                case 4:         // slt
                {
                    zero = (((int)inp1 < (int)inp2) ? "1" : "0");
                    break;
                }
                case 5:         // sltu
                {
                    zero = ((inp1 < inp2) ? "1" : "0");
                    break;
                }
                case 6:         // Sub
                {
                    unsigned res = inp1 - inp2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? "1" : "0";
                    break;
                }
                case 8:         // mul
                {
                    long long int res = (long long int)inp1 * (long long int)inp2;
                    res&=0x00000000FFFFFFFF;
                    result = intToBitsString((int)res);
                    zero = (res == 0) ? "1" : "0";
                    break;
                }
                case 9:         // mulh
                {
                    long long int res = (long long int)inp1 * (long long int)inp2;
                    res >>= 32;
                    result = intToBitsString(res);
                    zero = (res == 0) ? "1" : "0";
                    break;
                }
                case 11:        // mulhu
                {
                    long long unsigned res = (long long unsigned)inp1 * (long long unsigned)inp2;
                    res&=0x00000000FFFFFFFF;
                    result = unsignedToBitsString((unsigned)res);
                    zero = (res == 0) ? "1" : "0";
                    break;
                }
                case 13:        // sll
                {
                    unsigned res = inp1 << inp2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? "1" : "0";
                    break;
                }
                case 14:        // srl
                {
                    unsigned res = inp1 >> inp2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? "1" : "0";
                    break;
                }
                case 15:        // sra
                {
                    unsigned res = (int)inp1 >> inp2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? "1" : "0";
                    break;
                }
                case 24:        // div
                {
                    if(inp2 == 0)
                    {
                        result = string(32,'1');
                        zero = "0";
                        break;
                    }
                    unsigned res = (int)inp1 / (int)inp2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? "1" : "0";
                    break;
                }
                case 25:        // divu
                {
                    if(inp2 == 0)
                    {
                        result = string(32,'1');
                        zero = "0";
                        break;
                    }
                    unsigned res = inp1 / inp2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? "1" : "0";
                    break;
                }
                case 26:        // rem
                {
                    if(inp2 == 0)
                    {
                        result = intToBitsString((int)inp1);
                        zero = "0";
                        break;
                    }
                    int res = (int)inp1 % (int)inp2;
                    result = intToBitsString(res);
                    zero = (res == 0) ? "1" : "0";
                    break;
                }
                case 27:        // remu
                {
                    if(inp2 == 0)
                    {
                        result = unsignedToBitsString(inp1);
                        zero = "0";
                        break;
                    }
                    unsigned res = inp1 % inp2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? "1" : "0";
                    break;
                }
                case UINT_MAX:
                    result = string(32,'x');
                default:
                {
                    throw invalid_argument("Invalid ALUControl value");
                    break;
                }
            }
            
            for(auto port : OutputPorts)
            {
                assert(port != nullptr); // Ensure port is not null
                for (size_t i = 0; i < 32; i++)
                {
                    port[i] = result[i];
                }
            }
            // for(auto port : ZeroPorts)
            // {
            //     assert(port != nullptr); // Ensure port is not null

                // port[i] = zero[i];

            // }
        }
    ~ALUx32()
    {
        OutputPorts.clear();
        // ZeroPorts.clear();
    }
};

class RegisterMemx32 
{
    private:
        unordered_set<char*> Data1OutputPorts;
        unordered_set<char*> Data2OutputPorts;
        
        public:
        string Registers[32];
        string ReadReg1;
        string ReadReg2;
        string WriteReg;
        string WriteData;
        string WriteEnable;

        RegisterMemx32() 
        {
            ReadReg1 = string(5, '0');
            ReadReg2 = string(5, '0');
            WriteReg = string(5, '0');
            WriteData = string(32, '0');
            WriteEnable = '0';
            for(int i = 0; i < 32; i++)
            {
                if (i == 2)
                {
                    Registers[i] = string(32, '1');
                }
                Registers[i] = string(32, '0');
            }
        }

        void Reset()
        {
            ReadReg1 = string(5, '0');
            ReadReg2 = string(5, '0');
            WriteReg = string(5, '0');
            WriteData = string(32, '0');
            WriteEnable = "0";
            for(int i = 0; i < 32; i++)
            {
                Registers[i] = string(32, '0');
            }
        }

        void ConnectData1Output(char* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            // assert(connection->size() == 32); // Ensure connection is the correct size
            Data1OutputPorts.insert(connection);
        }

        void ConnectData2Output(char* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            // assert(connection->size() == 32); // Ensure connection is the correct size
            Data2OutputPorts.insert(connection);
        }

        void StepRead()
        {
            // assert(ReadReg1.size() == 5); // Ensure ReadReg1 is 5 bits
            // assert(ReadReg2.size() == 5); // Ensure ReadReg2 is 5 bits
            unsigned address1 = bitsStringToUnsigned(ReadReg1);
            unsigned address2 = bitsStringToUnsigned(ReadReg2);
            for(auto port : Data1OutputPorts)
            {
                assert(port != nullptr); // Ensure port is not null
                for (size_t i = 0; i < 32; i++)
                {
                    port[i] = Registers[address1][i];
                }
            }
            for(auto port : Data2OutputPorts)
            {
                assert(port != nullptr); // Ensure port is not null
                for (size_t i = 0; i < 32; i++)
                {
                    port[i] = Registers[address2][i];
                }
            }
        }
        
        void StepWrite()
        {
            if(WriteEnable == "0") return;
            // assert(WriteReg.size() == 5); // Ensure WriteReg is 5 bits
            unsigned address = bitsStringToUnsigned(WriteReg);
            if(address == 0) return;        // HARD CODE x0 to 0
            Registers[address] = WriteData;
        }
        ~RegisterMemx32()
        {
            Data1OutputPorts.clear();
            Data2OutputPorts.clear();
        }
};

class InstructionMemx32 
{
    private:
        unordered_set<char*> OutputPorts;
        vector<uint32_t> Instructions;
        unsigned InstrCount = 0;

    public:
        string PC;
        InstructionMemx32() 
        {
            this->Instructions=vector<uint32_t>(0);
            this->InstrCount = 0;
            PC = string(32, '0');
        }

        void Reset()
        {
            this->Instructions.clear();
            this->InstrCount = 0;
            PC = string(32, '0');
        }

        void ConnectOutput(char* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            // assert(connection->size() == 32); // Ensure connection is the correct size
            OutputPorts.insert(connection);
        }

        void AddInstruction(uint32_t instr)
        {
            Instructions.push_back(instr);
            InstrCount++;
        }

        void Step()
        {
            unsigned newpc = bitsStringToUnsigned(PC);
            assert(newpc%4 == 0); // Ensure newpc is a multiple of 4
            // assert(newpc/4 < InstrCount); // Ensure newpc is within range
            if (newpc/4 >= InstrCount)
            {
                for(auto port : OutputPorts)
                {
                    assert(port != nullptr); // Ensure port is not null
                    for (size_t i = 0; i < 32; i++)
                    {
                        port[i] = '0';
                    }
                    // *port = curr_instruction;
                }
                return;
            }
            
            string curr_instruction = unsignedToBitsString(Instructions[newpc/4]);
            for(auto port : OutputPorts)
            {
                assert(port != nullptr); // Ensure port is not null
                for (size_t i = 0; i < 32; i++)
                {
                    port[i] = curr_instruction[i];
                }
                
                // *port = curr_instruction;
            }
        }

        ~InstructionMemx32()
        {
            OutputPorts.clear();
        }               
};

class ImmediateGen 
{
    private:
        unordered_set<char*> OutputPorts;
    public:
        string Instruction;

        ImmediateGen() 
        {
            Instruction = string(32, '0');
        }

        void Reset()
        {
            Instruction = string(32, '0');
        }

        void ConnectOutput(char* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            // assert(connection->size() == 32); // Ensure connection is the correct size
            OutputPorts.insert(connection);
        }

        void Step()
        {
            string immediate = string(32, '0');
            if (Instruction.substr(25, 7) == "0110011" || Instruction.substr(25, 7) == "0111011") //Rtype
            {
                immediate = string(32, '0');
            }
            else if (Instruction.substr(25, 7) == "0000011" || Instruction.substr(25, 7) == "0010011" || Instruction.substr(25, 7) == "0011011" || Instruction.substr(25, 7) == "1100111" || Instruction.substr(25, 7) == "1110011") //I-type
            {
                for (int i = 0; i < 21; i++)
                {
                    immediate[i] = Instruction[0];
                }
                for (int i = 21; i < 27; i++)
                {
                    immediate[i] = Instruction[i-20];
                }
                for (int i = 27; i < 31; i++)
                {
                    immediate[i] = Instruction[i-20];
                }
                immediate[31] = Instruction[11];
            }
            else if (Instruction.substr(25, 7) == "0100011") //S type
            {
                for (int i = 0; i < 21; i++)
                {
                    immediate[i] = Instruction[0];
                }
                for (int i = 21; i < 27; i++)
                {
                    immediate[i] = Instruction[i-20];
                }
                for (int i = 27; i < 31; i++)
                {
                    immediate[i] = Instruction[i-6];
                }
                immediate[31] = Instruction[25];
            }
            else if (Instruction.substr(25, 7) == "1100011") //SB type Hopefully ab sahi hai
            {
                for (int i = 0; i < 20; i++)
                {
                    immediate[i] = Instruction[0];
                }
                for (int i = 21; i < 27; i++)
                {
                    immediate[i] = Instruction[i-20];
                }
                for (int i = 27; i < 31; i++)
                {
                    immediate[i] = Instruction[i-7];
                }
                immediate[20] = Instruction[24];
                immediate[31] = '0';
            }
            else if (Instruction.substr(25, 7) == "0010111" || Instruction.substr(25, 7) == "0110111") //Utype
            {
                for (int i = 0; i < 20; i++)
                {
                    immediate[i] = Instruction[i];
                }
                for (int i = 20; i < 32; i++)
                {
                    immediate[i] = '0';
                }
            }
            else if (Instruction.substr(25, 7) == "1101111") //UJ type
            {
                for (int i = 0; i < 12; i++)
                {
                    immediate[i] = Instruction[0];
                }
                for (int i = 12; i < 20; i++)
                {
                    immediate[i] = Instruction[i];
                }
                immediate[20] = Instruction[11];
                for (int i = 21; i < 31; i++)
                {
                    immediate[i] = Instruction[i-20];
                }
                immediate[31] = '0';
            }
            for(auto port : OutputPorts)
            {
                assert(port != nullptr); // Ensure port is not null
                for (size_t i = 0; i < 32; i++)
                {
                    port[i] = immediate[i];
                }
            }
        }

        ~ImmediateGen()
        {
            OutputPorts.clear();
        }
};

class ControlUnit           
{
    private:
        char* IF_Flush;

        char* RegWrite;
        char* MemToReg;

        char* MemRead;
        char* MemWrite;

        char* ALUSrc1;      // ALUSrc1 = "00" => Register, "01" => PC, "10" => 0
        char* ALUSrc2;      // ALUSrc2 = "00" => Register, "01" => Immediate, "10" => 4
        char* ALUOp;

        unordered_set<char*> IsBranch;

        char* IsJalr;

        char* Jump; 
    public:
        string opcode;
        string PC;
        ControlUnit()
        {
            opcode = string(7, '0');
            PC = string(32, '0');
        }

        void Reset()
        {
            opcode = string(7, '0');
            PC = string(32, '0');
        }

        void ConnectIFFlush(char* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            IF_Flush = connection;
        }

        void ConnectWriteBack(char* RegWriteConnection, char* MemToRegConnection)
        {
            assert(RegWriteConnection != nullptr); // Ensure connection is not null
            assert(MemToRegConnection != nullptr); // Ensure connection is not null
            RegWrite = RegWriteConnection;
            MemToReg = MemToRegConnection;
        }

        void ConnectMemory(char* MemReadConnection, char* MemWriteConnection)
        {
            assert(MemReadConnection != nullptr); // Ensure connection is not null
            assert(MemWriteConnection != nullptr); // Ensure connection is not null
            MemRead = MemReadConnection;
            MemWrite = MemWriteConnection;
        }



        void ConnectALU(char* ALUSrc1Connection, char* ALUSrc2Connection, char* ALUOpConnection)
        {
            assert(ALUSrc1Connection != nullptr); // Ensure connection is not null
            assert(ALUSrc2Connection != nullptr); // Ensure connection is not null
            assert(ALUOpConnection != nullptr); // Ensure connection is not null
            // assert(ALUOp->size() == 2); // Ensure ALUOp is 2 bits
            // assert(ALUSrc1->size() == 2);
            // assert(ALUSrc2->size() == 2); // Ensure ALUSrc2 is 2 bits
            ALUSrc1 = ALUSrc1Connection;
            ALUSrc2 = ALUSrc2Connection;
            ALUOp = ALUOpConnection;
        }

        void ConnectIsBranch(char* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            IsBranch.insert(connection);
        }

        void ConnectJalr(char* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            IsJalr = connection;
        }

        void ConnectJump(char* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            Jump = connection;
        }

        void Step()
        {
            unsigned OPCODE = bitsStringToUnsigned(opcode);
            char Branch = '0';
            switch(OPCODE)
            {
                case 0b0110011:         // R-type
                {
                    *IF_Flush = '0';
                    ALUSrc1[0] = '0';
                    ALUSrc1[1] = '0';
                    ALUSrc2[0] = '0';
                    ALUSrc2[1] = '0';
                    *MemToReg = '0';
                    *RegWrite = '1';
                    *MemRead = '0';
                    *MemWrite = '0';
                    Branch = '0';
                    *IsJalr = '0';
                    *Jump = '0';
                    ALUOp[0] = '1';
                    ALUOp[1] = '0';
                }
                break;

                case 0b0010011:         // I-type
                {
                    *IF_Flush = '0';
                    ALUSrc1[0] = '0';
                    ALUSrc1[1] = '0';
                    ALUSrc2[0] = '0';
                    ALUSrc2[1] = '1';
                    *MemToReg = '0';
                    *RegWrite = '1';
                    *MemRead = '0';
                    *MemWrite = '0';
                    Branch = '0';
                    *IsJalr = '0';
                    *Jump = '0';
                    ALUOp[0] = '1';
                    ALUOp[1] = '1';
                }
                break;

                case 0b0000011:         // Load
                {
                    *IF_Flush = '0';
                    ALUSrc1[0] = '0';
                    ALUSrc1[1] = '0';
                    ALUSrc2[0] = '0';
                    ALUSrc2[1] = '1';
                    *MemToReg = '1';
                    *RegWrite = '1';
                    *MemRead = '1';
                    *MemWrite = '0';
                    Branch = '0';
                    *IsJalr = '0';
                    *Jump = '0';
                    ALUOp[0] = '0';
                    ALUOp[1] = '0';
                }
                break;

                case 0b0100011:         // Store
                {
                    *IF_Flush = '0';
                    ALUSrc1[0] = '0';
                    ALUSrc1[1] = '0';
                    ALUSrc2[0] = '0';
                    ALUSrc2[1] = '1';
                    *MemToReg = '0';
                    *RegWrite = '0';
                    *MemRead = '0';
                    *MemWrite = '1';
                    Branch = '0';
                    *IsJalr = '0';
                    *Jump = '0';
                    ALUOp[0] = '0';
                    ALUOp[1] = '0';
                }
                break;

                case 0b1100111:         // JALR
                {
                    *IF_Flush = '1';
                    ALUSrc1[0] = '0';
                    ALUSrc1[1] = '1';
                    ALUSrc2[0] = '1';
                    ALUSrc2[1] = '0';
                    *MemToReg = '0';
                    *RegWrite = '1';
                    *MemRead = '0';
                    *MemWrite = '0';
                    Branch = '0';
                    *IsJalr = '1';
                    *Jump = '1';
                    ALUOp[0] = '0';
                    ALUOp[1] = '0';
                }
                break;

                case 0b1100011:         // Branch
                {
                    *IF_Flush = '0';    // Always predict that branch won't be taken
                    ALUSrc1[0] = '0';
                    ALUSrc1[1] = '0';
                    ALUSrc2[0] = '0';
                    ALUSrc2[1] = '0';
                    *MemToReg = '0';
                    *RegWrite = '0';
                    *MemRead = '0';
                    *MemWrite = '0';
                    Branch = '1';
                    *IsJalr = '0';
                    *Jump = '1';
                    ALUOp[0] = 'x';
                    ALUOp[1] = 'x';
                }
                break;

                case 0b0110111:         // LUI
                {
                    *IF_Flush = '0';
                    ALUSrc1[0] = '1';
                    ALUSrc1[1] = '0';
                    ALUSrc2[0] = '0';
                    ALUSrc2[1] = '1';
                    *MemToReg = '0';
                    *RegWrite = '1';
                    *MemRead = '0';
                    *MemWrite = '0';
                    Branch = '0';
                    *IsJalr = '0';
                    *Jump = '0';
                    ALUOp[0] = '0';
                    ALUOp[1] = '0';
                }
                break;
                case 0b0010111:         // AUIPC
                {
                    *IF_Flush = '0';
                    ALUSrc1[0] = '0';
                    ALUSrc1[1] = '1';
                    ALUSrc2[0] = '0';
                    ALUSrc2[1] = '1';
                    *MemToReg = '0';
                    *RegWrite = '1';
                    *MemRead = '0';
                    *MemWrite = '0';
                    Branch = '0';
                    *IsJalr = '0';
                    *Jump = '0';
                    ALUOp[0] = '0';
                    ALUOp[1] = '0';
                }
                break;

                case 0b1101111:         // JAL
                {
                    *IF_Flush = '1';
                    ALUSrc1[0] = '0';
                    ALUSrc1[1] = '1';
                    ALUSrc2[0] = '1';
                    ALUSrc2[1] = '0';
                    *MemToReg = '0';
                    *RegWrite = '1';
                    *MemRead = '0';
                    *MemWrite = '0';
                    Branch = '0';
                    *IsJalr = '0';
                    *Jump = '1';
                    ALUOp[0] = '0';
                    ALUOp[1] = '0';
                }
                break;
                case 0b0000000: // No op
                {
                    *IF_Flush = '0';
                    ALUSrc1[0] = '0';
                    ALUSrc1[1] = '0';
                    ALUSrc2[0] = '0';
                    ALUSrc2[1] = '0';
                    *MemToReg = '0';
                    *RegWrite = '0';
                    *MemRead = '0';
                    *MemWrite = '0';
                    Branch = '0';
                    *IsJalr = '0';
                    *Jump = '0';
                    ALUOp[0] = '0';
                    ALUOp[1] = '0';
                }
                break;
                default:
                {
                    cout<<endl<<";sldfjas;lkfjs"<<endl<<opcode<<endl<<endl<<endl;
                    throw invalid_argument("Invalid opcode value");
                }
            }
            for (auto port : IsBranch)
            {
                *port = Branch;
            }
        }
};

class ALUControlUnit
{
    private:
        char* OutputPort;

    public:
        string ALUOp;
        string Func3;
        string Func7;      // only 2 bits

        ALUControlUnit()
        {
            Func3 = "000";
            Func7 = "0000000";
            ALUOp = "00";
        }

        void Reset()
        {
            Func3 = "000";
            Func7 = "0000000";
            ALUOp = "00";
        }

        void ConnectOutput(char* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            OutputPort = connection;
        }

        void Step()
        {
            unsigned FUNC3 = bitsStringToUnsigned(Func3);
            unsigned FUNC7 = bitsStringToUnsigned(Func7);
            unsigned result = 0;
            
            
                if (ALUOp == "10")      // ARITHMETIC OPERATION
                {
                    if (FUNC3 == 0)
                    {
                        if (FUNC7 == 0x00)
                        {
                            result = 2;     // ADD
                        }
                        else if (FUNC7 == 0x01)
                        {
                            result = 8;     // MUL
                        }
                        else if (FUNC7 == 0x20)
                        {
                            result = 6;     // SUB
                        }
                        else 
                        {
                            throw invalid_argument("Invalid FUNC7 value");
                        }
                    }
                    else if (FUNC3 == 1)
                    {
                        if (FUNC7 == 0x00)
                        {
                            result = 13;    // sll
                        }
                        else if (FUNC7 == 0x01)
                        {
                            result = 9;     // MULH
                        }
                        else 
                        {
                            throw invalid_argument("Invalid FUNC7 value");
                        }
                    }
                    else if (FUNC3 == 2)
                    {
                        if (FUNC7 == 0x00)
                        {
                            result = 4;     // slt
                        }
                        else 
                        {
                            throw invalid_argument("Invalid FUNC7 value");
                        }
                    }
                    else if (FUNC3 == 3)
                    {
                        if (FUNC7 == 0x00)
                        {
                            result = 5;     // sltu
                        }
                        if (FUNC7 == 0x01)
                        {
                            result = 11;    // MULHU
                        }
                        else 
                        {
                            throw invalid_argument("Invalid FUNC7 value");
                        }
                    }
                    else if (FUNC3 == 4)
                    {
                        if (FUNC7 == 0x00)
                        {
                            result = 3;     // XOR
                        }
                        else if (FUNC7 == 0x01)
                        {
                            result = 24;    // DIV
                        }
                        else 
                        {
                            throw invalid_argument("Invalid FUNC7 value");
                        }
                    }
                    else if (FUNC3 == 5)
                    {
                        if (FUNC7 == 0x00)
                        {
                            result = 14;    // srl
                        }
                        else if (FUNC7 == 0x01)
                        {
                            result = 25;    // DIVU
                        }
                        else if (FUNC7 == 0x20)
                        {
                            result = 15;    // sra
                        }
                        else 
                        {
                            throw invalid_argument("Invalid FUNC7 value");
                        }
                        
                    }
                    else if (FUNC3 == 6)
                    {
                        if (FUNC7 == 0x00)
                        {
                            result = 1;     // OR
                        }
                        else if (FUNC7 == 0x01)
                        {
                            result = 26;    // REM
                        }
                        else 
                        {
                            throw invalid_argument("Invalid FUNC7 value");
                        }
                    }
                    else if (FUNC3 == 7)
                    {
                        if (FUNC7 == 0x00)
                        {
                            result = 0;     // AND
                        }
                        else if (FUNC7 == 0x01)
                        {
                            result = 27;    // REMU
                        }
                        else 
                        {
                            throw invalid_argument("Invalid FUNC7 value");
                        }
                    }
                    
                }
                
                else if (ALUOp == "11")    // ARITHMETIC IMMEDIATE OPERATION
                {
                    if (FUNC3 == 0x0)
                    {
                        result = 2;     // ADDI
                    }
                    else if (FUNC3 == 0x4)
                    {
                        result = 3;     // XORI
                    }
                    else if (FUNC3 == 0x6)
                    {
                        result = 1;     // ORI
                    }
                    else if (FUNC3 == 0x7)
                    {
                        result = 0;     // ANDI
                    }
                    else if (FUNC3 == 0x1)
                    {
                        result = 13;    // SLLI
                    }
                    else if (FUNC3 == 0x5)
                    {
                        if (FUNC7 == 0x00)  // ALTHOUGH THERE DOES NOT EXIST FUNC7 FOR I-TYPE INSTRUCTIONS, I NEEDED SOMETHING TO PASS 2 BITS OF IMMEDIATE. SO I USED FUNC7 FOR THAT
                        {
                            result = 14;    // SRLI
                        }
                        else if (FUNC7 == 0x20)
                        {
                            result = 15;    // SRAI
                        }
                        else
                        {
                            throw invalid_argument("Invalid FUNC7 value");
                        }
                    }
                    else if (FUNC3 == 0x2)
                    {
                        result = 4;     // SLTI
                    }
                    else if (FUNC3 == 0x3)
                    {
                        result = 5;     // SLTIU
                    }
                    else
                    {
                        throw invalid_argument("Invalid Func3 value");
                    }
                }

                else if (ALUOp == "00")     // ALUOP "00" is load, store, LUI, AUIPC, 
                {
                    result = 2;
                }
                else if(ALUOp == "xx")
                {
                    result = UINT_MAX;
                }
                else
                {
                    throw invalid_argument("Invalid opcode value in ALUCtrlUnit");
                }
            string res_string = unsignedToBitsString(result).substr(32-5);

            if (result == UINT_MAX)
            {
                for (size_t i = 0; i < 5; i++)
                {
                    OutputPort[i] = 'x';
                }   
                return;
            }

            for (size_t i = 0; i < 5; i++)
            {
                OutputPort[i] = res_string[i];
            }
        }

        ~ALUControlUnit()
        {
            OutputPort = nullptr;
        }
};

class ALUForwardingUnit
{
    private:
        char* CtrlMUX3{};
        char* CtrlMUX4{};

    public:
        string Reg1Addr{};
        string Reg2Addr{};
        string EX_MEM_RegisterRDAddr{};
        string MEM_WB_RegisterRDAddr{};
        
        string EX_MEM_RegWrite{};
        string MEM_WB_RegWrite{};
        string branch{};

        ALUForwardingUnit() 
        {
            Reg1Addr = string(5, '0');
            Reg2Addr = string(5, '0');

            EX_MEM_RegisterRDAddr = string(5, '0');
            MEM_WB_RegisterRDAddr = string(5, '0');
            EX_MEM_RegWrite = "0";
            MEM_WB_RegWrite = "0";
            branch = "0";
        }

        void Reset()
        {
            Reg1Addr = string(5, '0');
            Reg2Addr = string(5, '0');
            EX_MEM_RegisterRDAddr = string(5, '0');
            MEM_WB_RegisterRDAddr = string(5, '0');
            EX_MEM_RegWrite = "0";
            MEM_WB_RegWrite = "0";
            branch = "0";
        }

        void ConnectCtrlMUX3(char* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            CtrlMUX3 = connection;
        }

        void ConnectCtrlMUX4(char* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            CtrlMUX4 = connection;
        }

        void Step()
        {
            assert(CtrlMUX3 != nullptr);                    // ARJUN CONFIRM THIS////////////////////////////////////////////////////////////
            assert(CtrlMUX4 != nullptr);                    // ARJUN CONFIRM THIS////////////////////////////////////////////////////////////
            CtrlMUX3[0] = '0';
            CtrlMUX3[1] = '0';                               // ARJUN CONFIRM THIS////////////////////////////////////////////////////////////        
            CtrlMUX4[0] = '0';
            CtrlMUX4[1] = '0';                                 // ARJUN CONFIRM THIS////////////////////////////////////////////////////////////        
            if (branch == "1") return;                      // ARJUN CONFIRM THIS////////////////////////////////////////////////////////////
            string ctrlmux3_out = "00";
            string ctrlmux4_out = "00";
            if 
            (
                MEM_WB_RegWrite[0] == '1' 
                && MEM_WB_RegisterRDAddr != "00000" 
                && !(   
                        EX_MEM_RegWrite[0] == '1' 
                        && EX_MEM_RegisterRDAddr != "00000" 
                        && Reg1Addr == EX_MEM_RegisterRDAddr
                    ) 
                && Reg1Addr == MEM_WB_RegisterRDAddr
            )
            {
                ctrlmux3_out = "01";
            }

            if 
            (
                MEM_WB_RegWrite[0] == '1' 
                && MEM_WB_RegisterRDAddr != "00000" 
                && !(   
                        EX_MEM_RegWrite[0] == '1' 
                        && EX_MEM_RegisterRDAddr != "00000" 
                        && Reg2Addr == EX_MEM_RegisterRDAddr
                    ) 
                && Reg2Addr == MEM_WB_RegisterRDAddr
            )
            {
                ctrlmux4_out = "01";
            }

            if 
            (
                EX_MEM_RegWrite[0] == '1' 
                && EX_MEM_RegisterRDAddr != "00000" 
                && Reg1Addr == EX_MEM_RegisterRDAddr
            )
            {
                ctrlmux3_out="10";
            }

            if 
            (
                EX_MEM_RegWrite[0]=='1' 
                && EX_MEM_RegisterRDAddr!="00000" 
                && Reg2Addr==EX_MEM_RegisterRDAddr
            )
            {
                ctrlmux4_out="10";
            }
            
            CtrlMUX3[0] = ctrlmux3_out[0];
            CtrlMUX3[1] = ctrlmux3_out[1]; 
            CtrlMUX4[0] = ctrlmux4_out[0];
            CtrlMUX4[1] = ctrlmux4_out[1]; 
        }

        ~ALUForwardingUnit()
        {
            CtrlMUX3 = nullptr;
            CtrlMUX4 = nullptr;
        }
};

class BranchForwardingUnit
{
    private:
        char* BranchCMPMux1;
        char* BranchCMPMux2;

    public:
        string IF_ID_RegisterRs1;
        string IF_ID_RegisterRs2;

        string EX_MEM_RegisterRDAddr;
        string MEM_WB_RegisterRDAddr;

        string EX_MEM_RegWrite;
        string EX_MEM_MemRead;
        string Branch;

        BranchForwardingUnit() 
        {
            IF_ID_RegisterRs1 = string(5, '0');
            IF_ID_RegisterRs2 = string(5, '0');
            EX_MEM_RegisterRDAddr = string(5, '0');
            MEM_WB_RegisterRDAddr = string(5, '0');
            EX_MEM_RegWrite = "0";
            EX_MEM_MemRead = "0";
            Branch = "0";
        }

        void Reset()
        {
            IF_ID_RegisterRs1 = string(5, '0');
            IF_ID_RegisterRs2 = string(5, '0');
            EX_MEM_RegisterRDAddr = string(5, '0');
            EX_MEM_RegWrite = "0";
            Branch = "0";
        }

        void ConnectBranchCMPMux1(char* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            BranchCMPMux1 = connection;
        }

        void ConnectBranchCMPMux2(char* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            BranchCMPMux2 = connection;
        }

        void Step()
        {
            *BranchCMPMux1 = '0';
            *BranchCMPMux2 = '0';
            if (Branch == "0") return;
            assert(BranchCMPMux1 != nullptr); 
            assert(BranchCMPMux2 != nullptr); 
            // assert(IF_ID_RegisterRs1.size() == 5); 
            // assert(IF_ID_RegisterRs2.size() == 5); 
            // assert(EX_MEM_RegisterRDAddr.size() == 5); 
            // assert(MEM_WB_RegisterRDAddr.size() == 5); 
            string branch_cmp_mux1_out = "0";
            string branch_cmp_mux2_out = "0";
            if
            (
                EX_MEM_RegWrite == "1"
                && EX_MEM_MemRead == "0"
                && EX_MEM_RegisterRDAddr == IF_ID_RegisterRs1
            )
            {
                branch_cmp_mux1_out = "1";
            }
            else branch_cmp_mux1_out = "0";

            if
            (
                EX_MEM_RegWrite == "1"
                && EX_MEM_MemRead == "0"
                && EX_MEM_RegisterRDAddr == IF_ID_RegisterRs2
            )
            {
                branch_cmp_mux2_out = "1";
            }
            else branch_cmp_mux2_out = "0";
            *BranchCMPMux1 = branch_cmp_mux1_out[0];
            *BranchCMPMux2 = branch_cmp_mux2_out[0];
        }

        ~BranchForwardingUnit()
        {
            BranchCMPMux1 = nullptr;
            BranchCMPMux2 = nullptr;
        }   
};

class BranchCmp
{
    private:
        char* OutputPort;
    public:
        string Value1;
        string Value2;
        string Func3;
        string branch;

        BranchCmp()
        {
            Value1 = string(32, '0');
            Value2 = string(32, '0');
            branch = "0";
            Func3 = "000";
        }

        void Reset()
        {
            Value1 = string(32, '0');
            Value2 = string(32, '0');
            branch = "0";
            Func3 = "000";
        }

        void ConnectOutput(char* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            OutputPort = connection;
        }

        void Step()
        {
            *OutputPort = '1';
            if (branch == "0") return;
            unsigned FUNC3 = bitsStringToUnsigned(Func3);
            // assert(Value1.size() == 32); // Ensure Value1 is 32 bits
            // assert(Value2.size() == 32); // Ensure Value2 is 32 bits
            
            switch (FUNC3)
            {
                case 0:         // BEQ
                {
                    if (Value1 == Value2)
                    {
                        *OutputPort = '1';
                    }
                    else *OutputPort = '0';
                }
                break;

                case 1:         // BNE
                {
                    if (Value1 != Value2)
                    {
                        *OutputPort = '1';
                    }
                    else *OutputPort = '0';
                }
                break;

                case 4:         // BLT
                {
                    if (bitsStringToInt(Value1) < bitsStringToInt(Value2))
                    {
                        *OutputPort = '1';
                    }
                    else *OutputPort = '0';
                }
                break;
                case 5:         // BGE
                {
                    if (bitsStringToInt(Value1) >= bitsStringToInt(Value2))
                    {
                        *OutputPort = '1';
                    }
                    else *OutputPort = '0';
                }
                break;
                case 6:         // BLTU
                {
                    if (bitsStringToUnsigned(Value1) < bitsStringToUnsigned(Value2))
                    {
                        *OutputPort = '1';
                    }
                    else *OutputPort = '0';
                }
                break;
                case 7:         // BGEU
                {
                    if (bitsStringToUnsigned(Value1) >= bitsStringToUnsigned(Value2))
                    {
                        *OutputPort = '1';
                    }
                    else *OutputPort = '0';
                }
                break;
                default:
                {
                    throw invalid_argument("Invalid Func3 value");
                }

            }
        }
        ~BranchCmp()
        {
            OutputPort = nullptr;
        }
};

class DataMemory                        // FOLLOWING LITTLE-ENDIAN BYTE ORDER
{
    private:
        unordered_set<char*> OutputPorts;
    public:
        string Address;
        string WriteData;
        string Func3;
        unordered_map<string, string> Memory;
        string MemWrite;
        string MemRead;
        DataMemory()
        {
            Address = string(32,'0');
            WriteData = string(32,'0');
            Func3 = "000";
            MemWrite = "0";
            MemRead = "0";
        }

        void Reset()
        {
            Address = string(32,'0');
            WriteData = string(32,'0');
            Func3 = "000";
            MemWrite = "0";
            MemRead = "0";
            Memory.clear();
        }

        void ConnectOutput(char* connection)
        {
            assert(connection != nullptr);    // Ensure connection is not null
            // assert(connection->size() == 32); // Ensure connection is the correct size
            OutputPorts.insert(connection);
        }

        void StepWrite()
        {
            unsigned FUNC3 = bitsStringToUnsigned(Func3);
            if (MemWrite == "0") return;
            assert(MemRead == "0");
            if (FUNC3 == 0)         // store byte
            {
                Memory[Address] = string(8,'0');
                for (int i = 0; i < 8; i++)
                {
                    Memory[Address][i] = WriteData[24+i];
                }
            }
            else if(FUNC3 == 1)     // store halfword
            {
                unsigned temp = bitsStringToUnsigned(Address)+1;
                string tempaddr = unsignedToBitsString(temp);
                
                Memory[Address] = string(8,'0');
                Memory[tempaddr] = string(8,'0');
                for (int i = 0; i < 8; i++)
                {
                    Memory[tempaddr][i] = WriteData[16+i];
                }
                for (int i = 0; i < 8; i++)
                {
                    Memory[Address][i] = WriteData[24+i];
                }
                
            }
            else if(FUNC3 == 2)     // store word
            {
                unsigned temp = bitsStringToUnsigned(Address);

                string temp1 = unsignedToBitsString(temp+1);
                string temp2 = unsignedToBitsString(temp+2);
                string temp3 = unsignedToBitsString(temp+3);
                for (int i = 0; i < 8; i++)
                {
                    Memory[temp3][i] = WriteData[i];
                }
                for (int i = 0; i < 8; i++)
                {
                    Memory[temp2][i] = WriteData[8+i];
                }
                for (int i = 0; i < 8; i++)
                {
                    Memory[temp1][i] = WriteData[16+i];
                }
                for (int i = 0; i < 8; i++)
                {
                    Memory[Address][i] = WriteData[24+i];
                }
                
            }
        }

        void StepRead()
        {
            unsigned FUNC3 = bitsStringToUnsigned(Func3);
            if (MemRead == "0") return;
            assert(MemWrite == "0"); // Ensure MemWrite is 0
            string result = string(32,'0');
            if (FUNC3 == 0)          // load byte
            {
                for (int i = 0; i < 8; i++)
                {
                    result[24+i] = Memory[Address][i];
                }
                for (int i = 0; i < 24; i++)
                {
                    result[i] = Memory[Address][0];          // SIGN EXTENSION
                }
            }
            else if (FUNC3 == 1)     // load halfword
            {
                unsigned tempu = bitsStringToUnsigned(Address)+1;

                string temp = unsignedToBitsString(tempu);
                for (int i = 0; i < 16; i++)
                {
                    result[i] = Memory[temp][0];        // SIGN EXTENSION
                }
                for (int i = 0; i < 8; i++)
                {
                    result[16+i] = Memory[temp][i];
                }
                for (int i = 0; i < 8; i++)
                {
                    result[24+i] = Memory[Address][i];
                }
            }
            else if (FUNC3 == 2)     // load word
            {
                unsigned temp = bitsStringToUnsigned(Address);

                string temp1 = unsignedToBitsString(temp+1);
                string temp2 = unsignedToBitsString(temp+2);
                string temp3 = unsignedToBitsString(temp+3);
                for (int i = 0; i < 8; i++)
                {
                    result[i] = Memory[temp3][i];
                }
                for (int i = 0; i < 8; i++)
                {
                    result[8+i] = Memory[temp2][i];
                }
                for (int i = 0; i < 8; i++)
                {
                    result[16+i] = Memory[temp1][i];
                }
                for (int i = 0; i < 8; i++)
                {
                    result[24+i] = Memory[Address][i];
                }
            }
            else if (FUNC3 == 4)     // load byte unsigned
            {
                for (int i = 0; i < 8; i++)
                {
                    result[24+i] = Memory[Address][i];
                }
            }
            else if (FUNC3 == 5)     // load halfword unsigned
            {
                unsigned tempu = bitsStringToUnsigned(Address)+1;

                string temp = unsignedToBitsString(tempu);
                for (int i = 0; i < 8; i++)
                {
                    result[16+i] = Memory[temp][i];
                }
                for (int i = 0; i < 8; i++)
                {
                    result[24+i] = Memory[Address][i];
                }
            }
            for (auto port : OutputPorts)
            {
                assert(port != nullptr); // Ensure port is not null
                for (size_t i = 0; i < 32; i++)
                {
                    port[i] = result[i];
                }
            }
        }

        ~DataMemory()
        {
            OutputPorts.clear();
        }
};

class ANDGate
{
    private:
        unordered_set<char*> OutputPorts;
    public:
        string Input1;
        string Input2;

        ANDGate()
        { 
            Input1 = "0";
            Input2 = "0";
        }

        void Reset()
        {
            Input1 = "0";
            Input2 = "0";
        }

        void ConnectOutput(char* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            OutputPorts.insert(connection);
        }

        void Step()
        {
            string result = (Input1 == "1" && Input2 == "1") ? "1" : "0";
            for(auto port : OutputPorts)
            {
                assert(port != nullptr); // Ensure port is not null
                port[0] = result[0];
                
            }
        }

        ~ANDGate()
        {
            OutputPorts.clear();
        }
};

class ORGate
{
    private:
        unordered_set<char*> OutputPorts;
    public:
        string Input1;
        string Input2;

        ORGate()
        {
            Input1 = "0";
            Input2 = "0";
        }

        void Reset()
        {
            Input1 = "0";
            Input2 = "0";
        }

        void ConnectOutput(char* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            OutputPorts.insert(connection);
        }

        void Step()
        {
            string result = (Input1 == "1" || Input2 == "1") ? "1" : "0";
            for(auto port : OutputPorts)
            {
                assert(port != nullptr); // Ensure port is not null
                port[0] = result[0];
            }
        }

        ~ORGate()
        {
            OutputPorts.clear();
        }
};

class Adder
{
    private:
        unordered_set<char*> OutputPorts;
    public:
        string Input1;
        string Input2;
        size_t dataSizeBits;

        Adder(size_t dataSizeBitsInput)
        {
            dataSizeBits = dataSizeBitsInput;
            Input1 = string(dataSizeBits, '0');
            Input2 = string(dataSizeBits, '0');
        }

        void Reset()
        {
            Input1 = string(dataSizeBits, '0');
            Input2 = string(dataSizeBits, '0');
        }

        void ConnectOutput(char* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            // assert(connection->size() == dataSizeBits); // Ensure connection is the correct size
            OutputPorts.insert(connection);
        }

        void Step()
        {
            int inp1 = bitsStringToInt(Input1);
            int inp2 = bitsStringToInt(Input2);
            int res = inp1 + inp2;
            string result = intToBitsString(res).substr(32 - dataSizeBits);

            for(auto port : OutputPorts)
            {
                assert(port != nullptr); // Ensure port is not null
                for (size_t i = 0; i < dataSizeBits; i++)
                {
                    port[i] = result[i];
                }
            }
        }

        ~Adder()
        {
            OutputPorts.clear();
        }
};

class PCAdder : public Adder
{
    public:
        // string PC;

        PCAdder() : Adder(32)
        {
            // Input1 = PC;
            Input2 = unsignedToBitsString(4);
        }

        void Reset()
        {
            Adder::Reset();
            // Input1 = PC;
            Input2 = unsignedToBitsString(4);
        }
};

class LeftShift
{
    private:
        unordered_set<char*> OutputPorts;
    public:
        string Input;
        size_t dataSizeBits;

        LeftShift(size_t dataSizeBitsInput)
        {
            dataSizeBits = dataSizeBitsInput;
            Input = string(dataSizeBits, '0');
        }

        void Reset()
        {
            Input = string(dataSizeBits, '0');
        }

        void ConnectOutput(char* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            // assert(connection->size() == dataSizeBits); // Ensure connection is the correct size
            OutputPorts.insert(connection);
        }

        void Step()
        {
            string result = Input.substr(1) + '0';
            for(auto port : OutputPorts)
            {
                assert(port != nullptr); // Ensure port is not null
                for (size_t i = 0; i < dataSizeBits; i++)
                {
                    port[i] = result[i];
                }
            }
        }

        ~LeftShift()
        {
            OutputPorts.clear();
        }
};

// int main()
// {
//     string output;
//     string output2;
//     string output3;
//     output3 = string(32, '0');
//     output = "0101";
//     output2 = "000000";
    
//     MUX2x1 mux1 = MUX2x1(1);
//     MUX2x1 mux2 = MUX2x1(2);
//     MUX2x1 mux3 = MUX2x1(2);
//     MUX4x1 mux4 = MUX4x1(4);
//     Register reg = Register(174);
//     ALUx32 alu = ALUx32();
    
//     mux1.Input1 = "0";
//     mux1.Input2 = "1";
    
//     mux2.Input1 = "00";
//     mux2.Input2 = "01";
    
//     mux3.Input1 = "10";
//     mux3.Input2 = "11";
    
//     mux1.ConnectOutput(&mux2.InputSwitch);
//     mux1.ConnectOutput(&mux3.InputSwitch);
    
//     mux2.ConnectOutput(&mux4.Input2);
//     mux3.ConnectOutput(&mux4.Input2+2);
    
//     mux1.InputSwitch = '1';
//     mux4.InputSwitch = "01";
    
//     mux4.ConnectOutput(&output);
//     mux4.ConnectOutput(&reg.Data[13]);
    
//     reg.ConnectOutput(12,17,&output2);
//     reg.Write = "1";
//     reg.Flush = "0";

//     reg.ConnectOutput(12, 43, alu.Input1);
//     reg.ConnectOutput(0, 31, alu.Input2);
//     alu.ConnectResult(&output3);
//     alu.ALUControl = 1;

//     mux1.Step();
//     mux2.Step();
// }
