#include <zephyr/kernel.h>

// RTC0 Interrupt Handler
void RTC1_IRQHandler();
int main(void)
{
        // Initialize RTC
        NRF_RTC1->TASKS_CLEAR = 0x01;
        NRF_RTC1->INTENSET = RTC_INTENSET_COMPARE0_Enabled << RTC_INTENSET_COMPARE0_Pos;
        NRF_RTC1->EVTENSET = RTC_EVTENSET_COMPARE0_Enabled << RTC_EVTENSET_COMPARE0_Pos;
        NRF_RTC1->PRESCALER = 32;       // 32768/(32+1) = 992.96counts = 1sec
        NRF_RTC1->TASKS_START = 1;

        // Activate IRQ
        IRQ_DIRECT_CONNECT(RTC1_IRQn, 0, RTC1_IRQHandler, 0);
        irq_enable(RTC1_IRQn);
        
        while(true){
                // Main Loop
                __WFI();
        }
        return 0;
}

void RTC1_IRQHandler(){
        // Function for RTC0 Interrupt/Events
        if(NRF_RTC1->EVENTS_COMPARE[0] == 1){
                NRF_RTC1->EVENTS_COMPARE[0] = 0;
                
                // Print message
                uint32_t counter = NRF_RTC1->COUNTER;
                printk("Elapsed Time : %ds(%08d)\n", counter / 993, counter);        // 992.9 == 1sec
        }
}