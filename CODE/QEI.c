#include <stdint.h>
#include "tm4c1294ncpdt.h" 
#include "QEI.h"

void QEI_Init(void){ 
    SYSCTL_RCGCQEI_R  |= SYSCTL_RCGCQEI_R0; 
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R10; 
    while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R10)==0){}

    GPIO_PORTL_AFSEL_R |= 0x0E;             // Activa funcion alterna en PL1, PL2, PL3
    GPIO_PORTL_PCTL_R   = (GPIO_PORTL_PCTL_R & 0xFFFF000F) | 0x00006660; // Mapea PL1-3 a QEI
    GPIO_PORTL_DEN_R   |= 0x0E;

    while((SYSCTL_PRQEI_R & SYSCTL_PRQEI_R0)==0){}

    QEI0_CTL_R &= ~QEI_CTL_ENABLE;

    QEI0_CTL_R &= ~QEI_CTL_FILTCNT_M;
    QEI0_CTL_R |=  QEI_CTL_FILTEN;

    QEI0_CTL_R &= ~QEI_CTL_VELEN; 
    QEI0_CTL_R &= ~QEI_CTL_RESMODE; // Selecciona reset por MAXPOS (no por IDX)

    QEI0_CTL_R &= ~QEI_CTL_SIGMODE;         // SIGMODE=0 -> modo cuadratura PhA/PhB
    QEI0_CTL_R |=  QEI_CTL_CAPMODE;         // CAPMODE=1 -> cuenta flancos A y B

    QEI0_MAXPOS_R = 4095;
    QEI0_POS_R = 0;
    QEI0_CTL_R |= QEI_CTL_ENABLE;
}

uint16_t QEI_pos(void){ 
    return (uint16_t)QEI0_POS_R;            // Retorna los 16 bits bajos del contador
}
