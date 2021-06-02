#include "subghz_cli.h"
#include <furi.h>
#include <api-hal.h>
#include <api-hal-spi.h>
#include <cc1101.h>
#include <cc1101_regs.h>

static const uint8_t subghz_test_packet_data[] = {
    0x30, // 48bytes to transmit
    0x28, 0x28,  0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28 , 0x28,
    0x10, 0x11,  0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e , 0x1f,
    0x77, 0x77,  0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77 , 0x77,
};

typedef union {
    CC1101Status status;
    uint8_t data;
} status_t;


void subghz_cli_init() {
    Cli* cli = furi_record_open("cli");

    cli_add_command(cli, "subghz_tx_carrier", subghz_cli_command_tx_carrier, NULL);
    cli_add_command(cli, "subghz_rx_carrier", subghz_cli_command_rx_carrier, NULL);
    cli_add_command(cli, "tx", subghz_cli_command_tx_pt, NULL);
    cli_add_command(cli, "rx", subghz_cli_command_rx_pt, NULL);
    cli_add_command(cli, "d", subghz_cli_command_dump_registers, NULL);

    furi_record_close("cli");
}

void subghz_cli_command_tx_carrier(Cli* cli, string_t args, void* context) {
    uint32_t frequency;
    int ret = sscanf(string_get_cstr(args), "%lu", &frequency);
    if(ret != 1) {
        printf("sscanf returned %d, frequency: %lu\r\n", ret, frequency);
        cli_print_usage("subghz_tx_carrier", "<Frequency in HZ>", string_get_cstr(args));
        return;
    }

    if(frequency < 300000000 || frequency > 925000000) {
        printf("Frequency must be in 300000000...925000000 range, not %lu\r\n", frequency);
        return;
    }

    api_hal_subghz_reset();
    api_hal_subghz_load_preset(ApiHalSubGhzPresetOokAsync);
    frequency = api_hal_subghz_set_frequency(frequency);
    printf("Transmitting at frequency %lu Hz\r\n", frequency);
    printf("Press CTRL+C to stop\r\n");
    if(frequency < 400000000) {
        api_hal_subghz_set_path(ApiHalSubGhzPath315);
    } else if(frequency < 500000000) {
        api_hal_subghz_set_path(ApiHalSubGhzPath433);
    } else {
        api_hal_subghz_set_path(ApiHalSubGhzPath868);
    }

    hal_gpio_init(&gpio_cc1101_g0, GpioModeOutputPushPull, GpioPullNo, GpioSpeedLow);
    hal_gpio_write(&gpio_cc1101_g0, false);

    api_hal_subghz_tx();

    while(!cli_cmd_interrupt_received(cli)) {
        osDelay(250);
    }

    api_hal_subghz_reset();
    api_hal_subghz_set_path(ApiHalSubGhzPathIsolate);
    hal_gpio_init(&gpio_cc1101_g0, GpioModeAnalog, GpioPullNo, GpioSpeedLow);
}

void subghz_cli_command_rx_carrier(Cli* cli, string_t args, void* context) {
    uint32_t frequency;
    int ret = sscanf(string_get_cstr(args), "%lu", &frequency);
    if(ret != 1) {
        printf("sscanf returned %d, frequency: %lu\r\n", ret, frequency);
        cli_print_usage("subghz_tx_carrier", "<Frequency in HZ>", string_get_cstr(args));
        return;
    }

    if(frequency < 300000000 || frequency > 925000000) {
        printf("Frequency must be in 300000000...925000000 range, not %lu\r\n", frequency);
        return;
    }

    api_hal_subghz_reset();
    api_hal_subghz_load_preset(ApiHalSubGhzPresetOokAsync);
    frequency = api_hal_subghz_set_frequency(frequency);
    printf("Receiving at frequency %lu Hz\r\n", frequency);
    printf("Press CTRL+C to stop\r\n");
    if(frequency < 400000000) {
        api_hal_subghz_set_path(ApiHalSubGhzPath315);
    } else if(frequency < 500000000) {
        api_hal_subghz_set_path(ApiHalSubGhzPath433);
    } else {
        api_hal_subghz_set_path(ApiHalSubGhzPath868);
    }

    hal_gpio_init(&gpio_cc1101_g0, GpioModeAnalog, GpioPullNo, GpioSpeedLow);

    api_hal_subghz_rx();

    while(!cli_cmd_interrupt_received(cli)) {
        osDelay(250);
        printf("RSSI: %03.1fdbm\r", api_hal_subghz_get_rssi());
        fflush(stdout);
    }

    api_hal_subghz_reset();
    api_hal_subghz_set_path(ApiHalSubGhzPathIsolate);
}

