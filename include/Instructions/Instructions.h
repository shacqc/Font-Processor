#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <FontEngineInit.h>
#include <InstructionGroups/FunctionGroupZero.h>
#include <InstructionGroups/FunctionGroupOne.h>
#include <InstructionGroups/FunctionGroupTwo.h>
#include <InstructionGroups/FunctionGroupThree.h>
#include <InstructionGroups/FunctionGroupFour.h>
#include <InstructionGroups/FunctionGroupFive.h>
#include <InstructionGroups/FunctionGroupSix.h>
#include <InstructionGroups/FunctionGroupSeven.h>
#include <InstructionGroups/FunctionGroupEight.h>
#include <InstructionGroups/FunctionGroupCD.h>
#include <InstructionGroups/FunctionGroupEF.h>
#include <TrueFontTypes.h>
#include <FontEngineInit.h>


uint8_t instructionentry(uint8_t *instructionpointer, struct InstructionKit *MemoryLocations, struct glyf *glyf)
{
    if(glyf != NULL)
    {
        struct GraphicStates *RuntimeStates = InitializeGraphicStates();
        executionfunction(instructionpointer, MemoryLocations, RuntimeStates);
    }
    else 
    {
        executionfunction(instructionpointer, MemoryLocations, NULL);
    }
}

uint8_t executionfunction(uint8_t *instructionpointer, struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)
{
    while ((*instructionpointer++) != 45) //Char 45 is the "End Function" Command; instruction pointer will increment until equals "End Function"
    {
    InstructionExecute(instructionpointer, MemoryLocations, RuntimeStates);
    }
}

void InstructionExecute(uint8_t *instructionpointer, struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)
{
    char HighEnd = *instructionpointer & 240; 
    if(HighEnd < 8)
    {   
        if (HighEnd <4)  
            {
                if (HighEnd < 2)   
                {
                    switch (HighEnd)
                    {
                        case 0:
                            {
                                FunctionGroupZero(instructionpointer, MemoryLocations, RuntimeStates);
                            }
                        case 1:
                            {
                                FunctionGroupOne(instructionpointer, MemoryLocations, RuntimeStates);
                            }
                    }
                }
                else 
                {
                    switch (HighEnd)
                    {
                        case 2: 
                            {
                                FunctionGroupTwo(instructionpointer, MemoryLocations, RuntimeStates);
                            } 
                        case 3: 
                            {
                                FunctionGroupThree(instructionpointer, MemoryLocations, RuntimeStates);
                            }
                    }
                }
            }
            else if (HighEnd < 6) 
            {
                switch (HighEnd)
                {
                    case 4: 
                        {
                            FunctionGroupFour(instructionpointer, MemoryLocations, RuntimeStates);
                        } 
                    case 5: 
                        {
                            FunctionGroupFive(instructionpointer, MemoryLocations, RuntimeStates);
                        } 
                    }
            }

            else
            {
                switch (HighEnd)
                {
                    case 6 : 
                        {
                            FunctionGroupSix(instructionpointer, MemoryLocations, RuntimeStates);
                        } 
                    case 7 : 
                        {
                            FunctionGroupSeven(instructionpointer, MemoryLocations, RuntimeStates);
                        } 
                }
            }
    }        

    else if (HighEnd < 12)
    {
        if (HighEnd < 10)
        {
            switch (HighEnd)
            {
                case 8 :
                    {
                        FunctionGroupEight(instructionpointer, MemoryLocations, RuntimeStates);
                    }
            }
        }
        else
        { 
            NPUSH(instructionpointer, MemoryLocations);
        }
    }
            
    else if (HighEnd < 14)
    {   
        MDRP(instructionpointer, MemoryLocations, RuntimeStates);   
    } 

    else 
    {   
        MIRP(instructionpointer, MemoryLocations, RuntimeStates);
    }

    instructionpointer++;
}


uint32_t PUSH(uint8_t *instructionpointer, struct InstructionKit *MemoryLocations)
{   uint8_t *dummy; 
    *dummy = (*instructionpointer%10) -1;
    if (*dummy == 255)  //Next byte shall determine number loops and the loop starts on the next value; it compares to 255 since this is an unsigned value 
    {
        *dummy =  *(instructionpointer++);  //This should increment the instruction pointer; need to validate
        instructionpointer++;
        for (int i = 0; i < *dummy; i++)
        {
            MemoryLocations -> StackPointer++;
            *MemoryLocations -> StackPointer = *instructionpointer;
        }   
    }
}


uint32_t NPUSH(uint8_t *instructionpointer, struct InstructionKit *MemoryLocations)
{   
    uint8_t *dummy; 
    *dummy = *(instructionpointer + 1);
    if (*instructionpointer = 41)
    {
        instructionpointer += 2;  
        for(int i = 0; i < *dummy; i++)
        {
            *instructionpointer = (uint8_t)*(MemoryLocations -> StackPointer + i);
        }
    }
    
    else if (*dummy == 7)  //Instruction contained amount to be added -1
    {
        *dummy =  *(instructionpointer++)*2;  //This should increment the instruction pointer; need to validate
        instructionpointer++;
        for (int i =0; i < *dummy; i++)
        {
            MemoryLocations -> StackPointer++;
            *MemoryLocations -> StackPointer = *instructionpointer;
        }
    }

    else if (*dummy < 7)  //Instruction contained amount to be added -1
    {
        instructionpointer++;
        for (int i =0; i < *dummy; i++)
        {
            MemoryLocations -> StackPointer++;
            *MemoryLocations -> StackPointer = *instructionpointer;
        }
    }
    else 
    {
        *dummy = (*dummy - 8) *2;
        instructionpointer++;
        for (int i =0; i < *dummy; i++)
        {
            MemoryLocations -> StackPointer++;
            *MemoryLocations -> StackPointer = *instructionpointer;
        }
    }
        

    free(dummy);
    dummy = 0;

}



struct GraphicStates *InitializeGraphicStates()
{
    struct GraphicStates *instance;
    (*instance).zp[3];
    (*instance).rp[3];
    (*instance).conditonalpointer;
    (*instance).DeltaBase = 9;
    (*instance).DeltaShift = 3;
    (*instance).loop = 1;
    (*instance).InstructControl = 0;
    (*instance).autoFlip = 1;
    (*instance).roundState = 1;
    *(*instance).SuperBool = 0;
    (*instance).controlvalue_cut_in = 17;
    (*instance).single_width_cut_in = 0;
    (*instance).single_width_value = 0;
    (*instance).ProjectionVector[2];
    (*instance).ProjectionVector[0] = 0;
    (*instance).ProjectionVector[1] = 1; //default x
    (*instance).FreedomVector[2];  //default x
    (*instance).FreedomVector[0] = 0;
    (*instance).FreedomVector[1] = 1;    
    (*instance).DualProjectionVector[2];
    return instance;
};

