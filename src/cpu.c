#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

#define print_reg_col(label, reg) printf("%-5s %6d\n", label, reg);

void add_n(struct Z80CPU *cpu, uint8_t a);
void adc_n(struct Z80CPU *cpu, uint8_t a);
void sub_n(struct Z80CPU *cpu, uint8_t a);
void sbc_n(struct Z80CPU *cpu, uint8_t a);
void and_n(struct Z80CPU *cpu, uint8_t a);
void xor_n(struct Z80CPU *cpu, uint8_t a);
void or_n(struct Z80CPU *cpu, uint8_t a);
void cp_n(struct Z80CPU *cpu, uint8_t a);
void rlc(struct Z80CPU *cpu);
void rrc(struct Z80CPU *cpu);
void rl(struct Z80CPU *cpu);
void rr(struct Z80CPU *cpu);
void sla(struct Z80CPU *cpu);
void sra(struct Z80CPU *cpu);
void sll(struct Z80CPU *cpu);
void srl(struct Z80CPU *cpu);
void set_zero_flag(struct Z80CPU *cpu);
uint8_t check_condition_z(struct Z80CPU *cpu);
uint8_t check_condition_nz(struct Z80CPU *cpu);
void report_unknown(struct Z80CPU *cpu);

struct Z80CPU *newcpu() {
    struct Z80CPU *cpu = malloc(sizeof(struct Z80CPU));
    struct Z80CPU_REG *r = malloc(sizeof(struct Z80CPU_REG));
    cpu->r = r;

    r->a = 0;
    r->f = 0;
    r->b = 0;
    r->c = 0;
    r->d = 0;
    r->e = 0;
    r->h = 0;
    r->l = 0;
    r->a_p = 0;
    r->b_p = 0;
    r->c_p = 1;
    r->d_p = 0;
    r->e_p = 0;
    r->h_p = 0;
    r->l_p = 0;
    r->ix = 0;
    r->iy = 0;
    r->sp = 0;
    r->pc = 0;
    r->i = 0;
    r->r = 0;

    r->f_Z = 1;

    cpu->mem = NULL;

    cpu->r_table[R_B] = &cpu->r->b;
    cpu->r_table[R_C] = &cpu->r->c;
    cpu->r_table[R_D] = &cpu->r->d;
    cpu->r_table[R_E] = &cpu->r->e;
    cpu->r_table[R_H] = &cpu->r->h;
    cpu->r_table[R_L] = &cpu->r->l;
    cpu->r_table[R_HL] = NULL; // Best way to handle? Nothing goes here.
    cpu->r_table[R_A] = &cpu->r->a;

    cpu->rp_table[RP_BC] = &cpu->r->bc;
    cpu->rp_table[RP_DE] = &cpu->r->de;
    cpu->rp_table[RP_HL] = &cpu->r->hl;
    cpu->rp_table[RP_SP] = &cpu->r->sp;

    cpu->rp2_table[RP2_BC] = &cpu->r->bc;
    cpu->rp2_table[RP2_DE] = &cpu->r->de;
    cpu->rp2_table[RP2_HL] = &cpu->r->hl;
    cpu->rp2_table[RP2_AF] = &cpu->r->af;

    cpu->cc_table[CC_Z] = &check_condition_z;
    cpu->cc_table[CC_NZ] = &check_condition_nz;

    cpu->alu_table[ALU_ADD] = &add_n;
    cpu->alu_table[ALU_ADC] = &adc_n;
    cpu->alu_table[ALU_SUB] = &sub_n;
    cpu->alu_table[ALU_SBC] = &sbc_n;
    cpu->alu_table[ALU_AND] = &and_n;
    cpu->alu_table[ALU_XOR] = &xor_n;
    cpu->alu_table[ALU_OR] = &or_n;
    cpu->alu_table[ALU_CP] = &cp_n;

    cpu->rot_table[ROT_RLC] = &rlc;
    cpu->rot_table[ROT_RRC] = &rrc;
    cpu->rot_table[ROT_RL]  = &rl;
    cpu->rot_table[ROT_RR]  = &rr;
    cpu->rot_table[ROT_SLA] = &sla;
    cpu->rot_table[ROT_SRA] = &sra;
    cpu->rot_table[ROT_SLL] = &sll;
    cpu->rot_table[ROT_SRL] = &srl;

