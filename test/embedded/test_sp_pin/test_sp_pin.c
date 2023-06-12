#include <unity.h>
#include <soporte_placa.h>
#include <stm32f1xx.h>


void setUp(){
    __disable_irq();                       
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;     
    RCC->APB2RSTR = RCC_APB2RSTR_IOPBRST;   
    RCC->APB2RSTR = 0;                      
    RCC->APB2ENR &= ~RCC_APB2ENR_IOPBEN;    
    __enable_irq();                         
}
void tearDown (void) {

}

static void test_config_entrada(oid) {
    uint32_t OFFSET = 4U;                               
    uint32_t MASK_PIN9_MODO = (0xF << OFFSET);          
    uint32_t MODO_ESPERADO = 0b0100;                   
    SP_Pin_setModo(SP_PB9,SP_PIN_SALIDA);               
    uint32_t CRL_ANTES = GPIOB->CRL;                    
    uint32_t CRH_ANTES = GPIOB->CRH;
    uint32_t ODR_ANTES = GPIOB->ODR;

    SP_Pin_setModo(SP_PB9, SP_PIN_ENTRADA);         

    uint32_t CRL_DESPUES = GPIOB->CRL;                  
    uint32_t CRH_DESPUES = GPIOB->CRH;
    uint32_t ODR_DESPUES = GPIOB->ODR;
    uint32_t DELTA_CRL = CRL_ANTES ^ CRL_DESPUES;       
    uint32_t DELTA_CRH = CRH_ANTES ^ CRH_DESPUES; 
    uint32_t DELTA_ODR = ODR_ANTES ^ ODR_DESPUES;
    uint32_t MODO_ACTUAL = (GPIOB->CRH & MASK_PIN9_MODO) >> OFFSET; 
    bool CLOCK_B_ENCENDIDO = RCC->APB2ENR & RCC_APB2ENR_IOPBEN;     

    TEST_ASSERT_TRUE(CLOCK_B_ENCENDIDO);                            
    TEST_ASSERT_FALSE(DELTA_CRL);                                   
    TEST_ASSERT_FALSE(DELTA_ODR);                                   
    TEST_ASSERT_BITS_LOW(~MASK_PIN9_MODO,DELTA_CRH);                
    TEST_ASSERT_TRUE(MODO_ESPERADO & MODO_ACTUAL);                  
}

static void test_config_entradaPullUp(void) {
    uint32_t OFFSET = 4U;
    uint32_t MASK_CRH = (0xF << 4U);
    uint32_t MASK_ODR = (0b1 << 9U);                               
    uint32_t MODO_ESPERADO = 0b1000;                                
    SP_Pin_setModo(SP_PB9,SP_PIN_ENTRADA);
    uint32_t CRL_ANTES = GPIOB->CRL;
    uint32_t CRH_ANTES = GPIOB->CRH;
    uint32_t ODR_ANTES = GPIOB->ODR;

    SP_Pin_setModo(SP_PB9, SP_PIN_ENTRADA_PULLUP);

    uint32_t CRL_DESPUES = GPIOB->CRL;
    uint32_t CRH_DESPUES = GPIOB->CRH;
    uint32_t ODR_DESPUES = GPIOB->ODR;
    uint32_t DELTA_CRL = CRL_ANTES ^ CRL_DESPUES;
    uint32_t DELTA_CRH = CRH_ANTES ^ CRH_DESPUES; 
    uint32_t DELTA_ODR = ODR_ANTES ^ ODR_DESPUES;                   
    uint32_t MODO_ACTUAL = (GPIOB->CRH & MASK_CRH) >> OFFSET;
    bool CLOCK_B_ENCENDIDO = RCC->APB2ENR & RCC_APB2ENR_IOPBEN;

    TEST_ASSERT_TRUE(CLOCK_B_ENCENDIDO);
    TEST_ASSERT_FALSE(DELTA_CRL);
    TEST_ASSERT_BITS_LOW(~MASK_ODR,DELTA_ODR);                      
    TEST_ASSERT_BITS_LOW(~MASK_CRH,DELTA_CRH);
    TEST_ASSERT_TRUE(MODO_ESPERADO & MODO_ACTUAL);
    TEST_ASSERT_TRUE(MASK_ODR & ODR_DESPUES);                       
}

