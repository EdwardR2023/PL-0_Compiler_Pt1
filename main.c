#include <stdio.h>
#include <stdlib.h>
#define BuffSize 501

int base(int BP, int L, int *PAS) {
    int arb = BP;
    while (L > 0) {
        arb = PAS[arb];
        L--;
    }
    return arb;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (input == NULL) {
        perror("Error opening file");
        return 1;
    }


    int i = 0, PC = 0, SP = 500, BP = SP-1;
    int *PAS = (int *)calloc(BuffSize, sizeof(int));

    while (fscanf(input, "%d", &PAS[i]) == 1) {
        i++;
    }
    fclose(input);

    int HALT = 1, IR[3];
    while (HALT == 1) {
        IR[0] = PAS[PC]; // OP
        IR[1] = PAS[PC+1]; // L
        IR[2] = PAS[PC+2]; // M

        switch (IR[0]) {
            case 1: { // Literal push: sp ← sp-1; pas[sp] ← n
                SP--;
                PAS[SP] = IR[2];
                break;
            }
            case 2: { // Return
                switch(IR[2]){
                    case 0:{
                        int temp = SP;
                        SP = BP + 1;
                        PC = PAS[temp + 2];
                        BP = PAS[temp + 3];

                        PC-=3;
                        break;
                    }
                    case 1:{//Add
                        PAS[SP+1] = PAS[SP+1] + PAS[SP];
                        SP++;
                        break;
                    }
                    case 2:{//Subtract
                        PAS[SP+1] = PAS[SP+1] - PAS[SP];
                        SP++;
                        break;
                    }
                    case 3:{//Multiply
                        PAS[SP + 1]= PAS[SP] * PAS[SP + 1];
                        SP++;
                        break;
                    }
                    case 4:{//Divide
                        PAS[SP+1] = PAS[SP+1] / PAS[SP];
                        SP++;
                        break;
                    }
                    case 5:{//Equal
                        PAS[SP+1] = PAS[SP+1] == PAS[SP];
                        SP++;
                        break;
                    }
                    case 6:{//Not Equal
                        PAS[SP+1] = PAS[SP+1] != PAS[SP];
                        SP++;
                        break;
                    }
                    case 7:{//LSS
                        PAS[SP+1] = PAS[SP+1] < PAS[SP];
                        SP++;
                        break;
                    }
                    case 8:{//LEQ
                        PAS[SP+1] = PAS[SP+1] <= PAS[SP];
                        SP++;
                        break;
                    }
                    case 9:{//GTR
                        PAS[SP+1] = PAS[SP+1] > PAS[SP];
                        SP++;
                        break;
                    }
                    case 10:{//GEQ
                        PAS[SP+1] = PAS[SP+1] >= PAS[SP];
                        SP++;
                        break;
                    }
                    case 11:{//ODD
                        PAS[SP] = PAS[SP] % 2;
                        break;
                    }

                }
                break;
            }

            case 3: { // Load: sp ← sp - 1; pas[sp] ← pas[base(bp, n) - o];
                SP--;
                PAS[SP] = PAS[base(BP, IR[1], PAS) - IR[2]];
                break;
            }
            case 4: { // Store
                //Store value at top of stack in the stack location at
                //offset o from n lexicographical levels down
                //pas[base(bp, n) - o]  pas[sp];
                //sp = sp +1;
                PAS[base(BP,IR[1],PAS)- IR[2]] = PAS[SP];
                SP++;
                break;
            }
            case 5: { // Call
                //Call the procedure at code index p, generating a new activation record and setting PC to p: pas[sp - 1]  base(bp, n); /* static link (SL) pas[sp - 2]  bp;/* dynamic link (DL)
                //pas[sp - 3]  pc; /*return address (RA) bp  sp - 1; pc  p;

                PAS[SP - 1] = base(BP, IR[1], PAS);  // static link (SL)
                PAS[SP - 2] = BP;                    // dynamic link (DL)
                PAS[SP - 3] = PC+3;                // return address (RA)

                // Update BP, SP, and PC for the called procedure
                BP = SP - 1;
                PC = IR[2] -3;

                break;
            }
            case 6: { // Increment
                SP = SP - IR[2];
                break;
            }
            case 7: { // Jump
                PC = IR[2]-3;
                break;
            }
            case 8: { // Jump Conditional
                if(PAS[SP] == 0){
                    PC = IR[2]-3;
                    SP++;
                }

                break;
            }
            case 9: { // System

                switch (IR[2]) {
                    case 1: { // Output of the value in stack[SP] to standard output as a character and pop: putc(stack[sp]); sp ← sp+1
                        printf("%d\n",PAS[SP]);
                        SP++;
                        break;
                    }
                    case 2: { // Read an integer, as character value, from standard input (stdin) and store it on the top of the stack. sp ← sp-1; stack[sp] ← getc()
                        SP--;
                        char inputChar;
                        scanf("%c", &inputChar);

                        // Convert character to numeric value by subtracting ASCII value of '0'
                        PAS[SP] = inputChar - '0';
                        break;
                    }
                    case 3: {

                        HALT = 0;
                        break;
                    }

                }
                break;
            }


        }

        PC += 3;
        printf("PC: %d, BP: %d, SP: %d, | ", PC, BP, SP);
        for (int j = 499; j >= SP; j--) {
            printf("%d ", PAS[j]);
        }
        printf("\n");
    }


    free(PAS);
    return 0;
}
