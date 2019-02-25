/**************************************************************************//*!
 *  @file           hal_led.c
 *  @brief          [HAL] LED ドライバ API を定義したファイル。
 *  @author         Ryoji Morita
 *  @attention      none.
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2016.06.05
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
#define LED0_OUT    (14)
#define LED1_OUT    (15)
#define LED2_OUT    (23)
#define LED3_OUT    (24)


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

    pinMode( LED0_OUT, OUTPUT );
    pinMode( LED1_OUT, OUTPUT );
    pinMode( LED2_OUT, OUTPUT );
    pinMode( LED3_OUT, OUTPUT );

    return EN_TRUE;
}


/**************************************************************************//*!
 * @brief     LED を初期化する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
EHalBool_t
HalLed_Init(
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
 * @brief     LED を終了する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
HalLed_Fini(
    void  ///< [in] ナシ
){
    DBG_PRINT_TRACE( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     LED を点灯する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
HalLed_Set(
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

    flg = ( value & 0x04 ) >> 2;
    if( flg == 1 ){ digitalWrite( LED2_OUT, EN_HIGH ); }
    else          { digitalWrite( LED2_OUT, EN_LOW  ); }

    flg = ( value & 0x08 ) >> 3;
    if( flg == 1 ){ digitalWrite( LED3_OUT, EN_HIGH ); }
    else          { digitalWrite( LED3_OUT, EN_LOW  ); }

    return;
}


#ifdef __cplusplus
    }
#endif