static void test_config_entradaPullDown(void) {

    uint32_t OFFSET = 4U;
    uint32_t MASK_CRH = (0xF << OFFSET);
    uint32_t MASK_ODR = (0b1 << 9U);  
    uint32_t MODO_ESPERADO = 0b1000;
    SP_Pin_setModo(SP_PB9,SP_PIN_ENTRADA);
    uint32_t CRL_ANTES = GPIOB->CRL;
    uint32_t CRH_ANTES = GPIOB->CRH;
    uint32_t ODR_ANTES = GPIOB->ODR;

    SP_Pin_setModo(SP_PB9, SP_PIN_ENTRADA_PULLDN);

    uint32_t CRL_DESPUES = GPIOB->CRL;
    uint32_t CRH_DESPUES = GPIOB->CRH;
    uint32_t ODR_DESPUES = GPIOB->ODR;
    uint32_t DELTA_CRL = CRL_ANTES ^ CRL_DESPUES;
    uint32_t DELTA_CRH = CRH_ANTES ^ CRH_DESPUES; 
    uint32_t DELTA_ODR = ODR_ANTES ^ ODR_DESPUES;
    uint32_t MODO_ACTUAL = (GPIOB->CRH & MASK_CRH) >> OFFSET;
    bool CLOCK_B_ENCENDIDO = RCC->APB2ENR & RCC_APB2ENR_IOPBEN;

    TEST_ASSERT_TRUE(CLOCK_B_ENCENDIDO);
    TEST_ASSERT_FALSE(DELTA_CRL);
    TEST_ASSERT_BITS_LOW(~MASK_CRH,DELTA_CRH);
    TEST_ASSERT_BITS_LOW(~MASK_ODR,DELTA_ODR);  
    TEST_ASSERT_TRUE(MODO_ESPERADO & MODO_ACTUAL);
    TEST_ASSERT_FALSE(MASK_ODR & ODR_DESPUES);         
}

static void test_config_salida(void) {

    uint32_t OFFSET = 4U;
    uint32_t MASK_CRH = (0xF << OFFSET);
    uint32_t MODO_ESPERADO = 0b0010;
    SP_Pin_setModo(SP_PB9,SP_PIN_ENTRADA);
    uint32_t CRL_ANTES = GPIOB->CRL;
    uint32_t CRH_ANTES = GPIOB->CRH;
    uint32_t ODR_ANTES = GPIOB->ODR;

    SP_Pin_setModo(SP_PB9, SP_PIN_SALIDA);

    uint32_t CRL_DESPUES = GPIOB->CRL;
    uint32_t CRH_DESPUES = GPIOB->CRH;
    uint32_t ODR_DESPUES = GPIOB->ODR;
    uint32_t DELTA_CRL = CRL_ANTES ^ CRL_DESPUES;
    uint32_t DELTA_CRH = CRH_ANTES ^ CRH_DESPUES; 
    uint32_t DELTA_ODR = ODR_ANTES ^ ODR_DESPUES;
    uint32_t MODO_ACTUAL = (GPIOB->CRH & MASK_CRH) >> OFFSET;
    bool CLOCK_B_ENCENDIDO = RCC->APB2ENR & RCC_APB2ENR_IOPBEN;

    TEST_ASSERT_TRUE(CLOCK_B_ENCENDIDO);
    TEST_ASSERT_FALSE(DELTA_CRL);
    TEST_ASSERT_FALSE(DELTA_ODR);
    TEST_ASSERT_BITS_LOW(~MASK_CRH,DELTA_CRH);
    TEST_ASSERT_TRUE(MODO_ESPERADO & MODO_ACTUAL);
}

static void test_config_salidaDrenadorAbierto(void) {

    uint32_t OFFSET = 4U;
    uint32_t MASK_PIN9_MODO = (0xF << OFFSET);
    uint32_t MODO_ESPERADO = 0b0110;
    SP_Pin_setModo(SP_PB9,SP_PIN_ENTRADA);
    uint32_t CRL_ANTES = GPIOB->CRL;
    uint32_t CRH_ANTES = GPIOB->CRH;
    uint32_t ODR_ANTES = GPIOB->ODR;

    SP_Pin_setModo(SP_PB9, SP_PIN_SALIDA_OPEN_DRAIN);

    uint32_t CRL_DESPUES = GPIOB->CRL;
    uint32_t CRH_DESPUES = GPIOB->CRH;
    uint32_t ODR_DESPUES = GPIOB->ODR;
    uint32_t DELTA_CRL = CRL_ANTES ^ CRL_DESPUES;
    uint32_t DELTA_CRH = CRH_ANTES ^ CRH_DESPUES; 
    uint32_t DELTA_ODR = ODR_ANTES ^ ODR_DESPUES;
    uint32_t MODO_ACTUAL = (GPIOB->CRH & MASK_PIN9_MODO) >> OFFSET;
    bool CLOCK_B_ENCENDIDO = RCC->APB2ENR & RCC_APB2ENR_IOPBEN;

    TEST_ASSERT_TRUE(CLOCK_B_ENCENDIDO);
    TEST_ASSERT_FALSE(DELTA_CRL);
    TEST_ASSERT_FALSE(DELTA_ODR);
    TEST_ASSERT_BITS_LOW(~MASK_PIN9_MODO,DELTA_CRH);
    TEST_ASSERT_TRUE(MODO_ESPERADO & MODO_ACTUAL);
} 

