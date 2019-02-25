/*****************************************************************************//*!
 *  @file           log.h
 *  @brief          [APP] 外部公開 API を宣言したヘッダファイル。
 *  @author         Ryoji Morita
 *  @attention      none.
 *                  関数命名規則
 *                      通常関数 : App[モジュール名]_処理名()
 *  @sa             none.
 *  @bug            none.
 *  @warning        none.
 *  @version        1.00
 *  @last updated   2016.06.24
 ****************************************************************************** */

// 多重コンパイル抑止
#ifndef _APP_LOG_H
#define _APP_LOG_H

//**************************************************
/* include                                         */
//**************************************************
#include <stdio.h>
#include <unistd.h>


//**************************************************
/*! @def                                           */
//**************************************************
#define DBG_COLOR_WHITE  "\x1b[0;23m"
#define DBG_COLOR_RED    "\x1b[1;31m"
#define DBG_COLOR_GREEN  "\x1b[1;32m"
#define DBG_COLOR_YELLOW "\x1b[1;33m"
#define DBG_COLOR_BLUE   "\x1b[1;34m"
#define DBG_COLOR_PURPLE "\x1b[1;35m"
#define DBG_COLOR_AQUA   "\x1b[1;36m"
#define DBG_COLOR_GRARY  "\x1b[1;37m"

// Debug Print Log
#ifndef MY_NAME
#  define MY_NAME "---"
#endif

#ifdef DBG_PRINT /*DBG_PRINT----------------*/

    #define DBG_PRINT_ERROR(fmt, arg...) fprintf( stderr, "["MY_NAME"]" DBG_COLOR_RED    "[ERR]  " DBG_COLOR_WHITE " [%s:%d][" DBG_COLOR_GREEN "%s()" DBG_COLOR_WHITE "] " fmt, __FILE__, __LINE__, __FUNCTION__, ##arg )
    #define DBG_PRINT_WARN(fmt, arg...)  fprintf( stderr, "["MY_NAME"]" DBG_COLOR_BLUE   "[WARN] " DBG_COLOR_WHITE " [%s:%d][" DBG_COLOR_GREEN "%s()" DBG_COLOR_WHITE "] " fmt, __FILE__, __LINE__, __FUNCTION__, ##arg )
    #define DBG_PRINT_TRACE(fmt, arg...) fprintf( stdout, "["MY_NAME"]" DBG_COLOR_YELLOW "[TRACE]" DBG_COLOR_WHITE " [%s:%d][" DBG_COLOR_GREEN "%s()" DBG_COLOR_WHITE "] " fmt, __FILE__, __LINE__, __FUNCTION__, ##arg )
    #define DBG_PRINT_DEBUG(fmt, arg...) fprintf( stdout, "["MY_NAME"]" DBG_COLOR_PURPLE "[DEBUG]" DBG_COLOR_WHITE " "                                                     fmt, ##arg )

#else /*NO DBG_PRINT------------------------*/

    #define DBG_PRINT_ERROR(fmt, arg...) fprintf( stderr, "["MY_NAME"]" DBG_COLOR_RED    "[ERR]  " DBG_COLOR_WHITE " [%s:%d][" DBG_COLOR_GREEN "%s()" DBG_COLOR_WHITE "] " fmt, __FILE__, __LINE__, __FUNCTION__, ##arg )
    #define DBG_PRINT_WARN(fmt, arg...)  fprintf( stderr, "["MY_NAME"]" DBG_COLOR_BLUE   "[WARN] " DBG_COLOR_WHITE " [%s:%d][" DBG_COLOR_GREEN "%s()" DBG_COLOR_WHITE "] " fmt, __FILE__, __LINE__, __FUNCTION__, ##arg )
    #define DBG_PRINT_TRACE(fmt, arg...)
    #define DBG_PRINT_DEBUG(fmt, arg...)

#endif /*DBG_PRINT--------------------------*/


//**************************************************
/*! @enum                                          */
//**************************************************
// なし


//**************************************************
/*! @struct                                        */
//**************************************************
// なし


//********************************************************
/* 関数プロトタイプ宣言                                  */
//********************************************************
// なし


#endif  // _APP_LOG_H

