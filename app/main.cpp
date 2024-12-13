#include <pico/stdio.h>
#include <pico/time.h>
#include <hardware/i2c.h>
#include <hardware/gpio.h>
#include <u8g2.h>
#include <u8x8.h>

constexpr const uint8_t I2C_SDA_PIN = 6;
constexpr const uint8_t I2C_SCL_PIN = 5;

static uint8_t u8x8_byte_rp2040_hw_i2c(u8x8_t *p_u8x8, uint8_t p_msg, uint8_t p_arg_int, void *p_arg_ptr)
{
    switch (p_msg)
    {
    case U8X8_MSG_BYTE_INIT:
        i2c_init(i2c_default, p_u8x8->display_info->i2c_bus_clock_100kHz * 1e5);
        gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
        gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
        gpio_pull_up(I2C_SDA_PIN);
        gpio_pull_up(I2C_SCL_PIN);
        break;

    case U8X8_MSG_BYTE_SET_DC:
        break;

    case U8X8_MSG_BYTE_START_TRANSFER:
        break;

    case U8X8_MSG_BYTE_SEND:
        if (i2c_write_blocking(i2c_default, p_u8x8->i2c_address, reinterpret_cast<const uint8_t *>(p_arg_ptr), static_cast<size_t>(p_arg_int), false) == PICO_ERROR_GENERIC)
            panic("Fail to communicate with disiplay.");
        break;

    case U8X8_MSG_BYTE_END_TRANSFER:
        break;

    default:
        return false;
    }
    return true;
}

int main()
{
    stdio_init_all();
    u8g2_t display;
    u8g2_Setup_ssd1306_i2c_128x64_noname_1(&display, U8G2_R0, u8x8_byte_rp2040_hw_i2c, &u8x8_dummy_cb);
    u8g2_InitDisplay(&display);
    u8g2_SetPowerSave(&display, false);

    stdio_printf("Display initialized!");

    while (1)
    {
        u8g2_FirstPage(&display);
        do
        {
            u8g2_SetFont(&display, u8g2_font_ncenB14_tr);
            u8g2_DrawStr(&display, 0, 15, "Hello world!");
        } while (u8g2_NextPage(&display));
        stdio_printf("Display refreshed!");
        sleep_ms(1000);
    }

    stdio_deinit_all();
}