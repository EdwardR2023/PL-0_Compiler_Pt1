//Edward Rosales
//Alan Pons

#include <stdio.h>
#define BuffSize 501
int PAS[BuffSize] = {0};

//This function is used to find a variable in a different Activation Record some 'L' levels down:
int base(int BP, int L) {
    int arb = BP;
    while (L > 0) {
        arb = PAS[arb];
        L--;
    }
    return arb;
}

int main(int argc, char **argv) {
    //checks argument is provided in command line
    if (argc != 2) {
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    //checks if input from cmd line is valid
    if (input == NULL) {
        perror("Error opening file");
        return 1;
    }

    //initializes PC, SP, and BP
    int i = 0, PC = 0, SP = 500, BP = SP-1;

    //reads in from file and populates PAS
    while (fscanf(input, "%d", &PAS[i]) == 1) {
        i++;
    }
    fclose(input);

    int HALT = 1, IR[3], AR[167], arInd = 0;

    printf("OPCODE\tPC\tBP\tSP\tSTACK:\n");

    //loop where program runs till OP Code 9 0 2 is used to make HALT != 1
    while (HALT == 1) {
        PC += 3;
        IR[0] = PAS[PC - 3]; // OP
        IR[1] = PAS[PC - 2]; // L
        IR[2] = PAS[PC - 1]; // M

        switch (IR[0]) {
            case 1: { // Literal push: sp ← sp-1; pas[sp] ← n
                SP--;
                PAS[SP] = IR[2];
                break;
            }
            case 2: { // Return
                switch(IR[2]){
                    case 0:{//Returns from a subroutine is encoded 0 0 0 and restores the caller’s AR:

                        AR[--arInd] = 0;

                        SP = BP + 1;
                        BP = PAS[SP - 2];
                        PC = PAS[SP - 3];

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
                PAS[SP] = PAS[base(BP, IR[1]) - IR[2]];
                break;
            }
            case 4: { // Store
                //Store value at top of stack in the stack location at
                //offset o from n lexicographical levels down
                //pas[base(bp, n) - o]  pas[sp];
                //sp = sp +1;
                PAS[base(BP,IR[1])- IR[2]] = PAS[SP];
                SP++;
                break;
            }
            case 5: { // Call
                //Call the procedure at code index p, generating a new activation record and setting PC to p: pas[sp - 1]  base(bp, n); /* static link (SL) pas[sp - 2]  bp;/* dynamic link (DL)
                //pas[sp - 3]  pc; /*return address (RA) bp  sp - 1; pc  p;

                AR[arInd++] = SP - 1;

                PAS[SP - 1] = base(BP, IR[1]);  // static link (SL)
                PAS[SP - 2] = BP;                    // dynamic link (DL)
                PAS[SP - 3] = PC;                // return address (RA)

                // Update BP, SP, and PC for the called procedure
                BP = SP - 1;
                PC = IR[2];


                break;
            }
            case 6: { // Increment
                SP = SP - IR[2];
                break;
            }
            case 7: { // Jump
                PC = IR[2];
                break;
            }
            case 8: { // Jump Conditional
                if(PAS[SP] == 0){
                    PC = IR[2];
                }
                SP++;

                break;
            }
            case 9: { // System

                switch (IR[2]) {
                    case 1: { // Output of the value in stack[SP] to standard output as a character and pop: putc(stack[sp]); sp ← sp+1
                        printf("Output Result is: %d\n",PAS[SP]);
                        SP++;
                        break;
                    }
                    case 2: { // Read an integer, as character value, from standard input (stdin) and store it on the top of the stack. sp ← sp-1; stack[sp] ← getc()
                        SP--;
                        char inputChar;
                        printf("Please Enter an Integer:");
                        scanf("%c", &inputChar);

                        // Convert character to numeric value by subtracting ASCII value of '0'
                        PAS[SP] = inputChar - '0';
                        break;
                    }
                    //terminating command is read and thus HALT == 0 to break while loop
                    case 3: {

                        HALT = 0;
                        break;
                    }

                }
                break;
            }


        }
        //increments PC after ever cycle


        int barCheck = 0;

        //prints points and stack
        printf("%d %d %d\t%d\t%d\t%d\t",IR[0],IR[1],IR[2], PC, BP, SP);
        for (int j = 499; j >= SP; j--) {

            //checks if a | is need between activation records
            if(j == AR[barCheck]){
                printf("| ");
                barCheck++;
            }
            printf("%d ", PAS[j]);
        }
        printf("\n");
    }

    return 0;
}