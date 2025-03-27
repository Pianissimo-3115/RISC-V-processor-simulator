#include   "components.cpp"


class RV32NonForwarding
{
private:
public:
    Register PC_Reg = Register(32);
    InstructionMemx32 Instr_Mem = InstructionMemx32();
    PCAdder PCAdd4 = PCAdder();
    MUX2x1 PCSrc_MUX = MUX2x1(32); //This is MUX4x1 in WebRISC;
    ANDGate PCSrc_And = ANDGate();
    ORGate IFID_RegFlush_Or = ORGate();
    Register IF_ID_Reg = Register(64);

    RegisterMemx32 Register_Mem = RegisterMemx32();
    HazardDetectionUnitNoFwd HazardDetect_Unit = HazardDetectionUnitNoFwd();
    ControlUnit Control_Unit = ControlUnit();
    ImmediateGen ImmGen = ImmediateGen();
    BranchCmp BranchCmp_Unit = BranchCmp();
    MUX2x1 BranchCmpInp1Src_MUX = MUX2x1(32);
    MUX2x1 BranchCmpInp2Src_MUX = MUX2x1(32);
    MUX2x1 JumpBranch_MUX = MUX2x1(32);
    Adder BranchCalc_Adder = Adder(32);
    Adder JalrCalc_Adder = Adder(32);
    LeftShift BranchImm_LeftShift = LeftShift(32);
    // ORGate IDFlush_OR = ORGate();
    MUX2x1 IDFlush_MUX = MUX2x1(10);
    Register ID_EX_Reg = Register(163);

    ALUx32 ALU = ALUx32();
    ALUControlUnit ALUControl_Unit = ALUControlUnit();
    MUX4x1 ALUSrc1_MUX = MUX4x1(32);
    MUX4x1 ALUSrc2_MUX = MUX4x1(32);
    MUX2x1 EXFlush_WB_MUX = MUX2x1(2);
    MUX2x1 EXFlush_MEM_MUX = MUX2x1(2);
    Register EX_MEM_Reg = Register(76);

    DataMemory DataMemory_Unit = DataMemory();
    Register MEM_WB_Reg = Register(71);

    MUX2x1 WriteData_MUX = MUX2x1(32);

