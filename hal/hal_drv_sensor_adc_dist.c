/**************************************************************************//*!
 *  @file           hal_sensor_adc_dist.c
 *  @brief          [HAL] SENSOR (ADC) 距離センサ・ドライバ API を定義したファイル。
 *  @author         Ryoji Morita
 *  @attention      none.
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2019.02.25
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
#define LED0_OUT    (19)
#define LED1_OUT    (26)


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
static SHalSensor_t     g_dataFL;     // センサの値 : Front Left
static SHalSensor_t     g_dataFR;     // センサの値 : Front Right
static SHalSensor_t     g_dataFSL;    // センサの値 : Front Side Left
static SHalSensor_t     g_dataFSR;    // センサの値 : Front Side Right


//********************************************************
/* 関数プロトタイプ宣言                                  */
//********************************************************
static void         InitParam( void );
static EHalBool_t   InitReg( void );
static void         SetOffset( void );
static void         Led_Set( unsigned char value );




/**************************************************************************//*!
 * @brief     変数を初期化する。
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

    g_dataFL.cur = 0;                 // cur = センサの現在値 ( MCP3208 の AD 値 )
    g_dataFL.ofs = 0;                 // ofs = 初期化時に設定したセンサのオフセット値
    g_dataFL.max = MCP3208_MAX_VALE;  // max = センサの最大値
    g_dataFL.min = MCP3208_MAX_VALE;  // min = センサの最小値
    g_dataFL.err = 0;                 // err = cur - ofs
    g_dataFL.cur_rate = 0;            // cur_rate = ( cur / max ) * 100 ( %  )
    g_dataFL.cur_vol = 0;             // cur_vol = 電圧に換算した現在値 ( mV )

    g_dataFR.cur = 0;                 // cur = センサの現在値 ( MCP3208 の AD 値 )
    g_dataFR.ofs = 0;                 // ofs = 初期化時に設定したセンサのオフセット値
    g_dataFR.max = MCP3208_MAX_VALE;  // max = センサの最大値
    g_dataFR.min = MCP3208_MAX_VALE;  // min = センサの最小値
    g_dataFR.err = 0;                 // err = cur - ofs
    g_dataFR.cur_rate = 0;            // cur_rate = ( cur / max ) * 100 ( %  )
    g_dataFR.cur_vol = 0;             // cur_vol = 電圧に換算した現在値 ( mV )

    g_dataFSL.cur = 0;                 // cur = センサの現在値 ( MCP3208 の AD 値 )
    g_dataFSL.ofs = 0;                 // ofs = 初期化時に設定したセンサのオフセット値
    g_dataFSL.max = MCP3208_MAX_VALE;  // max = センサの最大値
    g_dataFSL.min = MCP3208_MAX_VALE;  // min = センサの最小値
    g_dataFSL.err = 0;                 // err = cur - ofs
    g_dataFSL.cur_rate = 0;            // cur_rate = ( cur / max ) * 100 ( %  )
    g_dataFSL.cur_vol = 0;             // cur_vol = 電圧に換算した現在値 ( mV )

    g_dataFSR.cur = 0;                 // cur = センサの現在値 ( MCP3208 の AD 値 )
    g_dataFSR.ofs = 0;                 // ofs = 初期化時に設定したセンサのオフセット値
    g_dataFSR.max = MCP3208_MAX_VALE;  // max = センサの最大値
    g_dataFSR.min = MCP3208_MAX_VALE;  // min = センサの最小値
    g_dataFSR.err = 0;                 // err = cur - ofs
    g_dataFSR.cur_rate = 0;            // cur_rate = ( cur / max ) * 100 ( %  )
    g_dataFSR.cur_vol = 0;             // cur_vol = 電圧に換算した現在値 ( mV )

    return;
}


/**************************************************************************//*!
 * @brief     H/W レジスタを初期化する。
 * @attention シングルモードで動作。
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

    pinMode( LED0_OUT, OUTPUT );
    pinMode( LED1_OUT, OUTPUT );

    return EN_TRUE;
}


/**************************************************************************//*!
 * @brief     センサのオフセット値をセットする。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
SetOffset(
    void  ///< [in] ナシ
){
    SHalSensor_t*   data;

    DBG_PRINT_TRACE( "\n\r" );

    data = HalSensorDist_GetFL();
    g_dataFL.ofs = data->cur;

    data = HalSensorDist_GetFR();
    g_dataFR.ofs = data->cur;

    data = HalSensorDist_GetFSL();
    g_dataFSL.ofs = data->cur;

    data = HalSensorDist_GetFSR();
    g_dataFSR.ofs = data->cur;

    return;
}


/**************************************************************************//*!
 * @brief     初期化処理
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 初期化成功, EN_FALSE : 初期化失敗
 *************************************************************************** */
