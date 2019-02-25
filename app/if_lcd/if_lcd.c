/**************************************************************************//*!
 *  @file           if_lcd.c
 *  @brief          [APP] LCD に文字を表示する。
 *  @author         Ryoji Morita
 *  @attention      none.
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2016.06.09
 *************************************************************************** */
#ifdef __cplusplus
    extern "C"{
#endif


//********************************************************
/* include                                               */
//********************************************************
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include "if_lcd.h"


//#define DBG_PRINT
#define MY_NAME "APP"
#include "../log/log.h"


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
 * @brief     エントリーモードの設定をする。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
AppIfLcd_Entry(
    int     curDir, ///< [in] カーソルの進む方向 [ インクリメント = 1, デクリメント = 0 ]
    int     sftDisp ///< [in] 表示のシフトON/OFF [ ON = 1, OFF = 0 ]
){
    int     cfg;

    DBG_PRINT_TRACE( "\n\r" );

    // I2C スレーブデバイスを LCD に変える
    HalCmnI2c_SetSlave( I2C_SLAVE_LCD );

    cfg = 0x04;
    if( curDir  ){ cfg |= 0x02; }
    if( sftDisp ){ cfg |= 0x01; }
    HalI2cLcd_Write( EN_LCD_CMD, cfg );

    return;
}


/**************************************************************************//*!
 * @brief     表示のオン/オフコントロールを設定する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
AppIfLcd_Ctrl(
    int     disp,   ///< [in] 表示のON/OFF [ ON = 1, OFF = 0 ]
    int     curDisp,///< [in] カーソル表示のON/OFF [ ON = 1, OFF = 0 ]
    int     blkDisp ///< [in] カーソルブリンクのON/OFF [ ON = 1, OFF = 0 ]
){
    int cfg;

    DBG_PRINT_TRACE( "\n\r" );

    // I2C スレーブデバイスを LCD に変える
    HalCmnI2c_SetSlave( I2C_SLAVE_LCD );

    cfg = 0x08;
    if( disp     ){ cfg |= 0x04; }
    if( curDisp  ){ cfg |= 0x02; }
    if( blkDisp  ){ cfg |= 0x01; }
    HalI2cLcd_Write( EN_LCD_CMD, cfg );

    return;
}


/**************************************************************************//*!
 * @brief     表示 or カーソルのシフトを設定する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
AppIfLcd_Shift(
    int     tgt,    ///< [in] 表示 or カーソルをシフト [ 表示=1, カーソル=0 ]
    int     dir     ///< [in] シフトする方向 [ 右シフト=1, 左シフト=0 ]
){
    int cfg;

    DBG_PRINT_TRACE( "\n\r" );

    // I2C スレーブデバイスを LCD に変える
    HalCmnI2c_SetSlave( I2C_SLAVE_LCD );

    cfg = 0x10;
    if( tgt ){ cfg |= 0x80; }
    if( dir ){ cfg |= 0x40; }
    HalI2cLcd_Write( EN_LCD_CMD, cfg );

    return;
}


/**************************************************************************//*!
 * @brief     カーソルをホームへ移動させる。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
AppIfLcd_CursorHome(
    void
){
    DBG_PRINT_TRACE( "\n\r" );

    // I2C スレーブデバイスを LCD に変える
    HalCmnI2c_SetSlave( I2C_SLAVE_LCD );

    HalI2cLcd_Write( EN_LCD_CMD, 0x02 );
    return;
}


/**************************************************************************//*!
 * @brief     カーソルを指定した (x, y) へ移動させる。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
AppIfLcd_CursorSet(
    int     x,  ///< [in] x 座標
    int     y   ///< [in] y 座標
){
    DBG_PRINT_TRACE( "\n\r" );

    // I2C スレーブデバイスを LCD に変える
    HalCmnI2c_SetSlave( I2C_SLAVE_LCD );

    HalI2cLcd_Write( EN_LCD_CMD, ( x + (y << 5) ) | 0x80 ); // (y << 5) == (y * 0x20)
    return;
}


/**************************************************************************//*!
 * @brief     表示をクリアし、カーソルをホーム ( x, y ) = ( 0, 0 ) へ移動させる。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
AppIfLcd_Clear(
    void
){
    DBG_PRINT_TRACE( "\n\r" );

    // I2C スレーブデバイスを LCD に変える
    HalCmnI2c_SetSlave( I2C_SLAVE_LCD );

    HalI2cLcd_Write( EN_LCD_CMD, 0x01 );
    AppIfLcd_CursorHome();
    return;
}


/**************************************************************************//*!
 * @brief     LCD に 1 文字を表示する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    成功時 = 出力した 1 文字 , 失敗時 = EOF
 *************************************************************************** */
int
AppIfLcd_Putc(
    int     c   ///< [in] 表示する 1 文字
){
    int         ret = EOF;
    EHalBool_t  res = EN_FALSE;

    DBG_PRINT_TRACE( "\n\r" );

    // I2C スレーブデバイスを LCD に変える
    HalCmnI2c_SetSlave( I2C_SLAVE_LCD );

    res = HalI2cLcd_Write( EN_LCD_DAT, c );

    if( res == EN_FALSE )
    {
        ret = EOF;
    } else
    {
        ret = c;
    }

    return ret;
}


/**************************************************************************//*!
 * @brief     LCD に文字列を表示する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    成功時 = 出力した文字数 , 失敗時 = EOF
 *************************************************************************** */
int
AppIfLcd_Puts(
    const char*     str     ///< [in] 表示する文字列
){
    int             ret = EOF;
    EHalBool_t      res = EN_FALSE;
    int             cnt = 0;

    DBG_PRINT_TRACE( "\n\r" );

    // I2C スレーブデバイスを LCD に変える
    HalCmnI2c_SetSlave( I2C_SLAVE_LCD );

    while( *str != '\0' )
    {
        res = HalI2cLcd_Write( EN_LCD_DAT, *str++ );

        if( res == EN_FALSE )
        {
            ret = EOF;
            break;
        } else
        {
            cnt++;
        }
    }

    if( cnt > 0 )
    {
        ret = cnt;
    } else
    {
        ret = EOF;
    }

    return ret;
}


/**************************************************************************//*!
 * @brief     LCD に文字列を printf() 関数フォーマットのように表示する。
 * @attention 浮動小数点関係は、未実装。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    成功時 = 出力した文字数 , 失敗時 = -1
 *************************************************************************** */
int
AppIfLcd_Printf(
    const char*     format,     ///< [in] 出力する文字列
    ...                         ///< [in] 可変個数引数
){
    int             len;
    char            buff[32];
    va_list         ap;

    DBG_PRINT_TRACE( "\n\r" );

    len = strlen( format );
    if( len > 64 )
    {
        DBG_PRINT_ERROR( "length of format is over 24. \n\r" );
        return -1;
    }

    memset( buff, '\0', sizeof(buff) );
    va_start( ap, format );
    vsprintf( buff, format, ap );
    va_end( ap );

    AppIfLcd_Puts( buff );

    len = strlen( buff );
    return len;
}


#ifdef __cplusplus
    }
#endif

