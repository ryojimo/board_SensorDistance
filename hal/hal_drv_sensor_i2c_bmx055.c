/**************************************************************************//*!
 *  @file           hal_sensor_i2c_bmx055_acc.c
 *  @brief          [HAL] SENSOR (I2C) BMX055 ドライバ API を定義したファイル。
 *  @author         Ryoji Morita
 *  @attention      none.
 *  @sa             none.
 *  @note           https://github.com/ControlEverythingCommunity/BMX055/blob/master/C/BMX055.c
 *                  https://qiita.com/john256/items/b0ad7a4222ad1583b124
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2010.02.15
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
static SHalSensor_t     g_dataAcc[3];   // センサの値 ( 加速度センサ   : 0:X-axis, 1:Y-axis, 2:Z-axis )
static SHalSensor_t     g_dataGyro[3];  // センサの値 ( ジャイロセンサ : 0:X-axis, 1:Y-axis, 2:Z-axis )
static SHalSensor_t     g_dataMag[3];   // センサの値 ( 磁気センサ     : 0:X-axis, 1:Y-axis, 2:Z-axis )


//********************************************************
/* 関数プロトタイプ宣言                                  */
//********************************************************
static void         InitParam( void );
static EHalBool_t   InitReg( void );

static void         InitDevice( void );

static EHalBool_t   SetConfigAcc( void );
static EHalBool_t   SetConfigGyro( void );
static EHalBool_t   SetConfigMag( void );

static void         SetOffsetAcc( void );




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
    int   i = 0;
    DBG_PRINT_TRACE( "\n\r" );

    for( i = 0; i < 3; i++ )
    {
        g_dataAcc[i].cur = 0;           // cur = センサの現在値 ( N? )
        g_dataAcc[i].ofs = 0;           // ofs = 初期化時に設定したセンサのオフセット値
        g_dataAcc[i].max = 0;           // max = センサの最大値
        g_dataAcc[i].min = 0xFFFFFFFF;  // min = センサの最小値
        g_dataAcc[i].err = 0;           // err = cur - ofs
        g_dataAcc[i].cur_rate = 0;      // cur_rate = ( cur / max ) * 100 ( %  )
        g_dataAcc[i].cur_vol = 0;       // cur_vol = 電圧に換算した現在値 ( mV )

        g_dataGyro[i].cur = 0;
        g_dataGyro[i].ofs = 0;
        g_dataGyro[i].max = 0;
        g_dataGyro[i].min = 0xFFFFFFFF;
        g_dataGyro[i].err = 0;
        g_dataGyro[i].cur_rate = 0;
        g_dataGyro[i].cur_vol = 0;

        g_dataMag[i].cur = 0;
        g_dataMag[i].ofs = 0;
        g_dataMag[i].max = 0;
        g_dataMag[i].min = 0xFFFFFFFF;
        g_dataMag[i].err = 0;
        g_dataMag[i].cur_rate = 0;
        g_dataMag[i].cur_vol = 0;
    }
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

    SetConfigAcc();
    usleep( 100 * 1000 );
    SetConfigGyro();
    usleep( 100 * 1000 );
    SetConfigMag();
    usleep( 300 * 1000 );
    return;
}


