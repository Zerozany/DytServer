/**
 ******************************************************************************
 * @file           : receive_struct.h
 * @author         : Nueck
 * @brief          : None
 * @attention      : None
 * @date           : 2024/11/1
 ******************************************************************************
 */

#ifndef RECEIVE_STRUCT_H
#define RECEIVE_STRUCT_H

enum SEEKER_STATE1_1 {
  VisibleLight = 0x00,
  InfraredWhiteHot = 0x01,
  InfraredBlackHot = 0x02,
  Adaptive = 0x00,
  Person = 0x01,
  Vehicle = 0x02,
  Building = 0x03,
  TargetAutoPromptOpen = 0x04,
  TargetAutoPromptClose = 0x00,
  TargetLockState = 0x08,
};

#endif // RECEIVE_STRUCT_H
