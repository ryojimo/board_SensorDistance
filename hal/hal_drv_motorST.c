/**************************************************************************//*!
 *  @file           hal_motorST.c
 *  @brief          [HAL] ステッピングモータ・ドライバ API を定義したファイル。
 *************************************************************************** */
#ifdef __cplusplus
    extern "C"{
#endif


//********************************************************
/* include                                               */
//********************************************************
#include <signal.h>
#include <string.h>
#include <sys/time.h>

#include <wiringPi.h>

#include "hal_cmn.h"
#include "hal.h"

//#define DBG_PRINT
#define MY_NAME "HAL"
#include "../app/log/log.h"


//********************************************************
/*! @def                                                 */
//********************************************************
#define MOTOR_ST_DIR_OUT        (22)
#define MOTOR_ST_CK_OUT         (27)
#define MOTOR_ST_E_OUT          (17)

#define MOTOR_ST_CCW()          digitalWrite( MOTOR_ST_DIR_OUT, EN_HIGH )
#define MOTOR_ST_CW()           digitalWrite( MOTOR_ST_DIR_OUT, EN_LOW )
#define MOTOR_ST_CK_HIGH()      digitalWrite( MOTOR_ST_CK_OUT, EN_HIGH )
#define MOTOR_ST_CK_LOW()       digitalWrite( MOTOR_ST_CK_OUT, EN_LOW )
#define MOTOR_ST_E_HIGH()       digitalWrite( MOTOR_ST_E_OUT, EN_HIGH )
#define MOTOR_ST_E_LOW()        digitalWrite( MOTOR_ST_E_OUT, EN_LOW )

// AOR = Angle Of Rotation
// PULSE = 1 PULSE で 0.45°回転。
// タイマ割り込み 1 回で 0.45°回転。
#define ANGLE_MIN               (0.45)  ///< @def :  指定可能な最小の回転角度

#define PULSE_ANGLE_360         (800)   ///< @def : 360°回転するパルス数 ( 360 / 0.45 = 800 )
#define PULSE_ANGLE_360x10      (8000)  ///< @def : 360°回転を 10 回するパルス数


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
static EHalMotorState_t g_status;
static int              g_cnt;
suseconds_t             g_usec;


//********************************************************
/* 関数プロトタイプ宣言                                  */
//********************************************************
static void         InitParam( void );
static EHalBool_t   InitReg( void );

static EHalBool_t   TimerStart( void );
static EHalBool_t   TimerStop( void );
static void         HalMotorST_IH_Set( int signum );




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
    void    ///< [in] ナシ
){
    DBG_PRINT_TRACE( "\n\r" );
    g_status = EN_MOTOR_STANDBY;
    g_cnt = 0;
    g_usec = 800;
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

    pinMode( MOTOR_ST_DIR_OUT, OUTPUT );
    pinMode( MOTOR_ST_CK_OUT, OUTPUT );
    pinMode( MOTOR_ST_E_OUT, OUTPUT );

    return EN_TRUE;
}


