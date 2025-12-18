// PWM.c
#include <stdint.h>
#include "tm4c1294ncpdt.h"
#include "PWM.h"

// PWM0A en PF0
void PWM0A_Init(uint16_t period, uint16_t duty){

  SYSCTL_RCGCPWM_R |= SYSCTL_RCGCPWM_R0;
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;
  while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5) == 0){};

  // PF0 como M0PWM0
  GPIO_PORTF_AFSEL_R |= 0x01;
  GPIO_PORTF_DEN_R   |= 0x01;
  GPIO_PORTF_PCTL_R   = (GPIO_PORTF_PCTL_R & 0xFFFFFFF0) | 0x00000006; // PCTL: PF0->PWM
  GPIO_PORTF_AMSEL_R &= ~0x01;

  while((SYSCTL_PRPWM_R & SYSCTL_PRPWM_R0) == 0){};

	// Usar reloj del sistema directamente (16 MHz, sin divisor)
	PWM0_CC_R &= ~PWM_CC_USEPWM;   // USEPWM = 0 -> ignora PWMDIV, usa el clock base


  PWM0_0_CTL_R = 0;

  // PF0=0 en LOAD, PF0=1 en CMPA (modo down-count)
  PWM0_0_GENA_R = (PWM_0_GENA_ACTLOAD_ZERO |
                   PWM_0_GENA_ACTCMPAD_ONE);

  PWM0_0_LOAD_R = period - 1;
  PWM0_0_CMPA_R = duty   - 1;

  PWM0_0_CTL_R   |= PWM_0_CTL_ENABLE; 
  PWM0_ENABLE_R  |= PWM_ENABLE_PWM0EN;
}

//Funcion para cmabiar en el proceso
void PWM0A_Duty(uint16_t duty){
  if(duty < 2) duty = 2;
  PWM0_0_CMPA_R = duty - 1;
}

//PWM0B en PF1 (opcional)
void PWM0B_Init(uint16_t period, uint16_t duty){ 

  SYSCTL_RCGCPWM_R |= SYSCTL_RCGCPWM_R0;
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;
  while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5) == 0){};

  // PF1 como M0PWM1
  GPIO_PORTF_AFSEL_R |= 0x02;
  GPIO_PORTF_DEN_R   |= 0x02;
  GPIO_PORTF_PCTL_R   = (GPIO_PORTF_PCTL_R & 0xFFFFFF0F) | 0x00000060; // PCTL: PF1->PWM
  GPIO_PORTF_AMSEL_R &= ~0x02; 
  while((SYSCTL_PRPWM_R & SYSCTL_PRPWM_R0) == 0){};

	// Usar reloj base del sistema (16 MHz, sin divisor)
  PWM0_CC_R &= ~PWM_CC_USEPWM;   // USEPWM = 0 -> ignora PWMDIV, usa clock base	
	/*
  PWM0_CC_R |=  PWM_CC_USEPWM;
  PWM0_CC_R &= ~PWM_CC_PWMDIV_M;
  PWM0_CC_R |=  PWM_CC_PWMDIV_64;
	*/
  PWM0_0_CTL_R = 0;                                        
  PWM0_0_GENB_R = (PWM_0_GENB_ACTLOAD_ZERO |
                   PWM_0_GENB_ACTCMPBD_ONE);

  PWM0_0_LOAD_R = period - 1;
  PWM0_0_CMPB_R = duty   - 1;

  PWM0_0_CTL_R   |= PWM_0_CTL_ENABLE;
  PWM0_ENABLE_R  |= PWM_ENABLE_PWM1EN;
}

void PWM0B_Duty(uint16_t duty){                              // Actualiza duty de PWM0B
  if(duty < 2) duty = 2;
  PWM0_0_CMPB_R = duty - 1;
}
