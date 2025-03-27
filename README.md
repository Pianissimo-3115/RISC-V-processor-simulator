# RISC-V Pipeline Simulator

## Design Decisions

In our implementation, we chose to build a complete RISC-V simulator rather than limiting it to just outputting the pipeline diagram. The simulator replicates the hardware logic as closely as possible.

To achieve this, we designed individual classes for every minute component, such as:
- MUXes
- AND gates
- OR gates
- ADDERs

This decision was made to minimize C++ abstraction and ensure the implementation closely mirrors actual hardware behavior.

## Possible Improvements
1. Implementing exception handling
2. Expanding the design to support 64-bit architecture
3. Adding branch prediction functionality

## Sources
- [WebRISC-V Model for Reference](https://webriscv.dii.unisi.it/index.php)
- [Opcodes, ALUOp, and ALUControl Tables](https://www.cs.sfu.ca~ashriram/Courses/CS295/assets/notebooks/RISCV/RISCV_CARD.pdf)
- [RISC-V Specification for Instruction Structure](https://csg.csail.mit.edu/6.375/6_375_2019_www/resources/riscv-spec.pdf)

