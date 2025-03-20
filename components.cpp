
#include <iostream>
#include <string>
#include <cstring>
#include <tuple>
#include <cstdint>
#include <vector>
// #include <bitset>
using namespace std;

void intToBitsCharArray(unsigned num, char output[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        output[size - 1 - i] = (num & (1 << i)) ? '1' : '0';  // Extract bit
    }
    output[size] = '\0';  // Null-terminate for safety
}



class MUX2x2 {
private:
    char * Output[8] = {NULL};
public:
    char *Input1 = NULL;
    char *Input2 = NULL;
    char *InputSwitch = NULL;
    size_t dataSizeBits;

    MUX2x2(size_t dataSizeBitsInput) {
        dataSizeBits = dataSizeBitsInput;
        Input1 = new char[dataSizeBits+1];
        memset(Input1, '0', dataSizeBits);
        Input2 = new char[dataSizeBits+1];
        memset(Input2, '0', dataSizeBits);
        InputSwitch = new char[1]{'0'};
    }


    void Reset() {
        memset(Input1, '0', dataSizeBits);
        memset(Input2, '0', dataSizeBits);
        *InputSwitch = {'0'};
    }

    void ConnectOutput(char * connection)
    {
        int i = 0;
        while (i<8)
        {
            if (Output[i]==NULL)
            {
                Output[i] = connection;
                return;
            }
            i++;
        }
        printf("No more outputs can be connected. Connection not made.");
        return;
    }

    void Step()
    {
        char * chosen;
        if ((*InputSwitch) == '0')
        {
            chosen = Input1;
        }
        else {
            chosen = Input2;
        }
        int i = 0;
        while (i<8)
        {
            if (Output[i]==NULL)
            {
                break;
            }
            for (int j = 0; j < dataSizeBits; j++)
            {
                *(Output[i]+j) = *(chosen+j);
            }
            i++;
        }
        
    }

    ~MUX2x2()
    {
        delete Input1;
        delete Input2;
        delete InputSwitch;
    }

};

class MUX4x4 {

private:
    char * Output[8] = {NULL};
public:
    char *Input1 = NULL;
    char *Input2 = NULL;
    char *Input3 = NULL;
    char *Input4 = NULL;
    char *InputSwitch = NULL;
    size_t dataSizeBits;

    MUX4x4(size_t dataSizeBitsInput) {
        dataSizeBits = dataSizeBitsInput;
        Input1 = new char[dataSizeBits+1];
        memset(Input1, '0', dataSizeBits);
        Input2 = new char[dataSizeBits+1];
        memset(Input2, '0', dataSizeBits);
        Input3 = new char[dataSizeBits+1];
        memset(Input3, '0', dataSizeBits);
        Input4 = new char[dataSizeBits+1];
        memset(Input4, '0', dataSizeBits);
        InputSwitch = new char[2];
        memset(InputSwitch, '0', 2);
    }


    void Reset() {
        memset(Input1, '0', dataSizeBits);
        memset(Input2, '0', dataSizeBits);
        memset(Input3, '0', dataSizeBits);
        memset(Input4, '0', dataSizeBits);
        memset(InputSwitch, '0', 2);
        
    }

    void ConnectOutput(char * connection)
    {
        int i = 0;
        while (i<8)
        {
            if (Output[i]==NULL)
            {
                Output[i] = connection;
                return;
            }
            i++;
        }
        printf("No more outputs can be connected. Connection not made.");
        return;
    }

    void Step()
    {
        char * chosen;
        if ((*InputSwitch) == '0')
        {
            if (*(InputSwitch+1) == '0') chosen = Input1;
            else chosen = Input2;
        }
        else {
            if (*(InputSwitch+1) == '0') chosen = Input3;
            else chosen = Input4;
        }
        
        int i = 0;
        while (i<8)
        {
            if (Output[i]==NULL)
            {
                break;
            }
            for (int j = 0; j < dataSizeBits; j++)
            {
                *(Output[i]+j) = *(chosen+j);
            }
            i++;
        }
        
    }

    ~MUX4x4()
    {
        delete Input1;
        delete Input2;
        delete Input3;
        delete Input4;
        delete InputSwitch;
    }

};

class Register {

private:
    tuple<int, int, char*> OutputSet[32]{};
public:
    char *Data = NULL;
    char *Write = NULL;
    char *Flush = NULL;
    size_t dataSizeBits;

