#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include  <TrueFontTypes.h>
#include  <FontEngineInit.h>
#include  <InstructionGroups/FunctionGroupSeven.h>

static inline void FunctionGroupSix(uint8_t *instructionpointer , struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)
{
    uint8_t LowEnd  = *instructionpointer & 15;
    F2Dot14 *StackPointer = (F2Dot14 *) MemoryLocations -> StackPointer;
    switch (LowEnd)

    if(LowEnd < 8)
    {   
        if (LowEnd <4)  
        {
            if (LowEnd < 2)   
            {
            
                switch (LowEnd)
                {   
                case 0 :
                        {
                            ADD(StackPointer);
                        }
                case 1 :
                        {
                            SUB(StackPointer);
                        }
                }
            }
            else 
            {
                switch (LowEnd)
                {
                case 2 : 
                        {
                            DIV(StackPointer);
                        } 
                case 3 : 
                        {
                            MUL(StackPointer);
                        }
                }
            }
        }
        else if (LowEnd < 6) 
        {
            switch (LowEnd)
            {
                case 4 : 
                        {
                            ft_abs(StackPointer);
                        } 
                case 5 : 
                        {
                            NEG(StackPointer);
                        } 
            }
        }
        else 
        {
            switch (LowEnd)
            {
                case 6 : 
                        {
                            ft_floor(*StackPointer);
                        } 
                case 7 : 
                        {
                            ft_ceil(*StackPointer);
                        } 
            }
        }
    }    

    else if (LowEnd < 8)
    {
     ROUND(LowEnd, RuntimeStates -> roundState, *StackPointer);   //Low End has the values for what to add
    }
}

static inline void ADD(F2Dot14 *StackPointer)
{   
    F2Dot14 *dummy; 
    *dummy = *StackPointer;
    StackPointer --;
    *StackPointer  = *StackPointer  + *dummy;
    free(dummy);
    dummy = 0;
    *(StackPointer + 1) = 0;
} 
static inline void SUB(F2Dot14 *StackPointer)
{
    F2Dot14 *dummy;
    *dummy = *StackPointer;
    StackPointer --;
    *StackPointer  = *StackPointer  - *dummy;
    free(dummy);
    dummy = 0;
    *(StackPointer + 1) = 0;
} 


static inline void DIV(F2Dot14 *StackPointer)
{   
    F2Dot14 *dummy; 
    *dummy = (F2Dot14)*StackPointer;
    StackPointer --;
    *StackPointer  = *StackPointer  / *dummy;
    free(dummy);
    dummy = 0;
    *(StackPointer + 1) = 0;
}

F2Dot14 MUL(F2Dot14 *StackPointer)
{   
    F2Dot14 *dummy; 
    *dummy = *StackPointer;
    StackPointer--;
    *StackPointer  = *StackPointer  * *dummy;
    free(dummy);
    dummy = 0;
    *(StackPointer + 1) = 0;
}

static inline uint8_t NEG(F2Dot14 *StackPointer)
{   
    *StackPointer  = -*(StackPointer);

}

static inline uint8_t ft_abs(F2Dot14 *StackPointer)
{
    if (*StackPointer  > 0)
        {
            *StackPointer  == -(*StackPointer);
        }
} 
static inline uint8_t ft_floor(F2Dot14 StackPointer)
{
    floor(StackPointer);
} 
static inline uint8_t ft_ceil(F2Dot14 StackPointer)
{
    ceil(StackPointer);
} 



F2Dot14 ROUND(char boolean, uint32_t roundState, F2Dot14 Arg)
{
    
    if (boolean < 3)
    {
        if(boolean == 1)
        {
            F2Dot14 Period = roundState & 192;
            F2Dot14 Phase = Period/(roundState  & 48);
            Arg -= Phase;
            Arg+= (F2Dot14)(((roundState  & 15) - 1)/Period);
            Arg *= (Period - Phase);
            floor(Arg);
            Arg /= Period;
            Arg += Phase;
        }
        else
        {
            F2Dot14 Period = (roundState & 192) * (sqrt(2)/2);
            F2Dot14 Phase = Period/(roundState  & 48);
            Arg -= Phase;
            Arg += (F2Dot14)(((roundState  & 15) - 1)/Period);
            Arg *= (Period - Phase);
            floor(Arg);
            Arg /= Period;
            Arg += Phase;
        }
        
    }

    else if (boolean < 12 )
    {
        Arg += boolean - 8; 
    }
    else 
    {
        Arg += boolean - 12; 
        roundState = 0;
    }

    if (roundState & 128 == 180)
        {

        }

    else if (roundState == 0)    //Round to Grid

        { 
           Arg = PointFiveRound(Arg);
        }
    else if  (roundState == 1)
        {
            if (StandardRound(Arg) == 1)
                {
                ft_ceil(Arg);
                }
            else
                {
                ft_floor(Arg);
                }
        }
    else if (roundState == 2)   // Round to DG
            {
            if (StandardRound(Arg) == 1)
                {
                Arg = ft_ceil(Arg) + 1;
                }
            else
                {
                Arg = ft_floor(Arg) - 1;
                }
            }

        
    else if (roundState == 3)       //Round down to Grid
            {
                    ft_floor(Arg);
            }
    else if (roundState == 4)       //Round up to Grid
            {
                    ft_ceil(Arg);
            }
    
}


float PointFiveRound(float Arg)
{
    int deci = (int)Arg & ((Sizeof(Arg) - 9) << 255);
    if ((int)Arg & (deci << 1) == 1)
    {
        if ((int)Arg & (deci - 1 << 1) == 1)
        {
            Arg = (int)Arg & (255 << 23);
            Arg = Arg + 1;
        }
        else 
        {
        Arg = (int)Arg & (255 << 23);
        Arg = (int)Arg | 23 << 1;
        }
    }
    else if ((int)Arg & (deci - 1 << 1) == 1)
    {
        Arg = (int)Arg & (255 << 23);
        Arg = (int)Arg | 23 << 1;

    }
    else 
    {
        Arg = (int)Arg & (255 << 23);
    }
}

int StandardRound(float Arg)
{
    int deci = (int)Arg & ((Sizeof(Arg) - 9) << 255);
    if ((int)Arg & deci == 1)
    {
        Arg = (int)Arg & (255 << 23);
        return 1;
    }
    else 
    {
        return 0;
    }
}