static inline CC1101Status get_data(uint8_t reg, uint8_t* data) {
    CC1101Status status;
    const ApiHalSpiDevice* device = api_hal_spi_device_get(ApiHalSpiDeviceIdSubGhz);

    status = cc1101_read_reg(device, reg, data);

    api_hal_spi_device_return(device);

    return status;
}

void subghz_cli_command_tx_pt(Cli* cli, string_t args, void* context) {
    uint32_t frequency;
    uint32_t pattern;
    uint32_t count;

    status_t status;
    uint8_t data;

    int ret = sscanf(string_get_cstr(args), "%lu %lu %lu", &frequency, &pattern, &count);
    if(ret != 3) {
        printf(
            "sscanf returned %d, frequency: %lu; pattern: %lu; count: %lu\r\n",
            ret,
            frequency,
            pattern,
            count);
        cli_print_usage(
            "subghz_tx_pt", "<Frequency in HZ> <Pattern> <Count>", string_get_cstr(args));
        return;
    }

    if(frequency < 300000000 || frequency > 925000000) {
        printf("Frequency must be in 300000000...925000000 range, not %lu\r\n", frequency);
        return;
    }
    if(pattern > 1) {
        printf("Pattern must be 1, not %lu\r\n", pattern);
    }

    api_hal_subghz_reset();
    api_hal_subghz_idle();
    const ApiHalSpiDevice* device = api_hal_spi_device_get(ApiHalSpiDeviceIdSubGhz);
    cc1101_flush_tx(device);
    api_hal_spi_device_return(device);

    api_hal_subghz_load_preset(ApiHalSubGhzPreset2FskPacket);

    frequency = api_hal_subghz_set_frequency(frequency);
    if(frequency < 400000000) {
        api_hal_subghz_set_path(ApiHalSubGhzPath315);
    } else if(frequency < 500000000) {
        api_hal_subghz_set_path(ApiHalSubGhzPath433);
    } else {
        api_hal_subghz_set_path(ApiHalSubGhzPath868);
    }
    hal_gpio_init(&gpio_cc1101_g0, GpioModeInput, GpioPullNo, GpioSpeedLow);

    status.status = get_data(CC1101_STATUS_TXBYTES | CC1101_BURST, &data);
    FURI_LOG_D("SUBGHZ CLI", "Status: %02X CC1101_STATUS_TXBYTES %02X", status.data, data);

    while(!cli_cmd_interrupt_received(cli) && count) {
        api_hal_subghz_idle();
        api_hal_subghz_write_packet(subghz_test_packet_data, sizeof(subghz_test_packet_data));
        api_hal_subghz_tx();
        while(!hal_gpio_read(&gpio_cc1101_g0)); // Wait for sync
        while(hal_gpio_read(&gpio_cc1101_g0)); // Wait end of transaction
        // count--;
    }

    api_hal_subghz_reset();
    api_hal_subghz_set_path(ApiHalSubGhzPathIsolate);    
    hal_gpio_init(&gpio_cc1101_g0, GpioModeAnalog, GpioPullNo, GpioSpeedLow);
}