    cpu->interrupts_enabled = true;

    return cpu;
}

uint8_t carry(struct Z80CPU *cpu) {
    return cpu->r->f & 1;
}

void set_zero_flag(struct Z80CPU *cpu) {
    if (cpu->r->a == 0)
        cpu->r->f_Z = 1;
    else
        cpu->r->f_Z = 0;
}

uint8_t check_condition_z(struct Z80CPU *cpu) {
    return cpu->r->f_Z;
}

uint8_t check_condition_nz(struct Z80CPU *cpu) {
    return cpu->r->f_Z ^ 1;
}

void add_n(struct Z80CPU *cpu, uint8_t n) {
    cpu->r->a += n;
}

void adc_n(struct Z80CPU *cpu, uint8_t n) {
    cpu->r->a += (n + carry(cpu));
}

void sub_n(struct Z80CPU *cpu, uint8_t n) {
    cpu->r->a -= n;
    set_zero_flag(cpu);
}

void sbc_n(struct Z80CPU *cpu, uint8_t n) {
    ; //TODO
}

void and_n(struct Z80CPU *cpu, uint8_t n) {
    cpu->r->a &= n;
}

void xor_n(struct Z80CPU *cpu, uint8_t n) {
    cpu->r->a ^= n;
}

void or_n(struct Z80CPU *cpu, uint8_t n) {
    cpu->r->a |= n;
}

void cp_n(struct Z80CPU *cpu, uint8_t n) {
    ; //TODO
}

void rlc(struct Z80CPU *cpu) {
    report_unknown(cpu);
}

void rrc(struct Z80CPU *cpu) {
    report_unknown(cpu);
}

void rl(struct Z80CPU *cpu) {
    report_unknown(cpu);
}

void rr(struct Z80CPU *cpu) {
    report_unknown(cpu);
}

void sla(struct Z80CPU *cpu) {
    report_unknown(cpu);
}

void sra(struct Z80CPU *cpu) {
    report_unknown(cpu);
}

void sll(struct Z80CPU *cpu) {
    report_unknown(cpu);
}

void srl(struct Z80CPU *cpu) {
    report_unknown(cpu);
}

/*----                  Helper functions                      ---*/
/*--------------------------------------------------------------*/

#define em_to_os(p, address) ((p)->mem + (address))
#define os_to_em(p, address) ((address) - (p)->mem)

#define LO_BYTE(x) (x & 0xFF)
#define HI_BYTE(x) (x >> 8)

/* Think about making these inlines or macros. */
uint8_t currb(struct Z80CPU *cpu) {
    return cpu->mem[cpu->r->pc - 1];
}

uint8_t nextb(struct Z80CPU *cpu) {
    return cpu->mem[cpu->r->pc++];
}

uint16_t sp_pop(struct Z80CPU *cpu) {
    uint8_t lo = *em_to_os(cpu, cpu->r->sp++);
    uint8_t hi = *em_to_os(cpu, cpu->r->sp++);
    return (hi << 8) + lo;
}

void sp_push(struct Z80CPU *cpu, uint16_t val) {
    *em_to_os(cpu, cpu->r->sp--) = HI_BYTE(val);
    *em_to_os(cpu, cpu->r->sp--) = LO_BYTE(val);
}

uint16_t nexttwob(struct Z80CPU *cpu) {
    uint8_t lo = nextb(cpu);
    uint8_t hi = nextb(cpu);
    return (hi << 8) + lo;
}

void report_unknown(struct Z80CPU *cpu) {
    fprintf(stderr, "Unknown opcode: %02x\n", currb(cpu));
}


/*---- Functions for groups of operations defined by x and z ---*/
/*--------------------------------------------------------------*/

void decode_cb(struct Z80CPU *cpu, uint8_t op_x) {
    report_unknown(cpu);
}

void decode_dd(struct Z80CPU *cpu) {
    report_unknown(cpu);
}

void decode_ed(struct Z80CPU *cpu) {
    report_unknown(cpu);
}

void decode_fd(struct Z80CPU *cpu) {
    report_unknown(cpu);
}

