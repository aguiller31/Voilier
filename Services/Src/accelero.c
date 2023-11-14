#include "../../MesServices/Include/accelero.h"
#include "stm32f10x.h"


#define first_bit 0x80 // Je veux lire 
#define BW_RATE_W 0x2C
#define BW_RATE_R 0xAC

#define POWER_CTL_W 0x2D
#define POWER_CTL_R 0xAD

#define DATA_FORMAT_W 0x31
#define DATA_FORMAT_R 0xB1



void Accelero_Init (SPI_TypeDef * SPI) {
		char tps;
	
	  MySPI_Init(SPI);
	
		MySPI_Send(BW_RATE_R);	// Je veux lit BW_RATE
		tps = MySPI_Read();			// Je lit BW_RATE
		tps  &= 0; 							// reset Bw_Rate
		tps |= 0x0A; 						// BW_rate  à 100Hz
		MySPI_Send(BW_RATE_W);	// Je veux écrire BW_RATE
		MySPI_Send(tps);				// J'écrit BW_RATE
	
		MySPI_Send(POWER_CTL_R);		// Je veux lit POWER_CTL
		tps = MySPI_Read();					// Je lit POWER_CTL
		tps  |= 1<<3; 							// measure de power ctrl = 1
		MySPI_Send(POWER_CTL_W);		// Je veux écrire POWER_CTL
		MySPI_Send(tps);						// J'écrit POWER_CTL
	
		MySPI_Send(DATA_FORMAT_R);	// Je veux lit DATA_FORMAT
		tps = MySPI_Read();					// Je lit POWER_CTL
		tps  &= ~(1<<2); 						// Justify de Data Format = 0
		tps	|= 0x0B; 								// Full_reso = 1 et Range bit = 11 (+- 16g) 
		MySPI_Send(DATA_FORMAT_W);	// Je veux écrire DATA_FORMAT
		MySPI_Send(tps);						// J'écrit POWER_CTL
		 
}
