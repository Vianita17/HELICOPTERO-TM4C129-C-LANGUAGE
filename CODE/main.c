
/*Codigo principal para un control por posicion usando PID
Modulos usados:
  PWM (Pulse Width Module)
  SPI (Synchronous Peripheral Interface)
  QEI (Quadrature Enconder Interface)

*/

#include <stdint.h>
#include "tm4c1294ncpdt.h"
#include "PWM.h"
#include "QEI.h"
#include "MAX7219.h"

// QEI
#define QEI_MIN           0      // Posicion minima del encoder
#define QEI_MAX           935    // Posicion maxima del encoder
#define QEI_MID           435    // Posicion media (equilibrio)

//umbral de referencia
#define REF_LOW        405    // Limite bajo de banda ideal
#define REF_HIGH      469    // Limite alto de banda ideal

//PID 
#define KP    0.3f    //Ganancia Proporcional
#define KI    0.03f   // Ganancia integral
#define KD    4.0f    // Ganancia derivativa
#define DT    0.01f   // Paso de tiempo logico -> 10ms

//PWM 
#define Fclk 16000000 //Frecuencia que esta trabajando el Reloj de la tiva
#define Fpwm 2000 //Frecuencia minima deseada

#define PWM_PERIOD  (uint16_t)(Fclk/Fpwm) //Periodo = Fclk/Fpwm

#define PWM_MIN    ((uint16_t)(PWM_PERIOD * 0.10f))  // Duty circle (10%)
#define PWM_MAX    ((uint16_t)(PWM_PERIOD * 0.80f))  // Duty circle (80%)
#define PWM_EQ     ((uint16_t)(PWM_PERIOD * 0.35f))  // Duty circle (35%)

#define I_MAX  ((float)PWM_PERIOD * 0.25f)  // Limite anti-windup de integral

//VARIABLES GLOBALES

    float error, prev_error = 0.0f;
    float integral = 0.0f;
    float derivative;
    float control;
    float duty_f;
    uint16_t duty;

//delay
static void Delay(void){ // Retardo simple por software -simple
    volatile uint32_t i;
    for(i = 0; i < 40000; i++){}
}

void ControlPID(uint16_t pos)
{
				// Error respecto al medio
        // pos baja -> error + -> duty sube 
        error = (float)((int32_t)QEI_MID - (int32_t)pos); // Calcula error (signo correcto)

        // 4) Banda ideal: 416..469 => error=0
        if(pos >= REF_LOW && pos <= REF_HIGH){
            error = 0.0f;
            prev_error = 0.0f;
        }

        // (anti windup)
        integral += error * KI * DT;
        if(integral >  I_MAX) integral =  I_MAX;
        if(integral < -I_MAX) integral = -I_MAX;

        // derivada
        derivative = (error - prev_error) / DT;

        // PID
        control = KP*error + integral + KD*derivative;
        prev_error = error;

        // Duty
        duty_f = (float)PWM_EQ + control;

        if(duty_f < (float)PWM_MIN) duty_f = (float)PWM_MIN;
        if(duty_f > (float)PWM_MAX) duty_f = (float)PWM_MAX;

        duty = (uint16_t)duty_f;

        PWM0A_Duty(duty);

}

// Funcion principal
int main(void){
		uint16_t Q_pos;
    QEI_Init();                                 // Inicializa QEI0 en cuadratura
		MAX7219_Init();                             // Inicializa MAX7219
    PWM0A_Init(PWM_PERIOD, PWM_EQ);// Inicializa PWM0A en PF0
    // PWM0B_Init(PWM_PERIOD, PWM_EQ);          // PWM0B opcional (PF1)

    while(1){
        Q_pos = QEI_pos();
        if(Q_pos > QEI_MAX) Q_pos = QEI_MAX;
        //Transmitimos poisicion QEI al MAX7219 
        Transmite_Number(Q_pos);
        ControlPID(Q_pos);//Hacemos el control PID
        Delay(); // Retardo para suavizar control
    }
}