    Register(size_t dataSizeBitsInput) {
        dataSizeBits = dataSizeBitsInput;
        Data = new char[dataSizeBits+1];
        memset(Data, '0', dataSizeBits);
        Write = new char[1+1];
        *Write = '0';
        Flush = new char[1+1];
        *Flush = '0';
    }


    void Reset() {
        memset(Data, '0', dataSizeBits);
        *Flush = '0';
        *Write = '0';
    }

    void ConnectOutput(unsigned int start, unsigned int end, char* connection)
    {
        if (start>=dataSizeBits || end>=dataSizeBits || end<start || connection==NULL)
        {
            printf("Invalid connection. Connection not made.");
            return;
        }
        
        int i = 0;
        while (i<32)
        {
            if (get<2>(OutputSet[i])==NULL)
            {
                OutputSet[i] = make_tuple(start, end, connection);
                return;
            }
            i++;
        }
        printf("No more outputs can be connected. Connection not made.");
        return;
    }

    void Step()
    {
        if (*Flush == '1')
        {
            memset(Data, '0', dataSizeBits);    
        }
        if (*Write == '1' || *Flush == '1')
        {
            int i = 0;
            while (i<32)
            {
                if (get<2>(OutputSet[i])==NULL)
                {
                    break;
                }
                int start = get<0>(OutputSet[i]);
                int end = get<1>(OutputSet[i]);
                char* out = get<2>(OutputSet[i]);
                for (int j = 0; j < end - start + 1; j++)
                {
                    *(out+j) = *(Data+start+j);
                }
                i++;
            }
        }
    }

    ~Register()
    {
        delete Data;
        delete Write;
        delete Flush;
    }

};
    

class ALUx32 {
private:
    char *Result[8] = {NULL};
    char *Zero[8] = {NULL};
    // char *Overflow[8] = {NULL};
    // char *CarryOut[8] = {NULL};

public:
    char *Input1 = NULL;
    char *Input2 = NULL;
    char *ALUOp = NULL;
    size_t dataSizeBits;

    ALUx32() {
        Input1 = new char[32+1];
        memset(Input1, '0', 32);
        Input2 = new char[32+1];
        memset(Input2, '0', 32);
        ALUOp = new char[4+1];
        memset(ALUOp, '0', 4);
    }


    void Reset() {
        memset(Input1, '0', 32);
        memset(Input2, '0', 32);
        memset(ALUOp, '0', 4);
    }

    void ConnectResult(char * connection)
    {
        int i = 0;
        while (i<8)
        {
            if (Result[i]==NULL)
            {
                Result[i] = connection;
                return;
            }
            i++;
        }
        printf("No more outputs can be connected. Connection not made.");
        return;
    }

    void ConnectZero(char * connection)
    {
        int i = 0;
        while (i<8)
        {
            if (Zero[i]==NULL)
            {
                Zero[i] = connection;
                return;
            }
            i++;
        }
        printf("No more outputs can be connected. Connection not made.");
        return;
    }

    // void ConnectOverflow(char * connection)
    // {
    //     int i = 0;
    //     while (i<8)
    //     {
    //         if (Overflow[i]==NULL)
    //         {
    //             Overflow[i] = connection;
    //             return;
    //         }
    //         i++;
    //     }
    //     printf("No more outputs can be connected. Connection not made.");
    //     return;
    // }

    // void ConnectCarryOut(char * connection)
    // {
    //     int i = 0;
    //     while (i<8)
    //     {
    //         if (CarryOut[i]==NULL)
    //         {
    //             CarryOut[i] = connection;
    //             return;
    //         }
    //         i++;
    //     }
    //     printf("No more outputs can be connected. Connection not made.");
    //     return;
    // }