    void MakeConnections(){
        PC_Reg.ConnectOutput(0, 31, &Instr_Mem.PC[0]);
        PC_Reg.ConnectOutput(0, 31, &IF_ID_Reg.Data[0]);
        PC_Reg.ConnectOutput(0, 31, &PCAdd4.Input1[0]);
        Instr_Mem.ConnectOutput(&IF_ID_Reg.Data[32]);
        PCAdd4.ConnectOutput(&PCSrc_MUX.Input1[0]);
        PCSrc_And.ConnectOutput(&PCSrc_MUX.InputSwitch[0]);
        PCSrc_MUX.ConnectOutput(&PC_Reg.Data[0]);
        IFID_RegFlush_Or.ConnectOutput(&IF_ID_Reg.FlushBit[0]);


        //  *** ID PHASE ***
        IF_ID_Reg.ConnectOutput(0, 31, &ID_EX_Reg.Data[10]);
        IF_ID_Reg.ConnectOutput(0, 31, &BranchCalc_Adder.Input1[0]);    
        IF_ID_Reg.ConnectOutput(44, 48, &HazardDetect_Unit.IF_ID_RegisterRs1[0]); //32 -> 31 bit, 63 -> 0bit 63-15 -> 15bit 63-19 -> 19bit //NOTE: Might have swapped registerRs1 and Rs2
        IF_ID_Reg.ConnectOutput(39, 43, &HazardDetect_Unit.IF_ID_RegisterRs2[0]);
        // IF_ID_Reg.ConnectOutput(57, 63, &HazardDetect_Unit.opcode[0]);
        // IF_ID_Reg.ConnectOutput(39, 43, &HazardDetect_Unit.IF_ID_RegisterRs2[0]);
        //IF_ID_Reg.ConnectOutput(49, 51, &Control_Unit.something[0]);    
        IF_ID_Reg.ConnectOutput(57, 63, &Control_Unit.opcode[0]);    
        IF_ID_Reg.ConnectOutput(49, 51, &BranchCmp_Unit.Func3[0]);    
        IF_ID_Reg.ConnectOutput(44, 48, &Register_Mem.ReadReg1[0]);
        IF_ID_Reg.ConnectOutput(39, 43, &Register_Mem.ReadReg2[0]);
        IF_ID_Reg.ConnectOutput(32, 63, &ImmGen.Instruction[0]);
        IF_ID_Reg.ConnectOutput(32, 38, &ID_EX_Reg.Data[10+128]);//funct7
        IF_ID_Reg.ConnectOutput(49, 51, &ID_EX_Reg.Data[10+128+7]);//funct3

        IF_ID_Reg.ConnectOutput(44, 48, &ID_EX_Reg.Data[10+128+10]); //Rs1 hopefully
        IF_ID_Reg.ConnectOutput(39, 43, &ID_EX_Reg.Data[10+128+10+5]); //Rs2 hopefully
        IF_ID_Reg.ConnectOutput(52, 56, &ID_EX_Reg.Data[10+128+10+5+5]); //Rd

        // IF_ID_Reg.ConnectOutput(44, 48, &BranchForward_Unit.IF_ID_RegisterRs1[0]); //These two might have been swapped by mistake. Must check
        // IF_ID_Reg.ConnectOutput(39, 43, &BranchForward_Unit.IF_ID_RegisterRs2[0]);

        HazardDetect_Unit.ConnectOutput(&IDFlush_MUX.InputSwitch[0]);
        HazardDetect_Unit.ConnectOutput(&IF_ID_Reg.StallBit[0]);
        HazardDetect_Unit.ConnectOutput(&PC_Reg.StallBit[0]);

        Control_Unit.ConnectIFFlush(&IFID_RegFlush_Or.Input1[0]);
        Control_Unit.ConnectWriteBack(&IDFlush_MUX.Input1[0], &IDFlush_MUX.Input1[1]);
        Control_Unit.ConnectMemory(&IDFlush_MUX.Input1[2], &IDFlush_MUX.Input1[3]);
        Control_Unit.ConnectALU(&IDFlush_MUX.Input1[6], &IDFlush_MUX.Input1[8], &IDFlush_MUX.Input1[4]);
        Control_Unit.ConnectIsBranch(&BranchCmp_Unit.branch[0]);
        // Control_Unit.ConnectIsBranch(&BranchForward_Unit.Branch[0]);
        Control_Unit.ConnectJalr(&JumpBranch_MUX.InputSwitch[0]);
        Control_Unit.ConnectJump(&PCSrc_And.Input2[0]);

        IDFlush_MUX.Input2 = string(IDFlush_MUX.dataSizeBits, '0');
        IDFlush_MUX.ConnectOutput(&ID_EX_Reg.Data[0]);

        BranchCalc_Adder.ConnectOutput(&JumpBranch_MUX.Input1[0]);
        JalrCalc_Adder.ConnectOutput(&JumpBranch_MUX.Input2[0]);
        JumpBranch_MUX.ConnectOutput(&PCSrc_MUX.Input2[0]);

        BranchCmp_Unit.ConnectOutput(&PCSrc_And.Input1[0]);
        PCSrc_And.ConnectOutput(&IFID_RegFlush_Or.Input2[0]);


        Register_Mem.ConnectData1Output(&JalrCalc_Adder.Input1[0]);
        Register_Mem.ConnectData1Output(&ID_EX_Reg.Data[10+32]);
        // Register_Mem.ConnectData1Output(&BranchCmpInp1Src_MUX.Input1[0]);
        Register_Mem.ConnectData1Output(&BranchCmp_Unit.Value1[0]);
        Register_Mem.ConnectData2Output(&ID_EX_Reg.Data[10+32+32]);
        // Register_Mem.ConnectData2Output(&BranchCmpInp2Src_MUX.Input1[0]);
        Register_Mem.ConnectData2Output(&BranchCmp_Unit.Value2[0]);

        // BranchImm_LeftShift.ConnectOutput(&BranchCalc_Adder.Input2[0]); vv
        ImmGen.ConnectOutput(&BranchCalc_Adder.Input2[0]); //Bypass of path formed by above and below as immediate generated by imgen is already shifted
        // ImmGen.ConnectOutput(&BranchImm_LeftShift.Input[0]); ^^
        ImmGen.ConnectOutput(&JalrCalc_Adder.Input2[0]);
        ImmGen.ConnectOutput(&ID_EX_Reg.Data[10+32+32+32]);

        // BranchForward_Unit.ConnectBranchCMPMux1(&BranchCmpInp1Src_MUX.InputSwitch[0]);
        // BranchForward_Unit.ConnectBranchCMPMux2(&BranchCmpInp2Src_MUX.InputSwitch[0]);
        // BranchCmpInp1Src_MUX.ConnectOutput(&BranchCmp_Unit.Value1[0]);
        // BranchCmpInp2Src_MUX.ConnectOutput(&BranchCmp_Unit.Value2[0]);


        // *** EX PHASE (Not eligible - Prereq not satisfied) ***
        ID_EX_Reg.ConnectOutput(0, 3, &EX_MEM_Reg.Data[0]);
        // ID_EX_Reg.ConnectOutput(1, 1, &HazardDetect_Unit.ID_EX_Memread[0]);
        ID_EX_Reg.ConnectOutput(0, 0, &HazardDetect_Unit.ID_EX_RegWrite[0]);
        ID_EX_Reg.ConnectOutput(4, 5, &ALUControl_Unit.ALUOp[0]);
        ID_EX_Reg.ConnectOutput(6, 7, &ALUSrc1_MUX.InputSwitch[0]);
        ID_EX_Reg.ConnectOutput(8, 9, &ALUSrc2_MUX.InputSwitch[0]);
        ID_EX_Reg.ConnectOutput(10, 41, &ALUSrc1_MUX.Input2[0]);
        // ID_EX_Reg.ConnectOutput(42, 73, &Input1Forwarding_MUX.Input1[0]);
        ID_EX_Reg.ConnectOutput(42, 73, &ALUSrc1_MUX.Input1[0]);
        // ID_EX_Reg.ConnectOutput(74, 105, &Input2Forwarding_MUX.Input1[0]);
        ID_EX_Reg.ConnectOutput(74, 105, &ALUSrc2_MUX.Input1[0]);
        ID_EX_Reg.ConnectOutput(74, 105, &EX_MEM_Reg.Data[2+2+32]);
        ID_EX_Reg.ConnectOutput(106, 137, &ALUSrc2_MUX.Input2[0]);
        ID_EX_Reg.ConnectOutput(138, 144, &ALUControl_Unit.Func7[0]);
        ID_EX_Reg.ConnectOutput(145, 147, &ALUControl_Unit.Func3[0]);
        ID_EX_Reg.ConnectOutput(145, 147, &EX_MEM_Reg.Data[73]); //Func3
        // ID_EX_Reg.ConnectOutput(148, 152, &ALUForwarding_Unit.Reg1Addr[0]); //Rs1
        // ID_EX_Reg.ConnectOutput(153, 157, &ALUForwarding_Unit.Reg2Addr[0]); //Rs2
        ID_EX_Reg.ConnectOutput(158, 162, &HazardDetect_Unit.ID_EX_RegisterRd[0]); //Rd
        ID_EX_Reg.ConnectOutput(158, 162, &EX_MEM_Reg.Data[68]); //Rd

        // Input1Forwarding_MUX.ConnectOutput(&ALUSrc1_MUX.Input1[0]);
        // Input2Forwarding_MUX.ConnectOutput(&ALUSrc2_MUX.Input1[0]);
        // Input2Forwarding_MUX.ConnectOutput(&EX_MEM_Reg.Data[36]);

        ALUSrc1_MUX.ConnectOutput(&ALU.Input1[0]);
        ALUSrc1_MUX.Input3 = string(ALUSrc1_MUX.dataSizeBits, '0');
        ALUSrc2_MUX.ConnectOutput(&ALU.Input2[0]);
        ALUSrc2_MUX.Input3 = string(ALUSrc2_MUX.dataSizeBits-3, '0')+"100"; // Set to 4

        ALUControl_Unit.ConnectOutput(&ALU.ALUControl[0]);

        ALU.ConnectResult(&EX_MEM_Reg.Data[4]);

        // ALUForwarding_Unit.ConnectCtrlMUX3(&Input1Forwarding_MUX.InputSwitch[0]);
        // ALUForwarding_Unit.ConnectCtrlMUX4(&Input2Forwarding_MUX.InputSwitch[0]);

        // *** MEM Phase *** 
        EX_MEM_Reg.ConnectOutput(0, 1, &MEM_WB_Reg.Data[0]); //RegWrite, MemToReg
        EX_MEM_Reg.ConnectOutput(0, 0, &HazardDetect_Unit.EX_MEM_RegWrite[0]); //RegWrite
        // EX_MEM_Reg.ConnectOutput(0, 0, &BranchForward_Unit.EX_MEM_RegWrite[0]); //RegWrite
        // EX_MEM_Reg.ConnectOutput(0, 0, &ALUForwarding_Unit.EX_MEM_RegWrite[0]); //RegWrite
        // EX_MEM_Reg.ConnectOutput(2, 2, &BranchForward_Unit.EX_MEM_MemRead[0]); //MemRead
        EX_MEM_Reg.ConnectOutput(2, 2, &DataMemory_Unit.MemRead[0]);
        // EX_MEM_Reg.ConnectOutput(2, 2, &HazardDetect_Unit.EX_MEM_Memread[0]);
        EX_MEM_Reg.ConnectOutput(3, 3, &DataMemory_Unit.MemWrite[0]);
        EX_MEM_Reg.ConnectOutput(4, 35, &DataMemory_Unit.Address[0]);
        EX_MEM_Reg.ConnectOutput(4, 35, &MEM_WB_Reg.Data[2+32]); //ALU output  
        // EX_MEM_Reg.ConnectOutput(4, 35, &Input1Forwarding_MUX.Input3[0]); //ALU output  
        // EX_MEM_Reg.ConnectOutput(4, 35, &Input2Forwarding_MUX.Input3[0]); //ALU output  
        // EX_MEM_Reg.ConnectOutput(4, 35, &BranchCmpInp1Src_MUX.Input2[0]); //ALU output  
        // EX_MEM_Reg.ConnectOutput(4, 35, &BranchCmpInp2Src_MUX.Input2[0]); //ALU output  
        EX_MEM_Reg.ConnectOutput(36, 67, &DataMemory_Unit.WriteData[0]);
        EX_MEM_Reg.ConnectOutput(68, 72, &MEM_WB_Reg.Data[2+32+32]); //Rd
        // EX_MEM_Reg.ConnectOutput(68, 72, &ALUForwarding_Unit.EX_MEM_RegisterRDAddr[0]); //Rd
        // EX_MEM_Reg.ConnectOutput(68, 72, &BranchForward_Unit.EX_MEM_RegisterRDAddr[0]); //Rd
        EX_MEM_Reg.ConnectOutput(68, 72, &HazardDetect_Unit.EX_MEM_RegisterRd[0]); //Rd
        EX_MEM_Reg.ConnectOutput(73, 75, &DataMemory_Unit.Func3[0]); //Func3

        DataMemory_Unit.ConnectOutput(&MEM_WB_Reg.Data[2]);

        // *** WB Phase ***
        MEM_WB_Reg.ConnectOutput(0, 0, &Register_Mem.WriteEnable[0]); //RegWrite
        // MEM_WB_Reg.ConnectOutput(0, 0, &ALUForwarding_Unit.MEM_WB_RegWrite[0]);
        MEM_WB_Reg.ConnectOutput(1, 1, &WriteData_MUX.InputSwitch[0]); //MemToReg
        MEM_WB_Reg.ConnectOutput(2, 33, &WriteData_MUX.Input2[0]);
        MEM_WB_Reg.ConnectOutput(34, 65, &WriteData_MUX.Input1[0]);
        // MEM_WB_Reg.ConnectOutput(66, 70, &ALUForwarding_Unit.MEM_WB_RegisterRDAddr[0]);
        // MEM_WB_Reg.ConnectOutput(66, 70, &BranchForward_Unit.MEM_WB_RegisterRDAddr[0]);
        MEM_WB_Reg.ConnectOutput(66, 70, &Register_Mem.WriteReg[0]);
        
        // WriteData_MUX.ConnectOutput(&Input1Forwarding_MUX.Input2[0]);
        // WriteData_MUX.ConnectOutput(&Input2Forwarding_MUX.Input2[0]);
        WriteData_MUX.ConnectOutput(&Register_Mem.WriteData[0]);

    }


