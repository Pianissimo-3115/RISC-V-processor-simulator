#include<bits/stdc++.h>
using namespace std;

using ControlSignal = tuple<char,       char,       char,       char,       char,       char,       string,     char,   char>;
//                          ALUSrc,     MemtoReg,   RegWrite,   MemRead,    MemWrite,   Branch,     ALUOp,      Jump,   nop

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
        unordered_set<string*> OutputPorts;
    public:
        string Input1;
        string Input2;
        char InputSwitch;
        size_t dataSizeBits;

        MUX2x1(size_t dataSizeBitsInput) 
        {
            dataSizeBits = dataSizeBitsInput;
            Input1 = string(dataSizeBits, '0');
            Input2 = string(dataSizeBits, '0');
            InputSwitch = '0';
        }

        void Reset()
        {
            Input1 = string(dataSizeBits, '0');
            Input2 = string(dataSizeBits, '0');
            InputSwitch = '0';
        }

        void ConnectOutput(string* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            assert(connection->size() == dataSizeBits); // Ensure connection is the correct size
            OutputPorts.insert(connection);
        }

        void Step()
        {
            for(auto port : OutputPorts)
            {
                assert(port != nullptr); // Ensure port is not null
                if (InputSwitch == '0')
                {
                    *port = Input1;
                }
                else
                {
                    *port = Input2;
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
        unordered_set<string*> OutputPorts;
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

        void ConnectOutput(string* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            assert(connection->size() == dataSizeBits); // Ensure connection is the correct size
            OutputPorts.insert(connection);
        }

        void Step()
        {
            assert(InputSwitch.size() == 2); // Ensure InputSwitch is 2 bits
            string chosen;
            if (InputSwitch == "00") chosen = Input1;
            else if (InputSwitch == "01") chosen = Input2;
            else if (InputSwitch == "10") chosen = Input3;
            else if (InputSwitch == "11") chosen = Input4;
            else throw invalid_argument("Invalid InputSwitch value");
            for(auto port : OutputPorts)
            {
                assert(port != nullptr); // Ensure port is not null
                *port = chosen;
            }
        }

        ~MUX4x1()
        {
            OutputPorts.clear();
        }
};

class Register  // Arjun can write this in this style yourself (maine tera implementation iska padha nhi so idk kaise hoga)
{
    private:

};

class HazardDetectionUnitNoFwd
{
    private:
        unordered_set<char*> OutputPorts;
    public:
        char ID_EX_Memread;
        char EX_MEM_Memread;
        string ID_EX_RegisterRd;
        string IF_ID_RegisterRs1;
        string IF_ID_RegisterRs2;
        string EX_MEM_RegisterRd;
        unsigned opcode;

        HazardDetectionUnitNoFwd()
        {
            ID_EX_Memread = '0';
            EX_MEM_Memread = '0';
            ID_EX_RegisterRd = string(5, '0');
            IF_ID_RegisterRs1 = string(5, '0');
            IF_ID_RegisterRs2 = string(5, '0');
            EX_MEM_RegisterRd = string(5, '0');
            opcode = 0;
        }

        void Reset()
        {
            ID_EX_Memread = '0';
            EX_MEM_Memread = '0';
            ID_EX_RegisterRd = string(5, '0');
            IF_ID_RegisterRs1 = string(5, '0');
            IF_ID_RegisterRs2 = string(5, '0');
            EX_MEM_RegisterRd = string(5, '0');
            opcode = 0;
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
                *port = '0';
            }
            if
            (
                (
                    ID_EX_Memread == '1'
                    && 
                    (
                        ID_EX_RegisterRd == IF_ID_RegisterRs1 
                        || ID_EX_RegisterRd == IF_ID_RegisterRs2
                    )
                )
                ||
                (
                    EX_MEM_Memread == '1'
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
                    *port = '1';
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
        char ID_EX_Memread;
        char EX_MEM_Memread;
        string ID_EX_RegisterRd;
        string IF_ID_RegisterRs1;
        string IF_ID_RegisterRs2;
        string EX_MEM_RegisterRd;
        unsigned opcode;

        HazardDetectionUnit()
        {
            ID_EX_Memread = '0';
            EX_MEM_Memread = '0';
            ID_EX_RegisterRd = string(5, '0');
            IF_ID_RegisterRs1 = string(5, '0');
            IF_ID_RegisterRs2 = string(5, '0');
            EX_MEM_RegisterRd = string(5, '0');
            opcode = 0;
        }

        void Reset()
        {
            ID_EX_Memread = '0';
            EX_MEM_Memread = '0';
            ID_EX_RegisterRd = string(5, '0');
            IF_ID_RegisterRs1 = string(5, '0');
            IF_ID_RegisterRs2 = string(5, '0');
            EX_MEM_RegisterRd = string(5, '0');
            opcode = 0;
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
                *port = '0';
            }
            if
            (
                (
                    ID_EX_Memread == '1'
                    && 
                    (
                        ID_EX_RegisterRd == IF_ID_RegisterRs1 
                        || ID_EX_RegisterRd == IF_ID_RegisterRs2
                    )
                )
                ||
                (
                    EX_MEM_Memread == '1'
                    &&
                    (
                        EX_MEM_RegisterRd == IF_ID_RegisterRs1
                        || EX_MEM_RegisterRd == IF_ID_RegisterRs2
                    )
                    && opcode == 0b1100011
                )
            )
            {
                for(auto port : OutputPorts)
                {
                    assert(port != nullptr); // Ensure port is not null
                    *port = '1';
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
        unordered_set<string*> OutputPorts;
        unordered_set<char*> ZeroPorts;
    public:
        string Input1;
        string Input2;
        unsigned ALUControl;

        ALUx32() 
        {
            Input1 = string(32, '0');
            Input2 = string(32, '0');
            ALUControl = 0;
        }

        void Reset()
        {
            Input1 = string(32, '0');
            Input2 = string(32, '0');
            ALUControl = 0;
        }

        void ConnectResult(string* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            assert(connection->size() == 32); // Ensure connection is the correct size
            OutputPorts.insert(connection);
        }

        void ConnectZero(char* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            ZeroPorts.insert(connection);
        }

        void Step()
        {

            unsigned inp1 = bitsStringToUnsigned(Input1);
            unsigned inp2 = bitsStringToUnsigned(Input2);
            string result;
            char zero;
            switch(ALUControl)
            {
                case 0:         // AND
                {
                    unsigned res = inp1 & inp2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? '1' : '0';
                    break;
                }
                case 1:         // OR
                {
                    unsigned res = inp1 | inp2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? '1' : '0';
                    break;
                }
                case 2:         // add
                {
                    unsigned res = inp1 + inp2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? '1' : '0';
                    break;
                }
                case 3:         // XOR
                {
                    unsigned res = inp1 ^ inp2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? '1' : '0';
                    break;
                }
                case 4:         // slt
                {
                    zero = (((int)inp1 < (int)inp2) ? '1' : '0');
                    break;
                }
                case 5:         // sltu
                {
                    zero = ((inp1 < inp2) ? '1' : '0');
                    break;
                }
                case 6:         // Sub
                {
                    unsigned res = inp1 - inp2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? '1' : '0';
                    break;
                }
                case 8:         // mul
                {
                    long long int res = (long long int)inp1 * (long long int)inp2;
                    res&=0x00000000FFFFFFFF;
                    result = intToBitsString((int)res);
                    zero = (res == 0) ? '1' : '0';
                    break;
                }
                case 9:         // mulh
                {
                    long long int res = (long long int)inp1 * (long long int)inp2;
                    res >>= 32;
                    result = intToBitsString(res);
                    zero = (res == 0) ? '1' : '0';
                    break;
                }
                case 11:        // mulhu
                {
                    long long unsigned res = (long long unsigned)inp1 * (long long unsigned)inp2;
                    res&=0x00000000FFFFFFFF;
                    result = unsignedToBitsString((unsigned)res);
                    zero = (res == 0) ? '1' : '0';
                    break;
                }
                case 13:        // sll
                {
                    unsigned res = inp1 << inp2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? '1' : '0';
                    break;
                }
                case 14:        // srl
                {
                    unsigned res = inp1 >> inp2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? '1' : '0';
                    break;
                }
                case 15:        // sra
                {
                    unsigned res = (int)inp1 >> inp2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? '1' : '0';
                    break;
                }
                case 24:        // div
                {
                    unsigned res = (int)inp1 / (int)inp2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? '1' : '0';
                    break;
                }
                case 25:        // divu
                {
                    unsigned res = inp1 / inp2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? '1' : '0';
                    break;
                }
                case 26:        // rem
                {
                    unsigned res = (int)inp1 % (int)inp2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? '1' : '0';
                    break;
                }
                case 27:        // remu
                {
                    unsigned res = inp1 % inp2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? '1' : '0';
                    break;
                }
                case INT_MAX:
                    result = string('x',32);
                default:
                {
                    throw invalid_argument("Invalid ALUControl value");
                    break;
                }
            }
            
            for(auto port : OutputPorts)
            {
                assert(port != nullptr); // Ensure port is not null
                *port = result;
            }
            for(auto port : ZeroPorts)
            {
                assert(port != nullptr); // Ensure port is not null
                *port = zero;
            }
        }
    ~ALUx32()
    {
        OutputPorts.clear();
        ZeroPorts.clear();
    }
};

class RegisterMemx32 
{
    private:
        unordered_set<string*> Data1OutputPorts;
        unordered_set<string*> Data2OutputPorts;
        string Registers[32];
    
    public:
        string ReadReg1;
        string ReadReg2;
        string WriteReg;
        string WriteData;
        char WriteEnable;

        RegisterMemx32() 
        {
            ReadReg1 = string(5, '0');
            ReadReg2 = string(5, '0');
            WriteReg = string(5, '0');
            WriteData = string(32, '0');
            WriteEnable = '0';
            for(int i = 0; i < 32; i++)
            {
                Registers[i] = string(32, '0');
            }
        }

        void Reset()
        {
            ReadReg1 = string(5, '0');
            ReadReg2 = string(5, '0');
            WriteReg = string(5, '0');
            WriteData = string(32, '0');
            WriteEnable = '0';
            for(int i = 0; i < 32; i++)
            {
                Registers[i] = string(32, '0');
            }
        }

        void ConnectData1Output(string* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            assert(connection->size() == 32); // Ensure connection is the correct size
            Data1OutputPorts.insert(connection);
        }

        void ConnectData2Output(string* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            assert(connection->size() == 32); // Ensure connection is the correct size
            Data2OutputPorts.insert(connection);
        }

        void StepRead()
        {
            assert(ReadReg1.size() == 5); // Ensure ReadReg1 is 5 bits
            assert(ReadReg2.size() == 5); // Ensure ReadReg2 is 5 bits
            unsigned address1 = bitsStringToUnsigned(ReadReg1);
            unsigned address2 = bitsStringToUnsigned(ReadReg2);
            for(auto port : Data1OutputPorts)
            {
                assert(port != nullptr); // Ensure port is not null
                *port = Registers[address1];
            }
            for(auto port : Data2OutputPorts)
            {
                assert(port != nullptr); // Ensure port is not null
                *port = Registers[address2];
            }
        }
        
        void StepWrite()
        {
            if(WriteEnable == '0') return;
            assert(WriteReg.size() == 5); // Ensure WriteReg is 5 bits
            unsigned address = bitsStringToUnsigned(WriteReg);
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
        unordered_set<string*> OutputPorts;
        vector<uint32_t> Instructions;
        unsigned InstrCount = 0;

    public:
        int PC;
        InstructionMemx32(unsigned size) 
        {
            this->Instructions=vector<uint32_t>(size, 0);
            this->InstrCount = size;
        }

        void Reset()
        {
            this->Instructions.clear();
            this->InstrCount = 0;
        }

        void ConnectOutput(string* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            assert(connection->size() == 32); // Ensure connection is the correct size
            OutputPorts.insert(connection);
        }

        void AddInstruction(uint32_t instr)
        {
            Instructions.push_back(instr);
            InstrCount++;
        }

        void Step()
        {
            assert(PC%4 == 0); // Ensure PC is a multiple of 4
            assert(PC/4 < InstrCount); // Ensure PC is within range
            string curr_instruction = unsignedToBitsString(Instructions[PC/4]);
            for(auto port : OutputPorts)
            {
                assert(port != nullptr); // Ensure port is not null
                *port = curr_instruction;
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
        unordered_set<string*> OutputPorts;
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

        void ConnectOutput(string* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            assert(connection->size() == 32); // Ensure connection is the correct size
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
                immediate[31] = Instruction[24];
            }
            else if (Instruction.substr(25, 7) == "1100011") //SB type
            {
                for (int i = 0; i < 20; i++)
                {
                    immediate[i] = Instruction[0];
                }
                immediate[20] = Instruction[24];
                for (int i = 21; i < 27; i++)
                {
                    immediate[i] = Instruction[i-20];
                }
                for (int i = 27; i < 31; i++)
                {
                    immediate[i] = Instruction[i-6];
                }
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
                *port = immediate;
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
        unordered_set<ControlSignal*> OutputPorts;
    public:
        unsigned opcode;
        char hazard{};
        ControlUnit()
        {
            opcode = 0;
            hazard = '0';
        }

        void Reset()
        {
            opcode = 0;
            hazard = '0';
        }

        void ConnectOutput(ControlSignal* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            OutputPorts.insert(connection);
        }

        void Step()
        {
            ControlSignal result{};
            if (hazard)
            {
                result = make_tuple('0', '0', '0', '0', '0', '0', "00", '0', '1');
                return;
            }
            switch(opcode)
            {
                case 0b0110011:         // R-type
                {
                    result = make_tuple('0', '0', '1', '0', '0', '0', "10", '0', '0');
                }
                break;

                case 0b0010011:         // I-type
                {
                    result = make_tuple('1', '0', '1', '0', '0', '0', "11", '0', '0');
                }
                break;

                case 0b0000011:         // Load
                {
                    result = make_tuple('1', '1', '1', '1', '0', '0', "00", '0', '0');
                }
                break;

                case 0b0100011:         // Store
                {
                    result = make_tuple('1', 'x', '0', '0', '1', '0', "00", '0', '0');
                }
                break;

                case 0b1100111:         // JALR
                {
                    result = make_tuple('1', '0', '1', '0', '0', '0', "00", '1', '0');
                }
                break;

                case 0b1100011:         // Branch
                {
                    result = make_tuple('0', 'x', '0', '0', '0', '1', "01", '0', '0');
                }
                break;

                case 0b0110111:         // LUI
                case 0b0010111:         // AUIPC
                {
                    result = make_tuple('1', '0', '1', '0', '0', '0', "11", '0', '0');
                }
                break;

                case 0b1101111:         // JAL
                {
                    result = make_tuple('x', '0', '1', '0', '0', '0', "xx", '1', '0');
                }
            }
            for(auto port : OutputPorts)
            {
                assert(port != nullptr); // Ensure port is not null
                *port = result;
            }
        }

        ~ControlUnit()
        {
            OutputPorts.clear();
        }
};

class ALUControlUnit
{
    private:
        unsigned* OutputPort;

    public:
        string ALUOp;
        unsigned Func3;
        unsigned Func7;      // only 2 bits

        ALUControlUnit()
        {
            Func3 = 0;
            Func7 = 0;
            ALUOp = "00";
        }

        void Reset()
        {
            ALUOp = "00";
            Func3 = 0;
            Func7 = 0;
        }

        void ConnectOutput(unsigned* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            OutputPort = connection;
        }

        void Step()
        {
            unsigned result = 0;
            
            
                if (ALUOp == "10")      // ARITHMETIC OPERATION
                {
                    if (Func3 == 0)
                    {
                        if (Func7 == 0x00)
                        {
                            result = 2;     // ADD
                        }
                        else if (Func7 == 0x20)
                        {
                            result = 6;     // SUB
                        }
                        else if (Func7 == 0x01)
                        {
                            result = 8;     // MUL
                        }
                        else 
                        {
                            throw invalid_argument("Invalid Func7 value");
                        }
                    }
                    else if (Func3 == 1)
                    {
                        if (Func7 == 1)
                        {
                            result = 9;     // MULH
                        }
                        else 
                        {
                            throw invalid_argument("Invalid Func7 value");
                        }
                    }
                    else if (Func3 == 3)
                    {
                        if (Func7 == 0x01)
                        {
                            result = 11;    // MULHU
                        }
                        else 
                        {
                            throw invalid_argument("Invalid Func7 value");
                        }
                    }
                    else if (Func3 == 4)
                    {
                        if (Func7 == 0x00)
                        {
                            result = 3;     // XOR
                        }
                        else if (Func7 == 0x01)
                        {
                            result = 24;    // DIV
                        }
                        else 
                        {
                            throw invalid_argument("Invalid Func7 value");
                        }
                    }
                    else if (Func3 == 5)
                    {
                        if (Func7 == 0x01)
                        {
                            result = 25;    // DIVU
                        }
                    }
                    else if (Func3 == 6)
                    {
                        if (Func7 == 0x00)
                        {
                            result = 1;     // OR
                        }
                        else if (Func7 == 0x01)
                        {
                            result = 26;    // REM
                        }
                        else 
                        {
                            throw invalid_argument("Invalid Func7 value");
                        }
                    }
                    else if (Func3 == 7)
                    {
                        if (Func7 == 0x00)
                        {
                            result = 0;     // AND
                        }
                        else if (Func7 == 0x01)
                        {
                            result = 27;    // REMU
                        }
                        else 
                        {
                            throw invalid_argument("Invalid Func7 value");
                        }
                    }
                    else if (Func3 == 0x1)
                    {
                        if (Func7 == 0x00)
                        {
                            result = 13;    // sll
                        }
                        else 
                        {
                            throw invalid_argument("Invalid Func7 value");
                        }
                    }
                    else if (Func3 == 0x5)
                    {
                        if (Func7 == 0x00)
                        {
                            result = 14;    // srl
                        }
                        else if (Func7 == 0x20)
                        {
                            result = 15;    // sra
                        }
                        else 
                        {
                            throw invalid_argument("Invalid Func7 value");
                        }
                    }
                    else if (Func3 == 0x2)
                    {
                        if (Func7 == 0x00)
                        {
                            result = 4;     // slt
                        }
                        else 
                        {
                            throw invalid_argument("Invalid Func7 value");
                        }
                    }
                    else if (Func3 == 0x3)
                    {
                        if (Func7 == 0x00)
                        {
                            result = 5;     // sltu
                        }
                        else 
                        {
                            throw invalid_argument("Invalid Func7 value");
                        }
                    }
                }
                
                else if (ALUOp == "11")    // ARITHMETIC IMMEDIATE OPERATION
                {
                    if (Func3 == 0x0)
                    {
                        result = 2;     // ADDI
                    }
                    else if (Func3 == 0x4)
                    {
                        result = 3;     // XORI
                    }
                    else if (Func3 == 0x6)
                    {
                        result = 1;     // ORI
                    }
                    else if (Func3 == 0x7)
                    {
                        result = 0;     // ANDI
                    }
                    else if (Func3 == 0x1)
                    {
                        result = 13;    // SLLI
                    }
                    else if (Func3 == 0x5)
                    {
                        if (Func7 == 0x00)  // ALTHOUGH THERE DOES NOT EXIST Func7 FOR I-TYPE INSTRUCTIONS, I NEEDED SOMETHING TO PASS 2 BITS OF IMMEDIATE. SO I USED Func7 FOR THAT
                        {
                            result = 14;    // SRLI
                        }
                        else if (Func7 == 0x20)
                        {
                            result = 15;    // SRAI
                        }
                        else
                        {
                            throw invalid_argument("Invalid Func7 value");
                        }
                    }
                    else if (Func3 == 0x2)
                    {
                        result = 4;     // SLTI
                    }
                    else if (Func3 == 0x3)
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
                    result = INT_MAX;
                }
                else
                {
                    throw invalid_argument("Invalid opcode value");
                }
            *OutputPort = result;
        }

        ~ALUControlUnit()
        {
            OutputPort = nullptr;
        }
};

class ALUForwardingUnit
{
    private:
        string* CtrlMUX3{};
        string* CtrlMUX4{};

    public:
        string Reg1Addr{};
        string Reg2Addr{};
        string EX_MEM_RegisterRDAddr{};
        string MEM_WB_RegisterRDAddr{};
        
        string EX_MEM_RegWrite{};
        string MEM_WB_RegWrite{};
        char branch{};

        ALUForwardingUnit() 
        {
            Reg1Addr = string(4, '0');
            Reg2Addr = string(4, '0');

            EX_MEM_RegisterRDAddr = string(4, '0');
            MEM_WB_RegisterRDAddr = string(4, '0');
            EX_MEM_RegWrite = "0";
            MEM_WB_RegWrite = "0";
            branch = '0';
        }

        void Reset()
        {
            Reg1Addr = string(4, '0');
            Reg2Addr = string(4, '0');
            EX_MEM_RegisterRDAddr = string(4, '0');
            MEM_WB_RegisterRDAddr = string(4, '0');
            EX_MEM_RegWrite = "0";
            MEM_WB_RegWrite = "0";
            branch = '0';
        }

        void ConnectCtrlMUX3(string* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            CtrlMUX3 = connection;
        }

        void ConnectCtrlMUX4(string* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            CtrlMUX4 = connection;
        }

        void Step()
        {
            assert(CtrlMUX3 != nullptr);                    // ARJUN CONFIRM THIS////////////////////////////////////////////////////////////
            assert(CtrlMUX4 != nullptr);                    // ARJUN CONFIRM THIS////////////////////////////////////////////////////////////
            *CtrlMUX3 = "00";                               // ARJUN CONFIRM THIS////////////////////////////////////////////////////////////        
            *CtrlMUX4 = "00";                               // ARJUN CONFIRM THIS////////////////////////////////////////////////////////////        
            if (branch == '1') return;                      // ARJUN CONFIRM THIS////////////////////////////////////////////////////////////
            string ctrlmux3_out = "00";
            string ctrlmux4_out = "00";
            if 
            (
                MEM_WB_RegWrite[0] == '1' 
                && MEM_WB_RegisterRDAddr != "0000" 
                && !(   
                        EX_MEM_RegWrite[0] == '1' 
                        && EX_MEM_RegisterRDAddr != "0000" 
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
                && MEM_WB_RegisterRDAddr != "0000" 
                && !(   
                        EX_MEM_RegWrite[0] == '1' 
                        && EX_MEM_RegisterRDAddr != "0000" 
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
                && EX_MEM_RegisterRDAddr != "0000" 
                && Reg1Addr == EX_MEM_RegisterRDAddr
            )
            {
                ctrlmux3_out="10";
            }

            if 
            (
                EX_MEM_RegWrite[0]=='1' 
                && EX_MEM_RegisterRDAddr!="0000" 
                && Reg2Addr==EX_MEM_RegisterRDAddr
            )
            {
                ctrlmux4_out="10";
            }
            
            *CtrlMUX3 = ctrlmux3_out;
            *CtrlMUX4 = ctrlmux4_out;
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
        char *BranchCMPMux1{};
        char *BranchCMPMux2{};

    public:
        string IF_ID_RegisterRs1{};
        string IF_ID_RegisterRs2{};

        string EX_MEM_RegisterRDAddr{};
        string MEM_WB_RegisterRDAddr{};

        char EX_MEM_RegWrite{};
        char Branch{};

        BranchForwardingUnit() 
        {
            IF_ID_RegisterRs1 = string(5, '0');
            IF_ID_RegisterRs2 = string(5, '0');
            EX_MEM_RegisterRDAddr = string(5, '0');
            EX_MEM_RegWrite = '0';
            Branch = '0';
        }

        void Reset()
        {
            IF_ID_RegisterRs1 = string(5, '0');
            IF_ID_RegisterRs2 = string(5, '0');
            EX_MEM_RegisterRDAddr = string(5, '0');
            EX_MEM_RegWrite = '0';
            Branch = '0';
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
            if (Branch == '0') return;
            assert(BranchCMPMux1 != nullptr); 
            assert(BranchCMPMux2 != nullptr); 
            assert(IF_ID_RegisterRs1.size() == 5); 
            assert(IF_ID_RegisterRs2.size() == 5); 
            assert(EX_MEM_RegisterRDAddr.size() == 5); 
            assert(MEM_WB_RegisterRDAddr.size() == 5); 
            char branch_cmp_mux1_out = '0';
            char branch_cmp_mux2_out = '0';
            if
            (
                EX_MEM_RegWrite == '1'
                && EX_MEM_RegisterRDAddr == IF_ID_RegisterRs1
            )
            {
                branch_cmp_mux1_out = '1';
            }
            else branch_cmp_mux1_out = '0';

            if
            (
                EX_MEM_RegWrite == '1'
                && EX_MEM_RegisterRDAddr == IF_ID_RegisterRs2
            )
            {
                branch_cmp_mux2_out = '1';
            }
            else branch_cmp_mux2_out = '0';
            *BranchCMPMux1 = branch_cmp_mux1_out;
            *BranchCMPMux2 = branch_cmp_mux2_out;
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
        unsigned Func3;
        char branch;

        BranchCmp()
        {
            Value1 = string(32, '0');
            Value2 = string(32, '0');
            branch = '0';
            Func3 = 0;
        }

        void Reset()
        {
            Value1 = string(32, '0');
            Value2 = string(32, '0');
            branch = '0';
            Func3 = 0;
        }

        void ConnectOutput(char* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            OutputPort = connection;
        }

        void Step()
        {
            *OutputPort = '0';
            if (branch == '0') return;
            assert(Value1.size() == 32); // Ensure Value1 is 32 bits
            assert(Value2.size() == 32); // Ensure Value2 is 32 bits
            
            switch (Func3)
            {
                case 0:         // BEQ
                {
                    if (Value1 == Value2)
                    {
                        *OutputPort = '1';
                    }
                }
                break;

                case 1:         // BNE
                {
                    if (Value1 != Value2)
                    {
                        *OutputPort = '1';
                    }
                }
                break;

                case 4:         // BLT
                {
                    if (bitsStringToInt(Value1) < bitsStringToInt(Value2))
                    {
                        *OutputPort = '1';
                    }
                }

                case 5:         // BGE
                {
                    if (bitsStringToInt(Value1) >= bitsStringToInt(Value2))
                    {
                        *OutputPort = '1';
                    }
                }
                
                case 6:         // BLTU
                {
                    if (bitsStringToUnsigned(Value1) < bitsStringToUnsigned(Value2))
                    {
                        *OutputPort = '1';
                    }
                }

                case 7:         // BGEU
                {
                    if (bitsStringToUnsigned(Value1) >= bitsStringToUnsigned(Value2))
                    {
                        *OutputPort = '1';
                    }
                }

            }
        }
        ~BranchCmp()
        {
            OutputPort = nullptr;
        }
};

class DataMemory
{
    private:
        unordered_set<string*> OutputPorts;
    public:
        unsigned Address;
        unsigned* WriteData;
        unsigned Func3;
        unordered_map<unsigned, unsigned> Memory;
        char MemWrite;
        char MemRead;
        DataMemory()
        {
            Address = 0;
            WriteData = nullptr;
            Func3 = 0;
            MemWrite = '0';
            MemRead = '0';
        }

        void Reset()
        {
            Address = 0;
            WriteData = nullptr;
            Func3 = 0;
            MemWrite = '0';
            MemRead = '0';
            Memory.clear();
        }

        void ConnectOutput(string* connection)
        {
            assert(connection != nullptr);    // Ensure connection is not null
            assert(connection->size() == 32); // Ensure connection is the correct size
            OutputPorts.insert(connection);
        }

        void StepWrite()
        {
            if (MemWrite == '0') return;
            assert(MemRead == '0');
            if (Func3 == 0)         // store byte
            {
                bool sign = (*WriteData >> 7) & 1;
                int res = *WriteData & ((1<<7)-1);
                if (sign) res = -res;
                Memory[Address] = (unsigned)res;
            }
            else if(Func3 == 1)     // store halfword
            {
                bool sign = (*WriteData >> 15) & 1;
                int res = *WriteData & ((1<<15)-1);
                if (sign) res = -res;
                Memory[Address] = (unsigned)res;
            }
            else if(Func3 == 2)     // store word
            {
                Memory[Address] = *WriteData;
            }
        }

        void StepRead()
        {
            if (MemRead == '0') return;
            assert(MemWrite == '0'); // Ensure MemWrite is 0
            int result = 0;
            if (Func3 == 0)          // load byte
            {
                result = Memory[Address] & ((1<<7)-1);
                if(Memory[Address] & (1<<7)) result = -result;
            }
            else if (Func3 == 1)     // load halfword
            {
                result = Memory[Address] & ((1<<15)-1);
                if(Memory[Address] & (1<<15)) result = -result;
            }
            else if (Func3 == 2)     // load word
            {
                result = Memory[Address];
            }
            else if (Func3 == 4)     // load byte unsigned
            {
                result = Memory[Address] & ((1<<7)-1);
            }
            else if (Func3 == 5)     // load halfword unsigned
            {
                result = Memory[Address] & ((1<<15)-1);
            }
            for (auto port : OutputPorts)
            {
                assert(port != nullptr); // Ensure port is not null
                *port = result;
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
        char Input1;
        char Input2;

        ANDGate()
        { 
            Input1 = '0';
            Input2 = '0';
        }

        void Reset()
        {
            Input1 = '0';
            Input2 = '0';
        }

        void ConnectOutput(char* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            OutputPorts.insert(connection);
        }

        void Step()
        {
            char result = (Input1 == '1' && Input2 == '1') ? '1' : '0';
            for(auto port : OutputPorts)
            {
                assert(port != nullptr); // Ensure port is not null
                *port = result;
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
        char Input1;
        char Input2;

        ORGate()
        {
            Input1 = '0';
            Input2 = '0';
        }

        void Reset()
        {
            Input1 = '0';
            Input2 = '0';
        }

        void ConnectOutput(char* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            OutputPorts.insert(connection);
        }

        void Step()
        {
            char result = (Input1 == '1' || Input2 == '1') ? '1' : '0';
            for(auto port : OutputPorts)
            {
                assert(port != nullptr); // Ensure port is not null
                *port = result;
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
        unordered_set<string*> OutputPorts;
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

        void ConnectOutput(string* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            assert(connection->size() == dataSizeBits); // Ensure connection is the correct size
            OutputPorts.insert(connection);
        }

        void Step()
        {
            unsigned inp1 = bitsStringToUnsigned(Input1);
            unsigned inp2 = bitsStringToUnsigned(Input2);
            unsigned res = inp1 + inp2;
            string result = unsignedToBitsString(res).substr(32 - dataSizeBits);

            for(auto port : OutputPorts)
            {
                assert(port != nullptr); // Ensure port is not null
                *port = result;
            }
        }

        ~Adder()
        {
            OutputPorts.clear();
        }
};

class PCAdder
{
    private:
        unordered_set<unsigned*> OutputPorts;
    public:
        unsigned PC;

        PCAdder()
        {
            PC = 0;
        }

        void Reset()
        {
            PC = 0;
        }

        void ConnectOutput(unsigned* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            OutputPorts.insert(connection);
        }

        void Step()
        {
            unsigned newPC = PC + 4;
            for(auto port : OutputPorts)
            {
                assert(port != nullptr); // Ensure port is not null
                *port = newPC;
            }
        }

        ~PCAdder()
        {
            OutputPorts.clear();
        }
};

class LeftShift
{
    private:
        unordered_set<string*> OutputPorts;
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

        void ConnectOutput(string* connection)
        {
            assert(connection != nullptr); // Ensure connection is not null
            assert(connection->size() == dataSizeBits); // Ensure connection is the correct size
            OutputPorts.insert(connection);
        }

        void Step()
        {
            string result = Input.substr(1) + '0';
            result = result.substr(1);
            for(auto port : OutputPorts)
            {
                assert(port != nullptr); // Ensure port is not null
                *port = result;
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