    void Step()
    {
        char Result_Out[32+1];
        // char Overflow_Out[1+1];
        // char CarryOut_Out[1+1];
        char Zero_Out[1+1];
        memset(Result_Out, '0', 32);
        if (strcmp(ALUOp, "00000")==0) //"00000" => AND
        {
            uint32_t val1 = stoul(Input1, nullptr, 2);
            uint32_t val2 = stoul(Input2, nullptr, 2);
            uint32_t out = val1 & val2;
            Zero_Out[0] = (out == 0) ? '1' : '0';
            intToBitsCharArray(out, Result_Out, 32);
        }
        else if (strcmp(ALUOp, "00001")==0) //"00001" => OR
        {
            uint32_t val1 = stoul(Input1, nullptr, 2);
            uint32_t val2 = stoul(Input2, nullptr, 2);
            uint32_t out = val1 | val2;
            Zero_Out[0] = (out == 0) ? '1' : '0';
            intToBitsCharArray(out, Result_Out, 32);
        }
        else if (strcmp(ALUOp, "00011")==0) //"00011" => XOR
        {
            uint32_t val1 = stoul(Input1, nullptr, 2);
            uint32_t val2 = stoul(Input2, nullptr, 2);
            uint32_t out = val1 ^ val2;
            Zero_Out[0] = (out == 0) ? '1' : '0';
            intToBitsCharArray(out, Result_Out, 32);
        }
        else if (strcmp(ALUOp, "00010")==0) //"00010" => add
        {
            uint32_t val1 = stoul(Input1, nullptr, 2);
            uint32_t val2 = stoul(Input2, nullptr, 2);
            uint32_t out = val1 + val2;
            Zero_Out[0] = (out == 0) ? '1' : '0';
            intToBitsCharArray(out, Result_Out, 32);
        }
        else if (strcmp(ALUOp, "00110")==0) //"00110" => sub
        {
            uint32_t val1 = stoul(Input1, nullptr, 2);
            uint32_t val2 = stoul(Input2, nullptr, 2);
            uint32_t out = val1 - val2;
            Zero_Out[0] = (out == 0) ? '1' : '0';
            intToBitsCharArray(out, Result_Out, 32);
        }
        else if (strcmp(ALUOp, "00100")==0) //"00100" => slt
        {
            int32_t val1 = stol(Input1, nullptr, 2);
            int32_t val2 = stol(Input2, nullptr, 2);
            uint32_t out = (val1 < val2) ? 1 : 0;
            Zero_Out[0] = (out == 0) ? '1' : '0';
            intToBitsCharArray(out, Result_Out, 32);
        }
        else if (strcmp(ALUOp, "00101")==0) //"00110" => sltu
        {
            uint32_t val1 = stoul(Input1, nullptr, 2);
            uint32_t val2 = stoul(Input2, nullptr, 2);
            uint32_t out = (val1 < val2) ? 1 : 0;
            Zero_Out[0] = (out == 0) ? '1' : '0';
            intToBitsCharArray(out, Result_Out, 32);
        }
        else if (strcmp(ALUOp, "01000")==0) //"01000" => mul //CONFIRM ALL THE TYPE CONVERSION AND SIGNED AND UNSIGNED STUF IN MUL MULH MULHU etc
        {
            uint32_t val1 = stoul(Input1, nullptr, 2);
            uint32_t val2 = stoul(Input2, nullptr, 2);
            uint32_t out = val1*val2;
            Zero_Out[0] = (out == 0) ? '1' : '0';
            intToBitsCharArray(out, Result_Out, 32);
        }
        else if (strcmp(ALUOp, "01001")==0) //"01001" => mulh
        {
            int32_t val1 = stol(Input1, nullptr, 2);
            int32_t val2 = stol(Input2, nullptr, 2);
            int64_t out = (int64_t) val1* (int64_t) val2;
            out >>= 32;
            Zero_Out[0] = (out == 0) ? '1' : '0';
            intToBitsCharArray(out, Result_Out, 32);
        }
        else if (strcmp(ALUOp, "01011")==0) //"01011" => mulhu
        {
            uint32_t val1 = stoul(Input1, nullptr, 2);
            uint32_t val2 = stoul(Input2, nullptr, 2);
            uint64_t out = (uint64_t) val1* (uint64_t) val2;
            out >>= 32;
            Zero_Out[0] = (out == 0) ? '1' : '0';
            intToBitsCharArray(out, Result_Out, 32);
        }
        else if (strcmp(ALUOp, "11000")==0) //"11000" => div
        {
            int32_t val1 = stol(Input1, nullptr, 2);
            int32_t val2 = stol(Input2, nullptr, 2);
            uint32_t out = val1 / val2; //Casted to unsigned type so that we are able to extract the bits easily.
            Zero_Out[0] = (out == 0) ? '1' : '0';
            intToBitsCharArray(out, Result_Out, 32);
        }
        else if (strcmp(ALUOp, "11001")==0) //"11001" => divu
        {
            uint32_t val1 = stoul(Input1, nullptr, 2);
            uint32_t val2 = stoul(Input2, nullptr, 2);
            uint32_t out = val1 / val2;
            Zero_Out[0] = (out == 0) ? '1' : '0';
            intToBitsCharArray(out, Result_Out, 32);
        }
        else if (strcmp(ALUOp, "11010")==0) //"11010" => rem
        {
            int32_t val1 = stol(Input1, nullptr, 2);
            int32_t val2 = stol(Input2, nullptr, 2);
            uint32_t out = val1 % val2; //Casted to unsigned type so that we are able to extract the bits easily.
            Zero_Out[0] = (out == 0) ? '1' : '0';
            intToBitsCharArray(out, Result_Out, 32);
        }
        else if (strcmp(ALUOp, "11011")==0) //"11011" => remu
        {
            uint32_t val1 = stoul(Input1, nullptr, 2);
            uint32_t val2 = stoul(Input2, nullptr, 2);
            uint32_t out = val1 % val2;
            Zero_Out[0] = (out == 0) ? '1' : '0';
            intToBitsCharArray(out, Result_Out, 32);
        }
        else if (strcmp(ALUOp, "01101")==0) //"01101" => sll
        {
            uint32_t val1 = stoul(Input1, nullptr, 2);
            uint32_t val2 = stoul(Input2, nullptr, 2);
            uint32_t out = val1 << val2;
            Zero_Out[0] = (out == 0) ? '1' : '0';
            intToBitsCharArray(out, Result_Out, 32);
        }
        else if (strcmp(ALUOp, "01110")==0) //"01110" => srl
        {
            uint32_t val1 = stoul(Input1, nullptr, 2);
            uint32_t val2 = stoul(Input2, nullptr, 2);
            uint32_t out = val1 >> val2;
            Zero_Out[0] = (out == 0) ? '1' : '0';
            intToBitsCharArray(out, Result_Out, 32);
        }
        else if (strcmp(ALUOp, "01111")==0) //"01111" => sra
        {
            int32_t val1 = stoul(Input1, nullptr, 2);
            uint32_t val2 = stoul(Input2, nullptr, 2);
            uint32_t out = val1 >> val2; //Casted to unsigned type so that we are able to extract the bits easily.
            Zero_Out[0] = (out == 0) ? '1' : '0';
            intToBitsCharArray(out, Result_Out, 32);
        }
        //Todo but not know if required: 10010:addw, 10110:subw, 11101:sllw, 11110:srlw, 11111:sraw, 10011:mulw, 10000:divw, 10001:divuw, 10100:remw, 10101:remuw.
        //Not tested all these cases properly. ( :-[ )

        int i = 0;
        while (i<8)
        {
            if (Result[i]==NULL)
            {
                break;
            }
            for (int j = 0; j < 32; j++)
            {
                *(Result[i]+j) = *(Result_Out+j);
            }
            i++;
        }
        
        i = 0;
        while (i<8)
        {
            if (Zero[i]==NULL)
            {
                break;
            }
            for (int j = 0; j < 1; j++)
            {
                *(Zero[i]+j) = *(Zero_Out+j);
            }
            i++;
        }

        // i = 0;
        // while (i<8)
        // {
        //     if (Overflow[i]==NULL)
        //     {
        //         break;
        //     }
        //     for (int j = 0; j < 1; j++)
        //     {
        //         *(Overflow[i]+j) = *(Overflow_Out+j);
        //     }
        //     i++;
        // }

        // i = 0;
        // while (i<8)
        // {
        //     if (CarryOut[i]==NULL)
        //     {
        //         break;
        //     }
        //     for (int j = 0; j < 1; j++)
        //     {
        //         *(CarryOut[i]+j) = *(CarryOut_Out+j);
        //     }
        //     i++;
        // }


    }