/**************************************************************************//*!
 * @brief     I2C SENSOR BMX055 ACC を初期化する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
EHalBool_t
HalSensorBmx055_Init(
    void  ///< [in] ナシ
){
    EHalBool_t      ret = EN_FALSE;

    DBG_PRINT_TRACE( "\n\r" );

    InitParam();
    ret = InitReg();
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "Unable to initialize I2C port. \n\r" );
        return ret;
    } else
    {
        InitDevice();
        SetOffsetAcc();
    }

    ret = EN_TRUE;
    return ret;
}


/**************************************************************************//*!
 * @brief     I2C SENSOR BMX055 ACC を終了する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
void
HalSensorBmx055_Fini(
    void  ///< [in] ナシ
){
    DBG_PRINT_TRACE( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     BMX055 加速度センサを設定する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
static EHalBool_t
SetConfigAcc(
    void  ///< [in] ナシ
){
    EHalBool_t      ret = EN_FALSE;
    unsigned char   buff[2];

    DBG_PRINT_TRACE( "\n\r" );

    // I2C スレーブデバイスを BMX055 ACC に変える
    HalCmnI2c_SetSlave( I2C_SLAVE_BMX055_ACC );

    buff[0] = 0x0F;                   // Select PMU_Range register
    buff[1] = 0x03;                   // Range = +/- 2g
    ret = HalCmnI2c_Write( buff, 2 );
    if( ret == EN_FALSE ){ goto err; }
    usleep( 100 * 1000 );

    buff[0] = 0x10;                   // Select PMU_BW register
    buff[1] = 0x0F;                   // Bandwidth = 1kHz
    ret = HalCmnI2c_Write( buff, 2 );
    if( ret == EN_FALSE ){ goto err; }
    usleep( 100 * 1000 );

    buff[0] = 0x11;                   // Select PMU_LPW register
    buff[1] = 0x00;                   // Normal mode, Sleep duration = 0.5ms
    ret = HalCmnI2c_Write( buff, 2 );
    if( ret == EN_FALSE ){ goto err; }
    usleep( 100 * 1000 );

    return EN_TRUE;
err:
    DBG_PRINT_ERROR( "fail to setup config of acc. \n\r" );
    return EN_FALSE;
}


/**************************************************************************//*!
 * @brief     BMX055 ジャイロセンサを設定する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
static EHalBool_t
SetConfigGyro(
    void  ///< [in] ナシ
){
    EHalBool_t      ret = EN_FALSE;
    unsigned char   buff[2];

    DBG_PRINT_TRACE( "\n\r" );

    // I2C スレーブデバイスを BMX055 ACC に変える
    HalCmnI2c_SetSlave( I2C_SLAVE_BMX055_GYRO );

    buff[0] = 0x0F;                   // Select Range register
    buff[1] = 0x04;                   // Range = +/- 125degree/s
    ret = HalCmnI2c_Write( buff, 2 );
    if( ret == EN_FALSE ){ goto err; }
    usleep( 100 * 1000 );

    buff[0] = 0x10;                   // Select Bandwidth register
    buff[1] = 0x07;                   // Bandwidth = 100Hz
    ret = HalCmnI2c_Write( buff, 2 );
    if( ret == EN_FALSE ){ goto err; }
    usleep( 100 * 1000 );

    buff[0] = 0x11;                   // Select LPM1 register
    buff[1] = 0x00;                   // Normal mode, Sleep duration = 2ms
    ret = HalCmnI2c_Write( buff, 2 );
    if( ret == EN_FALSE ){ goto err; }
    usleep( 100 * 1000 );

    return EN_TRUE;
err:
    DBG_PRINT_ERROR( "fail to setup config of gyro. \n\r" );
    return EN_FALSE;
}


/**************************************************************************//*!
 * @brief     BMX055 磁気センサを設定する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
static EHalBool_t
SetConfigMag(
    void  ///< [in] ナシ
){
    EHalBool_t      ret = EN_FALSE;
    unsigned char   buff[2];

    DBG_PRINT_TRACE( "\n\r" );

    // I2C スレーブデバイスを BMX055 MAG に変える
    HalCmnI2c_SetSlave( I2C_SLAVE_BMX055_MAG );

#if 0
    buff[0] = 0x4B;                   // Select Mag register
    buff[1] = 0x83;                   // Soft reset
    ret = HalCmnI2c_Write( buff, 2 );
    if( ret == EN_FALSE ){ DBG_PRINT_ERROR( "01 \n\r" ); goto err; }
    usleep( 100 * 1000 );
#endif

    buff[0] = 0x4B;                   // Select Mag register
    buff[1] = 0x01;                   // Soft reset
    ret = HalCmnI2c_Write( buff, 2 );
    if( ret == EN_FALSE ){ DBG_PRINT_ERROR( "02 \n\r" ); goto err; }
    usleep( 100 * 1000 );

    buff[0] = 0x4C;                   // Select Mag register
    buff[1] = 0x00;                   // Normal Mode, ODR = 10 Hz
    ret = HalCmnI2c_Write( buff, 2 );
    if( ret == EN_FALSE ){ DBG_PRINT_ERROR( "03 \n\r" ); goto err; }

    buff[0] = 0x4E;                   // Select Mag register
    buff[1] = 0x84;                   // X, Y, Z-Axis enabled
    ret = HalCmnI2c_Write( buff, 2 );
    if( ret == EN_FALSE ){ DBG_PRINT_ERROR( "04 \n\r" ); goto err; }

    buff[0] = 0x51;                   // Select Mag register
    buff[1] = 0x04;                   // No. of Repetitions for X-Y Axis = 9
    ret = HalCmnI2c_Write( buff, 2 );
    if( ret == EN_FALSE ){ DBG_PRINT_ERROR( "05 \n\r" ); goto err; }

    buff[0] = 0x52;                   // Select Mag register
    buff[1] = 0x16;                   // No. of Repetitions for Z-Axis = 15
    ret = HalCmnI2c_Write( buff, 2 );
    if( ret == EN_FALSE ){ DBG_PRINT_ERROR( "06 \n\r" ); goto err; }

    return EN_TRUE;
err:
    DBG_PRINT_ERROR( "fail to setup config of mag. \n\r" );
    return EN_FALSE;
}


/**************************************************************************//*!
 * @brief     BMX055 加速度センサのオフセット値をセットする。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    なし。
 *************************************************************************** */
