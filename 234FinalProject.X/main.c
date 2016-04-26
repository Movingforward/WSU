#include <xc.h>             /* contains Vector Name/Number Macros */
#include <sys/attribs.h>    /* contains __ISR() Macros */
#include <stdio.h>

/************************Oscillator Settings****************************/
#pragma config FNOSC = PRIPLL // Oscillator selection
#pragma config POSCMOD = EC // Primary oscillator mode
#pragma config FPLLIDIV = DIV_2 // PLL input divider
#pragma config FPLLMUL = MUL_20 // PLL multiplier
#pragma config FPLLODIV = DIV_1 // PLL output divider
#pragma config FPBDIV = DIV_8 // Peripheral bus clock divider

#pragma config FSOSCEN = OFF // Secondary oscillator enable
#pragma config IESO = OFF // Internal/external clock switchover
#pragma config FCKSM = CSDCMD // Clock switching (CSx)/Clock monitor(CMx)
#pragma config OSCIOFNC = OFF // Clock output on OSCO pin enable
#pragma config UPLLEN = ON // USB PLL enable
#pragma config UPLLIDIV = DIV_2 // USB PLL input divider

/******************Other Peripheral Device settings*********************/
#pragma config FWDTEN = OFF // Watchdog timer enable
#pragma config WDTPS = PS1024 // Watchdog timer post-scaler

/*******************Code Protection settings******************************/
#pragma config CP = OFF // Code protection
#pragma config BWP = OFF // Boot flash write protect
#pragma config PWP = OFF // Program flash write protect


typedef enum{Line_Search, Line_Track, Stop} Operation_Mode;
typedef enum{On, Off} Power_State_Mode;
typedef enum{Forward, Backward, TurnRight,TurnLeft} Movement_Mode;
Power_State_Mode Power = Off; // on or off state
Operation_Mode mode = Stop; //operation state
Movement_Mode mode2 = Backward;   //movement state

/*********************LOCAL PROTOTYPES*******************************/
void InitializeSystem();
void Initialize_Timer1();
void Initialize_Timer2();
void Initialize_Timer3();
void Initialize_Timer4();
void Initialize_OC();// Initialize hardware and global variables
void Initialize_IO();
void Initialize_IC();
void Interrupts_enable();
void Motor_Forward();
void Motor_Backward();
void centerRight();
void turnRight();
void centerLeft();
void TurnLeft();
void Stop();
void Power_On();
void Find_Line();




/** main() ********************************************************************/

unsigned int Timer3OV2 = 0;
unsigned int Timer3OV3 = 0;


int main(void)
{
        InitializeSystem();
        Interrupts_enable();
    
        while(1)
        {
             if (PORTAbits.RA6 == 1)
             {       
                 Power = On;
             }       
             else if (PORTAbits.RA7 == 1)            
             {
                 Power = Off;
             } 
        }
} 

void InitializeSystem()
{
    Initialize_IO();
    Initialize_OC();
    Initialize_Timer1();
    Initialize_Timer2();
    Initialize_Timer3();
    Initialize_Timer4();
    Initialize_IC();
} 

void Initialize_IC()
{
// Initialize IC2
    TRISDbits.TRISD9 = 1;
    IC2CON= 0x0000;
    IC2CONbits.SIDL = 1; // halt in CPU Idle Mode
    IC2CONbits.C32 = 0; // 16bit timer;
    IC2CONbits.ICTMR = 0; // select Timer 3;
    IC2CONbits.ICI = 3; // interrupt every 4 events
    IC2CONbits.ICM = 5; // increment every 16 rising edge
    
    
    // Initialize IC3
    TRISDbits.TRISD10 = 1;
    IC3CON= 0x0000;
    IC3CONbits.SIDL = 1; // halt in CPU Idle Mode
    IC3CONbits.C32 = 0; // 16bit timer;
    IC3CONbits.ICTMR = 0; // select Timer 3;
    IC3CONbits.ICI = 3; // interrupt every fourth event
    IC3CONbits.ICM = 5; // increment every 16 rising edge
    
    
}