void subghz_cli_command_rx_pt(Cli* cli, string_t args, void* context) {
    uint32_t frequency;

    status_t status;
    uint8_t data;
    GpioPin pin = {GPIOA, GPIO_PIN_7};
    hal_gpio_init(&pin, GpioModeOutputPushPull, GpioPullNo, GpioSpeedLow);

    int ret = sscanf(string_get_cstr(args), "%lu", &frequency);
    if(ret != 1) {
        printf(
            "sscanf returned %d, frequency: %lu\r\n", ret, frequency);
        cli_print_usage(
            "subghz_rx_pt", "<Frequency in HZ>", string_get_cstr(args));
        return;
    }

    if(frequency < 300000000 || frequency > 925000000) {
        printf("Frequency must be in 300000000...925000000 range, not %lu\r\n", frequency);
        return;
    }

    api_hal_subghz_reset();
    api_hal_subghz_idle();
    api_hal_subghz_load_preset(ApiHalSubGhzPreset2FskPacket);

    frequency = api_hal_subghz_set_frequency(frequency);
    if(frequency < 400000000) {
        api_hal_subghz_set_path(ApiHalSubGhzPath315);
    } else if(frequency < 500000000) {
        api_hal_subghz_set_path(ApiHalSubGhzPath433);
    } else {
        api_hal_subghz_set_path(ApiHalSubGhzPath868);
    }
    hal_gpio_init(&gpio_cc1101_g0, GpioModeInput, GpioPullNo, GpioSpeedLow);

    status.status = get_data(CC1101_STATUS_RXBYTES | CC1101_BURST, &data);
    printf("Status: %02X   CC1101_STATUS_RXBYTES %02X\r\n", status.data, data);

    api_hal_subghz_flush_rx();
    api_hal_subghz_rx();

    uint8_t buff_rx[64] = {1};
    uint8_t size = 0;
    while(!cli_cmd_interrupt_received(cli)) {
        hal_gpio_write(&pin, hal_gpio_read(&gpio_cc1101_g0));
        if(hal_gpio_read(&gpio_cc1101_g0)) {
            while(hal_gpio_read(&gpio_cc1101_g0)); // Wait reception
            api_hal_subghz_read_packet(buff_rx, &size);
            if(size > 0) {
                printf("Received %d bytes\r\n", size);
                for(uint8_t i =0; i < size; i++) {
                    printf("%02X ", i % 2 ? buff_rx[i] : buff_rx[i] >> 1);
                }
                printf("\r\n\r\n");
                osDelay(10);
                api_hal_subghz_idle();
                api_hal_subghz_flush_rx();
                api_hal_subghz_rx();
            }
            continue;
        }
        // api_hal_subghz_flush_rx();
        status.status = get_data(CC1101_STATUS_RXBYTES | CC1101_BURST, &data);
        printf("Status: %02X   CC1101_STATUS_RXBYTES %02X\r\n", status.data, data);
        
        if((status.data & 0xF0) == 0) {
            api_hal_subghz_idle();
            api_hal_subghz_flush_rx();
            api_hal_subghz_rx();
        }
        if((status.data & 0xF0) == 0x60) {
            api_hal_subghz_idle();
            api_hal_subghz_flush_rx();
            api_hal_subghz_rx();
        }
            // osDelay(10);
    }

    api_hal_subghz_reset();
    api_hal_subghz_set_path(ApiHalSubGhzPathIsolate);    
    hal_gpio_init(&gpio_cc1101_g0, GpioModeAnalog, GpioPullNo, GpioSpeedLow);
}

static uint8_t gd(const ApiHalSpiDevice* device, uint8_t reg) {
    uint8_t data;
    cc1101_read_reg(device, reg, &data);
    return data;
}


