#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SYMBOLS 100
#define MAX_LINE 256


struct Symbol
{
    char name[50];
    int address;
};

struct Symbol sym[MAX_SYMBOLS];

int symCount = 0;

struct Opcode
{
    char name[20];
    int opcode;
    int hasOperand;
};

struct Opcode optable[] =
{
    {"ldc",0,1},
    {"adc",1,1},
    {"ldl",2,1},
    {"stl",3,1},
    {"ldnl",4,1},
    {"stnl",5,1},
    {"add",6,0},
    {"sub",7,0},
    {"shl",8,0},
    {"shr",9,0},
    {"adj",10,1},
    {"a2sp",11,0},
    {"sp2a",12,0},
    {"call",13,1},
    {"return",14,0},
    {"brz",15,1},
    {"brlz",16,1},
    {"br",17,1},
    {"HALT",18,0},
    {"mul",19,0},
    {"div",20,0}
};

int opcodeCount = 21;

int findOpcode(char name[])
{
    int i;
    for(i=0;i<opcodeCount;i++)
    {
        if(strcmp(optable[i].name,name)==0)
        {
            return i;
        }
    }
    return -1;
}

int findSymbol(char name[])
{
    int i;
    for(i=0;i<symCount;i++)
    {
        if(strcmp(sym[i].name,name)==0)
        {
            return sym[i].address;
        }
    }
    return -1;
}

int validLabel(char name[])
{
    int i;
    if(name[0]=='\0')
    {
        return 0;
    }
    if(!isalpha((unsigned char)name[0]) &&
       name[0]!='_')
    {
        return 0;
    }
    for(i=1;name[i]!='\0';i++)
    {
        if(!isalnum((unsigned char)name[i]) &&
           name[i]!='_')
        {
            return 0;
        }
    }
    return 1;
}

int addSymbol(char name[],int address)
{
    if(findSymbol(name)!=-1)
    {
        return 0;
    }
    if(symCount>=MAX_SYMBOLS)
    {
        return 0;
    }
    strcpy(sym[symCount].name,name);
    sym[symCount].address=address;
    symCount++;
    return 1;
}

