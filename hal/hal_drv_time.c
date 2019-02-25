/**************************************************************************//*!
 *  @file           hal_time.c
 *  @brief          [HAL] 時間・ドライバ API を定義したファイル。
 *  @author         Ryoji Morita
 *  @attention      none.
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2016.06.27
 *************************************************************************** */
#ifdef __cplusplus
    extern "C"{
#endif


//********************************************************
/* include                                               */
//********************************************************
#include <time.h>

#include "hal_cmn.h"
#include "hal.h"


//#define DBG_PRINT
#define MY_NAME "HAL"
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
static SHalTime_t   g_data;


//********************************************************
/* 関数プロトタイプ宣言                                  */
//********************************************************
static void         InitParam( void );
static EHalBool_t   InitReg( void );




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

    g_data.wait  = 0;
    g_data.usec  = 0;
    g_data.msec  = 0;
    g_data.sec   = 0;
    g_data.min   = 0;
    g_data.hour  = 0;
    g_data.day   = 0;
    g_data.month = 0;
    g_data.year  = 0;
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
    return EN_TRUE;
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
HalTime_Init(
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

    ret = EN_TRUE;
    return ret;
}


/**************************************************************************//*!
 * @brief     時間変数のアドレスを返す。( 地方時 ( Local time ) )
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    時間変数のアドレス
 *************************************************************************** */
SHalTime_t*
HalTime_GetLocaltime(
    void  ///< [in] ナシ
){
    time_t      timer;
    struct tm*  local;

    DBG_PRINT_TRACE( "\n\r" );

    // 現在時刻を取得
    timer = time( NULL );

     // 地方時に変換
    local = localtime( &timer );

    g_data.wait  = 0;
    g_data.usec  = 0;
    g_data.msec  = 0;
    g_data.sec   = local->tm_sec;
    g_data.min   = local->tm_min;
    g_data.hour  = local->tm_hour;
    g_data.day   = local->tm_mday;
    g_data.month = local->tm_mon + 1;
    g_data.year  = local->tm_year + 1900;

#if 0
    // 表示
    AppIfPc_Printf( "local time : " );
    AppIfPc_Printf( "%04d/", g_data.year  );
    AppIfPc_Printf( "%02d/", g_data.month );
    AppIfPc_Printf( "%02d ", g_data.day   );
    AppIfPc_Printf( "%02d:", g_data.hour  );
    AppIfPc_Printf( "%02d:", g_data.min   );
    AppIfPc_Printf( "%02d" , g_data.sec   );
    AppIfPc_Printf( "\n" );
#endif

    return &g_data;
}


/**************************************************************************//*!
 * @brief     時間変数のアドレスを返す。( 協定世界時 ( UTC ) )
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    時間変数のアドレス
 *************************************************************************** */
SHalTime_t*
HalTime_GetUTC(
    void  ///< [in] ナシ
){
    time_t      timer;
    struct tm*  utc;

    DBG_PRINT_TRACE( "\n\r" );

    // 現在時刻を取得
    timer = time( NULL );

     // UTC に変換
    utc = gmtime( &timer );

    g_data.wait  = 0;
    g_data.usec  = 0;
    g_data.msec  = 0;
    g_data.sec   = utc->tm_sec;
    g_data.min   = utc->tm_min;
    g_data.hour  = utc->tm_hour;
    g_data.day   = utc->tm_mday;
    g_data.month = utc->tm_mon + 1;
    g_data.year  = utc->tm_year + 1900;

#if 0
    // 表示
    AppIfPc_Printf( "UTC : " );
    AppIfPc_Printf( "%04d/", g_data.year  );
    AppIfPc_Printf( "%02d/", g_data.month );
    AppIfPc_Printf( "%02d ", g_data.day   );
    AppIfPc_Printf( "%02d:", g_data.hour  );
    AppIfPc_Printf( "%02d:", g_data.min   );
    AppIfPc_Printf( "%02d" , g_data.sec   );
    AppIfPc_Printf( "\n" );
#endif

    return &g_data;
}


#ifdef __cplusplus
    }
#endif