    ~ALUx32()
    {
        delete Input1;
        delete Input2;
        delete ALUOp;
    }

};

class RegisterMemx32 {
private:
    (char *) Data1Outputs[8] = {NULL};
    (char *) Data2Outputs[8] = {NULL};
    char Registers[32][32+1];
    
public:
    char *ReadReg1 = NULL;
    char *ReadReg2 = NULL;
    char *WriteReg = NULL;
    char *WriteData = NULL;
    char *RegWrite = NULL;

    RegisterMemx32() {
        ReadReg1 = new char[5+1];
        memset(ReadReg1, '0', 5);
        ReadReg2 = new char[5+1];
        memset(ReadReg2, '0', 5);
        WriteReg = new char[5+1];
        memset(WriteReg, '0', 5);
        WriteData = new char[32+1];
        memset(WriteData, '0', 32);
        RegWrite = new char[1+1];
        (*RegWrite) = '0';
        
        for (int i = 0; i < 32; i++)
        {
            for (int j = 0; j < 32; j++)
            {
                Registers[i][j] = '0';
            }
        }
        
        
    }


    void Reset() {
        memset(ReadReg1, '0', 5);
        memset(ReadReg2, '0', 5);
        memset(WriteReg, '0', 5);
        memset(WriteData, '0', 32);
        (*RegWrite) = '0';
        
        for (int i = 0; i < 32; i++)
        {
            for (int j = 0; j < 32; j++)
            {
                Registers[i][j] = '0';
            }
        }
    }

