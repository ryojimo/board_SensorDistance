/**************************************************************************//*!
 *  @file           hal_pushsw.c
 *  @brief          [HAL] SW ドライバ API を定義したファイル。
 *  @author         Ryoji Morita
 *  @attention      none.
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2017.09.12
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
#define PUSH_SW0_IN     (16)
#define PUSH_SW1_IN     (20)
#define PUSH_SW2_IN     (21)


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

    pinMode( PUSH_SW0_IN, INPUT );
    pinMode( PUSH_SW1_IN, INPUT );
    pinMode( PUSH_SW2_IN, INPUT );

    return EN_TRUE;
}


/**************************************************************************//*!
 * @brief     SW を初期化する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
EHalBool_t
HalPushSw_Init(
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
 * @brief     SW を終了する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
HalPushSw_Fini(
    void  ///< [in] ナシ
){
    DBG_PRINT_TRACE( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     SW の入力を取得する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : SW が押されている, EN_FALSE : SW が押されていない
 *************************************************************************** */
EHalBool_t
HalPushSw_Get(
    EHalPushSw_t    which   ///< [in] ターゲット SW
){
    int             state = 0;
    EHalBool_t      ret = 0;

    DBG_PRINT_TRACE( "\n\r" );

    switch( which )
    {
    case EN_PUSH_SW_0 : state = digitalRead( PUSH_SW0_IN ); break;
    case EN_PUSH_SW_1 : state = digitalRead( PUSH_SW1_IN ); break;
    case EN_PUSH_SW_2 : state = digitalRead( PUSH_SW2_IN ); break;
    default           : break;
    }

    // SW は Active-Low 回路なので state が 0 の時が
    // SW が押されているということになる
    if( state == 0 )
    {
        usleep( 150 * 1000 );   // チャタリング防止
        ret = EN_TRUE;
    } else
    {
        ret = EN_FALSE;
    }

    return ret;
}


#ifdef __cplusplus
    }
#endif

