#pragma once

const unsigned char BigNumber_64x32[11][256] =
{
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x1F,0x00,0x00,0xFE,0x7F,0x00,0x80,0xFF,0xFF,0x01,0xC0,0x0F,0xF8,0x01,0xC0,0x07,0xE0,0x03,0xE0,0x03,0xC0,0x07,0xF0,0x01,0xC0,0x07,0xF0,0x01,0x80,0x0F,0xF0,0x00,0x80,0x0F,0xF8,0x00,0x80,0x0F,0xF8,0x00,0x00,0x0F,0xF8,0x00,0x00,0x1F,0x78,0x00,0x00,0x1F,0x78,0x00,0x00,0x1F,0x78,0x00,0x00,0x1F,0x7C,0x00,0x00,0x1F,0x7C,0x00,0x00,0x1F,0x7C,0x00,0x00,0x1F,0x7C,0x00,0x00,0x1F,0x78,0x00,0x00,0x1F,0x78,0x00,0x00,0x1F,0x78,0x00,0x00,0x0F,0xF8,0x00,0x00,0x0F,0xF8,0x00,0x80,0x0F,0xF8,0x00,0x80,0x0F,0xF0,0x01,0x80,0x07,0xF0,0x01,0xC0,0x07,0xE0,0x03,0xC0,0x03,0xE0,0x07,0xE0,0x03,0xC0,0x0F,0xF8,0x01,0x80,0xFF,0xFF,0x00,0x00,0xFF,0x7F,0x00,0x00,0xFC,0x1F,0x00,0x00,0xC0,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },/*"0",0*/
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x1E,0x00,0x00,0x80,0x1F,0x00,0x00,0xE0,0x1F,0x00,0x00,0xFC,0x1F,0x00,0x80,0xFF,0x1F,0x00,0xC0,0x3F,0x1F,0x00,0xC0,0x0F,0x1F,0x00,0xC0,0x03,0x1F,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },/*"1",1*/
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x1F,0x00,0xC0,0xFF,0x7F,0x00,0xE0,0xFF,0xFF,0x00,0xE0,0x03,0xFC,0x01,0xE0,0x00,0xF0,0x03,0x20,0x00,0xE0,0x03,0x00,0x00,0xE0,0x03,0x00,0x00,0xC0,0x07,0x00,0x00,0xC0,0x07,0x00,0x00,0xC0,0x07,0x00,0x00,0xC0,0x03,0x00,0x00,0xE0,0x03,0x00,0x00,0xE0,0x03,0x00,0x00,0xF0,0x01,0x00,0x00,0xF8,0x01,0x00,0x00,0xFC,0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x3F,0x00,0x00,0xC0,0x0F,0x00,0x00,0xF0,0x07,0x00,0x00,0xF8,0x01,0x00,0x00,0x7E,0x00,0x00,0x00,0x3F,0x00,0x00,0x80,0x0F,0x00,0x00,0xC0,0x07,0x00,0x00,0xE0,0x03,0x00,0x00,0xF0,0x01,0x00,0x00,0xF0,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0xF8,0xFF,0xFF,0x0F,0xF8,0xFF,0xFF,0x0F,0xF8,0xFF,0xFF,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },/*"2",2*/
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xFF,0x0F,0x00,0xE0,0xFF,0x3F,0x00,0xE0,0xFF,0x7F,0x00,0xE0,0x00,0xFC,0x00,0x20,0x00,0xF8,0x01,0x00,0x00,0xF0,0x01,0x00,0x00,0xE0,0x01,0x00,0x00,0xE0,0x03,0x00,0x00,0xE0,0x01,0x00,0x00,0xE0,0x01,0x00,0x00,0xF0,0x01,0x00,0x00,0xF0,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x7E,0x00,0x00,0xFF,0x1F,0x00,0x00,0xFF,0x03,0x00,0x00,0xFF,0x1F,0x00,0x00,0xFF,0x7F,0x00,0x00,0x00,0xFC,0x01,0x00,0x00,0xF0,0x03,0x00,0x00,0xE0,0x03,0x00,0x00,0xC0,0x07,0x00,0x00,0xC0,0x07,0x00,0x00,0xC0,0x07,0x00,0x00,0xC0,0x07,0x00,0x00,0xC0,0x07,0x00,0x00,0xC0,0x07,0x00,0x00,0xE0,0x03,0x18,0x00,0xF0,0x01,0x78,0x00,0xFC,0x01,0xF8,0xFF,0xFF,0x00,0xF8,0xFF,0x3F,0x00,0xE0,0xFF,0x0F,0x00,0x00,0x7C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },/*"3",3*/
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0xFC,0x00,0x00,0x00,0xFC,0x00,0x00,0x00,0xFE,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0xFF,0x00,0x00,0x80,0xFF,0x00,0x00,0xC0,0xFB,0x00,0x00,0xE0,0xF9,0x00,0x00,0xF0,0xF9,0x00,0x00,0xF0,0xF8,0x00,0x00,0x78,0xF8,0x00,0x00,0x3C,0xF8,0x00,0x00,0x1E,0xF8,0x00,0x00,0x1F,0xF8,0x00,0x80,0x0F,0xF8,0x00,0xC0,0x07,0xF8,0x00,0xE0,0x03,0xF8,0x00,0xF0,0x01,0xF8,0x00,0xF8,0x00,0xF8,0x00,0x7C,0x00,0xF8,0x00,0x3E,0x00,0xF8,0x00,0xFF,0xFF,0xFF,0x1F,0xFF,0xFF,0xFF,0x1F,0xFF,0xFF,0xFF,0x1F,0x00,0x00,0xF8,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },/*"4",4*/
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xFF,0xFF,0x03,0x80,0xFF,0xFF,0x03,0x80,0xFF,0xFF,0x03,0x80,0x07,0x00,0x00,0xC0,0x07,0x00,0x00,0xC0,0x07,0x00,0x00,0xC0,0x07,0x00,0x00,0xC0,0x03,0x00,0x00,0xC0,0x03,0x00,0x00,0xC0,0x03,0x00,0x00,0xC0,0x03,0x00,0x00,0xC0,0x03,0x00,0x00,0xC0,0x03,0x00,0x00,0xC0,0xFF,0x07,0x00,0xC0,0xFF,0x3F,0x00,0xE0,0xFF,0xFF,0x00,0x20,0x80,0xFF,0x01,0x00,0x00,0xF8,0x03,0x00,0x00,0xE0,0x03,0x00,0x00,0xE0,0x07,0x00,0x00,0xC0,0x07,0x00,0x00,0xC0,0x07,0x00,0x00,0x80,0x07,0x00,0x00,0x80,0x07,0x00,0x00,0x80,0x07,0x00,0x00,0xC0,0x07,0x00,0x00,0xC0,0x07,0x00,0x00,0xE0,0x03,0x00,0x00,0xF0,0x03,0xE0,0x00,0xFC,0x01,0xE0,0xFF,0xFF,0x00,0xE0,0xFF,0x3F,0x00,0xE0,0xFF,0x0F,0x00,0x00,0x78,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },/*"5",5*/
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xFF,0x03,0x00,0xF0,0xFF,0x03,0x00,0xFC,0xFF,0x03,0x00,0x7E,0x00,0x02,0x00,0x1F,0x00,0x00,0x80,0x0F,0x00,0x00,0xC0,0x07,0x00,0x00,0xC0,0x03,0x00,0x00,0xE0,0x03,0x00,0x00,0xE0,0x01,0x00,0x00,0xF0,0x01,0x00,0x00,0xF0,0x01,0x00,0x00,0xF0,0x00,0x07,0x00,0xF0,0xF0,0x7F,0x00,0xF8,0xFC,0xFF,0x01,0xF8,0xFE,0xFF,0x03,0xF8,0x0F,0xE0,0x07,0xF8,0x07,0xC0,0x0F,0xF8,0x03,0x80,0x0F,0xF8,0x01,0x80,0x1F,0xF8,0x01,0x00,0x1F,0xF8,0x00,0x00,0x1F,0xF8,0x00,0x00,0x1F,0xF0,0x00,0x00,0x1F,0xF0,0x01,0x00,0x1F,0xF0,0x01,0x00,0x1F,0xE0,0x01,0x80,0x0F,0xE0,0x03,0x80,0x0F,0xC0,0x07,0xC0,0x07,0x80,0x1F,0xF0,0x03,0x80,0xFF,0xFF,0x01,0x00,0xFE,0xFF,0x00,0x00,0xF8,0x3F,0x00,0x00,0x80,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },/*"6",6*/
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xFF,0xFF,0x1F,0xF8,0xFF,0xFF,0x1F,0xF8,0xFF,0xFF,0x0F,0x00,0x00,0x80,0x0F,0x00,0x00,0xC0,0x07,0x00,0x00,0xC0,0x03,0x00,0x00,0xE0,0x03,0x00,0x00,0xF0,0x01,0x00,0x00,0xF0,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x78,0x00,0x00,0x00,0x7C,0x00,0x00,0x00,0x3E,0x00,0x00,0x00,0x1E,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x0F,0x00,0x00,0x80,0x0F,0x00,0x00,0x80,0x07,0x00,0x00,0xC0,0x07,0x00,0x00,0xC0,0x03,0x00,0x00,0xE0,0x03,0x00,0x00,0xE0,0x01,0x00,0x00,0xF0,0x01,0x00,0x00,0xF0,0x01,0x00,0x00,0xF8,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x7C,0x00,0x00,0x00,0x7C,0x00,0x00,0x00,0x7C,0x00,0x00,0x00,0x3E,0x00,0x00,0x00,0x3E,0x00,0x00,0x00,0x3E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },/*"7",7*/
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x1F,0x00,0x00,0xFF,0x7F,0x00,0x80,0xFF,0xFF,0x01,0xC0,0x0F,0xF8,0x03,0xE0,0x07,0xE0,0x03,0xE0,0x03,0xC0,0x07,0xF0,0x01,0xC0,0x07,0xF0,0x01,0xC0,0x07,0xF0,0x01,0xC0,0x07,0xE0,0x01,0xC0,0x07,0xE0,0x03,0xC0,0x07,0xE0,0x03,0xE0,0x03,0xC0,0x07,0xF0,0x01,0x80,0x3F,0xFC,0x00,0x00,0xFE,0x7F,0x00,0x00,0xFE,0x3F,0x00,0x80,0xFF,0xFF,0x00,0xC0,0x0F,0xF0,0x03,0xE0,0x03,0xE0,0x07,0xF0,0x01,0xC0,0x0F,0xF8,0x00,0x80,0x0F,0xF8,0x00,0x00,0x1F,0xF8,0x00,0x00,0x1F,0xF8,0x00,0x00,0x1F,0xF8,0x00,0x00,0x1F,0xF8,0x00,0x00,0x1F,0xF8,0x00,0x80,0x0F,0xF8,0x01,0x80,0x0F,0xF0,0x03,0xE0,0x07,0xE0,0x0F,0xF0,0x07,0xC0,0xFF,0xFF,0x03,0x80,0xFF,0xFF,0x00,0x00,0xFE,0x3F,0x00,0x00,0x80,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },/*"8",8*/
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x1F,0x00,0x00,0xFF,0x7F,0x00,0xC0,0xFF,0xFF,0x00,0xE0,0x0F,0xF8,0x01,0xE0,0x03,0xE0,0x03,0xF0,0x01,0xC0,0x03,0xF8,0x00,0xC0,0x07,0xF8,0x00,0x80,0x07,0xF8,0x00,0x80,0x0F,0x78,0x00,0x80,0x0F,0x78,0x00,0x80,0x0F,0xF8,0x00,0x80,0x0F,0xF8,0x00,0x80,0x0F,0xF8,0x00,0x80,0x0F,0xF8,0x01,0xC0,0x0F,0xF0,0x03,0xE0,0x0F,0xE0,0x07,0xF0,0x0F,0xC0,0xFF,0x7F,0x0F,0x80,0xFF,0x3F,0x0F,0x00,0xFF,0x1F,0x0F,0x00,0xF8,0x83,0x0F,0x00,0x00,0x80,0x0F,0x00,0x00,0x80,0x07,0x00,0x00,0x80,0x07,0x00,0x00,0xC0,0x07,0x00,0x00,0xC0,0x03,0x00,0x00,0xE0,0x03,0x00,0x00,0xF0,0x01,0x00,0x00,0xF8,0x00,0x60,0x00,0x7E,0x00,0xE0,0xFF,0x3F,0x00,0xE0,0xFF,0x1F,0x00,0xE0,0xFF,0x03,0x00,0x00,0x3C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },/*"9",9*/
	{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x00,0x00,0x00,0xF0,0x01,0x00,0x00,0xF8,0x01,0x00,0x00,0xF8,0x01,0x00,0x00,0xF0,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },/*".",10*/
};