static void
SetOffsetAcc(
    void  ///< [in] ナシ
){
    SHalSensor_t*   data;

    DBG_PRINT_TRACE( "\n\r" );

    data = HalSensorBmx055_GetAcc( EN_SEN_BMX055_X );
    g_dataAcc[0].ofs = data->cur;

    data = HalSensorBmx055_GetAcc( EN_SEN_BMX055_Y );
    g_dataAcc[1].ofs = data->cur;

    data = HalSensorBmx055_GetAcc( EN_SEN_BMX055_Z );
    g_dataAcc[2].ofs = data->cur;

    return;
}


/**************************************************************************//*!
 * @brief     BMX055 加速度センサ値を読み出す
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    センサ変数へのポインタ
 *************************************************************************** */
SHalSensor_t*
HalSensorBmx055_GetAcc(
    EHalSensorBMX055_t  which   ///< [in] 対象 axis ( X-axis or Y-axis or Z-axis )
){
    EHalBool_t          ret = EN_FALSE;
    SHalSensor_t*       data = NULL;
    unsigned char       buff[6];
    double              dataX = 0;     // センサの計測値
    double              dataY = 0;     // センサの計測値
    double              dataZ = 0;     // センサの計測値

    DBG_PRINT_TRACE( "\n\r" );

    // I2C スレーブデバイスを BMX055 ACC に変える
    HalCmnI2c_SetSlave( I2C_SLAVE_BMX055_ACC );

    // Read 6 bytes of data from register
    buff[0] = 0x02;
    ret = HalCmnI2c_Write( buff, 1 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write 0x2 to i2c slave. \n\r" );
        goto err;
    }

    ret = HalCmnI2c_Read( buff, 6 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to read data from i2c slave. \n\r" );
        goto err;
    }

    // Convert the data
    dataX = (double)((buff[1] * 256 + (buff[0] & 0xF0)) / 16);
    if( dataX > 2047 ){ dataX -= 4096; }

    dataY = (double)((buff[3] * 256 + (buff[2] & 0xF0)) / 16);
    if( dataY > 2047 ){ dataY -= 4096; }

    dataZ = (double)((buff[5] * 256 + (buff[4] & 0xF0)) / 16);
    if( dataZ > 2047 ){ dataZ -= 4096; }

    dataX = dataX * 0.0098; // renge +-2g
    dataY = dataY * 0.0098; // renge +-2g
    dataZ = dataZ * 0.0098; // renge +-2g

    // グローバル変数を更新する
    HalCmn_UpdateSenData( &g_dataAcc[0], dataX );
    HalCmn_UpdateSenData( &g_dataAcc[1], dataY );
    HalCmn_UpdateSenData( &g_dataAcc[2], dataZ );

err:
    switch( which )
    {
    case EN_SEN_BMX055_X : data = &g_dataAcc[0]; break;
    case EN_SEN_BMX055_Y : data = &g_dataAcc[1]; break;
    case EN_SEN_BMX055_Z : data = &g_dataAcc[2]; break;
    default                  : break;
    }

    return data;
}


/**************************************************************************//*!
 * @brief     BMX055 ジャイロセンサ値を読み出す
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    センサ変数へのポインタ
 *************************************************************************** */