    void DoCycle()
    {
        MEM_WB_Reg.Step();
        EX_MEM_Reg.Step();
        ID_EX_Reg.Step();
        IF_ID_Reg.Step();
        PC_Reg.Step();

        WriteData_MUX.Step();
        Register_Mem.StepWrite();

        PCAdd4.Step();
        Instr_Mem.Step();

        ImmGen.Step();
        Control_Unit.Step();
        // BranchImm_LeftShift.Step();
        HazardDetect_Unit.Step();
        IDFlush_MUX.Step();
        Register_Mem.StepRead();
        BranchCalc_Adder.Step();
        JalrCalc_Adder.Step();
        JumpBranch_MUX.Step();
        // BranchForward_Unit.Step();
        BranchCmpInp1Src_MUX.Step();
        BranchCmpInp2Src_MUX.Step();
        BranchCmp_Unit.Step();

        PCSrc_And.Step();
        PCSrc_MUX.Step();
        IFID_RegFlush_Or.Step();

        // ALUForwarding_Unit.Step();
        // Input1Forwarding_MUX.Step();
        // Input2Forwarding_MUX.Step();
        ALUSrc1_MUX.Step();
        ALUSrc2_MUX.Step();
        ALUControl_Unit.Step();
        ALU.Step();

        DataMemory_Unit.StepWrite();
        DataMemory_Unit.StepRead();

    }

    void LoadInstructions(vector<unsigned> instructions){
        for (auto instr: instructions)
        {
            Instr_Mem.AddInstruction(instr);
        }
        
    }

};