// TODO TEST
void decode_0_0(struct Z80CPU *cpu, uint8_t op_y) {
    uint16_t tmp;
    switch (op_y) {
    case 0:
        break;
    case 1:
        tmp = cpu->r->af;
        cpu->r->af = cpu->r->af_p;
        cpu->r->af_p = tmp;
        break;
    case 2:
        cpu->r->b--;
        if (cpu->r->b != 0)
            cpu->r->pc += (int8_t)nextb(cpu);
        break;
    case 3:
        cpu->r->pc += (int8_t)(nextb(cpu));
        break;
    case 4:
    case 5:
    case 6:
    case 7:
        if (cpu->cc_table[op_y - 4](cpu))
            cpu->r->pc += (int8_t)(nextb(cpu));
        break;
    default:
        report_unknown(cpu);
    }
}

// DONE
void decode_0_1(struct Z80CPU *cpu, uint8_t op_q, uint8_t op_p) {
    switch (op_q) {
    case 0:
        *cpu->rp_table[op_p] = nexttwob(cpu);
        return;
    case 1:
        cpu->r->hl += *cpu->rp_table[op_p];
        return;
    default:
        report_unknown(cpu);
        return;
    }
}

// DONE
void decode_0_2(struct Z80CPU *cpu, uint8_t op_q, uint8_t op_p) {
    switch (op_q) {
    case 0:
        switch (op_p) {
        case 0:
            memcpy(em_to_os(cpu, cpu->r->bc), &cpu->r->a, 1);
            return;
        case 1:
            memcpy(em_to_os(cpu, cpu->r->de), &cpu->r->a, 1);
            return;
        case 2:
            memcpy(em_to_os(cpu, nexttwob(cpu)), &cpu->r->hl, 2);
            return;
        case 3:
            memcpy(em_to_os(cpu, nexttwob(cpu)), &cpu->r->a, 1);
            return;
        default:
            report_unknown(cpu);
        }
        return;
    case 1:
        switch (op_p) {
        case 0:
            memcpy(&cpu->r->a, em_to_os(cpu, cpu->r->bc), 1);
            return;
        case 1:
            memcpy(&cpu->r->a, em_to_os(cpu, cpu->r->de), 1);
            return;
        case 2:
            memcpy(&cpu->r->hl, em_to_os(cpu, nexttwob(cpu)), 2);
            return;
        case 3:
            memcpy(&cpu->r->a, em_to_os(cpu, nexttwob(cpu)), 1);
            return;
        default:
            report_unknown(cpu);
        }
        return;
    default:
        report_unknown(cpu);
    }
}

// DONE
void decode_0_3(struct Z80CPU *cpu, uint8_t op_q, uint8_t op_p) {
    switch (op_q) {
    case 0:
        *cpu->rp_table[op_p] += 1;
        return;
    case 1:
        *cpu->rp_table[op_p] -= 1;
        return;
    default:
        report_unknown(cpu);
        return;
    }
}

// DONE
void decode_0_4(struct Z80CPU *cpu, uint8_t op_y) {
    *cpu->r_table[op_y] += 1;
}

// DONE
void decode_0_5(struct Z80CPU *cpu, uint8_t op_y) {
    *cpu->r_table[op_y] -= 1;
}

// DONE
void decode_0_6(struct Z80CPU *cpu, uint8_t op_y) {
    if (op_y == 6)
        *((cpu->r->h << 8) + cpu->r->l + cpu->mem) = nextb(cpu);
    else
        *(cpu->r_table[op_y]) = nextb(cpu);
}

// TODO
void decode_0_7(struct Z80CPU *cpu, uint8_t op_y) {
    uint8_t tmp;
    switch (op_y) {
    case 0:
        cpu->r->f_C = cpu->r->a >> 7;
        cpu->r->a <<= 1;
        cpu->r->a |= cpu->r->f_C;
        break;
    case 1:
        cpu->r->f_C = cpu->r->a & 1;
        cpu->r->a >>= 1;
        cpu->r->a |= cpu->r->f_C << 7;
        break;
    case 2:
        tmp = cpu->r->a >> 7;
        cpu->r->a <<= 1;
        cpu->r->a |= cpu->r->f_C;
        cpu->r->f_C = tmp;
    case 3:
        tmp = cpu->r->a & 1;
        cpu->r->a >>= 1;
        cpu->r->a |= cpu->r->f_C << 7;
        cpu->r->f_C = tmp;
        break;
    case 5:
        cpu->r->a ^= 0xFF;
        // Is this correct?
        cpu->r->f_H = 1;
        cpu->r->f_N = 1;
        break;
    case 6:
        cpu->r->f_C = 1;;
        break;
    case 7:
        cpu->r->f_C ^= 1;;
        break;
    default:
        report_unknown(cpu);
    }
}

