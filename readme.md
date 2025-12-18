# Proyecto: Control de Posición PID (TM4C1294NCPDT)

## Descripción General

Este proyecto implementa un sistema de control por Posición utilizando un algoritmo PID (Proporcional-Integral-Derivativo) para mantener un mecanismo  en una posición de equilibrio. El sistema utiliza un codificador de cuadratura (QEI) para medir la posición actual y un módulo PWM para accionar el motor que ejerce el control.

El código está diseñado para la familia de microcontroladores **TM4C1294NCPDT** de Texas Instruments.

## Módulos de Hardware Utilizados

El proyecto depende de la correcta inicialización y uso de los siguientes módulos:

| Módulo | Función |
| :----- | :------ |
| **QEI** | Lectura de la posición del codificador de cuadratura. |
| **PWM** | Generación de la señal de ancho de pulso modulado para el motor. |
| **SPI** | Comunicación serial con el controlador de display (MAX7219). |
| **MAX7219**| Control de un display de 7 segmentos para visualizar la posición actual. |

## Parámetros y Constantes Clave

### 1. Codificador de Cuadratura (QEI)

Se define la banda de operación y la referencia de equilibrio:

| Constante | Valor | Descripción |
| :-------- | :---- | :---------- |
| `QEI_MIN` | 0 | Posición mínima del encoder. |
| `QEI_MAX` | 935 | Posición máxima del encoder. |
| `QEI_MID` | 435 | **Posición de Referencia (Set-Point)**. |
| `REF_LOW` | 405 | Límite bajo de la banda ideal (umbral de inactividad). |
| `REF_HIGH`| 469 | Límite alto de la banda ideal (umbral de inactividad). |

### 2. Control PID

Se utilizan ganancias y un paso de tiempo lógico para el cálculo de la derivada y la integral:

| Constante | Valor | Función |
| :-------- | :---- | :------ |
| `KP` | 0.3f | Ganancia Proporcional. |
| `KI` | 0.03f | Ganancia Integral. |
| `KD` | 4.0f | Ganancia Derivativa. |
| `DT` | 0.01f | Paso de tiempo lógico (10ms). |
| `I_MAX` | 25% del período PWM | Límite para la acumulación integral (**Anti-Windup**). |

### 3. PWM

La señal PWM opera a una frecuencia mínima deseada de 2000 Hz, asumiendo un reloj del sistema de 16 MHz.

| Constante | Valor | Descripción |
| :-------- | :---- | :---------- |
| `Fclk` | 16 MHz | Frecuencia del reloj del microcontrolador. |
| `Fpwm` | 2 KHz | Frecuencia PWM deseada. |
| `PWM_PERIOD`| `Fclk / Fpwm` | Valor del contador para el período PWM. |
| `PWM_MIN` | 10% del Período | Ciclo de trabajo mínimo. |
| `PWM_MAX` | 80% del Período | Ciclo de trabajo máximo. |
| `PWM_EQ` | 35% del Período | Ciclo de trabajo de equilibrio (usado como offset base). |


## Licencia

Este proyecto está licenciado bajo la Licencia **MIT**.

Consulte el archivo [LICENSE](LICENSE) para obtener los detalles completos.