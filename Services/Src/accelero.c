#include "../../MesServices/Include/accelero.h"
#include "stm32f10x.h"


#define first_bit 0x80 // Je veux lire 
#define BW_RATE_W 0x2C
// #define BW_RATE_R 0xAC
char BW_RATE_R = first_bit | BW_RATE_W;

#define POWER_CTL_W 0x2D
#define POWER_CTL_R 0xAD

#define DATA_FORMAT_W 0x31
#define DATA_FORMAT_R 0xB1



void Accelero_Init (SPI_TypeDef * SPI) {
		char tps;
	
	  MySPI_Init(SPI);
	

		tps = 0x0A; 								// BW_rate  à 100Hz
		MySPI_Set_NSS();						// CS à 0
		MySPI_Send(BW_RATE_W);			// Je veux écrire BW_RATE
		MySPI_Send(tps);						// J'écrit BW_RATE

		MySPI_Send(POWER_CTL_R);		// Je veux lit POWER_CTL
		tps = MySPI_Read();					// Je lit POWER_CTL
		MySPI_Clear_NSS();					// CS à 1	
		tps  |= 1<<3; 							// measure de power ctrl = 1
		MySPI_Set_NSS();						// CS à 0	
		MySPI_Send(POWER_CTL_W);		// Je veux écrire POWER_CTL
		MySPI_Send(tps);						// J'écrit POWER_CTL

	
		MySPI_Send(DATA_FORMAT_R);	// Je veux lit DATA_FORMAT
		tps = MySPI_Read();					// Je lit POWER_CTL
		MySPI_Clear_NSS();					// CS à 1	
		tps  &= ~(1<<2); 						// Justify de Data Format = 0
		tps	|= 0x0B; 								// Full_reso = 1 et Range bit = 11 (+- 16g)
		MySPI_Set_NSS();						// CS à 0
		MySPI_Send(DATA_FORMAT_W);	// Je veux écrire DATA_FORMAT
		MySPI_Send(tps);						// J'écrit POWER_CTL
		MySPI_Clear_NSS();					// CS à 1	
		 
}
