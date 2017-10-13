# TSE Z80 Emulator

Implemented instructions:

- NOP
- ADD A,r
- SUB A,r
- ADC A,r
- AND A,r
- XOR A,r
- OR A,r
- LD r,n
- LD (HL),n
- LD (BC),A
- LD (DE),A
- LD (nn),A
- LD (nn),HL
- LD A,(BC)
- LD A,(DE)
- LD A,(nn)
- LD HL,(nn)
- JP cc, nn
- JP nn
- HALT

where r and cc are found in [disassembly tables](http://www.z80.info/decoding.htm).

The [zasm](https://k1.spdns.de/Develop/Projects/zasm/Distributions/) assembler can be used to assemble the programs in assembly/src/.

Resources:
- http://sgate.emt.bme.hu/patai/publications/z80guide/
- http://www.z80.info/
