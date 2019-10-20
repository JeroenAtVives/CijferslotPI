#include "PJ_RPI.h"
#include <stdio.h>

#define relaisPin 17

#define codeLength 4

#define kolomlinks_pin 6
#define kolom2_pin 13
#define kolom3_pin 19
#define kolomrechts_pin 26

#define rij1_pin 12
#define rij2_pin 16
#define rij3_pin 20
#define rij4_pin 21

int ingegevenCode[4];
int juisteCode[4] = {1,12,5,6};
int aantalIngegevensCijfers;

// aantal is een pointer naar een integer die bijhoudt hoeveel cijfers er als ingedrukt zijn. 
// newNumber is een integer die het nieuwe ingedrukte nummer van de knop zal bevatten.
AddNumberToCode(int newNumber, int *aantal){

	*aantal = *aantal +1;

	// De arraylengte updaten...
	int arrrayLength =  *aantal;

	// Het nieuwe nummer toevoegen aan de array...
	ingegevenCode[*aantal-1] = newNumber;

	// Enkele nuttige info displayen...
	printf("Het aantal ingegeven cijfers is %d \n",*aantal);
	printf("Het nummer %d is toegevoegd \n",newNumber);
	printf("De lengte van de array is %d \n",arrrayLength);
	printf("\n");

	// De lengte van de ingegeven array vergelijken met de lengte van de juiste code.
	if(arrrayLength == codeLength){
		printf("Er zijn 4 nummers ingevoerd geweest\n");
	}
}

// Returnt 1 als de ingegeven code overeen komt met de juiste code, retourneert anders 0.
int CompareEnteredWithCorrectCode(int(*juisteCode)[codeLength], int(*ingegevenCode)[codeLength]){

	// printf("Eerste nummer van de ingegeven code: %d \n",*(*ingegevenCode));
	// printf("Tweede nummer van de ingegeven code: %d \n",*(*ingegevenCode+1));
	// printf("Derde nummer van de ingegeven code: %d \n",*(*ingegevenCode +2));
	// printf("Vierde nummer van de ingegeven code: %d \n",*(*ingegevenCode + 3));

	// printf("Eerste nummer van de juiste code: %d \n",*(*ingegevenCode));
	// printf("Tweede nummer van de juiste code: %d \n",*(*ingegevenCode+1));
	// printf("Derde nummer van de juiste code: %d \n",*(*ingegevenCode +2));
	// printf("Vierde nummer van de juiste code: %d \n",*(*ingegevenCode + 3));	

	for (int i = 0; i < codeLength ; i++)
	{
		printf("Forlus -> i = %d ingegeven nummer: %d \n",i,*(*ingegevenCode + i));	
		printf("Forlus -> i = %d juist nummer: %d \n",i,*(*juisteCode + i));	
		printf("\n");	


		if((*(*juisteCode + i)) != (*(*ingegevenCode + i))){
			printf("Vanaf index %d klopt de ingegeven code niet meer \n\n",i);
			return 0;
		}
	}
	return 1;
}


void AansturenSafeRelais(int pin,int time){
	// Toggle 17 (blink a led!)
	GPIO_SET = 1 << pin;
	sleep(time);

	GPIO_CLR = 1 << pin;
	sleep(time);
}

void ActivateKolom(int data) {
	if(data & 0x08){
		GPIO_SET = 1 << kolomlinks_pin;
    	//printf("Kolom 1 is actief\n");
	}
	else
		GPIO_CLR = 1 << kolomlinks_pin;
	if(data & 0x04){
		GPIO_SET = 1 << kolom2_pin;
    	//printf("Kolom 2 is actief\n");
	}		
	else
		GPIO_CLR = 1 << kolom2_pin;
	if(data & 0x02){
		GPIO_SET = 1 << kolom3_pin;
    	//printf("Kolom 3 is actief\n");
	}
	else
		GPIO_CLR = 1 << kolom3_pin;
	if(data & 0x01){
		GPIO_SET = 1 << kolomrechts_pin;
    	//printf("Kolom 4 is actief\n");
	}
	else
		GPIO_CLR = 1 << kolomrechts_pin;
}

