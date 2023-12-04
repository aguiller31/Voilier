/*********************************************************************
 * @file  app.c
 * @author Antoine Guillermin
 * @brief Fichier source de l'application
 *********************************************************************/
 
#include "app.h"

// Services
SystickService *SysSer;
CommunicationService *ComSer;
RotationService *RotSer;
BatterieService *BatSer;
BordageService *BorSer;

/**
 * @brief Writes the current time to the communication service.
 */
void Application_WriteTime()
{
    char T[3];
    Get_Time(T);
    ComSer->WriteTime(ComSer, T);

}

/**
 * @brief Callback function for Systick to get battery level.
 * 
 * @param time The current time.
 */
void Callback_Systick_GetBattery(int time)
{
    BatSer->GetBatteryLevel(BatSer);
}

/**
 * @brief Callback function for Systick to handle bordage.
 * 
 * @param time The current time.
 */
void Callback_Systick_Bordage(int time)
{
    int angle_girouette, angle_roulis;

    angle_roulis = Angle_Roulis(ACCELERO_SPI);
    if (angle_roulis == 1) {
        Application_WriteTime();
        ComSer->SendAlert(ComSer,ALERT_DLR);
        BorSer->Lacher(BorSer);
    } else {
        angle_girouette = getAngleGirouette2();
        BorSer->Change(BorSer, angle_girouette);
    }
}

/**
 * @brief Callback function for Systick to send information every 3 seconds.
 * 
 * @param time The current time.
 */
void Callback_Systick_Infos(int time)
{
    // every 3 seconds, send the current bordage information, i.e., the sail opening angle
    Application_WriteTime();
    ComSer->WriteString(ComSer, " - Angle d'ouverture de voile : ");
    ComSer->WriteInt(ComSer, BorSer->GetTeta(BorSer));
    ComSer->WriteStringNL(ComSer, " degres.");
}

/**
 * @brief Callback function for communication service for tribord direction.
 * 
 * @param val The value received.
 */
void Callback_Communication_Tribord(signed char val)
{
    RotSer->SetDirection(RotSer, RIGHT);
    RotSer->SetSpeed(RotSer, (int)val);
}

/**
 * @brief Callback function for communication service for babord direction.
 * 
 * @param val The value received.
 */
void Callback_Communication_Babord(signed char val)
{
    RotSer->SetDirection(RotSer, LEFT);
    RotSer->SetSpeed(RotSer, (int)val);
}

/**
 * @brief Callback function for communication service when receiving 0.
 */
void Callback_Communication_0()
{
    RotSer->SetSpeed(RotSer, 0);
}

/**
 * @brief Callback function for battery service.
 * 
 * @param battery_level The battery level.
 */
void Callback_Batterie(int battery_level)
{
    if (battery_level < BATTERIE_LOW_LEVEL) {
        Application_WriteTime();
        ComSer->SendAlert(ComSer, ALERT_LOW_BATTERY);
    }
}

/**
 * @brief Sets up all the required services.
 * 
 * @param This Pointer to the application instance.
 */
static void SetupServices(Application *This)
{
    Set_Time();
    SysSer = New_Systick();
    SysSer->Setup(SysSer);

    Accelero_Init(ACCELERO_SPI);

    ComSer = New_Communication();
    ComSer->Start(ComSer);

    RotSer = New_Rotation();
    RotSer->Setup(RotSer);

    BatSer = New_Batterie();
    BatSer->Setup(BatSer);

    BorSer = New_Bordage();
    BorSer->Setup(BorSer);
}

/**
 * @brief Sets up all the callback functions.
 * 
 * @param This Pointer to the application instance.
 */
static void SetupCallbacks(Application *This)
{
    // SYSTICK
    This->Callback_pointeur_Systick_GetBattery = Callback_Systick_GetBattery;
    This->Callback_pointeur_Systick_Bordage = Callback_Systick_Bordage;
    This->Callback_pointeur_Systick_Infos = Callback_Systick_Infos;

    // ROTATION
#if ROTATION_MODE == 1
    This->Callback_pointeur_Communication_Babord = Callback_Communication_Babord;
    This->Callback_pointeur_Communication_Tribord = Callback_Communication_Tribord;
#else
    This->Callback_pointeur_Communication_Babord = Callback_Communication_Tribord;
    This->Callback_pointeur_Communication_Tribord = Callback_Communication_Babord;
#endif
    This->Callback_pointeur_Communication_0 = Callback_Communication_0;

    // BATTERIE
    This->Callback_pointeur_Batterie = Callback_Batterie;
}

/**
 * @brief Starts communication service.
 * 
 * @param This Pointer to the application instance.
 */
static void StartCommunication(Application *This)
{
    ComSer->RegisterReadDirection(ComSer, BABORD, This->Callback_pointeur_Communication_Babord);
    ComSer->RegisterReadDirection(ComSer, TRIBORD, This->Callback_pointeur_Communication_Tribord);
    ComSer->RegisterReadChar(ComSer, 0, This->Callback_pointeur_Communication_0);
    ComSer->Read(ComSer);
}

/**
 * @brief Starts battery service.
 * 
 * @param This Pointer to the application instance.
 */
static void StartBatteryWatch(Application *This)
{
    BatSer->RegisterBatteryLevel(BatSer, This->Callback_pointeur_Batterie);
}

/**
 * @brief Starts girouette service.
 * 
 * @param This Pointer to the application instance.
 */
static void StartGirouetteWatch(Application *This)
{
    InitGirouette2(GIROUETTE_TIMER);
    TIMER_BASE_START(GIROUETTE_TIMER);
}

/**
 * @brief Starts the Systick service.
 * 
 * @param This Pointer to the application instance.
 */
static void StartSystick(Application *This)
{
    SysSer->Register(SysSer, BATTERIE_SYSTIC_PERIOD, This->Callback_pointeur_Systick_GetBattery);   // every 3 seconds
    SysSer->Register(SysSer, BORDAGE_SYSTIC_PERIOD, This->Callback_pointeur_Systick_Bordage);      // every 20 ms
    SysSer->Register(SysSer, GENERAL_INFO_SYSTIC_PERIOD, This->Callback_pointeur_Systick_Infos);
    SysSer->Start(SysSer);
}

/**
 * @brief Sets up the application.
 * 
 * @param This Pointer to the application instance.
 */
void Application_Setup(Application *This)
{
    SetupServices(This);
    SetupCallbacks(This);
}

/**
 * @brief Starts the application.
 * 
 * @param This Pointer to the application instance.
 */
void Application_Start(Application *This)
{
    StartCommunication(This);
    StartBatteryWatch(This);
    StartGirouetteWatch(This);
    StartSystick(This);
}

/**
 * @brief Initializes the application instance.
 * 
 * @param This Pointer to the application instance.
 */
static void Application_Init(Application *This)
{
    This->Setup = Application_Setup;
    This->Start = Application_Start;
}

/**
 * @brief Frees the memory allocated for the application instance.
 * 
 * @param This Pointer to the application instance.
 */
void Application_New_Free(Application *This)
{
    free(This);
}

/**
 * @brief Creates a new instance of the application.
 * 
 * @return Pointer to the new instance of the application.
 */
Application *New_Application()
{
    Application *This = malloc(sizeof(Application));
    if (!This)
        return NULL;
    Application_Init(This);
    This->Free = Application_New_Free;
    return This;
}