void subghz_cli_command_dump_registers(Cli* cli, string_t args, void* context) {

    const ApiHalSpiDevice* device = api_hal_spi_device_get(ApiHalSpiDeviceIdSubGhz);

    printf("CC1101_IOCFG2  %02X\r\n", gd(device, CC1101_IOCFG2  ));
    printf("CC1101_IOCFG1  %02X\r\n", gd(device, CC1101_IOCFG1  ));
    printf("CC1101_IOCFG0  %02X\r\n", gd(device, CC1101_IOCFG0  ));
    printf("CC1101_FIFOTHR %02X\r\n", gd(device, CC1101_FIFOTHR ));
    printf("CC1101_SYNC1   %02X\r\n", gd(device, CC1101_SYNC1   ));
    printf("CC1101_SYNC0   %02X\r\n", gd(device, CC1101_SYNC0   ));
    printf("CC1101_PKTLEN  %02X\r\n", gd(device, CC1101_PKTLEN  ));
    printf("CC1101_PKTCTRL1%02X\r\n", gd(device, CC1101_PKTCTRL1));
    printf("CC1101_PKTCTRL0%02X\r\n", gd(device, CC1101_PKTCTRL0));
    printf("CC1101_ADDR    %02X\r\n", gd(device, CC1101_ADDR    ));
    printf("CC1101_CHANNR  %02X\r\n", gd(device, CC1101_CHANNR  ));
    printf("CC1101_FSCTRL1 %02X\r\n", gd(device, CC1101_FSCTRL1 ));
    printf("CC1101_FSCTRL0 %02X\r\n", gd(device, CC1101_FSCTRL0 ));
    printf("CC1101_FREQ2   %02X\r\n", gd(device, CC1101_FREQ2   ));
    printf("CC1101_FREQ1   %02X\r\n", gd(device, CC1101_FREQ1   ));
    printf("CC1101_FREQ0   %02X\r\n", gd(device, CC1101_FREQ0   ));
    printf("CC1101_MDMCFG4 %02X\r\n", gd(device, CC1101_MDMCFG4 ));
    printf("CC1101_MDMCFG3 %02X\r\n", gd(device, CC1101_MDMCFG3 ));
    printf("CC1101_MDMCFG2 %02X\r\n", gd(device, CC1101_MDMCFG2 ));
    printf("CC1101_MDMCFG1 %02X\r\n", gd(device, CC1101_MDMCFG1 ));
    printf("CC1101_MDMCFG0 %02X\r\n", gd(device, CC1101_MDMCFG0 ));
    printf("CC1101_DEVIATN %02X\r\n", gd(device, CC1101_DEVIATN ));
    printf("CC1101_MCSM2   %02X\r\n", gd(device, CC1101_MCSM2   ));
    printf("CC1101_MCSM1   %02X\r\n", gd(device, CC1101_MCSM1   ));
    printf("CC1101_MCSM0   %02X\r\n", gd(device, CC1101_MCSM0   ));
    printf("CC1101_FOCCFG  %02X\r\n", gd(device, CC1101_FOCCFG  ));
    printf("CC1101_BSCFG   %02X\r\n", gd(device, CC1101_BSCFG   ));
    printf("CC1101_AGCTRL2 %02X\r\n", gd(device, CC1101_AGCTRL2 ));
    printf("CC1101_AGCTRL1 %02X\r\n", gd(device, CC1101_AGCTRL1 ));
    printf("CC1101_AGCTRL0 %02X\r\n", gd(device, CC1101_AGCTRL0 ));
    printf("CC1101_WOREVT1 %02X\r\n", gd(device, CC1101_WOREVT1 ));
    printf("CC1101_WOREVT0 %02X\r\n", gd(device, CC1101_WOREVT0 ));
    printf("CC1101_WORCTRL %02X\r\n", gd(device, CC1101_WORCTRL ));
    printf("CC1101_FREND1  %02X\r\n", gd(device, CC1101_FREND1  ));
    printf("CC1101_FREND0  %02X\r\n", gd(device, CC1101_FREND0  ));
    printf("CC1101_FSCAL3  %02X\r\n", gd(device, CC1101_FSCAL3  ));
    printf("CC1101_FSCAL2  %02X\r\n", gd(device, CC1101_FSCAL2  ));
    printf("CC1101_FSCAL1  %02X\r\n", gd(device, CC1101_FSCAL1  ));
    printf("CC1101_FSCAL0  %02X\r\n", gd(device, CC1101_FSCAL0  ));
    printf("CC1101_RCCTRL1 %02X\r\n", gd(device, CC1101_RCCTRL1 ));
    printf("CC1101_RCCTRL0 %02X\r\n", gd(device, CC1101_RCCTRL0 ));
    printf("CC1101_FSTEST  %02X\r\n", gd(device, CC1101_FSTEST  ));
    printf("CC1101_PTEST   %02X\r\n", gd(device, CC1101_PTEST   ));
    printf("CC1101_AGCTEST %02X\r\n", gd(device, CC1101_AGCTEST ));
    printf("CC1101_TEST2   %02X\r\n", gd(device, CC1101_TEST2   ));
    printf("CC1101_TEST1   %02X\r\n", gd(device, CC1101_TEST1   ));
    printf("CC1101_TEST0   %02X\r\n", gd(device, CC1101_TEST0   ));
    api_hal_spi_device_return(device);

}