int main()
{

	int rijnummer;
	int kolomnummer;

	int Knopnumbers[4][4] = {{1,2,3,4},   
						{5,6,7,8},
						{9,10,11,12},
						{13,14,15,16}};
	
	 

	if(map_peripheral(&gpio) == -1) 
	{
       	 	printf("Failed to map the physical GPIO registers into the virtual memory space.\n");
        	return -1;
    	}

	// Define relaisPin as output
	INP_GPIO(relaisPin);
	OUT_GPIO(relaisPin);

	// 4 outputs voor de kolommen.
	INP_GPIO(kolomlinks_pin);
	OUT_GPIO(kolomlinks_pin);
	INP_GPIO(kolom2_pin);
	OUT_GPIO(kolom2_pin);
	INP_GPIO(kolom3_pin);	
	OUT_GPIO(kolom3_pin);
	INP_GPIO(kolomrechts_pin);	
	OUT_GPIO(kolomrechts_pin);

	// 4 inputs voor de rijen.
	INP_GPIO(rij1_pin);
	INP_GPIO(rij2_pin);
	INP_GPIO(rij3_pin);
	INP_GPIO(rij4_pin);

	while(1)
	{
		// Een kolom gedurende een bepaalde tijd actief maken.
		for(kolomnummer =1;kolomnummer<=4;kolomnummer++)
		{
				switch(kolomnummer)
				{
					case 1:
						ActivateKolom(0x08);// Dat is de meeste linkse kolom activeren.
						sleep(1);
						break;
					case 2:
						ActivateKolom(0x04);
						sleep(1);
						break;
					case 3:
						ActivateKolom(0x02);						
						sleep(1);
						break;
					case 4:
						ActivateKolom(0x01);						
						sleep(1);
						break;
					default:
						break;
				}
		
			for(rijnummer=1;rijnummer<=4;rijnummer++)  // De rijen via een tweede for-lus controleren.
			{
				switch(rijnummer)
				{
					case 1:
						// Is een rij actief?
						if(GPIO_READ(rij1_pin)) 
						{
							// Wacht dan tot de desbetreffende knop losgelaten wordt...
							while(GPIO_READ(rij1_pin));
							
							// Voeg het nummer van de ingedrukte knop toe aan de ingegeven code.
							AddNumberToCode(Knopnumbers[rijnummer-1][kolomnummer-1],&aantalIngegevensCijfers);
						}

						break;
					case 2:
						if(GPIO_READ(rij2_pin))
						{
							while(GPIO_READ(rij2_pin));							
							AddNumberToCode(Knopnumbers[rijnummer-1][kolomnummer-1],&aantalIngegevensCijfers);

						}
						break;
				
					case 3:

						if(GPIO_READ(rij3_pin))
						{
							while(GPIO_READ(rij3_pin));							
							AddNumberToCode(Knopnumbers[rijnummer-1][kolomnummer-1],&aantalIngegevensCijfers);

						}
						break;
					case 4:
						if(GPIO_READ(rij4_pin))
						{
							if((rijnummer == 4) && (kolomnummer == 4)){
								while(GPIO_READ(rij4_pin));	
								// Je drukte op bevestigen ('=').
								if(aantalIngegevensCijfers == codeLength){
									printf("De code bestaat uit %d cijfers \n",codeLength);
									if(CompareEnteredWithCorrectCode(&juisteCode,&ingegevenCode)){
										// Unlock safe...
										printf("Je hebt de juiste code ingedrukt. The safe wordt unlocked \n");
										AansturenSafeRelais(relaisPin,3);

									}else{
										printf("Je hebt niet de juiste code ingedrukt. \n");
										printf("Probeer opnieuw... \n\n");
										aantalIngegevensCijfers = 0;										
									}
								}
								else{
									printf("De code moet bestaan uit %d nummers \n",codeLength);
									printf("Probeer opnieuw... \n\n");
									aantalIngegevensCijfers = 0;
								}

							}
							else{
							while(GPIO_READ(rij4_pin));							
							AddNumberToCode(Knopnumbers[rijnummer-1][kolomnummer-1],&aantalIngegevensCijfers);
						
							}
						}
						break;
					default:
						break;
				}				

			}			
		}		
	}

	return 0;	

}

