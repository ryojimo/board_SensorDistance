/**************************************************************************//*!
 *  @file           sys.c
 *  @brief          [SYS] システム関係の API を定義する。
 *  @author         Ryoji Morita
 *  @attention      none.
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2016.06.24
 *************************************************************************** */
#ifdef __cplusplus
    extern "C"{
#endif


//********************************************************
/* include                                               */
//********************************************************
#include "../hal/hal.h"
#include "../app/if_lcd/if_lcd.h"
#include "../app/if_pc/if_pc.h"

#include "sys.h"


//#define DBG_PRINT
#define MY_NAME "SYS"
#include "../app/log/log.h"


//********************************************************
/*! @def                                                 */
//********************************************************
// なし


//********************************************************
/*! @enum                                                */
//********************************************************
// なし


//********************************************************
/*! @struct                                              */
//********************************************************
// なし


//********************************************************
/* モジュールグローバル変数                              */
//********************************************************
// なし


//********************************************************
/* 関数プロトタイプ宣言                                  */
//********************************************************
// なし




/**************************************************************************//*!
 * @brief     システムを初期化する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
Sys_Init(
    void    ///< [in] ナシ
){
    DBG_PRINT_TRACE( "\n\r" );

    HalCmnGpio_Init();
    HalCmnI2c_Init();
    HalCmnSpi_Init();

    HalI2cLcd_Init();
    HalLed_Init();
    HalMotorDC_Init();
    HalMotorDC2_Init();
    HalMotorST_Init();
    HalMotorSV_Init();
    HalPushSw_Init();
    HalSensorBmx055_Init();

    // SENSOR (ADC)
    HalSensorPm_Init();

    HalTime_Init();

    return;
}


/**************************************************************************//*!
 * @brief     システムを終了処理する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
Sys_Fini(
    void    ///< [in] ナシ
){
    DBG_PRINT_TRACE( "\n\r" );

    HalI2cLcd_Fini();
    HalLed_Fini();
    HalMotorDC_Fini();
    HalMotorDC2_Fini();
    HalMotorST_Fini();
    HalMotorSV_Fini();
    HalPushSw_Fini();
    HalSensorBmx055_Fini();

    // SENSOR (ADC)
    HalSensorPm_Fini();

//  HalTime モジュールに Fini() 処理はない

    HalCmnGpio_Fini();
    HalCmnI2c_Fini();
    HalCmnSpi_Fini();

    return;
}


/**************************************************************************//*!
 * @brief     システム情報を表示する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
Sys_ShowInfo(
    void    ///< [in] ナシ
){
    SHalTime_t*     date;

    DBG_PRINT_TRACE( "\n\r" );

    AppIfLcd_Clear();
    AppIfLcd_Ctrl( 1, 0, 0 );

    AppIfLcd_CursorSet( 0, 0 );
    AppIfLcd_Printf( "RP3 BOARD SENSOR" );
    usleep( 2000 * 1000 );  // 2sec 表示

    AppIfLcd_CursorSet( 0, 1 );
    date = HalTime_GetLocaltime();
    AppIfLcd_Printf( "%04d/%02d/%02d",
                    date->year, date->month, date->day );

    AppIfPc_Printf( "[Compiler Info]================= \n\r" );
    AppIfPc_Printf( "sizeof(char)  = %d \n\r", sizeof(char) );
    AppIfPc_Printf( "sizeof(short) = %d \n\r", sizeof(short) );
    AppIfPc_Printf( "sizeof(int)   = %d \n\r", sizeof(int) );
    AppIfPc_Printf( "sizeof(long)  = %d \n\r", sizeof(long) );
    AppIfPc_Printf( "[System Info]=================== \n\r" );
    AppIfPc_Printf( "S/W Ver.      = 0.01 \n\r" );

    date = HalTime_GetLocaltime();
    AppIfPc_Printf( "Date(local)   = " );
    AppIfPc_Printf( "%04d/%02d/%02d %02d:%02d:%02d",
            date->year, date->month, date->day,
            date->hour, date->min,   date->sec );
    AppIfPc_Printf( "\n\r" );

    date = HalTime_GetUTC();
    AppIfPc_Printf( "Date(UTC)     = " );
    AppIfPc_Printf( "%04d/%02d/%02d %02d:%02d:%02d",
            date->year, date->month, date->day,
            date->hour, date->min,   date->sec );
    AppIfPc_Printf( "\n\r" );
    AppIfPc_Printf( "================================ \n\r" );

    AppIfPc_Puts( "RP3 BOARD SENSOR\n\r" );

    AppIfPc_Puts( "Launch .... \n\r" );
    AppIfPc_Puts( "\n\r" );
    AppIfPc_Puts( "\n\r" );

    return;
}


#ifdef __cplusplus
    }
#endif

