/**************************************************************************//*!
 *  @file           hal_cmn_i2c.c
 *  @brief          [HAL] I2C の共通 API を定義したファイル。
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
#include <fcntl.h>
#include <sys/mman.h>

#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

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
typedef struct {
    int                 fd; // "/dev/i2c-*" のファイルデスクリプタ
} SHalCmnI2c_t;


//********************************************************
/* モジュールグローバル変数                              */
//********************************************************
static SHalCmnI2c_t     g_param;


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

    g_param.fd = -1;
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
    EHalBool_t      ret = EN_FALSE;

    DBG_PRINT_TRACE( "\n\r" );

    g_param.fd = open( "/dev/i2c-1", O_RDWR );
    if( g_param.fd < 0 )
    {
        DBG_PRINT_ERROR( "Failed to open /dev/i2c-1, try change permission. \n\r" );
        return ret;
    }

    ret = EN_TRUE;
    return ret;
}


/**************************************************************************//*!
 * @brief     I2C デバイスをオープンする。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
EHalBool_t
HalCmnI2c_Init(
    void
){
    EHalBool_t      ret = EN_FALSE;

    DBG_PRINT_TRACE( "\n\r" );

    InitParam();
    ret = InitReg();

    return ret;
}


/**************************************************************************//*!
 * @brief     I2C デバイスをクローズする。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
HalCmnI2c_Fini(
    void
){
    DBG_PRINT_TRACE( "\n\r" );

    close( g_param.fd );
    return;
}


/**************************************************************************//*!
 * @brief     I2C スレーブデバイスのアドレスをセットする。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
EHalBool_t
HalCmnI2c_SetSlave(
    unsigned char   address   ///< [in] スレーブデバイスのアドレス
){
    EHalBool_t      ret = EN_FALSE;
    int             res;

    DBG_PRINT_TRACE( "\n\r" );

    res = ioctl( g_param.fd, I2C_SLAVE, address );
    if( res < 0) {
        DBG_PRINT_WARN( "Unable to get bus access to talk to i2c slave. \n\r" );
        close( g_param.fd );
        return ret;
    }

    ret = EN_TRUE;
    return ret;
}


/**************************************************************************//*!
 * @brief     I2C スレーブデバイスに値を書き込む。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
EHalBool_t
HalCmnI2c_Write(
    unsigned char*  data,   ///< [in] スレーブデバイスへ送るデータ
    unsigned int    size    ///< [in] 送るデータサイズ
){
    EHalBool_t      ret = EN_FALSE;
    int             res = -1;

    DBG_PRINT_TRACE( "\n\r" );

    res = write( g_param.fd, data, size );
    if( res != size )
    {
        DBG_PRINT_WARN( "fail to write data to i2c slave. \n\r" );
        return ret;
    }

    ret = EN_TRUE;
    return ret;
}


/**************************************************************************//*!
 * @brief     I2C スレーブデバイスから値を読み出す。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
EHalBool_t
HalCmnI2c_Read(
    unsigned char*  data,   ///< [out] スレーブデバイスからのデータを格納するバッファ
    unsigned int    size    ///< [in]  受け取るデータサイズ
){
    EHalBool_t      ret = EN_FALSE;
    int             res = -1;

    DBG_PRINT_TRACE( "\n\r" );

    res = read( g_param.fd, data, size );
    if( res != size )
    {
        DBG_PRINT_WARN( "fail to read data from i2c slave. \n\r" );
        return ret;
    }

    ret = EN_TRUE;
    return ret;
}


#ifdef __cplusplus
    }
#endif

