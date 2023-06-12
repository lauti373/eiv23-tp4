#include <soporte_placa.h>
#include "controlador_luz.h"
#include "controlador_de_pulsaciones.h"
#include "pulsador.h"
#include "despacho_retardado.h"
#include <stddef.h>


#define PIN_LUZ SP_PIN_LED
#define PIN_PULSADOR SP_PB9

#define HISTERESIS_ANTIRREBOTE 5

#define LUZ_ON 0

#define PULSADOR_NIVEL_ACTIVO 0

#define TIEMPO_ON 60000

#define TIEMPO_TRES_PULSACIONES 2000


static Maquina * controlLuz;
static Maquina * controlPulsaciones;
static Pulsador pulsador[1];
static DespachoRetardado despachoRetardado[1];

/**
 * @brief Inicializa el estado del programa para iniciar la ejecución
 * 
 */
static void setup(void);


int main(void){    
    setup();
    for (;;){
        Maquina_procesa(controlPulsaciones);
        Maquina_procesa(controlLuz);
        DespachoRetardado_procesarDespacho(despachoRetardado);
        Pulsador_procesa(pulsador);
    }
    return 0;
}



static void setup(void){
    static ControladorLuz instanciaControlador;
    static ControladorDePulsaciones instanciaPulsaciones;
    
    SP_init();
    
    DespachoRetardado_init(despachoRetardado);

    ControladorLuz_init(&instanciaControlador,TIEMPO_ON,PIN_LUZ,LUZ_ON,despachoRetardado);
    controlLuz = ControladorLuz_asMaquina(&instanciaControlador);
    Maquina_procesa(controlLuz); // Reset inicializa pin con luz apagada
    
    ControladorDePulsaciones_init(&instanciaPulsaciones,controlLuz,despachoRetardado,TIEMPO_TRES_PULSACIONES
);
    controlPulsaciones = ControladorDePulsaciones_asMaquina(&instanciaPulsaciones);

    Pulsador_init(pulsador, 
                  controlPulsaciones,
                  EV_BOTON_PULSADO,
                  PIN_PULSADOR,
                  PULSADOR_NIVEL_ACTIVO,
                  HISTERESIS_ANTIRREBOTE);
}