    void ConnectData1(char * connection)
    {
        int i = 0;
        while (i<8)
        {
            if (Data1Outputs[i]==NULL)
            {
                Data1Outputs[i] = connection;
                return;
            }
            i++;
        }
        printf("No more outputs can be connected. Connection not made.");
        return;
    }

    void ConnectData2(char * connection)
    {
        int i = 0;
        while (i<8)
        {
            if (Data2Outputs[i]==NULL)
            {
                Data2Outputs[i] = connection;
                return;
            }
            i++;
        }
        printf("No more outputs can be connected. Connection not made.");
        return;
    }

    void StepRead()
    {
        int addr1 = stoi(ReadReg1, nullptr, 2);
        int addr2 = stoi(ReadReg1, nullptr, 2);
        int i = 0;
        while (i<8)
        {
            if (Data1Outputs[i]==NULL)
            {
                break;
            }
            for (int j = 0; j < 32; j++)
            {
                *(Data1Outputs[i]+j) = *(Registers[addr1]+j);
            }
            i++;
        }

        i = 0;
        while (i<8)
        {
            if (Data2Outputs[i]==NULL)
            {
                break;
            }
            for (int j = 0; j < 32; j++)
            {
                *(Data2Outputs[i]+j) = *(Registers[addr2]+j);
            }
            i++;
        }
        
    }

    ~RegisterMemx32()
    {
        delete ReadReg1;
        delete ReadReg2;
        delete WriteReg;
        delete WriteData;
        delete RegWrite;
    }
};


class InstructionMemx32 {
private:
    (char *) InstrOutputs[8] = {NULL};
    vector<uint32_t> Instructions;
    int InstrCount = 0;
public:
    char *InstAddr = NULL;

    InstructionMemx32() {
        InstAddr = new char[32+1];
        memset(InstAddr, '0', 32);

    }

    void Reset() {
        memset(InstAddr, '0', 32);
        Instructions.clear();
        InstrCount = 0;
    }

    void ConnectOutput(char * connection)
    {
        int i = 0;
        while (i<8)
        {
            if (InstrOutputs[i]==NULL)
            {
                InstrOutputs[i] = connection;
                return;
            }
            i++;
        }
        printf("No more outputs can be connected. Connection not made.");
        return;
    }

    void AddInstruction(uint32_t instr)
    {
        Instructions.push_back(instr);
        InstrCount++;
    }

    void Step()
    {
        uint32_t addr = (stoi(InstAddr, nullptr, 2)) >> 2; //Note: confirm this line. shift right by 2 is for dividing by 4 because instruction is 4 byte.
        if (addr/4 >= InstrCount)
        {
            printf("Instruction Address out of range.");
            return;
        }
        
        int i = 0;
        char curr_instruction[32];
        intToBitsCharArray(Instructions[addr], curr_instruction, 32);
        while (i<8)
        {
            if (InstrOutputs[i]==NULL)
            {
                break;
            }

            for (int j = 0; j < 32; j++)
            {
                *(InstrOutputs[i]+j) = *(curr_instruction+j);
            }
            i++;
        }
    }

    ~InstructionMemx32()
    {
        delete InstAddr;
    }
};

class ImmediateGen {
private:
    (char *) ImmOutputs[8] = {NULL};

public:
    char *Instruction = NULL;

    ImmediateGen() {
        Instruction = new char[32+1];
        memset(Instruction, '0', 32);

    }

    void Reset() {
        memset(Instruction, '0', 32);
    }

    void ConnectOutput(char * connection)
    {
        int i = 0;
        while (i<8)
        {
            if (ImmOutputs[i]==NULL)
            {
                ImmOutputs[i] = connection;
                return;
            }
            i++;
        }
        printf("No more outputs can be connected. Connection not made.");
        return;
    }

