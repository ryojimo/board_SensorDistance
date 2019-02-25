/**************************************************************************//*!
 *  @file           hal_motorSV.c
 *  @brief          [HAL] サーボモータ・ドライバ API を定義したファイル。
 *  @attention      01. PWM サイクル : 20ms (= 50Hz )
 *                  02. 制御パルス   : 0.5ms ～ 2.4ms
 *                      0.5 ms : duty= 2.5 % の時 -90°
 *                      1.45ms : duty= 7.25% の時   0°
 *                      2.4 ms : duty=12.0 % の時 +90°
 *************************************************************************** */
#ifdef __cplusplus
    extern "C"{
#endif


//********************************************************
/* include                                               */
//********************************************************
#include <wiringPi.h>

#include "hal_cmn.h"
#include "hal.h"


//#define DBG_PRINT
#define MY_NAME "HAL"
#include "../app/log/log.h"


//********************************************************
/*! @def                                                 */
//********************************************************
#define MOTOR_OUT    (18)


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
static void         InitParam( void );
static EHalBool_t   InitReg( void );




/**************************************************************************//*!
 * @brief     ファイルスコープ内のグローバル変数を初期化する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
InitParam(
    void  ///< [in] ナシ
){
    DBG_PRINT_TRACE( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     H/W レジスタを初期化する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
static EHalBool_t
InitReg(
    void  ///< [in] ナシ
){
    DBG_PRINT_TRACE( "\n\r" );
    return EN_TRUE;
}


/**************************************************************************//*!
 * @brief     サーボモータを初期化する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
EHalBool_t
HalMotorSV_Init(
    void  ///< [in] ナシ
){
    EHalBool_t      ret = EN_FALSE;

    DBG_PRINT_TRACE( "\n\r" );

    InitParam();
    ret = InitReg();
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "Unable to initialize GPIO port. \n\r" );
        return ret;
    }

    ret = EN_TRUE;
    return ret;
}


/**************************************************************************//*!
 * @brief     サーボモータを終了する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
HalMotorSV_Fini(
    void  ///< [in] ナシ
){
    DBG_PRINT_TRACE( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     サーボモータを回す。
 * @note      PWM のカウンタのカウントアップを 5kHz (= 0.2ms ) の速さでカウントアップする設定
 *              => 19.2MHz / clock(=3840) = 5kHz
 *            100 カウントアップで PWM 1 周期に設定
 *              => 0.2ms * cnt(=100) = 20ms (= 50Hz )
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
HalMotorSV_SetPwmDuty(
    EHalMotorState_t    status, ///< [in] モータの状態
    int                 rate    ///< [in] デューティ比 : 0% ～ 100% まで
){
    unsigned int        value = 0;
    unsigned int        clock = 3840;
    unsigned int        cnt = 100;

    DBG_PRINT_TRACE( "status = %d \n\r", status );
    DBG_PRINT_TRACE( "rate   = %d%% \n\r", rate );

    // デューティ比 = value / range
    value = rate;

    if( status == EN_MOTOR_STANDBY )
    {
        pinMode( MOTOR_OUT, OUTPUT );
        digitalWrite( MOTOR_OUT, EN_LOW );
    } else if( status == EN_MOTOR_BRAKE )
    {
        pinMode( MOTOR_OUT, PWM_OUTPUT );
        pwmSetMode( PWM_MODE_MS );
        pwmSetClock( clock );
        pwmSetRange( cnt );
        pwmWrite( MOTOR_OUT, 0 );
    } else if( status == EN_MOTOR_CCW || status == EN_MOTOR_CW )
    {
        pinMode( MOTOR_OUT, PWM_OUTPUT );
        pwmSetMode( PWM_MODE_MS );
        pwmSetClock( clock );
        pwmSetRange( cnt );
        pwmWrite( MOTOR_OUT, value );
    } else if( status == EN_MOTOR_STOP )
    {
        pinMode( MOTOR_OUT, PWM_OUTPUT );
        pwmSetMode( PWM_MODE_MS );
        pwmSetClock( clock );
        pwmSetRange( cnt );
        pwmWrite( MOTOR_OUT, 0 );
    } else
    {
        ;
    }

    return;
}


#ifdef __cplusplus
    }
#endif