// DONE
void decode_1_n(struct Z80CPU *cpu, uint8_t op_y, uint8_t op_z) {
    *cpu->r_table[op_y] = *cpu->r_table[op_z];
}

// DONE
void decode_2_n(struct Z80CPU *cpu, uint8_t op_y, uint8_t op_z) {
    cpu->alu_table[op_y](cpu, *(cpu->r_table[op_z]));
}

// DONE
void decode_3_0(struct Z80CPU *cpu, uint8_t op_y) {
    uint8_t cc = cpu->cc_table[op_y](cpu);
    if (cc)
        cpu->r->pc = sp_pop(cpu);
}

// Z80: TODO
// GB:  DONE
void decode_3_1(struct Z80CPU *cpu, uint8_t op_q, uint8_t op_p) {
    switch (op_q) {
    case 0:
        *cpu->rp2_table[op_p] = sp_pop(cpu);
        break;
    case 1:
        switch(op_p) {
        case 0:
            cpu->r->pc = sp_pop(cpu);
            break;
        case 2:
            cpu->r->pc = cpu->r->hl;
            break;
        case 3:
            cpu->r->sp = cpu->r->hl;
            break;
        default:
            report_unknown(cpu);
        }
        break;
    default:
       report_unknown(cpu);
    }
}

// DONE
void decode_3_2(struct Z80CPU *cpu, uint8_t op_y) {
    uint8_t cc = cpu->cc_table[op_y](cpu);
    if (cc)
        cpu->r->pc = nexttwob(cpu);
}

// DONE
void decode_3_3(struct Z80CPU *cpu, uint8_t op_y) {
    switch (op_y) {
    case 0:
        cpu->r->pc = nexttwob(cpu);
        break;
    case 1:
        decode_cb(cpu, 3);
        break;
    case 2:
        // GB ABSENT
    case 3:
        // GB ABSENT
    case 4:
        // GB ABSENT
    case 5:
        // GB ABSENT
        report_unknown(cpu);
        break;
    case 6:
        cpu->interrupts_enabled = false;
    case 7:
        cpu->interrupts_enabled = true;
    }
}

// DONE
void decode_3_4(struct Z80CPU *cpu, uint8_t op_y) {
    if (cpu->cc_table[op_y](cpu)) {
        sp_push(cpu, (uint16_t)cpu->r->pc+3);
        cpu->r->pc = nexttwob(cpu);
    }
}

// DONE
void decode_3_5(struct Z80CPU *cpu, uint8_t op_q, uint8_t op_p) {
    switch (op_q) {
    case 0:
        sp_push(cpu, *cpu->rp2_table[op_p]);
        break;
    case 1:
        switch (op_p) {
        case 0:
            sp_push(cpu, (uint16_t)cpu->r->pc+3);
            cpu->r->pc = nexttwob(cpu);
            break;
        case 1:
            decode_dd(cpu);
            break;
        case 2:
            decode_ed(cpu);
            break;
        case 3:
            decode_fd(cpu);
            break;
        }
    }
}

// DONE
void decode_3_6(struct Z80CPU *cpu, uint8_t op_y) {
    cpu->alu_table[op_y](cpu, nextb(cpu));
}

// DONE
void decode_3_7(struct Z80CPU *cpu, uint8_t op_y) {
    sp_push(cpu, (uint16_t)cpu->r->pc+3);
    cpu->r->pc = op_y*8;
}

void halt(struct Z80CPU *cpu) {
    if (verbose)
        printf("HALT\n");
}

/*----              Execute single instruction                --*/
/*--------------------------------------------------------------*/

