/**************************************************************************//*!
 *  @file           hal_i2c_pca9685.c
 *  @brief          [HAL] I2C PCA9685 ドライバ API を定義したファイル。
 *  @author         Ryoji Morita
 *  @attention      none.
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2019.01.17
 *************************************************************************** */
#ifdef __cplusplus
    extern "C"{
#endif


//********************************************************
/* include                                               */
//********************************************************
#include <math.h>

#include "hal_cmn.h"
#include "hal.h"


//#define DBG_PRINT
#define MY_NAME "HAL"
#include "../app/log/log.h"


//********************************************************
/*! @def                                                 */
//********************************************************
#define PCA9685_SUBADR1   (0x2)
#define PCA9685_SUBADR2   (0x3)
#define PCA9685_SUBADR3   (0x4)

#define PCA9685_MODE1     (0x0)
#define PCA9685_PRESCALE  (0xFE)

#define LED0_ON_L         (0x6)
#define LED0_ON_H         (0x7)
#define LED0_OFF_L        (0x8)
#define LED0_OFF_H        (0x9)

#define ALLLED_ON_L       (0xFA)
#define ALLLED_ON_H       (0xFB)
#define ALLLED_OFF_L      (0xFC)
#define ALLLED_OFF_H      (0xFD)


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
static EHalBool_t   SetPwmFreq( double freq );
static EHalBool_t   SetPwm( unsigned char ch, unsigned int on, unsigned int off );




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
    SetPwmFreq( 50 ); // PWM 周期を 50Hz に設定 ( 20ms )
    return;
}


