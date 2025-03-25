#include <components.cpp>


class RV32Forwarding
{
private:
    Register PC_Reg;
    InstructionMemx32 Instr_Mem;
    PCAdder PCAdd4;
    MUX4x1 PCSrc_MUX;
    ANDGate PCSrc_And;
    Register IF_ID_Reg;

    RegisterMemx32 Register_Mem;
    HazardDetectionUnit HazardDetect_Unit;
    ControlUnit Control_Unit;
    ImmediateGen ImmGen;
    BranchCmp BranchCmp_Unit;
    BranchForwardingUnit BranchForward_Unit;
    MUX2x1 JumpBranch_MUX;
    Adder BranchCalc_Adder;
    Adder JalrCalc_Adder;
    LeftShift BranchImm_LeftShift;
    ORGate IDFlush_OR;
    MUX2x1 IDFlush_MUX;
    Register ID_EX_Reg;

    ALUx32 ALU;
    ALUControlUnit ALUControl_Unit;
    ALUForwardingUnit ALUForwarding_Unit;
    MUX4x1 Input1Forwarding_MUX;
    MUX4x1 Input2Forwarding_MUX;
    MUX4x1 ALUSrc1_MUX;
    MUX4x1 ALUSrc2_MUX;
    MUX2x1 EXFlush_WB_MUX;
    MUX2x1 EXFlush_MEM_MUX;
    Register EX_MEM_Reg;

    DataMemory DataMemory_Unit;
    Register MEM_WB_Reg;

    MUX2x1 WriteData_MUX;
public:
    RV32Forwarding(unsigned int instr_count) //REGISTER SIZES SAHI KARNE HAI
    {
        PC_Reg = Register(32);
        Instr_Mem = InstructionMemx32(instr_count);
        PCAdd4 = PCAdder();
        PCSrc_MUX = MUX4x1();
        PCSrc_And = ANDGate();
        IF_ID_Reg = Register(400);
    
        Register_Mem = RegisterMemx32();
        HazardDetect_Unit = HazardDetectionUnit();
        Control_Unit = ControlUnit();
        ImmGen = ImmediateGen();
        BranchCmp_Unit = BranchCmp();
        BranchForward_Unit = BranchForwardingUnit();
        JumpBranch_MUX = MUX2x1();
        BranchCalc_Adder = Adder();
        JalrCalc_Adder = Adder();
        BranchImm_LeftShift = LeftShift();
        IDFlush_OR = ORGate();
        IDFlush_MUX = MUX2x1();
        ID_EX_Reg = Register(400);
    
        ALU = ALUx32();
        ALUControl_Unit = ALUControlUnit();
        ALUForwarding_Unit = ALUForwardingUnit();
        Input1Forwarding_MUX = MUX4x1();
        Input2Forwarding_MUX = MUX4x1();
        ALUSrc1_MUX = MUX4x1();
        ALUSrc2_MUX = MUX4x1();
        EXFlush_WB_MUX = MUX2x1();
        EXFlush_MEM_MUX = MUX2x1();
        EX_MEM_Reg = Register(400);
    
        DataMemory_Unit = DataMemory();
        MEM_WB_Reg = Register(400);
    
        WriteData_MUX = MUX2x1();
    }

    MakeConnections(){
        PC_Reg.ConnectOutput(0, 31, &Instr_Mem.PC);
        PC_Reg.ConnectOutput(0, 31, &IF_ID_Reg.Data);
        PC_Reg.ConnectOutput(0, 31, &PCAdd4.PC);
        Instr_Mem.ConnectOutput((&IF_ID_Reg.Data)+32)

    }



    ~RV32Forwarding();
};