    void Step()
    {
        
        char immediate[32+1];
        if (strcmp(Instruction+25, "0110011")==0 || strcmp(Instruction+25, "0111011")==0) //Rtype
        {
            memset(immediate, '0', 32);
        }
        else if (strcmp(Instruction+25, "0000011")==0 || strcmp(Instruction+25, "0010011")==0 || strcmp(Instruction+25, "0011011")==0 || strcmp(Instruction+25, "1100111")==0 || strcmp(Instruction+25, "1110011")==0) //I-type
        {
            memset(immediate, Instruction[0], 21);
            strncpy(immediate+21, Instruction+1, 6);
            strncpy(immediate+27, Instruction+7, 4);
            immediate[31] = Instruction[11];
        }
        else if (strcmp(Instruction+25, "0100011")==0) //S type
        {
            memset(immediate, Instruction[0], 21);
            strncpy(immediate+21, Instruction+1, 6);
            strncpy(immediate+27, Instruction+21, 4);
            immediate[31] = Instruction[24];
        }
        else if (strcmp(Instruction+25, "1100011")==0) //SB type
        {
            memset(immediate, Instruction[0], 20);
            immediate[20] = Instruction[24];
            strncpy(immediate+21, Instruction+1, 6);
            strncpy(immediate+27, Instruction+21, 4);
            immediate[31] = '0';
        }
        else if (strcmp(Instruction+25, "0010111")==0 || strcmp(Instruction+25, "0110111")==0) //Utype
        {
            strncpy(immediate, Instruction, 20);
            memset(immediate+20, '0', 12);
        }
        else if (strcmp(Instruction+25, "1101111")==0) //UJ type
        {
            memset(immediate, Instruction[0], 12);
            strncpy(immediate+12, Instruction+12, 8);
            immediate[20] = Instruction[11];
            strncpy(immediate+21, Instruction+1, 10);
            immediate[31] = '0';
        }
        
        
        int i = 0;
        while (i<8)
        {
            if (ImmOutputs[i]==NULL)
            {
                break;
            }

            for (int j = 0; j < 32; j++)
            {
                *(ImmOutputs[i]+j) = *(immediate+j);
            }
            i++;
        }
    }

    ~ImmediateGen()
    {
        delete Instruction;
    }
};

enum ControlOutputs {IF_FLUSH, ID_FLUSH, EX_FLUSH, CONTROL_WB, CONTROL_M, CONTROL_EX, IS_BRANCH, IS_JALR, EXCEPTION, JUMP};
class ControlUnit {
private:
    (char *) IF_Flush[8] = {NULL}; //1 Bit
    (char *) ID_Flush[8] = {NULL}; //1 Bit
    (char *) EX_Flush[8] = {NULL}; //1 Bit

    (char *) Control_WB[8] = {NULL}; //2 Bit Mem2Reg(1) RegWrite(1)
    (char *) Control_M[8] = {NULL}; //5 Bit??? MemWrite(1) MemRead(1)
    (char *) Control_EX[8] = {NULL}; //6 Bit ALUSrc1(2) ALUSrc2(2) ALUOp(2)

    (char *) IsBranch[8] = {NULL}; //1 Bit
    (char *) IsJalr[8] = {NULL}; //1 Bit

    (char *) Exception[8] = {NULL}; //1 Bit
    (char *) Jump[8] = {NULL}; //1 Bit
    
public:
    char *Instruction = NULL;

    ControlUnit() {
        Instruction = new char[32+1];
        memset(Instruction, '0', 32);

    }

    void Reset() {
        memset(Instruction, '0', 32);
    }

    void ConnectOutput(ControlOutputs type, char * connection)
    {

        int i = 0;
        char ** selected;
        switch (type)
        {
            case IF_FLUSH:
                selected = IF_Flush;
                break;
            case ID_FLUSH:
                selected = ID_Flush;
                break;
            case EX_FLUSH:
                selected = EX_Flush;
                break;
            case CONTROL_WB:
                selected = Control_WB;
                break;
            case CONTROL_EX:
                selected = Control_EX;
                break;
            case CONTROL_M:
                selected = Control_M;
                break;
            case IS_BRANCH:
                selected = IsBranch;
                break;
            case IS_JALR:
                selected = IsJalr;
                break;
            case EXCEPTION:
                selected = Exception;
                break;
            case JUMP:
                selected = Jump;
                break;
            default:
                break;
        }
        while (i<8)
        {
            if (selected[i]==NULL)
            {
                selected[i] = connection;
                return;
            }
            i++;
        }
        printf("No more outputs can be connected. Connection not made.");
        return;
    }

