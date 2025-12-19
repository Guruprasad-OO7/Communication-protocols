#include "DAVE.h"
#include "CAN_NODE/can_node.h"
#include "CAN_NODE/can_node_conf.h"


#define CCU_CLOCK_HZ   64000000U   // Assumed CCU clock

uint8_t can_data[8] = {0};

volatile uint16_t pot_adc_value = 0;

uint32_t echo_pulse_ticks = 0;
uint32_t echo_time_us     = 0;
uint16_t distance_cm      = 0;

CAPTURE_STATUS_t cap_status;

int main(void)
{
    DAVE_STATUS_t status;

    status = DAVE_Init();
    if (status != DAVE_STATUS_SUCCESS)
    {
        while (1);
    }

    // Start CAPTURE timer
    CAPTURE_Start(&CAPTURE_0);

    while (1)
    {
        // 1️⃣ Read potentiometer
        pot_adc_value = ADC_MEASUREMENT_GetResult(&ADC_MEASUREMENT_Channel_A_handle);

        // 2️⃣ Trigger ultrasonic (10 µs pulse)
        DIGITAL_IO_SetOutputHigh(&ULTRA_TRIG);
        for (uint32_t i = 0; i < 500; i++);
        DIGITAL_IO_SetOutputLow(&ULTRA_TRIG);

        // 3️⃣ Read captured echo pulse width
        cap_status = CAPTURE_GetCapturedTime(&CAPTURE_0, &echo_pulse_ticks);

        if (cap_status == CAPTURE_STATUS_SUCCESS && echo_pulse_ticks < 60000)
        {
            // Convert ticks → microseconds
            echo_time_us = echo_pulse_ticks / (CCU_CLOCK_HZ / 1000000U);

            // Convert time → distance
            distance_cm = echo_time_us / 58;
        }

        // Pack CAN payload
        can_data[0] = (uint8_t)(pot_adc_value & 0xFF);
        can_data[1] = (uint8_t)((pot_adc_value >> 8) & 0xFF);

        can_data[2] = (uint8_t)(distance_cm & 0xFF);
        can_data[3] = (uint8_t)((distance_cm >> 8) & 0xFF);

        // Update CAN message object data
        CAN_NODE_MO_UpdateData(&CAN_NODE_0_LMO_01_Config, can_data);

        // Transmit CAN frame
        CAN_NODE_MO_Transmit(&CAN_NODE_0_LMO_01_Config);


        // 6️⃣ Delay (~100 ms) — IMPORTANT for HC-SR04
        for (uint32_t i = 0; i < 800000; i++);
    }
}