/**************************************************************************//*!
 * @brief     I2C PCA9685 を初期化する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
EHalBool_t
HalI2cPca9685_Init(
    void  ///< [in] ナシ
){
    EHalBool_t      ret = EN_FALSE;

    DBG_PRINT_TRACE( "\n\r" );

    // I2C スレーブデバイスを PCA9685 に変える
    HalCmnI2c_SetSlave( I2C_SLAVE_PCA9685 );

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
HalI2cPca9685_Fini(
    void  ///< [in] ナシ
){
    DBG_PRINT_TRACE( "\n\r" );
    return;
}


/**************************************************************************//*!
 * @brief     PWM 周波数を設定する
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
static EHalBool_t
SetPwmFreq(
    double          freq            ///< [in] PWM 周波数
){
    EHalBool_t      ret = EN_FALSE;
    unsigned char   regAddr;        // コマンドのレジスタ・アドレスをセット
    unsigned char   oldreg = 0x0;
    unsigned char   buff[2];
    double          prescaleval = 25000000;
    unsigned int    prescale = 0;

    prescaleval /= 4096;
    prescaleval /= freq;
    prescaleval -= 1;
    prescale = floor( prescaleval + 0.5 );

    DBG_PRINT_TRACE( "\n\r" );

    // I2C スレーブデバイスを PCA9685 に変える
    HalCmnI2c_SetSlave( I2C_SLAVE_PCA9685 );

    // PCA9685_MODE1 に 0x0 を書き込む ( reset )
    buff[0] = PCA9685_MODE1;
    buff[1] = 0x0;
    ret = HalCmnI2c_Write( buff, 2 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write data to i2c slave. \n\r" );
        return ret;
    }

    // PCA9685_MODE1 のデータをスレーブデバイスから読み出す
    regAddr = PCA9685_MODE1;
    ret = HalCmnI2c_Write( &regAddr, 1 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write 0x0 to i2c slave. \n\r" );
        return ret;
    }

    ret = HalCmnI2c_Read( &oldreg, 1 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to read data from i2c slave. \n\r" );
        return ret;
    }

    // PCA9685_MODE1 に (buff & 0x7F) | 0x10 を書き込む
    buff[0] = PCA9685_MODE1;
    buff[1] = (oldreg & 0x7F) | 0x10;
    ret = HalCmnI2c_Write( buff, 2 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write data to i2c slave. \n\r" );
        return ret;
    }

    // PCA9685_PRESCALE に prescale を書き込む
    buff[0] = PCA9685_PRESCALE;
    buff[1] = prescale;
    ret = HalCmnI2c_Write( buff, 2 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write data to i2c slave. \n\r" );
        return ret;
    }

    // PCA9685_MODE1 に oldreg を書き込む
    buff[0] = PCA9685_MODE1;
    buff[1] = oldreg;
    ret = HalCmnI2c_Write( buff, 2 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write data to i2c slave. \n\r" );
        return ret;
    }

    usleep( 5 * 1000 );

    // PCA9685_MODE1 に oldreg | 0xa1 を書き込む
    buff[0] = PCA9685_MODE1;
    buff[1] = oldreg | 0xA1;
    ret = HalCmnI2c_Write( buff, 2 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write data to i2c slave. \n\r" );
        return ret;
    }

    return EN_TRUE;
}


/**************************************************************************//*!
 * @brief     指定した ch に PWM 波形を出力する。
 * @attention なし。
 * @note      なし。
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
static EHalBool_t
SetPwm(
    unsigned char   ch,     ///< [in] 対象の ch ( 0 ～ 15 )
    unsigned int    on,     ///< [in] PWM の H 出力する時間 ( 基本的に 0 をセット )
    unsigned int    off     ///< [in] PWM の L 出力する時間
){
    EHalBool_t      ret = EN_FALSE;
    unsigned char   buff[5];

//    DBG_PRINT_TRACE( "\n\r" );

    // I2C スレーブデバイスを PCA9685 に変える
    HalCmnI2c_SetSlave( I2C_SLAVE_PCA9685 );

    buff[0] = LED0_ON_L + ( 4 * ch );
    buff[1] = on;
    buff[2] = on >> 8;
    buff[3] = off;
    buff[4] = off >> 8;

    ret = HalCmnI2c_Write( buff, 5 );
    if( ret == EN_FALSE )
    {
        DBG_PRINT_ERROR( "fail to write data to i2c slave. \n\r" );
        return ret;
    }

    ret = EN_TRUE;
    return ret;
}


/**************************************************************************//*!
 * @brief     指定した ch に PWM 波形を出力する。
 * @attention なし。
 * @note      一般的な SERVO MOTOR の場合、PWM duty 比はは「3% ~ 12%」まで。( サーボモータの仕様 )
 *            https://www.tohuandkonsome.site/entry/2017/08/24/101259
 * @sa        なし。
 * @author    Ryoji Morita
 * @return    EN_TRUE : 成功, EN_FALSE : 失敗
 *************************************************************************** */
EHalBool_t
HalI2cPca9685_SetPwmDuty(
    unsigned char       ch,     ///< [in] 対象の ch ( 0 ～ 15 )
    EHalMotorState_t    status, ///< [in] モータの状態
    double              rate    ///< [in] デューティ比 : 0.0% ～ 100.0% まで
){
    EHalBool_t      ret = EN_FALSE;
    unsigned int    on = 0;
    unsigned int    off = 0;

//    DBG_PRINT_TRACE( "\n\r" );

    on = 0;
    off = 0xFFF * rate / 100;

    // I2C スレーブデバイスを PCA9685 に変える
    HalCmnI2c_SetSlave( I2C_SLAVE_PCA9685 );

    if( status == EN_MOTOR_STANDBY )
    {
        ret = SetPwm( ch, on, 0 );
    } else if( status == EN_MOTOR_BRAKE )
    {
        ret = SetPwm( ch, on, 0 );
    } else if( status == EN_MOTOR_CCW || status == EN_MOTOR_CW )
    {
        ret = SetPwm( ch, on, off );
    } else if( status == EN_MOTOR_STOP )
    {
        ret = SetPwm( ch, on, 0 );
    } else
    {
        ;
    }

    return ret;
}


#ifdef __cplusplus
    }
#endif

