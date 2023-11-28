#include "Accelerometre_Service.h"

char BW_RATE_R = (first_bit | BW_RATE_W);


void Accelero_Init (SPI_TypeDef * SPI) {
	
		char tps;
	
		MyGPIO_Init(ACCELERO_GPIO_TX, ACCELERO_GPIO_TX_PIN, In_Floating);
	/*	MyGPIO_Init(ACCELERO_GPIO_CS, ACCELERO_GPIO_CS_PIN, AltOut_Ppull);
		MyGPIO_Init(ACCELERO_GPIO_CLK, ACCELERO_GPIO_CLK_PIN, AltOut_Ppull);*/
		MyGPIO_Init(ACCELERO_GPIO_RX, ACCELERO_GPIO_RX_PIN, AltOut_Ppull);
	
		
	  MySPI_Init(SPI);
		tps = 0x0A; 									// BW_rate  Ã  100Hz
		
		wait();
	
		MySPI_Clear_NSS();						// CS Ã  0
		MySPI_Send(BW_RATE_W);				// Je veux Ã©crire BW_RATE
		MySPI_Send(tps);							// J'Ã©crit BW_RATE
		MySPI_Set_NSS();							// CS Ã  1
	
		wait();

		MySPI_Clear_NSS();						// CS Ã  0
		MySPI_Send(POWER_CTL_R);			// Je veux lire POWER_CTL
		tps = MySPI_Read();						// Je lit POWER_CTL
		MySPI_Set_NSS();							// CS Ã  1
	
		wait();
	
		tps  |= 1<<3; 								// measure de power ctrl = 1
	
		MySPI_Clear_NSS();						// CS Ã  0	
		MySPI_Send(POWER_CTL_W);			// Je veux Ã©crire POWER_CTL
		MySPI_Send(tps);							// J'Ã©crit POWER_CTL
		MySPI_Set_NSS();							// CS Ã  1
		
		wait();
		
		MySPI_Clear_NSS();						// CS Ã  0
		MySPI_Send(DATA_FORMAT_R);		// Je veux lire DATA_FORMAT
		tps = MySPI_Read();						// Je lit DATA_FORMAT
		MySPI_Set_NSS();							// CS Ã  1
		
		wait();
		
		tps  &= ~(1<<2); 							// Justify de Data Format = 0
		tps	|= 0x0B; 									// Full_reso = 1 et Range bit = 11 (+- 16g)
		
		MySPI_Clear_NSS();						// CS Ã  0
		MySPI_Send(DATA_FORMAT_W);		// Je veux Ã©crire DATA_FORMAT
		MySPI_Send(tps);							// J'Ã©crit POWER_CTL
		MySPI_Set_NSS();							// CS Ã  1	
		 
}


int	Angle_Roulis (SPI_TypeDef * SPI) {
	
	char x0,x1,y0,y1,z0,z1;

	int16_t y,Y;
	
	MySPI_Clear_NSS();						// CS Ã  0
	MySPI_Send(X0_R_MB);			// Je veux Lire BW_RATE
	x0 = MySPI_Read();
	x1 = MySPI_Read();
	y0 = MySPI_Read();
	y1 = MySPI_Read();
	z0 = MySPI_Read();
	z1 = MySPI_Read();
	MySPI_Set_NSS();						// CS Ã  0
	
		// ConcatÃ©nation des chaÃ®nes en une nouvelle chaÃ®ne
		y = (y1<<8) + y0;
		if (y > 0) {
			Y = y;
		}
		else{
			Y = -y;
		}
		
	
		if (Y>128){
			return 1;
		}	// si Y>Z alors on chavire
			else {
			return 0;}			
		
}

void wait(){
	int n_sec = 6;
	do
{
	n_sec=n_sec-1;
}while(n_sec);

}
