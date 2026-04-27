#include <xc.h>
#include <sys/attribs.h>
#include "Pic32Mx3Conf.h"

#define NOTE_G4   392  // 440/2^(2/12) = 392, 2 semit mai jos
#define NOTE_A4   440
#define NOTE_B4   494   //2 semit mai sus
#define NOTE_C5   523  //3 semit mai sus
#define REST        0

typedef struct {
    unsigned int freq;     
    unsigned int duration; // durat în milisecunde pt ca am in timer2 tic tac uri la 1ms
} Note;

#define QTR_MS   500
#define HLF_MS  1000

static const Note melody[] = {
    { NOTE_G4,  QTR_MS }, //happy
    { NOTE_G4,  QTR_MS }, //birth
    { NOTE_A4,  HLF_MS },  //day
    { NOTE_G4,  HLF_MS },  //to
    { NOTE_C5,  HLF_MS },  //yo
    { NOTE_B4,  HLF_MS },   //uuuuu
    { REST,     QTR_MS }
};
#define MELODY_LEN (sizeof(melody)/sizeof(melody[0]))  // dim totala a rand in octeti impartita la dim in oct a unui sg elem, ca sa vad cate struc imi incap


void ConfigureTimer3_OC1(unsigned int initialFreq) {
    tris_A_OUT  = 0;     //setez pinul de la oc1 ca output
    ansel_A_OUT = 0;     //ansel a out e pinul rpb14, si e 0 ca sa ii dezactivez fct analogiga si sa fie digitala
    rp_A_OUT    = 0x0C;  // pag 144 din datasheet, iesirea sa fie pe OC1

    // setez PR3 pt prima frecven
    PR3 = (unsigned int)(PB_FRQ / (8U * (initialFreq ? initialFreq : 1)) + 0.5);
    //pag 187
    T3CONbits.TCKPS = 3;  // prescalar 1:8, adica timer3 are 40/8=5, pag 174
    T3CONbits.ON    = 1;  // porneste Timer3

    OC1CONbits.OCTSEL = 1; // Timer3 e sursa
    OC1CONbits.OCM    = 6; // pwm, 110 e 6 in zecimal
    OC1CONbits.ON     = 1; // porneste OC1

    // intrerupere Timer3
    IPC3bits.T3IP = 7;    // prioritate max la Timer 3
    IPC3bits.T3IS = 3;    // subprioritate max -.-
    IFS0bits.T3IF = 0;    // restez cand termin cu o nota
    IEC0bits.T3IE = 1;    // cand termin o nota trec la urm

    macro_enable_interrupts();   //activ intreruperilor globale ca sa imi ia procesorul intreruperile
}

void __ISR(_TIMER_3_VECTOR, IPL7AUTO) Timer3Handler(void) {  //7 e prior max, intrerupere Timer3 
    
    OC1RS = PR3 / 2;      // 50%
    IFS0bits.T3IF = 0;    // restart
}


void ConfigureTimer2_1ms(void) {
    T2CONbits.ON    = 0;   
    T2CONbits.TCKPS = 7;   // 1/256=6.4ys-7

    PR2 = 156;       //pbclk=40/265=156; 1/126=6.4ys; 6.4ys x 1ms = 156   
                     //Timer2 numara pana la 156 dupa care sch nota
    
    // configure interrupt for Timer2
    IPC2bits.T2IP = 3; // prior medie
    IPC2bits.T2IS = 0; // subprior mic
    IFS0bits.T2IF = 0; // resetez cand termin cu o nota
    IEC0bits.T2IE = 1; //  cand termin o nota trec la urm

    T2CONbits.ON = 1;  // porneste Timer2
}

void __ISR(_TIMER_2_VECTOR, IPL3AUTO) Timer2Handler(void) {
    static unsigned int idx        = 0;   //indicele meu curent din melodie
    static unsigned int ms_counter = melody[0].duration;   // cate ms imi mai are nota
    Note n;

    IFS0bits.T2IF = 0;   // resetare

    if (--ms_counter == 0) {  //scad o ms
        // cand ajunge la 0 imi trece la urm nota
        idx = (idx + 1) % MELODY_LEN;
        n   = melody[idx];  //nota mea noua ce specificati are

        // reconfigurare PWM
        if (n.freq) {
            PR3 = (unsigned int)(PB_FRQ / (8U * n.freq) + 0.5);
            OC1CONbits.OCM = 6;  // PWM on
        } else {
            OC1CONbits.OCM = 0;  // pauza
        }

        // resetare ms pt noua nota
        ms_counter = n.duration;
    }
}

int main(void) {
   
    ConfigureTimer3_OC1(melody[0].freq);
    ConfigureTimer2_1ms();
    while (1) {;}
    return 0;
}
