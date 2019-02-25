/**************************************************************************//*!
 *  @file           hal_cmn_spi_mcp3208.c
 *  @brief          [HAL] SPI AD コンバータ MCP3208 ドライバ API を定義したファイル。
 *  @author         Ryoji Morita
 *  @attention      none.
 *  @sa             none.
 *  @note           none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2016.06.23
 *************************************************************************** */
#ifdef __cplusplus
    extern "C"{
#endif


//********************************************************
/* include                                               */
//********************************************************
#include "hal_cmn.h"


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
// なし




/**************************************************************************//*!
 * @brief     MCP3208 の対象の ch の AD 値を読み出す
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    MCP3208 の AD 値
 *************************************************************************** */
unsigned int
HalCmnSpiMcp3208_Get(
    EHalSensorMcp3208_t which   ///< [in] 対象のセンサ
){
    unsigned char       send[3];
    unsigned char       recv[3];
    unsigned int        data = 0;

    DBG_PRINT_TRACE( "\n\r" );

    send[0] = ( which & 0x04 ) ? 0x07 : 0x06;
    send[1] = ( which & 0x03 ) << 6;
    send[2] = 0;

    HalCmnSpi_RecvN( send, recv, 3 );

    data = ((recv[1] & 0x0f) << 8) | recv[2];

    return data;
}


#ifdef __cplusplus
    }
#endif