EHalBool_t
HalSensorDist_Init(
    void  ///< [in] ナシ
){
    EHalBool_t      ret = EN_FALSE;

    DBG_PRINT_TRACE( "\n\r" );

    InitParam();
    ret = InitReg();
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "Unable to initialize spi port. \n\r" );
        return ret;
    }

    SetOffset();
    ret = EN_TRUE;
    return ret;
}


/**************************************************************************//*!
 * @brief     終了処理
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
HalSensorDist_Fini(
    void  ///< [in] ナシ
){
    DBG_PRINT_TRACE( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     点灯する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
Led_Set(
    unsigned char   value   ///< [in] 点灯する値
){
    unsigned char   flg = 0;

    DBG_PRINT_TRACE( "\n\r" );

    flg = ( value & 0x01 ) >> 0;
    if( flg == 1 ){ digitalWrite( LED0_OUT, EN_HIGH ); }
    else          { digitalWrite( LED0_OUT, EN_LOW  ); }

    flg = ( value & 0x02 ) >> 1;
    if( flg == 1 ){ digitalWrite( LED1_OUT, EN_HIGH ); }
    else          { digitalWrite( LED1_OUT, EN_LOW  ); }

    return;
}


/**************************************************************************//*!
 * @brief     センサ変数のアドレスを返す。
 * @attention なし。
 * @note      Front Left
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    センサ変数のアドレス
 *************************************************************************** */
SHalSensor_t*
HalSensorDist_GetFL(
    void  ///< [in] ナシ
){
    unsigned int    data = 0;

    DBG_PRINT_TRACE( "\n\r" );
    Led_Set( 0x03 );
    data = HalCmnSpiMcp3208_Get( EN_MCP3208_CH_0 );
    HalCmn_UpdateSenData( &g_dataFL, (double)data );
    Led_Set( 0x00 );
    return &g_dataFL;
}


SHalSensor_t*
HalSensorDist_GetFR(
    void  ///< [in] ナシ
){
    unsigned int    data = 0;

    DBG_PRINT_TRACE( "\n\r" );
    Led_Set( 0x03 );
    data = HalCmnSpiMcp3208_Get( EN_MCP3208_CH_1 );
    HalCmn_UpdateSenData( &g_dataFR, (double)data );
    Led_Set( 0x00 );
    return &g_dataFR;
}

SHalSensor_t*
HalSensorDist_GetFSL(
    void  ///< [in] ナシ
){
    unsigned int    data = 0;

    DBG_PRINT_TRACE( "\n\r" );
    Led_Set( 0x03 );
    data = HalCmnSpiMcp3208_Get( EN_MCP3208_CH_2 );
    HalCmn_UpdateSenData( &g_dataFSL, (double)data );
    Led_Set( 0x00 );
    return &g_dataFSL;
}


SHalSensor_t*
HalSensorDist_GetFSR(
    void  ///< [in] ナシ
){
    unsigned int    data = 0;

    DBG_PRINT_TRACE( "\n\r" );
    Led_Set( 0x03 );
    data = HalCmnSpiMcp3208_Get( EN_MCP3208_CH_3 );
    HalCmn_UpdateSenData( &g_dataFSR, (double)data );
    Led_Set( 0x00 );
    return &g_dataFSR;
}


#ifdef __cplusplus
    }
#endif

