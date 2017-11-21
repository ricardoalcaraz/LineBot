/*
 * LineBot.h
 *
 *  Created on: Nov 15, 2017
 *      Author: cinna
 */

#ifndef LINEBOT_H_
#define LINEBOT_H_

#include "Arduino.h"

class LineBot {
public:
	LineBot();
	void begin();
	void LineFollow();
	void moveForward();
	void stop();
	void turnRight();
	void turnLeft();
	void timedTurnRight();
	void timedTurnLeft();
	void spinAround();
	void setMaxSpeed(uint8_t maxSpeed);
	//void setIRSensors(uint8_t backLeftIR, uint8_t backRightIR);

	uint8_t* getIRData();

	virtual ~LineBot();
private:
	uint8_t maxSpeed;
	uint16_t IR_data[4];
};

#endif /* LINEBOT_H_ */
