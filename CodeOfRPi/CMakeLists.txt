cmake_minimum_required(VERSION 3.13.4)

project(Mosq)

#######################################
add_library(lServo SHARED
	src/Servo.cpp)

add_library(l::Servo ALIAS lServo)

target_include_directories(lServo 
	PUBLIC
		${PROJECT_SOURCE_DIR}/include)
#######################################

set(SOURCES 
	src/MQTT_Subscriber.cpp)

target_include_directories(lServo 
	PRIVATE
		${PROJECT_SOURCE_DIR}/include)

add_executable(Mosq ${SOURCES})


target_link_libraries(Mosq 
PUBLIC
	l::Servo
PUBLIC
	bcm2835
	mosquitto)

