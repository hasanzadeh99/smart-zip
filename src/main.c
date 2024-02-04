#include <zephyr/kernel.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
// #include "bm_lite.h"


#include ".\BMLite_sdk\bmlite_if.h"
#include ".\BMLite_sdk\hcp_tiny.h"
#include ".\BMLite_sdk\platform.h"
#include ".\BMLite_sdk\bmlite_hal.h"
#include ".\BMLite_sdk\hal_spi.h"


#define DATA_BUFFER_SIZE (1024*5)
static uint8_t hcp_txrx_buffer[MTU];
static uint8_t hcp_data_buffer[DATA_BUFFER_SIZE];

static HCP_comm_t hcp_chain = {

    .read = platform_bmlite_spi_receive,
    .write = platform_bmlite_spi_send,
    .pkt_buffer = hcp_data_buffer,
    .txrx_buffer = hcp_txrx_buffer,
    .pkt_size = 0,
    .pkt_size_max = sizeof(hcp_data_buffer),
    .phy_rx_timeout = 2000,

};

//#define CS_N 17       //for Use PCB
//#define RST_PIN 20    //for Use PCB
//#define IRQ_PIN 22    //for Use PCB


#define RX_PIN_BMLITE 06       //for Use DK board
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
	.operation = SPI_OP_MODE_MASTER | SPI_WORD_SET(8) | SPI_TRANSFER_MSB ,	
			 
	.operation = SPI_WORD_SET(8),
		

	.frequency = 1000000,
	.slave = 0,
};


int spi_transfer(uint8_t *tx_buffer, uint8_t *rx_buffer, size_t length){
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

		printk("main92SPI error: %d\n", err);

	} else {

		// for(int i=0;i<length;i++){

		// 	printk("TX %d sent: %02X \n", i, tx_buffer[i]);
		// 	printk("RX %d recv: %02X \n", i, rx_buffer[i]);

		// }

	
	}	


	return err;
}

static volatile uint32_t systick;
volatile uint32_t button_pressed_time = 0;

static uint32_t btn_press_start;
static uint32_t btn_pressed = 0;

uint32_t hal_timebase_get_tick(void)
{


    return (uint32_t)k_uptime_get();  // Convert uptime in ms to ticks

}

bool hal_bmlite_get_status(void)
{
	int value = gpio_pin_get(dev, IRQ_PIN);
	return value;
    
}

void platform_bmlite_reset(void){

	gpio_pin_set(dev, RST_PIN, 0);
	k_sleep(K_MSEC(100));
	gpio_pin_set(dev, RST_PIN, 1);
	k_sleep(K_MSEC(100));

}

uint8_t tx_buffer[20];
uint8_t rx_buffer[20];

void main(void)
{
int ret;



	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);


	ret = gpio_pin_configure(dev, RST_PIN , GPIO_OUTPUT_ACTIVE);    //RST _ PIN
	ret = gpio_pin_configure(dev, CS_N , GPIO_OUTPUT_ACTIVE);       //CS_N _ PIN


	ret = gpio_pin_configure(dev, RX_PIN_BMLITE , GPIO_OUTPUT_ACTIVE);       //RX _ PIN

	ret = gpio_pin_configure(dev, IRQ_PIN, GPIO_INPUT);						// IRQ PIN for BMLite

	


	if(!device_is_ready(spi0_dev)){

		printk("Error: SPI device not ready\n");
		return;

	}

	for(int i=0; i<20; i++){
		tx_buffer[i] = i;
		rx_buffer[i] = 2*i;
	}


	char version[100];
	uint16_t template_id;
	uint32_t current_id = 0;
	bool match;

	
	

	k_sleep(K_MSEC(1000));

	// gpio_pin_set(dev, RX_PIN_BMLITE, 1);

	platform_bmlite_reset();

	k_sleep(K_MSEC(1000));

	printk("Start Program\n");

	memset(version, 0, 100);
	fpc_bep_result_t res = bep_version(&hcp_chain, version, 99);

	if (res == FPC_BEP_RESULT_OK){

		printk("-> FPC - Version Read OK!");
		printk("-> FPC - Version: %s", version); 

	}else{

		printk("ERROR in Reading Version ******  res is: %d\n",res);
	
	}


	printk("ُI am in while... reading version...\n");
	platform_bmlite_reset();
	k_sleep(K_MSEC(2000));
	res = bep_version(&hcp_chain, version, 99);
	if (res == FPC_BEP_RESULT_OK){
		printk("-> FPC - Version Read OK!");
		printk("-> FPC - Version: %s", version); 
	}else{
		printk("Versionnnnnnnn******  res is: %d\n",res);
	}

	
	gpio_pin_toggle_dt(&led);


	while(1){

		printk("ُHello ...\n");
		k_sleep(K_MSEC(20));
		res = bep_enroll_finger(&hcp_chain);
		printk("***************after enroll finger********\n");
		printk("bep_enroll_finger res is: %d\n",res);
		if(res== FPC_BEP_RESULT_OK) {

			res = bep_template_save(&hcp_chain, current_id++);
			if(res== FPC_BEP_RESULT_OK) printk("HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH\n\n\n");

		}



		res = sensor_wait_finger_not_present(&hcp_chain, 0);
		printk("***************after wait_finger_not_present********\n");
		printk("sensor_wait_finger_not_present res is: %d\n",res);


		// k_sleep(K_MSEC(2000));
		res = bep_identify_finger(&hcp_chain, 0, &template_id, &match);
		if(res== FPC_BEP_RESULT_OK) {

			printk("FFFFFFFFFFFFFFFFFFFFFFFFFF\n\n\n\n\n");
			gpio_pin_toggle_dt(&led);
		}
		// printk("***************after identify finger********\n");
		// printk("bep_identify_finger res is:  %d\n",res);
		// k_sleep(K_MSEC(2000));
		// if (res == FPC_BEP_RESULT_TIMEOUT || res == FPC_BEP_RESULT_IO_ERROR) {
		// 	platform_bmlite_reset();
		// 	printk("***************after bmlite reset********\n");
		// 	printk("bep_platform_bmlite_reset res is:  %d\n",res);
		// 	continue;
		// } else if (res != FPC_BEP_RESULT_OK) {
		// 	continue;
		// }



		// k_sleep(K_MSEC(2000));
		// gpio_pin_toggle_dt(&led);


	}

}