/**************************************************************************//*!
 * @brief     ステッピング・モータを初期化する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
EHalBool_t
HalMotorST_Init(
    void  ///< [in] ナシ
){
    EHalBool_t          ret = EN_FALSE;
    struct sigaction    action;

    DBG_PRINT_TRACE( "\n\r" );

    InitParam();
    ret = InitReg();
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "Unable to initialize GPIO port. \n\r" );
        return ret;
    }

    memset( &action, 0, sizeof(action) );

    // set signal handler
    action.sa_handler = HalMotorST_IH_Set;
    action.sa_flags = SA_RESTART;
    sigemptyset( &action.sa_mask );

    if( sigaction( SIGALRM, &action, NULL ) < 0 )
    {
        perror( "sigaction error" );
        return ret;
    }

    ret = EN_TRUE;
    return ret;
}


/**************************************************************************//*!
 * @brief     ステッピング・モータを終了する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
HalMotorST_Fini(
    void  ///< [in] ナシ
){
    DBG_PRINT_TRACE( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     ▼▼▼▼割り込みハンドラ API▼▼▼▼
 *            [Interrupt Handler] ステッピング・モータをまわす
 * @attention 周期ハンドラから定期的に呼び出すこと。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
HalMotorST_IH_Set(
    int         signum  ///< [in] シグナル No
){
    static int  pulse = 0;
    static int  cnt = 0;

//    DBG_PRINT_TRACE( "\n\r" );    // 割り込みハンドラはログ出力しない

    switch( pulse )
    {
    case  0 : MOTOR_ST_CK_HIGH(); break;
    case  1 : MOTOR_ST_CK_LOW();  break;
    default : break;
    }

    pulse   = ( pulse + 1 ) % 2;
    cnt = ( cnt + 1 ) % PULSE_ANGLE_360x10;

    if( cnt > g_cnt )
    {
        pulse = 0;
        cnt = 0;
        HalMotorST_SetStatus( EN_MOTOR_STOP );
    }

    return;
}


/**************************************************************************//*!
 * @brief     インターバルタイマーを開始する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
static EHalBool_t
TimerStart(
    void  ///< [in] ナシ
){
    EHalBool_t          ret = EN_FALSE;
    struct itimerval    timer;

    timer.it_value.tv_sec     = 0;
    timer.it_value.tv_usec    = 1;
    timer.it_interval.tv_sec  = 0;
    timer.it_interval.tv_usec = g_usec;

    if( setitimer( ITIMER_REAL, &timer, NULL ) < 0 )
    {
        perror( "setitimer error" );
        return ret;
    }

    ret = EN_TRUE;
    return ret;
}


/**************************************************************************//*!
 * @brief     インターバルタイマーを停止する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
static EHalBool_t
TimerStop(
    void  ///< [in] ナシ
){
    EHalBool_t          ret = EN_FALSE;
    struct itimerval    timer;

    timer.it_value.tv_sec     = 0;
    timer.it_value.tv_usec    = 0;
    timer.it_interval.tv_sec  = 0;
    timer.it_interval.tv_usec = 0;

    if( setitimer( ITIMER_REAL, &timer, NULL ) < 0 )
    {
        perror( "setitimer error" );
        return ret;
    }

    ret = EN_TRUE;
    return ret;
}


/**************************************************************************//*!
 * @brief     ステッピング・モータの状態をセットする。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
HalMotorST_SetStatus(
    EHalMotorState_t    status  ///< [in] 新しいステータス
){
    DBG_PRINT_TRACE( "status = %d \n\r", status );

    if( g_status != status )
    {
        g_status = status;

        switch( status )
        {
        case EN_MOTOR_STANDBY :
        case EN_MOTOR_STOP :
        case EN_MOTOR_BRAKE :
            MOTOR_ST_E_LOW();   // 出力 Disable
        break;
        case EN_MOTOR_CW :
            MOTOR_ST_CW();      // CW 方向の回転設定
            MOTOR_ST_E_HIGH();  // 出力 Able
        break;
        case EN_MOTOR_CCW :
            MOTOR_ST_CCW();     // CCW 方向の回転設定
            MOTOR_ST_E_HIGH();  // 出力 Able
        break;
        default :
            ;   // 何もしない
        break;
        }

        if( status == EN_MOTOR_CCW ||
            status == EN_MOTOR_CW   )
        {
            TimerStart();
        } else
        {
            TimerStop();
        }
    }

    return;
}


/**************************************************************************//*!
 * @brief     ステッピング・モータの状態と回転角度をセットする。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
HalMotorST_SetAngle(
    EHalMotorState_t    status, ///< [in] 状態
    double              angle   ///< [in] 回転角度
){
    DBG_PRINT_TRACE( "status = %d \n\r", status );
    DBG_PRINT_TRACE( "angle  = %5.2f \n\r", angle );

    g_cnt = angle / ANGLE_MIN;

    HalMotorST_SetStatus( status );

    DBG_PRINT_TRACE( "g_cnt = %d \n\r", g_cnt );
    return;
}


/**************************************************************************//*!
 * @brief     ステッピング・モータの回転速度をセットする。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
HalMotorST_SetSpeed(
    unsigned int    time_us ///< [in] ステッピング・モータを 1 pulse 分 (= 0.45°) 回転させる時間
                            //        us 単位
                            //        200us ? 100,000 us 位の値がモータが回転できる妥当値
){
    DBG_PRINT_TRACE( "time_us = %d \n\r", time_us );
    if( time_us <= 0 )
    {
        g_usec = 1;
    } else
    {
        g_usec = time_us;
    }

    TimerStop();
    if( g_status == EN_MOTOR_CCW ||
        g_status == EN_MOTOR_CW   )
    {
        TimerStart();
    }
    return;
}


#ifdef __cplusplus
    }
#endif

