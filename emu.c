/* CS2102 IITPSx MiniProject 2026
 * emu.c - IITPSx emulator
 * Author: YOUR NAME / YOUR USER ID
 * Declaration: This is my own work, except where otherwise stated.
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MEM_SIZE 65536
#define MAX_STEPS 10000000L

static int memory[MEM_SIZE];
static int A,B,PC,SP;
static long steps;

static int load_object(const char *name)
{
    FILE *f;
    unsigned char b[4];
    int n,u;
    f=fopen(name,"rb");
    if (!f) { perror(name); return -1; }
    n=0;
    while (fread(b,1,4,f)==4) {
        if (n>=MEM_SIZE) { fprintf(stderr,"object is too large\n"); fclose(f); return -1; }
        u=(int)((unsigned int)b[0] | ((unsigned int)b[1]<<8) |
                ((unsigned int)b[2]<<16) | ((unsigned int)b[3]<<24));
        memory[n]=u; n++;
    }
    if (!feof(f)) { fprintf(stderr,"incomplete object word\n"); fclose(f); return -1; }
    fclose(f);
    return n;
}

static int valid_address(int x)
{
    return x>=0 && x<MEM_SIZE;
}

static void dump_memory(const char *name, int words)
{
    FILE *f;
    int i;
    f=fopen(name,"w");
    if (!f) { perror(name); return; }
    for (i=0;i<words;i++) fprintf(f,"%04X  %08X\n",i,(unsigned int)memory[i]);
    fclose(f);
}

static int execute(int program_words)
{
    int word,opcode,operand,address;
    unsigned int uw;
    while (steps<MAX_STEPS) {
        if (!valid_address(PC)) { fprintf(stderr,"Runtime error: PC out of range: %d\n",PC); return 1; }
        word=memory[PC];
        PC++;
        uw=(unsigned int)word;
        opcode=(int)(uw&255U);
        operand=(int)(word>>8);
        switch (opcode) {
        case 0: B=A; A=operand; break;
        case 1: A=A+operand; break;
        case 2:
            address=SP+operand;
            if (!valid_address(address)) { fprintf(stderr,"Runtime error: ldl address %d\n",address); return 1; }
            B=A; A=memory[address]; break;
        case 3:
            address=SP+operand;
            if (!valid_address(address)) { fprintf(stderr,"Runtime error: stl address %d\n",address); return 1; }
            memory[address]=A; A=B; break;
        case 4:
            address=A+operand;
            if (!valid_address(address)) { fprintf(stderr,"Runtime error: ldnl address %d\n",address); return 1; }
            A=memory[address]; break;
        case 5:
            address=A+operand;
            if (!valid_address(address)) { fprintf(stderr,"Runtime error: stnl address %d\n",address); return 1; }
            memory[address]=B; break;
        case 6: A=B+A; break;
        case 7: A=B-A; break;
        case 8: A=B << A; break;
        case 9: A=B >> A; break;
        case 10: SP=SP+operand; break;
        case 11: SP=A; A=B; break;
        case 12: B=A; A=SP; break;
        case 13: B=A; A=PC; PC=PC+operand; break;
        case 14: PC=A; A=B; break;
        case 15: if (A==0) PC=PC+operand; break;
        case 16: if (A<0) PC=PC+operand; break;
        case 17: PC=PC+operand; break;
        case 18: return 0;
        case 19: A=B*A; break;
        case 20:
            if (A==0) { fprintf(stderr,"Runtime error: division by zero at PC %d\n",PC-1); return 1; }
            A=B/A; break;
        default:
            fprintf(stderr,"Runtime error: unknown opcode %d at address %d\n",opcode,PC-1);
            return 1;
        }
        steps++;
        if (PC<0 || PC>=MEM_SIZE) { fprintf(stderr,"Runtime error: PC out of range: %d\n",PC); return 1; }
    }
    fprintf(stderr,"Runtime error: step limit exceeded (possible infinite loop)\n");
    (void)program_words;
    return 1;
}

int main(int argc,char **argv)
{
    int words,rc;
    const char *dumpname;
    if (argc<2 || argc>3) {
        fprintf(stderr,"Usage: %s program.o [memory.dump]\n",argv[0]); return 1;
    }
    words=load_object(argv[1]);
    if (words<0) return 1;
    A=0; B=0; PC=0; SP=0; steps=0;
    rc=execute(words);
    printf("A = %d (0x%08X)\n",A,(unsigned int)A);
    printf("B = %d (0x%08X)\n",B,(unsigned int)B);
    printf("PC = %d\n",PC);
    printf("SP = %d\n",SP);
    printf("Instructions executed = %ld\n",steps);
    dumpname=(argc==3)?argv[2]:"memory.dump";
    dump_memory(dumpname,words>0?words:1);
    return rc;
}
