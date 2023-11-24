#include "app.h"

//Services
SystickService * SysSer;
CommunicationService * ComSer;
RotationService * RotSer;
BatterieService * BatSer;
HorlogeService * HorSer;
BordageService * BorSer;
		
//Variables
int main_angle_girouette;

void Callback_Systick_GetAngleGirouette(int time)
{
	if(time%GIROUETTE_SYSTIC_PERIOD ==0)
		main_angle_girouette=getAngleGirouette();
}
void Callback_Systick_GetBattery(int time)
{
	if(time%BATTERIE_SYSTIC_PERIOD ==0)
		BatSer->GetBatteryLevel(BatSer);
}

void Callback_Systick_Bordage(int time)
{
	if(time%BORDAGE_SYSTIC_PERIOD ==0){
		BorSer->Change(BorSer,main_angle_girouette);
	}
}
void Callback_Systick_Infos(int time)
{
	char * angle;
	if(time%300 ==0){//toutes les 3 secondes on envoie l�information de bordage en cours, cad l�angle d�ouverture de voile 
		ComSer->WriteString(ComSer,"Angle d'ouverture de voile : ");
		sprintf(angle, "%d", BorSer->GetTeta(BorSer));
		ComSer->WriteString(ComSer,angle);
		ComSer->WriteStringNL(ComSer," degres.");
	}
}

void Callback_Communication_Tribord(signed char val){
	RotSer->SetDirection(RotSer,RIGHT);
	RotSer->SetSpeed(RotSer,(int)val);
}

void Callback_Communication_Babord(signed char val){
	RotSer->SetDirection(RotSer,LEFT);
	RotSer->SetSpeed(RotSer,(int)val);
}
void Callback_Communication_0(){
	RotSer->SetSpeed(RotSer,0);
}

void Callback_Batterie(int battery_level){
	if(battery_level < BATTERIE_LOW_LEVEL){
		ComSer->SendAlert(ComSer,ALERT_LOW_BATTERY);
	}
}
static void SetupServices(Application *This)
{
	SysSer = New_Systick();
	SysSer->Setup(SysSer);
	
	
	ComSer = New_Communication();
	ComSer->Start(ComSer);
	
	RotSer = New_Rotation();
	RotSer->Setup(RotSer);
	
	BatSer = New_Batterie();
	BatSer->Setup(BatSer);
	
	HorSer = New_Horloge();
	HorSer->Setup(HorSer);
	
	BorSer = New_Bordage();
	BorSer->Setup(BorSer);
}
static void SetupCallbacks(Application *This)
{
	//SYSTICK
	This->Callback_pointeur_Systick_GetBattery = Callback_Systick_GetBattery;
	This->Callback_pointeur_Systick_Bordage = Callback_Systick_Bordage;
	This->Callback_pointeur_Systick_GetAngleGirouette = Callback_Systick_GetAngleGirouette;
	
	//ROTATION
	This->Callback_pointeur_Communication_Babord = Callback_Communication_Babord ;
	This->Callback_pointeur_Communication_Tribord = Callback_Communication_Tribord ;
	This->Callback_pointeur_Communication_0 = Callback_Communication_0 ;
	
	//BATTERIE
	This->Callback_pointeur_Batterie = Callback_Batterie ;
}
static void StartCommunication(Application *This)
{
	ComSer->RegisterReadDirection(ComSer,BABORD,This->Callback_pointeur_Communication_Babord);
	ComSer->RegisterReadDirection(ComSer,TRIBORD,This->Callback_pointeur_Communication_Tribord);
	ComSer->RegisterReadChar(ComSer,0,This->Callback_pointeur_Communication_0);
	ComSer->Read(ComSer);
}
static void StartBatteryWatch(Application *This)
{
	BatSer->RegisterBatteryLevel(BatSer,This->Callback_pointeur_Batterie);
}
static void StartGirouetteWatch(Application *This)
{
	InitGirouette();
}
static void StartSystick(Application *This)
{
	SysSer->Register(SysSer,This->Callback_pointeur_Systick_GetBattery); //pour toutes les 3sec
	SysSer->Register(SysSer,This->Callback_pointeur_Systick_Bordage); //pour toutes les 20 ms
	SysSer->Register(SysSer,This->Callback_pointeur_Systick_GetAngleGirouette);
	SysSer->Start(SysSer);
}
void Application_Setup(Application *This)
{
	SetupServices(This);
	SetupCallbacks(This);
}
void Application_Start(Application *This)
{
	StartCommunication(This);
	StartBatteryWatch(This);
	StartGirouetteWatch(This);
	StartSystick(This);
}
/**
 * @brief Initialise l'instance de l'application
 * 
 * @param This Pointeur vers l'instance de l'application
 */
static void Application_Init(Application *This)
{
	This->Setup = Application_Setup;
	This->Start = Application_Start;
}

/**
 * @brief Lib�re la m�moire allou�e pour l'instance de l'application
 * 
 * @param This Pointeur vers l'instance de l'application
 */
void Application_New_Free(Application *This)
{
    free(This);        
}

/**
 * @brief Cr�e une nouvelle instance de l'application
 * @details Alloue la m�moire et initialise l'instance de l'application
 * 
 * @return Pointeur vers la nouvelle instance de l'application
 */
Application *New_Application()
{
    Application *This = malloc(sizeof(Application));
    if(!This) return NULL;
    Application_Init(This);
    This->Free = Application_New_Free;
    return This;
}