void Initialize_IO()
{
    //buttons 1 & 2 = inputs
    TRISAbits.TRISA6 = 1;
    TRISAbits.TRISA7 = 1;
    
    //LEDs
    TRISBbits.TRISB10 = 0;
    TRISBbits.TRISB11 = 0;
    TRISBbits.TRISB12 = 0;
    TRISBbits.TRISB13 = 0;
    
    //LF Sensor ports
    TRISGbits.TRISG12 = 1; 
    TRISGbits.TRISG13 = 1;
    TRISGbits.TRISG14 = 1; 
    TRISGbits.TRISG15 = 1; 
    
    //Motor Direction bits
    TRISDbits.TRISD6 =0; 
    TRISDbits.TRISD7 =0; 
    
    TRISDbits.TRISD1 =0;
    LATDbits.LATD1 = 0;
    TRISDbits.TRISD2 =0;
    LATDbits.LATD2 =0; 
}

void Initialize_OC()
{
     // output compare
    OC2CONbits.OCM =6;
    OC2R = 0;
    OC2RS = 0;
    OC2CONbits.ON=1;
      
    OC3CONbits.OCM =6;
    OC3R = 0;
    OC3RS = 0;
    OC3CONbits.ON = 1;
    
    /* Set Interrupt Controller for multi-vector mode */
}

void Initialize_Timer1()
{
    T1CONbits.TON = 0;
    T1CONbits.TCKPS = 3;
    PR1 = 250;
    TMR1 = 0;   
    T1CONbits.ON =1;
}

void Initialize_Timer2()
{
    T2CONbits.TON = 0;
    T2CONbits.TCKPS = 7;
    PR2 = 2000;
    TMR2 = 0;   
    T2CONbits.ON =1;
}

void Initialize_Timer3()
{
    T3CONbits.TON = 0;
    T3CONbits.TCKPS = 7;
    PR3 = 0xFFFF;
    TMR3 = 0;   
    T3CONbits.ON =1;
}

void Initialize_Timer4()
{
    T4CON = 0x0;
    T4CONSET = 0x0070;
    PR4 = 0xFFFF;
    TMR4 = 0;   
    T4CONSET = 0x8000;
}

void Interrupts_enable()
{
    // Timer one interrupt is enabled when it enters the Path search mode
    IPC1bits.T1IP = 7;
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 0; 
    
    // Interrupt for IC2
    IFS0bits.IC2IF =0;
    IPC2bits.IC2IP = 7;
    IEC0bits.IC2IE = 1;
    
    //// Interrupt for IC3
    IFS0bits.IC3IF =0;
    IPC3bits.IC3IP = 7;
    IEC0bits.IC3IE = 1;
    
    //turn ON IC2 & IC3
    IC2CONbits.ON = 1;
    IC3CONbits.ON= 1;
    
     //Interrupt for Timer1
    IFS0bits.T1IF =0;
    IPC1bits.T1IP = 7;
    IEC0bits.T1IE = 1;
    
    INTCONbits.MVEC = 1;
    __builtin_enable_interrupts();
}

void Motor_Forward()
{
    LATDbits.LATD6 = 1; 
    LATDbits.LATD7 = 0;  
    OC2RS = 800;
    OC3RS = 800;
}

void Stop()
{
    OC2RS = 0;
    OC3RS = 0;      
}

void centerRight()
{
    LATDbits.LATD6 = 1; 
    LATDbits.LATD7 = 0; 
    OC2RS = 700; //OC for right motor
    OC3RS = 400; //OC for left motor
}

void turnRight()
{
    LATDbits.LATD6 = 0; 
    LATDbits.LATD7 = 0; 
    OC2RS = 800; //Right motor speed
    OC3RS = 800; //Left motor speed
}

void centerLeft()
{
    LATDbits.LATD6 = 1; 
    LATDbits.LATD7 = 0; 
    OC2RS = 400;
    OC3RS = 700;
}

void TurnLeft()
{
    LATDbits.LATD6 = 1; 
    LATDbits.LATD7 = 1; 
    OC2RS = 800;
    OC3RS = 800;
}

void Motor_Backward()
{
        LATDbits.LATD6 = 0; 
        LATDbits.LATD7 = 1; 
        OC2RS = 800;
        OC3RS = 800;
}

