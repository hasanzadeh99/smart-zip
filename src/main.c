#include <zephyr/kernel.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
// #include "bm_lite.h"

//#define CS_N 17       //for Use PCB
//#define RST_PIN 20    //for Use PCB
//#define IRQ_PIN 22    //for Use PCB


#define CS_N 04       //for Use DK board
#define RST_PIN 03    //for Use DK board
#define IRQ_PIN 28	  //for Use DK board

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

#define SPI0_NODE DT_NODELABEL(spi1)       //for Use DK board
// #define SPI0_NODE DT_NODELABEL(spi0)       //for Use PCB
static const struct device * spi0_dev = DEVICE_DT_GET(SPI0_NODE);

const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(gpio0));

#define LED1_NODE DT_ALIAS(led1)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED1_NODE, gpios);


static const struct spi_config spi_cfg = {
	// .operation = SPI_OP_MODE_MASTER | SPI_WORD_SET(8) | SPI_TRANSFER_MSB |
	// 	     SPI_MODE_CPOL | SPI_MODE_CPHA,	
			 
	.operation = SPI_WORD_SET(8),
		

	.frequency = 125000,
	.slave = 0,
};

void platform_bmlite_reset(void);

void spi_read_write(uint8_t *tx_buffer, uint8_t *rx_buffer, size_t length){
int err;

	const struct spi_buf tx_buf = {
		.buf = tx_buffer,
		.len = length,
	};
	const struct spi_buf_set tx = {
		.buffers = &tx_buf,
		.count = 1
	};

	struct spi_buf rx_buf = {
		.buf = rx_buffer,
		.len = length,
	};
	const struct spi_buf_set rx = {
		.buffers = &rx_buf,
		.count = 1
	};
	
    gpio_pin_set(dev, CS_N, 0);
	err = spi_transceive(spi0_dev, &spi_cfg, &tx, &rx);
    gpio_pin_set(dev, CS_N, 1);

	if (err) {

		printk("SPI error: %d\n", err);

	} else {

		for(int i=0;i<length;i++){

			printk("TX %d sent: %d\n", i, tx_buffer[i]);
			printk("RX %d recv: %d\n", i, rx_buffer[i]);

		}

	
	}	

}


uint8_t tx_buffer[20];
uint8_t rx_buffer[20];

void main(void)
{
int ret;



	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);


	ret = gpio_pin_configure(dev, RST_PIN , GPIO_OUTPUT_ACTIVE);    //RST _ PIN
	ret = gpio_pin_configure(dev, CS_N , GPIO_OUTPUT_ACTIVE);       //CS_N _ PIN


	printk("SPIM Example\n");
	printk("Error is : %d",ret);

	if(!device_is_ready(spi0_dev)){

		printk("Error: SPI device not ready\n");
		return;

	}

	for(int i=0; i<20; i++){
		tx_buffer[i] = i;
		rx_buffer[i] = 2*i;
	}


	while(1){

		printk("Salam\n");	
	//	spi_read_write(tx_buffer,rx_buffer,1);
		k_sleep(K_MSEC(1000));
		gpio_pin_toggle_dt(&led);


	}

}





void platform_bmlite_reset(void)     //reset the chip for 100ms
{

    gpio_pin_set(dev, RST_PIN, 0);
    k_sleep(K_MSEC(100));
    gpio_pin_set(dev, RST_PIN, 1);
    k_sleep(K_MSEC(100));

}



