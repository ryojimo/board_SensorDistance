/**************************************************************************//*!
 *  @file           hal_cmn_spi.c
 *  @brief          [HAL] SPI の共通 API を定義したファイル。
 *  @author         Ryoji Morita
 *  @attention      none.
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2016.06.14
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
#include <linux/spi/spidev.h>

#include "hal_cmn.h"


//#define DBG_PRINT
#define MY_NAME "HAL"
#include "../app/log/log.h"


//********************************************************
/*! @def                                                 */
//********************************************************
#define SPI_SPEED       (8000000)   //  クロック 8MHz ( デフォルト )
#define SPI_BITS        (8)         //  ビット数 ( 8bit のみ可能 )
#define SPI_DELAY       (0)
#define SPI_BLOCKSIZE   (2048)      //  ブロック転送サイズ


//********************************************************
/*! @enum                                                */
//********************************************************
// なし


//********************************************************
/*! @struct                                              */
//********************************************************
typedef struct {
    int                     fd; // "/dev/spidev0.*" のファイルデスクリプタ
    struct spi_ioc_transfer tr;
} SHalCmnSpi_t;


//********************************************************
/* モジュールグローバル変数                              */
//********************************************************
static SHalCmnSpi_t     g_param;


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

    g_param.tr.tx_buf        = (unsigned int)0;
    g_param.tr.rx_buf        = (unsigned int)NULL;
    g_param.tr.len           = 1;
    g_param.tr.speed_hz      = SPI_SPEED;
    g_param.tr.delay_usecs   = SPI_DELAY;
    g_param.tr.bits_per_word = SPI_BITS;
    g_param.tr.cs_change     = 0;
    return;
}


