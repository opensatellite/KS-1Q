#include "os.h"
#include "imumagd.h"
#include "LIS3MDL.h"

typedef struct 
{
	pin cs;
	spi_handle_t* spi;
	pin drdy;
}magd_interface_t;

magd_interface_t  magd_cfg=
{
	.cs = PIN_MAGD_CS,
	.spi = MAGD_SPI,
	.drdy = PIN_MAGD_DRDY
};

static pin cs;
static spi_handle_t* spi;
static pin drdy;

static void interface_init(magd_interface_t *cfg)
{
	cs = cfg->cs;
	spi = cfg->spi;
	drdy = cfg->drdy;
	
	pinmode(cs,PUSHPULL);
	pinhigh(cs);

	pinmode(drdy,PULLUP);
	//pin_setirq(cfg->irq, EXTI_Trigger_Falling);
	//__pin_confignvic(cfg->irq, 10, 0);

	spi_init(spi, 3, 8, 256);
}

#define magd_rw(D)		spi_rw(spi,D)
#define magd_cs_l()		pinlow(cs)
#define magd_cs_h()		pinhigh(cs)
#define magd_drdy()		pinread(drdy)

static void LIS3MDL_rreg(uint8_t addr, uint8_t* data,uint8_t cnt)
{	
	if (cnt > 1)
		addr|= 0xC0;
	else addr|= 0x80;

	magd_cs_l();
	
	for(uint8_t i= 0xff;i>0;i--);

	magd_rw(addr);

	while (cnt--)
	{
		*data++ = magd_rw(0xff);
	}

	for(uint8_t i= 0xff;i>0;i--);
	
	magd_cs_h();
}

static void LIS3MDL_wreg(uint8_t addr, uint8_t* data, uint8_t cnt)
{
	if (cnt > 1)	addr|= 0x40;

	magd_cs_l();
	
	//for(uint8_t i= 0xff;i>0;i--);

	magd_rw(addr);

	while (cnt--)
	{
		magd_rw(*data++);
	}

	magd_cs_h();
}

uint8_t wreg[5] ={0,0,0,0,0};

uint8_t LIS3MDL_init()
{
	interface_init(&magd_cfg);

	uint8_t id;
	LIS3MDL_rreg(ID, &id, 1);
	if (id != 0x3d) return 0;

	uint8_t creg_cfg[] = MAGD_CFG;
	LIS3MDL_wreg(CTRL, creg_cfg, 5);
	
	
	 LIS3MDL_rreg(CTRL, wreg, 5);

	return 1;
}

uint8_t magd_rdata(int16_t data[4])
{	
	if (magd_drdy())
	{
		uint8_t x_data[2];
		LIS3MDL_rreg(OUT_X, x_data, 2);
		data[0] = x_data[1] << 8 | x_data[0];
		
		uint8_t y_data[2];
		LIS3MDL_rreg(OUT_Y, y_data, 2);
		data[1] = y_data[1] << 8 | y_data[0];
		
		uint8_t z_data[2];
		LIS3MDL_rreg(OUT_Z, z_data, 2);
		data[2] = z_data[1] << 8 | z_data[0];
		
		uint8_t temp_data[2];
		LIS3MDL_rreg(OUT_TEMP, temp_data, 2);
		data[3] = temp_data[1] << 8 | temp_data[0];
		
		return 1;
	}
	return 0;
}

//void magd_thread()

