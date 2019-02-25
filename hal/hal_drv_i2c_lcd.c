/**************************************************************************//*!
 *  @file           hal_i2c_lcd.c
 *  @brief          [HAL] I2C LCD ドライバ API を定義したファイル。
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
// なし


//********************************************************
/* 関数プロトタイプ宣言                                  */
//********************************************************
static void         InitParam( void );
static EHalBool_t   InitReg( void );

static void         InitDevice( void );




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
 * @brief     デバイスを初期化する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
InitDevice(
    void  ///< [in] ナシ
){
    DBG_PRINT_TRACE( "\n\r" );

    usleep( 100 * 1000 );

    HalI2cLcd_Write( EN_LCD_CMD, 0x01 );    // Clear Display
    usleep( 20 * 1000 );
    HalI2cLcd_Write( EN_LCD_CMD, 0x02 );    // Return Home
    usleep( 20 * 1000 );
    HalI2cLcd_Write( EN_LCD_CMD, 0x0F );    // Send Display ON command
    usleep( 20 * 1000 );
    HalI2cLcd_Write( EN_LCD_CMD, 0x01 );    // Clear Display
    usleep( 20 * 1000 );

    return;
}


/**************************************************************************//*!
 * @brief     I2C LCD を初期化する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
EHalBool_t
HalI2cLcd_Init(
    void  ///< [in] ナシ
){
    EHalBool_t      ret = EN_FALSE;

    DBG_PRINT_TRACE( "\n\r" );

    // I2C スレーブデバイスを LCD に変える
    HalCmnI2c_SetSlave( I2C_SLAVE_LCD );

    InitParam();
    ret = InitReg();
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "Unable to initialize I2C port. \n\r" );
        return ret;
    } else
    {
        InitDevice();
    }

    ret = EN_TRUE;
    return ret;
}


/**************************************************************************//*!
 * @brief     I2C LCD を終了する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
HalI2cLcd_Fini(
    void  ///< [in] ナシ
){
    DBG_PRINT_TRACE( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     レジスタを指定して コマンドを書き込む。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
EHalBool_t
HalI2cLcd_Write(
    EHalLcdMode_t   rs,     ///< [in] Register Select
    unsigned char   code    ///< [in] LCD に書き込む 1 Byte
){
    EHalBool_t      ret = EN_FALSE;
    unsigned char   buff[2];

    DBG_PRINT_TRACE( "\n\r" );

    if(      rs == EN_LCD_CMD ){ buff[0] = 0x00; }
    else if( rs == EN_LCD_DAT ){ buff[0] = 0x40; }

    buff[1] = code;

    ret = HalCmnI2c_Write( buff, 2 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write data to i2c slave. \n\r" );
        return ret;
    }

    ret = EN_TRUE;
    return ret;
}


#ifdef __cplusplus
    }
#endif