/**************************************************************************//*!
 * @brief     H/W レジスタを初期化する。
 * @attention なし。
 * @note      SPI_MODE_0 : CE 端子が通常 L で動作時に H 出力。SCLK の立ち上がり ( LOW  -> HIGH ) のタイミングで信号線から 1 ビットのデータを受信・送信する
 *            SPI_MODE_1 : CE 端子が通常 L で動作時に H 出力。SCLK の立ち下がり ( HIGH -> LOW  ) のタイミングで信号線から 1 ビットのデータを受信・送信する
 *            SPI_MODE_2 : CE 端子が通常 H で動作時に L 出力。SCLK の立ち下がり ( HIGH -> LOW  ) のタイミングで信号線から 1 ビットのデータを受信・送信する
 *            SPI_MODE_3 : CE 端子が通常 H で動作時に L 出力。SCLK の立ち上がり ( LOW  -> HIGH ) のタイミングで信号線から 1 ビットのデータを受信・送信する
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
static EHalBool_t
InitReg(
    void  ///< [in] ナシ
){
    EHalBool_t      ret = EN_FALSE;

    int res = -1;
    int speed = SPI_SPEED;
    int bits = SPI_BITS;
    int mode = SPI_MODE_0;

    DBG_PRINT_TRACE( "\n\r" );

    g_param.fd = open( "/dev/spidev0.0", O_RDWR );
    if( g_param.fd < 0 )
    {
        DBG_PRINT_ERROR( "Failed to open /dev/spidev0.0, try change permission. \n\r" );
        return ret;
    }

    res = ioctl( g_param.fd, SPI_IOC_WR_MODE, &mode );
    if( res < 0 )
    {
        DBG_PRINT_ERROR( "Failed to setup SPI_IOC_WR_MODE. \n\r" );
        close( g_param.fd );
        return ret;
    }

    res = ioctl( g_param.fd, SPI_IOC_RD_MODE, &mode );
    if( res < 0 )
    {
        DBG_PRINT_ERROR( "Failed to setup SPI_IOC_RD_MODE. \n\r" );
        close( g_param.fd );
        return ret;
    }

    res = ioctl( g_param.fd, SPI_IOC_WR_BITS_PER_WORD, &bits );
    if( res < 0 )
    {
        DBG_PRINT_ERROR( "Failed to setup SPI_IOC_WR_BITS_PER_WORD. \n\r" );
        close( g_param.fd );
        return ret;
    }

    res = ioctl( g_param.fd, SPI_IOC_RD_BITS_PER_WORD, &bits );
    if( res < 0 )
    {
        DBG_PRINT_ERROR( "Failed to setup SPI_IOC_RD_BITS_PER_WORD. \n\r" );
        close( g_param.fd );
        return ret;
    }

    res = ioctl( g_param.fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed );
    if( res < 0 )
    {
        DBG_PRINT_ERROR( "Failed to setup SPI_IOC_WR_MAX_SPEED_HZ. \n\r" );
        close( g_param.fd );
        return ret;
    }

    res = ioctl( g_param.fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed );
    if( res < 0 )
    {
        DBG_PRINT_ERROR( "Failed to setup SPI_IOC_RD_MAX_SPEED_HZ. \n\r" );
        close( g_param.fd );
        return ret;
    }

    ret = EN_TRUE;
    return ret;
}


/**************************************************************************//*!
 * @brief     SPI デバイスをオープンする。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
EHalBool_t
HalCmnSpi_Init(
    void
){
    EHalBool_t      ret = EN_FALSE;

    DBG_PRINT_TRACE( "\n\r" );

    InitParam();
    ret = InitReg();

    return ret;
}


/**************************************************************************//*!
 * @brief     SPI デバイスをクローズする。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
void
HalCmnSpi_Fini(
    void
){
    DBG_PRINT_TRACE( "\n\r" );

    close( g_param.fd );
    return;
}


/**************************************************************************//*!
 * @brief     SPI スレーブデバイスに 1 Byte データを単発送信する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
EHalBool_t
HalCmnSpi_Send(
    unsigned char   data    ///< [in] スレーブデバイスへ送るデータ
){
    EHalBool_t      ret = EN_FALSE;
    int             res = -1;

    DBG_PRINT_TRACE( "\n\r" );

    g_param.tr.tx_buf = (unsigned int)data;
    g_param.tr.rx_buf = (unsigned int)NULL;
    g_param.tr.len    = 1;

    res = ioctl( g_param.fd, SPI_IOC_MESSAGE(1), &g_param.tr );
    if( res < 0 )
    {
        DBG_PRINT_ERROR( "error: cannot send spi message. \n\r" );
        return ret;
    }

    ret = EN_TRUE;
    return ret;
}


/**************************************************************************//*!
 * @brief     SPI スレーブデバイスに N Byte データを送信する。 ( N <= SPI_BUFFERSIZE )
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
EHalBool_t
HalCmnSpi_SendN(
    unsigned char*  data,   ///< [in] スレーブデバイスへ送るデータ
    int             size    ///< [in] 送信する Byte 数 ( n <= SPI_BUFFERSIZE )
){
    EHalBool_t      ret = EN_FALSE;
    int             res = -1;

    DBG_PRINT_TRACE( "\n\r" );

    g_param.tr.tx_buf = (unsigned int)data;
    g_param.tr.rx_buf = (unsigned int)NULL;
    g_param.tr.len    = size;

    res = ioctl( g_param.fd, SPI_IOC_MESSAGE(1), &g_param.tr );
    if( res < 0 )
    {
        DBG_PRINT_ERROR( "error: cannot send spi message. \n\r" );
        return ret;
    }

    ret = EN_TRUE;
    return ret;
}


/**************************************************************************//*!
 * @brief     SPI スレーブデバイスに SPI_BUFFERSIZE Byte 以上のデータを送信する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
EHalBool_t
HalCmnSpi_SendBuffer(
    unsigned char*  data,   ///< [in] スレーブデバイスへ送るデータ
    int             size    ///< [in] 送信する Byte 数 ( n <= SPI_BUFFERSIZE )
){
    EHalBool_t      ret = EN_FALSE;
    int             i = 0;
    int             numBlock = 0;
    int             lastBlock = 0;

    DBG_PRINT_TRACE( "\n\r" );

    // ブロック数の計算
    numBlock  = size / SPI_BLOCKSIZE;
    lastBlock = size % SPI_BLOCKSIZE;

    g_param.tr.tx_buf = (unsigned int)data;
    g_param.tr.rx_buf = (unsigned int)NULL;
    g_param.tr.len    = size;

    for( i = 0; i < numBlock; i++ )
    {
        ret = HalCmnSpi_SendN( data, SPI_BLOCKSIZE );
        if( ret == EN_FALSE )
        {
            DBG_PRINT_ERROR( "error: cannot send spi message. \n\r" );
            goto err;
        }
        data += SPI_BLOCKSIZE;
    }

    //  残りを送信
    if( lastBlock )
    {
        ret = HalCmnSpi_SendN( data, lastBlock );
        if( ret == EN_FALSE )
        {
            DBG_PRINT_ERROR( "error: cannot send spi message. \n\r" );
            goto err;
        }
    }

    ret = EN_TRUE;
err:
    return ret;
}


/**************************************************************************//*!
 * @brief     SPI スレーブデバイスから値を読み出す。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
EHalBool_t
HalCmnSpi_RecvN(
    unsigned char*  send,   ///< [in]  スレーブデバイスへ送るデータ
    unsigned char*  recv,   ///< [out] スレーブデバイスからのデータを格納するバッファ
    unsigned int    size    ///< [in]  受け取るデータサイズ
){
    EHalBool_t      ret = EN_FALSE;
    int             res = -1;

    DBG_PRINT_TRACE( "\n\r" );

    g_param.tr.tx_buf = (unsigned int)send;
    g_param.tr.rx_buf = (unsigned int)recv;
    g_param.tr.len    = size;

    res = ioctl( g_param.fd, SPI_IOC_MESSAGE(1), &g_param.tr );
    if( res < 0 )
    {
        DBG_PRINT_ERROR( "error: cannot send spi message. \n\r" );
        return ret;
    }

    ret = EN_TRUE;
    return ret;
}


#ifdef __cplusplus
    }
#endif