SHalSensor_t*
HalSensorBmx055_GetGyro(
    EHalSensorBMX055_t  which   ///< [in] 対象 axis ( X-axis or Y-axis or Z-axis )
){
    EHalBool_t          ret = EN_FALSE;
    SHalSensor_t*       data = NULL;
    unsigned char       buff[6];
    double              dataX = 0;     // センサの計測値
    double              dataY = 0;     // センサの計測値
    double              dataZ = 0;     // センサの計測値

    DBG_PRINT_TRACE( "\n\r" );

    // I2C スレーブデバイスを BMX055 GYRO に変える
    HalCmnI2c_SetSlave( I2C_SLAVE_BMX055_GYRO );

    // Read 6 bytes of data from register
    buff[0] = 0x02;
    ret = HalCmnI2c_Write( buff, 1 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write 0x2 to i2c slave. \n\r" );
        goto err;
    }

    ret = HalCmnI2c_Read( buff, 6 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to read data from i2c slave. \n\r" );
        goto err;
    }

    // Convert the data
    dataX = (double)(buff[1] * 256 + buff[0]);
    if( dataX > 32767 ){ dataX -= 65536; }

    dataY = (double)(buff[3] * 256 + buff[2]);
    if( dataY > 32767 ){ dataY -= 65536; }

    dataZ = (double)(buff[5] * 256 + buff[4]);
    if( dataZ > 32767 ){ dataZ -= 65536; }

    dataX = dataX * 0.0038; //  Full scale = +/- 125 degree/s
    dataY = dataY * 0.0038; //  Full scale = +/- 125 degree/s
    dataZ = dataZ * 0.0038; //  Full scale = +/- 125 degree/s

    // グローバル変数を更新する
    HalCmn_UpdateSenData( &g_dataGyro[0], dataX );
    HalCmn_UpdateSenData( &g_dataGyro[1], dataY );
    HalCmn_UpdateSenData( &g_dataGyro[2], dataZ );

err:
    switch( which )
    {
    case EN_SEN_BMX055_X : data = &g_dataGyro[0]; break;
    case EN_SEN_BMX055_Y : data = &g_dataGyro[1]; break;
    case EN_SEN_BMX055_Z : data = &g_dataGyro[2]; break;
    default                  : break;
    }

    return data;
}


/**************************************************************************//*!
 * @brief     BMX055 磁気センサ値を読み出す
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    センサ変数へのポインタ
 *************************************************************************** */
SHalSensor_t*
HalSensorBmx055_GetMag(
    EHalSensorBMX055_t  which   ///< [in] 対象 axis ( X-axis or Y-axis or Z-axis )
){
    EHalBool_t          ret = EN_FALSE;
    SHalSensor_t*       data = NULL;
    unsigned char       buff[8];
    double              dataX = 0;     // センサの計測値
    double              dataY = 0;     // センサの計測値
    double              dataZ = 0;     // センサの計測値

    DBG_PRINT_TRACE( "\n\r" );

    // I2C スレーブデバイスを BMX055 MAG に変える
    HalCmnI2c_SetSlave( I2C_SLAVE_BMX055_MAG );

    // Read 6 bytes of data from register
    buff[0] = 0x42;
    ret = HalCmnI2c_Write( buff, 1 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write 0x42 to i2c slave. \n\r" );
        goto err;
    }

    ret = HalCmnI2c_Read( buff, 8 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to read data from i2c slave. \n\r" );
        goto err;
    }

    // Convert the data
    dataX = (double)( (buff[1] << 8) | (buff[0] >> 3) );
    if( dataX > 4095 ){ dataX -= 8192; }

    dataY = (double)( (buff[3] << 8) | (buff[2] >> 3) );
    if( dataY > 4095 ){ dataY -= 8192; }

    dataZ = (double)( (buff[5] << 8) | (buff[4] >> 3) );
    if( dataZ > 4095 ){ dataZ -= 8192; }

    // グローバル変数を更新する
    HalCmn_UpdateSenData( &g_dataMag[0], dataX );
    HalCmn_UpdateSenData( &g_dataMag[1], dataY );
    HalCmn_UpdateSenData( &g_dataMag[2], dataZ );

err:
    switch( which )
    {
    case EN_SEN_BMX055_X : data = &g_dataMag[0]; break;
    case EN_SEN_BMX055_Y : data = &g_dataMag[1]; break;
    case EN_SEN_BMX055_Z : data = &g_dataMag[2]; break;
    default                  : break;
    }

    return data;
}


#ifdef __cplusplus
    }
#endif

