/**************************************************************************//*!
 *  @file           hal_motorDC2.c
 *  @brief          [HAL] DC モータ・ドライバ API を定義したファイル。
 *  @author         Ryoji Morita
 *  @attention      none.
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2019.01.17
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
#define MOTOR_OUT    (12)


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
 * @brief     DC モータを初期化する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
EHalBool_t
HalMotorDC2_Init(
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

    HalMotorDC2_SetPwmDuty( EN_MOTOR_STOP, 0 );
    ret = EN_TRUE;
    return ret;
}


/**************************************************************************//*!
 * @brief     DC モータを終了する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
HalMotorDC2_Fini(
    void  ///< [in] ナシ
){
    DBG_PRINT_TRACE( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     DC モータを回す。
 * @note      PWM のカウンタのカウントアップを 5kHz (= 0.2ms ) の速さでカウントアップする設定
 *              => 19.2MHz / clock(=3840) = 5kHz
 *            100 カウントアップで PWM 1 周期に設定
 *              => 0.2ms * cnt(=100) = 20ms (= 50Hz )
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
HalMotorDC2_SetPwmDuty(
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

