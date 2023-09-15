#ifndef __MUSIC_H__
#define __MUSIC_H__

#include "sys.h"

void DoSum(u8 *Str, u8 len);
void Uart_SendCMD(u8 CMD ,u8 feedback , u16 dat);
void SendCmd(u8 len);
void send_byte(unsigned char s);

void setMusic(u16 side);
void setMusicFolder(u16 side);
void setVolume(u16 side);
void start(void);
void stop(void);

#endif

