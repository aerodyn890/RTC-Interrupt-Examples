#include <zephyr/kernel.h>

// RTC0 Interrupt Handler
void RTC2_IRQHandler();


// Global Variables
uint32_t counter = 0;
int main(void)
{
        // Initialize RTC
        NRF_RTC2->TASKS_CLEAR = 0x01;
        NRF_RTC2->INTENSET = RTC_INTENSET_COMPARE0_Enabled << RTC_INTENSET_COMPARE0_Pos;
        NRF_RTC2->EVTENSET = RTC_EVTENSET_COMPARE0_Enabled << RTC_EVTENSET_COMPARE0_Pos;
        NRF_RTC2->PRESCALER = 32;       // 32768/(32+1) = 992.96counts = 1sec
        NRF_RTC2->TASKS_START = 1;

        // Set RTC Compare Register
        counter = NRF_RTC2->COUNTER + 993;
        NRF_RTC2->CC[0] = counter;   // Call RTC1 Compare0 Handler after 1sec

        // Activate IRQ
        IRQ_DIRECT_CONNECT(RTC2_IRQn, 0, RTC2_IRQHandler, 0);
        irq_enable(RTC2_IRQn);
        
        while(true){
                // Main Loop
                //__WFI();
                k_usleep(1);
        }
        return 0;
}

void RTC2_IRQHandler(){
        // Function for RTC0 Interrupt/Events
        if(NRF_RTC2->EVENTS_COMPARE[0] == 1){
                NRF_RTC2->EVENTS_COMPARE[0] = 0;
                
                // Print message
                printk("Elapsed Time : %ds(%08d)\n", counter / 993, counter);        // 992.9 == 1sec

                // Set next compare register for repeat
                counter = NRF_RTC2->COUNTER + 993;
                NRF_RTC2->CC[0] = counter;
        }
}