void pass1(FILE *fp)
{
    char line[MAX_LINE];
    char work[MAX_LINE];
    int address=0;
    int lineNumber=0;
    printf("\n========== PASS 1 ==========\n");
    while(fgets(line,MAX_LINE,fp)!=NULL)
    {
        char *colon;
        char label[50];
        char mnemonic[50];
        char operand[100];
        lineNumber++;
        strcpy(work,line);
        {
            char *p;
            p=strchr(work,';');
            if(p!=NULL)
            {
                *p='\0';
            }
        }
        work[strcspn(work,"\n")]='\0';
        if(strlen(work)==0)
        {
            continue;
        }
        label[0]='\0';
        mnemonic[0]='\0';
        operand[0]='\0';

        colon=strchr(work,':');
        if(colon!=NULL)
        {
            int length;
            length=(int)(colon-work);
            strncpy(label,work,length);
            label[length]='\0';
            if(!validLabel(label))
            {
                printf("Line %d: Invalid label %s\n",
                       lineNumber,label);
                continue;
            }

            if(!addSymbol(label,address))
            {
                printf("Line %d: Duplicate label %s\n",
                       lineNumber,label);
            }

            memmove(work,
                    colon+1,
                    strlen(colon+1)+1);
        }

        sscanf(work,"%49s %99s",
               mnemonic,
               operand);

        if(strcmp(mnemonic,"SET")==0)
        {
            int value;
            if(label[0]=='\0')
            {
                printf("Line %d: SET requires a label\n",
                       lineNumber);
                continue;
            }
            value=(int)strtol(operand,NULL,0);
            {
                int i;
                for(i=0;i<symCount;i++)
                {
                    if(strcmp(sym[i].name,label)==0)
                    {
                        sym[i].address=value;
                        break;
                    }
                }
            }
            continue;
        }
        if(findOpcode(mnemonic)==-1)
        {
            printf("Line %d: Unknown instruction %s\n",
                   lineNumber,mnemonic);

            continue;
        }
        address++;
    }

    printf("\nSymbol Table:\n");
    printf("-------------------------\n");
    {
        int i;
        for(i=0;i<symCount;i++)
        {
            printf("%-15s %d\n",
                   sym[i].name,
                   sym[i].address);
        }
    }
    printf("-------------------------\n");
}
void pass2(FILE *fp,
           FILE *obj,
           FILE *lst)
{
    char line[MAX_LINE];
    char work[MAX_LINE];

    int address = 0;
    int lineNumber = 0;

    printf("\n========== PASS 2 ==========\n");

    while(fgets(line, MAX_LINE, fp) != NULL)
    {
        char *colon;

        char label[50];
        char mnemonic[50];
        char operand[100];

        int opcode;
        int operandValue;
        int machineCode;

        lineNumber++;

        strcpy(work, line);

        {
            char *p;

            p = strchr(work, ';');

            if(p != NULL)
            {
                *p = '\0';
            }
        }

        work[strcspn(work, "\n")] = '\0';

        if(strlen(work) == 0)
        {
            continue;
        }

        label[0] = '\0';
        mnemonic[0] = '\0';
        operand[0] = '\0';

        colon = strchr(work, ':');

        if(colon != NULL)
        {
            int length;

            length = (int)(colon - work);

            strncpy(label, work, length);
            label[length] = '\0';

            memmove(work,
                    colon + 1,
                    strlen(colon + 1) + 1);
        }

        sscanf(work, "%49s %99s",
               mnemonic,
               operand);

        if(strcmp(mnemonic, "SET") == 0)
        {
            continue;
        }

        opcode = findOpcode(mnemonic);

        if(opcode == -1)
        {
            printf("Line %d: Unknown instruction %s\n",
                   lineNumber, mnemonic);

            continue;
        }

        operandValue = 0;

        if(optable[opcode].hasOperand)
        {
            char *end;
            long value;

            value = strtol(operand, &end, 0);

            if(*operand != '\0' && *end == '\0')
            {
                operandValue = (int)value;
            }
            else
            {
                operandValue = findSymbol(operand);

                if(operandValue == -1)
                {
                    printf("Line %d: Undefined label %s\n",
                           lineNumber, operand);

                    continue;
                }

                if(strcmp(mnemonic, "br") == 0 ||
                   strcmp(mnemonic, "brz") == 0 ||
                   strcmp(mnemonic, "brlz") == 0 ||
                   strcmp(mnemonic, "call") == 0)
                {
                    operandValue =
                        operandValue - (address + 1);
                }
            }
        }

        machineCode = (operandValue << 8) | opcode;

        fwrite(&machineCode,
               sizeof(int),
               1,
               obj);

        fprintf(lst,
                "%04d  %08X  %s\n",
                address,
                (unsigned int)machineCode,
                work);

        printf("%04d  %08X  %s\n",
               address,
               (unsigned int)machineCode,
               work);

        address++;
    }
}
int main(int argc,char *argv[])
{
    FILE *fp;
    FILE *obj;
    FILE *lst;
    if(argc!=4)
    {
        printf("Usage: asm input.asm output.o output.lst\n");
        return 1;
    }

    fp=fopen(argv[1],"r");
    if(fp==NULL)
    {
        printf("Cannot open input file\n");
        return 1;
    }
    pass1(fp);
    rewind(fp);
    obj=fopen(argv[2],"wb");
    lst=fopen(argv[3],"w");
    if(obj==NULL || lst==NULL)
    {
        printf("Cannot create output files\n");
        fclose(fp);
        if(obj!=NULL)
        {
            fclose(obj);
        }
        if(lst!=NULL)
        {
            fclose(lst);
        }
        return 1;
    }
    pass2(fp,obj,lst);
    fclose(fp);
    fclose(obj);
    fclose(lst);
    printf("\nAssembly completed.\n");
    return 0;
}