void __ISR (_TIMER_1_VECTOR, IPL7SOFT) T1Interrupt(void)
{
    //Indexing Variable Declarations
   int index = 0;
   int rotate = 0;
    
    if (Power == On)
    {
        if (mode == Stop)
        {
            if (PORTGbits.RG15 == 0 || PORTGbits.RG14 == 0 || PORTGbits.RG13 == 0 || PORTGbits.RG12 == 0) // Picks up a line
            {
                mode = Line_Track;
            }
            else if(PORTGbits.RG15 == 1 && PORTGbits.RG14 == 1 && PORTGbits.RG13 == 1 && PORTGbits.RG12 == 1) // No line found
            {
                mode = Line_Search;
            }
        }
        else if (mode == Line_Track) //Found line
        {
            
                Motor_Forward(); //By default move forward
            
                
                if (PORTGbits.RG12 == 0 && PORTGbits.RG14 == 1 && PORTGbits.RG15 == 1) //
                {
                    TurnLeft();
                }

                
                else if (PORTGbits.RG15 == 0 && PORTGbits.RG12 == 1 && PORTGbits.RG13 == 1) 
                {
                    turnRight();
                }  
                
                else if (PORTGbits.RG13 == 0 && PORTGbits.RG12 == 1 && PORTGbits.RG14 == 1 && PORTGbits.RG15 == 1)
                {
                    centerLeft();
                }
                else if (PORTGbits.RG14 == 0 && PORTGbits.RG12 == 1 && PORTGbits.RG13 == 1 && PORTGbits.RG15 == 1)
                {
                    centerRight();
                }
                
                else if (PORTGbits.RG15 == 0 && PORTGbits.RG14 == 0 && PORTGbits.RG13 == 0 && PORTGbits.RG12 == 1) 
                {
                    turnRight();
                }
                else if (PORTGbits.RG12 == 0 && PORTGbits.RG13 == 0 && PORTGbits.RG14 == 0 && PORTGbits.RG15 == 1) 
                {
                    TurnLeft();
                }
                else if (PORTGbits.RG12 == 0 && PORTGbits.RG13 == 0 && PORTGbits.RG14 == 0 && PORTGbits.RG15 == 0)
                {
                    TMR4 = 0;
                    index = 0;
                    while (index < 2)
                    {
                        while (TMR4 < 39063) //Second Calculation used from Project 1
                        {
                            TurnLeft();
                        }
                        index++;
                    }
                    TMR4 = 0;
                    index = 0;
                    while (index < 2)
                    {
                        while (TMR4 < 39063)
                        {
                            centerLeft();
                        }
                        index++;
                    }
                    
                }
                //Goes off line
                else if (PORTCbits.RC12 == 1 && PORTCbits.RC13 == 1 && PORTCbits.RC14 == 1 && PORTCbits.RC15 == 1)
                {
                    mode = Line_Search;
                }
            
        }
        else if (mode == Line_Search)
        {
            index = 0;
            rotate = 0;
            
            while (rotate < 4)
            {
                //Moves forward for 3 seconds to abide by project requirements
                index = 0;
                while (index < 10)
                {
                    TMR4 = 0;
                    while (TMR4 < 39063)
                    {
                        if (PORTGbits.RG12 == 0 || PORTGbits.RG13 == 0 || PORTGbits.RG14 == 0 || PORTGbits.RG15 == 0)
                        {
                            mode = Line_Track;
                            goto end;
                        }
                        if (PORTAbits.RA7 == 1)
                        {
                            mode = Stop;
                            Power = Off;
                            goto end;
                        }
                        mode2 = Forward;
                        Motor_Forward();
                    }
                    index++;
                }
                //Backwards Distance = Forwards Difference
                index = 0;
                while (index < 10)
                {
                    TMR4 = 0;
                    while (TMR4 < 39063)
                    {
                        if (PORTGbits.RG12 == 0 || PORTGbits.RG13 == 0 || PORTGbits.RG14 == 0 || PORTGbits.RG15 == 0)
                        {
                           mode = Line_Track;
                           goto end;
                        }
                        if (PORTAbits.RA7 == 1)
                        {
                            mode = Stop;
                            Power = Off;
                            goto end;
                        }
                        mode2 = Backward;
                        Motor_Backward();
                    }
                    index++;
                }

                //Rotates 90 Degrees
                index = 0;
                while (index < 5)
                {
                    TMR4 = 0;
                    while (TMR4 < 9766)
                    {
                        if (PORTGbits.RG12 == 0 || PORTGbits.RG13 == 0 || PORTGbits.RG14 == 0 || PORTGbits.RG15 == 0)
                        {
                           mode = Line_Track;
                           goto end;
                        }
                        if (PORTAbits.RA7 == 1)
                        {
                            mode = Stop;
                            Power = Off;
                            goto end;
                        }
                        mode2 = TurnLeft;
                        TurnLeft();
                    }
                    index++;
                }
                rotate++;
            }

            if (rotate == 4)
            {
                Power = Off;
            }

            end:;           
        }
    }
    else if (Power == Off)
    {
        mode = Stop;
        Stop();
    }
    
    IFS0bits.T1IF = 0;
}