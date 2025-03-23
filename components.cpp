#include<bits/stdc++.h>
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

class ALUx32
{
    private:
        unordered_set<string*> OutputPorts;
        unordered_set<char*> ZeroPorts;
    public:
        unsigned Input1;
        unsigned Input2;
        unsigned ALUControl;

        ALUx32() 
        {
            Input1 = 0;
            Input2 = 0;
            ALUControl = 0;
        }

        void Reset()
        {
            Input1 = 0;
            Input2 = 0;
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
            string result;
            char zero;
            switch(ALUControl)
            {
                case 0:
                    unsigned res = Input1 & Input2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? '1' : '0';
                    break;
                case 1:         // subtract, for beq (basically useless if we decide to do beq in ID stage)
                    unsigned res = Input1 | Input2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? '1' : '0';
                    break;
                case 2:
                    unsigned res = Input1 + Input2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? '1' : '0';
                    break;
                case 3:
                    unsigned res = Input1 ^ Input2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? '1' : '0';
                    break;
                case 4:
                    zero = (((int)Input1 < (int)Input2) ? '1' : '0');
                    break;
                case 5:
                    zero = ((Input1 < Input2) ? '1' : '0');
                    break;
                case 6:
                    unsigned res = Input1 - Input2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? '1' : '0';
                    break;
                case 8:
                    unsigned res = Input1 * Input2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? '1' : '0';
                    break;
                case 9:
                    long long int res = (long long int)Input1 * (long long int)Input2;
                    res >>= 32;
                    result = intToBitsString(res);
                    zero = (res == 0) ? '1' : '0';
                    break;
                case 11:
                    long long unsigned res = (long long unsigned)Input1 * (long long unsigned)Input2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? '1' : '0';
                    break;
                case 13:
                    unsigned res = Input1 << Input2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? '1' : '0';
                    break;
                case 14:
                    unsigned res = Input1 >> Input2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? '1' : '0';
                    break;
                case 15:
                    unsigned res = (int)Input1 >> Input2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? '1' : '0';
                    break;
                case 24:
                    unsigned res = (int)Input1 / (int)Input2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? '1' : '0';
                    break;
                case 25:
                    unsigned res = Input1 / Input2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? '1' : '0';
                    break;
                case 26:
                    unsigned res = (int)Input1 % (int)Input2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? '1' : '0';
                    break;
                case 27:
                    unsigned res = Input1 % Input2;
                    result = unsignedToBitsString(res);
                    zero = (res == 0) ? '1' : '0';
                    break;
                default:
                    throw invalid_argument("Invalid ALUControl value");
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

class ControlUnit       // YE MAI KAL KARTA HU
{
};

class ALUControlUnit    // baadme krta hu
{
};

class ALUForwardingUnit
{
    private:
        string* CtrlMUX3{};
        string* CtrlMUX4{};

    public:
        string Reg1Addr{};
        string Reg2Addr{};

        string ExMemRegisterRDAddr{};
        string MemWbRegisterRDAddr{};

        string ExMemRegWrite{};
        string MemWbRegWrite{};

        ALUForwardingUnit() 
        {
            Reg1Addr = string(4, '0');
            Reg2Addr = string(4, '0');
            ExMemRegisterRDAddr = string(4, '0');
            MemWbRegisterRDAddr = string(4, '0');
            ExMemRegWrite = "0";
            MemWbRegWrite = "0";
        }

        void Reset()
        {
            Reg1Addr = string(4, '0');
            Reg2Addr = string(4, '0');
            ExMemRegisterRDAddr = string(4, '0');
            MemWbRegisterRDAddr = string(4, '0');
            ExMemRegWrite = "0";
            MemWbRegWrite = "0";
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
            string ctrlmux3_out="00";
            string ctrlmux4_out="00";
            if (
                MemWbRegWrite[0]=='1' 
                && MemWbRegisterRDAddr!="0000" 
                && !(   ExMemRegWrite[0]=='1' 
                        && ExMemRegisterRDAddr!="0000" 
                        && Reg1Addr==ExMemRegisterRDAddr
                    ) 
                && Reg1Addr==MemWbRegisterRDAddr
            )
            {
                ctrlmux3_out="01";
            }

            if (
                MemWbRegWrite[0]=='1' 
                && MemWbRegisterRDAddr!="0000" 
                && !(   ExMemRegWrite[0]=='1' 
                        && ExMemRegisterRDAddr!="0000" 
                        && Reg2Addr==ExMemRegisterRDAddr
                    ) 
                && Reg2Addr==MemWbRegisterRDAddr
            )
            {
                ctrlmux4_out="01";
            }

            if (
                ExMemRegWrite[0]=='1' 
                && ExMemRegisterRDAddr!="0000" 
                && Reg1Addr==ExMemRegisterRDAddr
            )
            {
                ctrlmux3_out="10";
            }

            if (
                ExMemRegWrite[0]=='1' 
                && ExMemRegisterRDAddr!="0000" 
                && Reg2Addr==ExMemRegisterRDAddr
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

class DataMemory        // baadme krlunga
{

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