static void test_escribir_uno (void) {

    SP_Pin_setModo(SP_PB9,SP_PIN_SALIDA_OPEN_DRAIN);
    SP_Pin_write(SP_PB9,0);
    uint32_t CRL_ANTES = GPIOB->CRL;
    uint32_t CRH_ANTES = GPIOB->CRH;
    uint32_t ODR_ANTES = GPIOB->ODR;
    uint32_t MASK_ODR = 0b1 << 9U;

    SP_Pin_write(SP_PB9,1);

    uint32_t CRL_DESPUES = GPIOB->CRL;
    uint32_t CRH_DESPUES = GPIOB->CRH;
    uint32_t ODR_DESPUES = GPIOB->ODR;
    uint32_t DELTA_CRL = CRL_ANTES ^ CRL_DESPUES;
    uint32_t DELTA_CRH = CRH_ANTES ^ CRH_DESPUES; 
    uint32_t DELTA_ODR = ODR_ANTES ^ ODR_DESPUES;

    TEST_ASSERT_FALSE(DELTA_CRL);
    TEST_ASSERT_FALSE(DELTA_CRH);
    TEST_ASSERT_BITS_LOW(~MASK_ODR,DELTA_ODR);
    TEST_ASSERT_TRUE(MASK_ODR & DELTA_ODR);             
    TEST_ASSERT_TRUE(MASK_ODR & ODR_DESPUES);          
}

static void test_escribir_cero (void) {

    SP_Pin_setModo(SP_PB9,SP_PIN_SALIDA_OPEN_DRAIN);
    SP_Pin_write(SP_PB9,1);
    uint32_t CRL_ANTES = GPIOB->CRL;
    uint32_t CRH_ANTES = GPIOB->CRH;
    uint32_t ODR_ANTES = GPIOB->ODR;
    uint32_t MASK_ODR = 0b1 << 9U;

    SP_Pin_write(SP_PB9,0);
    uint32_t CRL_DESPUES = GPIOB->CRL;
    uint32_t CRH_DESPUES = GPIOB->CRH;
    uint32_t ODR_DESPUES = GPIOB->ODR;
    uint32_t DELTA_CRL = CRL_ANTES ^ CRL_DESPUES;
    uint32_t DELTA_CRH = CRH_ANTES ^ CRH_DESPUES; 
    uint32_t DELTA_ODR = ODR_ANTES ^ ODR_DESPUES;

    TEST_ASSERT_FALSE(DELTA_CRL);
    TEST_ASSERT_FALSE(DELTA_CRH);
    TEST_ASSERT_BITS_LOW(~MASK_ODR,DELTA_ODR);
    TEST_ASSERT_TRUE(MASK_ODR & DELTA_ODR);         
    TEST_ASSERT_FALSE(MASK_ODR & ODR_DESPUES);     
    }

static void test_leer_uno (void) {                       

    SP_Pin_setModo(SP_PB9,SP_PIN_SALIDA);
    SP_Pin_write(SP_PB9,1);
    SP_Pin_setModo(SP_PB9,SP_PIN_ENTRADA);
    SP_Pin_write(SP_PB9,0);

    bool valor_1 = SP_Pin_read(SP_PB9);

TEST_ASSERT_EQUAL(1,valor_1);
}

static void test_leer_cero (void) {                       
    SP_Pin_setModo(SP_PB9,SP_PIN_SALIDA);
    SP_Pin_write(SP_PB9,0);
    SP_Pin_setModo(SP_PB9,SP_PIN_ENTRADA);
    SP_Pin_write(SP_PB9,1);

    bool valor_0 = SP_Pin_read(SP_PB9);

    TEST_ASSERT_EQUAL(0,valor_0);                   
}
static void test_jlag_pines (void) {                                            
    static SP_HPin const pinesJtag[] = {SP_PA15,SP_PB3,SP_PB4};                 
    static size_t const NUM_PINES_JTAG = sizeof(pinesJtag)/sizeof(*pinesJtag);  
    for(size_t i=0;i<NUM_PINES_JTAG;++i){                                       
        SP_HPin const hPin = pinesJtag[i];                                      
        SP_Pin_setModo(hPin,SP_PIN_SALIDA);                                     
        SP_Pin_write(hPin,0);                                                   
        SP_Pin_setModo(hPin,SP_PIN_ENTRADA);                                    
        bool const valor_0 = SP_Pin_read(hPin);                                 
        SP_Pin_setModo(hPin,SP_PIN_SALIDA);                                     
        SP_Pin_write(hPin,1);                                                   
        SP_Pin_setModo(hPin,SP_PIN_ENTRADA);                                    
        bool const valor_1 = SP_Pin_read(hPin);                                
        TEST_ASSERT_FALSE(valor_0);                                             
        TEST_ASSERT_TRUE(valor_1);
    }
}


int main (void) {
    SP_init();
    SP_Tiempo_delay(1500);
    UNITY_BEGIN();
    RUN_TEST(test_config_entrada) 
    RUN_TEST(test_config_entradaPullup);
    RUN_TEST(test_config_entradaPullDown);
    RUN_TEST(test_config_salida);
    RUN_TEST(test_config_salidaDrenadorAbierto);
    RUN_TEST(test_escribir_cero);
    RUN_TEST(test_escribir_uno);
    RUN_TEST(test_leer_uno);
    RUN_TEST(test_leer_cero);
    RUN_TEST(test_jlag_pines);
    UNITY_END();
    return 0;
}