    void Step()
    {
        
        char immediate[32+1];
        if (strcmp(Instruction+25, "0110011")==0 || strcmp(Instruction+25, "0111011")==0) //Rtype
        {
            memset(immediate, '0', 32);
        }
        else if (strcmp(Instruction+25, "0000011")==0 || strcmp(Instruction+25, "0010011")==0 || strcmp(Instruction+25, "0011011")==0 || strcmp(Instruction+25, "1100111")==0 || strcmp(Instruction+25, "1110011")==0) //I-type
        {
            memset(immediate, Instruction[0], 21);
            strncpy(immediate+21, Instruction+1, 6);
            strncpy(immediate+27, Instruction+7, 4);
            immediate[31] = Instruction[11];
        }
        else if (strcmp(Instruction+25, "0100011")==0) //S type
        {
            memset(immediate, Instruction[0], 21);
            strncpy(immediate+21, Instruction+1, 6);
            strncpy(immediate+27, Instruction+21, 4);
            immediate[31] = Instruction[24];
        }
        else if (strcmp(Instruction+25, "1100011")==0) //SB type
        {
            memset(immediate, Instruction[0], 20);
            immediate[20] = Instruction[24];
            strncpy(immediate+21, Instruction+1, 6);
            strncpy(immediate+27, Instruction+21, 4);
            immediate[31] = '0';
        }
        else if (strcmp(Instruction+25, "0010111")==0 || strcmp(Instruction+25, "0110111")==0) //Utype
        {
            strncpy(immediate, Instruction, 20);
            memset(immediate+20, '0', 12);
        }
        else if (strcmp(Instruction+25, "1101111")==0) //UJ type
        {
            memset(immediate, Instruction[0], 12);
            strncpy(immediate+12, Instruction+12, 8);
            immediate[20] = Instruction[11];
            strncpy(immediate+21, Instruction+1, 10);
            immediate[31] = '0';
        }
        
        
        int i = 0;
        while (i<8)
        {
            if (ImmOutputs[i]==NULL)
            {
                break;
            }

            for (int j = 0; j < 32; j++)
            {
                *(ImmOutputs[i]+j) = *(immediate+j);
            }
            i++;
        }
    }

    ~ImmediateGen()
    {
        delete Instruction;
    }
};


int main(){

    char output[4+1];
    char output2[6+1];
    char output3[32+1];
    output3[32] = '\0';
    strcpy(output, "0101");
    strcpy(output2, "000000");
    
    MUX2x2 mux1 = MUX2x2(1);
    MUX2x2 mux2 = MUX2x2(2);
    MUX2x2 mux3 = MUX2x2(2);
    MUX4x4 mux4 = MUX4x4(4);
    Register reg = Register(174);
    ALUx32 alu = ALUx32();
    
    strcpy(mux1.Input1, "0");
    strcpy(mux1.Input2, "1");
    
    strcpy(mux2.Input1, "00");
    strcpy(mux2.Input2, "01");
    
    strcpy(mux3.Input1, "10");
    strcpy(mux3.Input2, "11");
    
    mux1.ConnectOutput(mux2.InputSwitch);
    mux1.ConnectOutput(mux3.InputSwitch);
    
    mux2.ConnectOutput(mux4.Input2);
    mux3.ConnectOutput(mux4.Input2+2);
    
    strcpy(mux1.InputSwitch, "1");
    strcpy(mux4.InputSwitch, "01");
    
    mux4.ConnectOutput(output);
    mux4.ConnectOutput(reg.Data+13);

    reg.ConnectOutput(12,17,output2);
    strcpy(reg.Write, "1");
    strcpy(reg.Flush, "0");

    reg.ConnectOutput(12, 43, alu.Input1);
    reg.ConnectOutput(0, 31, alu.Input2);
    alu.ConnectResult(output3);
    strcpy(alu.ALUOp, "0001");

    mux1.Step();
    mux2.Step();
    mux3.Step();
    mux4.Step();
    reg.Step();
    alu.Step();

    printf("%s\n", output);
    printf("%s\n", output2);
    printf("%s\n", output3);

    

}