int execute(struct Z80CPU *cpu) {

    uint8_t op = nextb(cpu);

    uint8_t op_x = op >> 6;
    uint8_t op_y = (op >> 3) % 8;
    uint8_t op_z = op % 8;
    uint8_t op_p = op_y >> 1;
    uint8_t op_q = op_y % 2;

    if (verbose)
        printf("Opcode: %02x x: %d y: %d z: %d p %d q %d\n", op, op_x, op_y, op_z, op_p, op_q);

    switch (op_x) {
        case 0:
            switch (op_z) {
                case 0:
                    decode_0_0(cpu, op_y);
                    break;
                case 1:
                    decode_0_1(cpu, op_q, op_p);
                    break;
                case 2:
                    decode_0_2(cpu, op_q, op_p);
                    break;
                case 3:
                    decode_0_3(cpu, op_q, op_p);
                    break;
                case 4:
                    decode_0_4(cpu, op_y);
                    break;
                case 5:
                    decode_0_5(cpu, op_y);
                    break;
                case 6:
                    decode_0_6(cpu, op_y);
                    break;
                case 7:
                    decode_0_7(cpu, op_y);
                    break;
                default:
                    report_unknown(cpu);
            }
            break;
        case 1:
            if (op_z == 6 && op_y == 6) {
                halt(cpu);
                return -1;
            } else {
                decode_1_n(cpu, op_y, op_z);
                break;
            }
            break;
        case 2:
            decode_2_n(cpu, op_y, op_z);
            break;
        case 3:
            switch (op_z) {
                case 0:
                    decode_3_0(cpu, op_y);
                    break;
                case 1:
                    decode_3_1(cpu, op_q, op_p);
                    break;
                case 2:
                    decode_3_2(cpu, op_y);
                    break;
                case 3:
                    decode_3_3(cpu, op_y);
                    break;
                case 4:
                    decode_3_4(cpu, op_y);
                    break;
                case 5:
                    decode_3_5(cpu, op_q, op_p);
                    break;
                case 6:
                    decode_3_6(cpu, op_y);
                    break;
                case 7:
                    decode_3_7(cpu, op_y);
                    break;
                default:
                    report_unknown(cpu);
            }
            break;
        default:
            report_unknown(cpu);
    }
    set_zero_flag(cpu);
    return 0;
}

void registerdump(struct Z80CPU *cpu) {
    printf("%-3s %-3s %-3s %-3s %-3s %-3s %-3s %-3s %-3s %-3s",
            "a", "f", "b", "c", "d", "e", "h", "l", "i", "r");
    printf("\n");
    printf("%-3d %-3d %-3d %-3d %-3d %-3d %-3d %-3d %-3d %-3d",
            cpu->r->a, cpu->r->f, cpu->r->b, cpu->r->c, cpu->r->d, cpu->r->e, cpu->r->h, cpu->r->l, cpu->r->i, cpu->r->r);
    printf("\n");
    printf("%-6s %-6s %-6s",
           "bc", "de", "hl");
    printf("\n");
    printf("%-6d %-6d %-6d",
           cpu->r->bc, cpu->r->de, cpu->r->hl);
    printf("\n");
    printf("%-3s %-3s %-3s %-3s %-3s %-3s %-3s %-3s %-3s %-3s %-3s",
            "ap", "bp", "cp", "dp", "ep", "hp", "lp", "ix", "iy", "sp", "pc");
    printf("\n");
    printf("%-3d %-3d %-3d %-3d %-3d %-3d %-3d %-3d %-3d %-3d %-3d",
            cpu->r->a_p, cpu->r->b_p, cpu->r->c_p, cpu->r->d_p, cpu->r->e_p, cpu->r->h_p, cpu->r->l_p, cpu->r->ix, cpu->r->iy, cpu->r->sp, cpu->r->pc);
    printf("\n");
    printf("%-6s %-6s %-6s",
           "bc_p", "de_p", "hl_p");
    printf("\n");
    printf("%-6d %-6d %-6d",
           cpu->r->bc_p, cpu->r->de_p, cpu->r->hl_p);
    printf("\n");
    printf("%-3s %-3s %-3s %-3s %-3s %-3s %-3s %-3s",
            "S", "Z", "F5", "H", "F3", "PV", "N", "C");
    printf("\n");
    printf("%-3d %-3d %-3d %-3d %-3d %-3d %-3d %-3d",
            cpu->r->f_S, cpu->r->f_Z, cpu->r->f_F5, cpu->r->f_H, cpu->r->f_F3, cpu->r->f_PV, cpu->r->f_N, cpu->r->f_C);
    printf("\n");
}

