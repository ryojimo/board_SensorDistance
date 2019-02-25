cmake_minimum_required( VERSION 3.7.2 )

# Set project information
project( board_Sensor VERSION 0.0.1 LANGUAGES C )
set( PROJECT_BRIEF "A motor system on RaspberryPi." )

# Options
set( CMAKE_C_FLAGS "-v -O2 -Wall" )
include_directories( /usr/local/include )
link_directories( /usr/local/lib )
add_definitions( -lrt -lwiringPi -Wl,-Map=board.map )

# Targets.
set( h_app ./app/if_lcd/ ./app/if_pc/ ./app/log/ )
set( h_hal ./hal/ )
set( h_sys ./sys/ )
set( h_all ${h_app} ${h_hal} ${h_sys} )
include_directories( ${h_all} )
message( "h_all: " ${h_all} "\n" )

file( GLOB c_app  ./app/if_lcd/*.c ./app/if_pc/*.c ./app/log/*.c )
file( GLOB c_hal  ./hal/*.c )
file( GLOB c_sys  ./sys/*.c )
file( GLOB c_main ./main.c )
set( c_all ${c_app} ${c_hal} ${c_sys} ${c_main} )
message( "c_all: " ${c_all} "\n" )

# Build and Link
add_executable( board.out ${c_all} )
target_link_libraries( board.out wiringPi )

