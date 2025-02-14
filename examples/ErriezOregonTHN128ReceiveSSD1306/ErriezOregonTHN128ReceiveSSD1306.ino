/*
 * MIT License
 *
 * Copyright (c) 2020 Erriez
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <Arduino.h>
#include <LowPower.h>
#include <ErriezOregonTHN128Esp32Receive.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSerif9pt7b.h>

// Connect RF receive to Arduino pin 2 (INT0) or pin 3 (INT1)
#define RF_RX_PIN     2

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Created by http://oleddisplay.squix.ch/ Consider a donation
// In case of problems make sure that you are using the font file with the correct version!
const uint8_t Lato_Black_24Bitmaps[] PROGMEM = {

        // Bitmap Data:
        0x00, // ' '
        0x71,0xC7,0x1C,0x71,0xC7,0x1C,0x71,0xC7,0x1C,0x00,0x07,0x3E,0xF9,0xC0, // '!'
        0xE7,0x73,0xB9,0xDC,0xEE,0x77,0x39,0x98, // '"'
        0x0E,0x70,0x1C,0xE0,0x39,0xC0,0x73,0x81,0xE7,0x0F,0xFF,0x9F,0xFF,0x3F,0xFC,0x1C,0xE0,0x39,0xC3,0xFF,0xE7,0xFF,0xCF,0xFF,0x87,0x38,0x0E,0x70,0x1C,0xE0,0x39,0xC0,0xE3,0x80, // '#'
        0x01,0x80,0x06,0x00,0xFC,0x07,0xFC,0x3F,0xF8,0xF7,0xC7,0x98,0x1E,0xE0,0x7B,0x80,0xFE,0x03,0xFC,0x07,0xFC,0x07,0xF8,0x0D,0xE0,0x33,0x80,0xCE,0x33,0x79,0xFF,0xEF,0xFF,0x0F,0xF8,0x07,0x00,0x18,0x00,0x60,0x00, // '$'
        0x3C,0x03,0xCF,0xC0,0xE3,0xFC,0x3C,0x73,0x8F,0x0E,0x71,0xC1,0xCE,0x70,0x3F,0xDE,0x03,0xF7,0x80,0x3C,0xE0,0x00,0x39,0xE0,0x0F,0x7E,0x03,0xDF,0xE0,0x73,0x9C,0x1C,0x73,0x87,0x8E,0x71,0xE1,0xCE,0x38,0x1F,0x8E,0x01,0xE0, // '%'
        0x07,0x80,0x07,0xF8,0x01,0xFF,0x00,0xF3,0xC0,0x38,0x40,0x0F,0x00,0x03,0xC0,0x00,0x78,0x00,0x1F,0x0E,0x1F,0xE3,0x8F,0x7D,0xE3,0xCF,0xF0,0xF1,0xFC,0x3C,0x3E,0x0F,0x0F,0x83,0xFF,0xF0,0x7F,0xFE,0x07,0xC3,0xE0, // '&'
        0xEE,0xEE,0xEE,0x60, // '''
        0x10,0xE3,0x9E,0x71,0xCF,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0xCF,0x1C,0x70,0xE3,0x84, // '('
        0x43,0x0E,0x38,0xF1,0xC7,0x1E,0x78,0xE3,0x8E,0x38,0xE3,0x8E,0x79,0xC7,0x1C,0xF3,0x8E,0x10, // ')'
        0x18,0x0C,0x36,0xCF,0xE1,0xC3,0xF3,0xED,0x32,0x18,0x00, // '*'
        0x0E,0x00,0x70,0x03,0x80,0x1C,0x00,0xE0,0x7F,0xFB,0xFF,0xDF,0xFE,0x0E,0x00,0x70,0x03,0x80,0x1C,0x00,0xE0,0x00, // '+'
        0xE7,0xBC,0xE6,0x33,0x10, // ','
        0xFE,0xFE,0xFE, // '-'
        0xE7,0xBD,0xC0, // '.'
        0x01,0xC0,0x70,0x0E,0x01,0xC0,0x70,0x0E,0x03,0x80,0x70,0x0E,0x03,0x80,0x70,0x0E,0x03,0x80,0x70,0x1E,0x03,0x80,0x70,0x1C,0x03,0x80,0x00, // '/'
        0x0F,0x80,0x7F,0x83,0xFF,0x1F,0x3C,0x78,0x79,0xE1,0xE7,0x07,0x9C,0x0E,0xF0,0x3B,0xC0,0xE7,0x03,0x9C,0x1E,0x78,0x79,0xE1,0xE3,0x8F,0x0F,0xFC,0x1F,0xE0,0x1E,0x00, // '0'
        0x07,0x80,0x7C,0x07,0xE0,0x7F,0x0F,0xF8,0x3B,0xC0,0x9E,0x00,0xF0,0x07,0x80,0x3C,0x01,0xE0,0x0F,0x00,0x78,0x03,0xC0,0x1E,0x07,0xFE,0x3F,0xF1,0xFF,0x80, // '1'
        0x1F,0x81,0xFE,0x1F,0xF9,0xE1,0xCF,0x0F,0x30,0x78,0x03,0xC0,0x3C,0x01,0xE0,0x1E,0x01,0xF0,0x1F,0x01,0xF0,0x1F,0x01,0xF0,0x1F,0xFE,0xFF,0xF7,0xFF,0x80, // '2'
        0x0F,0x81,0xFE,0x1F,0xF9,0xE1,0xEF,0x0F,0x00,0x78,0x07,0x80,0xF8,0x07,0xC0,0x3F,0x00,0x7C,0x01,0xE0,0x0F,0x70,0x7B,0xC7,0xCF,0xFC,0x7F,0xC0,0xFC,0x00, // '3'
        0x00,0xF0,0x07,0xC0,0x3F,0x01,0xFC,0x07,0xF0,0x3F,0xC1,0xEF,0x07,0x3C,0x3C,0xF1,0xE3,0xCF,0x0F,0x3F,0xFE,0xFF,0xF9,0xFF,0xE0,0x0F,0x00,0x3C,0x00,0xF0,0x03,0xC0, // '4'
        0x3F,0xE1,0xFF,0x0F,0xF8,0x70,0x07,0x80,0x38,0x01,0xFC,0x0F,0xF8,0x7F,0xE0,0x8F,0x00,0x3C,0x01,0xE0,0x0F,0x00,0x79,0x87,0x9F,0xFC,0xFF,0xC1,0xF8,0x00, // '5'
        0x03,0xE0,0x3E,0x03,0xE0,0x1E,0x01,0xE0,0x1F,0x00,0xF0,0x0F,0xF8,0xFF,0xE7,0xCF,0xBC,0x3D,0xC0,0xEE,0x07,0x70,0x7B,0xC3,0xCF,0xFC,0x3F,0xC0,0xFC,0x00, // '6'
        0xFF,0xF7,0xFF,0xBF,0xFC,0x01,0xE0,0x1E,0x00,0xF0,0x0F,0x00,0x78,0x07,0xC0,0x3C,0x01,0xE0,0x1E,0x00,0xF0,0x0F,0x00,0x78,0x07,0x80,0x3C,0x03,0xC0,0x00, // '7'
        0x1F,0x81,0xFE,0x1F,0xF9,0xE1,0xCF,0x0F,0x78,0x79,0xC3,0x8F,0xFC,0x1F,0x83,0xFF,0x3C,0x3D,0xE1,0xEF,0x0F,0x78,0x7B,0xC3,0xCF,0xFC,0x7F,0xE0,0xFC,0x00, // '8'
        0x1F,0x81,0xFE,0x1F,0xF9,0xE1,0xCF,0x0F,0x70,0x7B,0xC3,0xDE,0x1E,0xFF,0xF3,0xFF,0x0F,0xF8,0x07,0x80,0x78,0x03,0xC0,0x3C,0x03,0xC0,0x3E,0x03,0xE0,0x00, // '9'
        0x77,0xBC,0xE0,0x00,0x00,0x03,0xBD,0xE7,0x00, // ':'
        0x77,0xBC,0xE0,0x00,0x00,0x77,0xBC,0xE3,0x33,0x88, // ';'
        0x00,0x80,0xE0,0xF8,0xFC,0x7E,0x3E,0x0F,0xC0,0xFC,0x0F,0x81,0xE0,0x18,0x02, // '<'
        0xFF,0xDF,0xFB,0xFF,0x00,0x0F,0xFD,0xFF,0xBF,0xF0, // '='
        0x80,0x38,0x0F,0x83,0xF8,0x3F,0x03,0xE1,0xF9,0xF8,0xF8,0x3C,0x0C,0x00,0x00, // '>'
        0x3E,0x1F,0xE3,0xFE,0x21,0xC0,0x3C,0x07,0x01,0xE0,0x78,0x1E,0x03,0x80,0x70,0x0C,0x00,0x00,0x00,0x06,0x01,0xE0,0x3C,0x03,0x00, // '?'
        0x01,0xF0,0x00,0x7F,0xC0,0x0F,0xFF,0x01,0xE0,0xF8,0x38,0x03,0x87,0x00,0x1C,0x70,0xFC,0xCE,0x1F,0xCC,0xE3,0xFC,0xEE,0x79,0xCE,0xE7,0x18,0xEE,0x73,0x8C,0xE7,0x39,0xCE,0x7F,0xF8,0x77,0xFF,0x87,0x3C,0xE0,0x38,0x00,0x03,0xE0,0x38,0x1F,0xFF,0x80,0x7F,0xF0,0x01,0xFC,0x00, // '@'
        0x03,0xE0,0x00,0xF8,0x00,0x7F,0x00,0x1F,0xC0,0x07,0x70,0x03,0xDE,0x00,0xF7,0x80,0x38,0xE0,0x1E,0x3C,0x07,0x8F,0x03,0xC1,0xE0,0xFF,0xF8,0x3F,0xFE,0x1F,0xFF,0xC7,0x80,0xF1,0xE0,0x3C,0xF0,0x07,0xBC,0x01,0xE0, // 'A'
        0xFF,0xC3,0xFF,0xCF,0xFF,0x3C,0x3E,0xF0,0x7B,0xC1,0xEF,0x0F,0x3F,0xF8,0xFF,0xC3,0xFF,0xCF,0x0F,0xBC,0x1E,0xF0,0x7B,0xC1,0xEF,0x0F,0xBF,0xFC,0xFF,0xE3,0xFF,0x00, // 'B'
        0x07,0xF0,0x1F,0xFC,0x3F,0xFC,0x7C,0x18,0x78,0x00,0xF8,0x00,0xF0,0x00,0xF0,0x00,0xF0,0x00,0xF0,0x00,0xF0,0x00,0xF0,0x00,0xF8,0x00,0x78,0x00,0x7C,0x1C,0x3F,0xFE,0x1F,0xFC,0x07,0xF0, // 'C'
        0xFF,0xC0,0x7F,0xF8,0x3F,0xFE,0x1E,0x0F,0x8F,0x03,0xC7,0x80,0xF3,0xC0,0x79,0xE0,0x3C,0xF0,0x1F,0x78,0x0F,0xBC,0x07,0x9E,0x03,0xCF,0x01,0xE7,0x81,0xF3,0xC1,0xF1,0xFF,0xF0,0xFF,0xF0,0x7F,0xE0,0x00, // 'D'
        0xFF,0xEF,0xFE,0xFF,0xEF,0x00,0xF0,0x0F,0x00,0xF0,0x0F,0xF8,0xFF,0x8F,0xF8,0xF0,0x0F,0x00,0xF0,0x0F,0x00,0xF0,0x0F,0xFE,0xFF,0xEF,0xFE, // 'E'
        0xFF,0xEF,0xFE,0xFF,0xEF,0x00,0xF0,0x0F,0x00,0xF0,0x0F,0x00,0xFF,0xCF,0xFC,0xFF,0xCF,0x00,0xF0,0x0F,0x00,0xF0,0x0F,0x00,0xF0,0x0F,0x00, // 'F'
        0x03,0xF0,0x1F,0xFE,0x3F,0xFE,0x7C,0x0C,0x78,0x00,0xF8,0x00,0xF0,0x00,0xF0,0x00,0xF0,0x00,0xF0,0x7E,0xF0,0x7E,0xF0,0x7E,0xF8,0x0E,0x78,0x0E,0x7C,0x0E,0x3F,0xFE,0x0F,0xFE,0x03,0xF8, // 'G'
        0xF0,0x1E,0xF0,0x1E,0xF0,0x1E,0xF0,0x1E,0xF0,0x1E,0xF0,0x1E,0xF0,0x1E,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0xF0,0x1E,0xF0,0x1E,0xF0,0x1E,0xF0,0x1E,0xF0,0x1E,0xF0,0x1E,0xF0,0x1E,0xF0,0x1E, // 'H'
        0xF7,0xBD,0xEF,0x7B,0xDE,0xF7,0xBD,0xEF,0x7B,0xDE,0xF7,0x80, // 'I'
        0x07,0x81,0xE0,0x78,0x1E,0x07,0x81,0xE0,0x78,0x1E,0x07,0x81,0xE0,0x78,0x1E,0x07,0x81,0xE0,0xF1,0xFC,0xFE,0x3F,0x00, // 'J'
        0xF0,0x3E,0xF0,0x7C,0xF0,0xF8,0xF0,0xF0,0xF1,0xF0,0xF3,0xE0,0xF3,0xC0,0xFF,0x80,0xFF,0x80,0xFF,0xC0,0xF3,0xE0,0xF1,0xE0,0xF1,0xF0,0xF0,0xF8,0xF0,0xF8,0xF0,0x7C,0xF0,0x3C,0xF0,0x3E, // 'K'
        0xF0,0x1E,0x03,0xC0,0x78,0x0F,0x01,0xE0,0x3C,0x07,0x80,0xF0,0x1E,0x03,0xC0,0x78,0x0F,0x01,0xE0,0x3C,0x07,0xFE,0xFF,0xDF,0xF8, // 'L'
        0xF0,0x01,0xEF,0x80,0x3E,0xF8,0x03,0xEF,0xC0,0x7E,0xFC,0x07,0xEF,0xE0,0xFE,0xFE,0x0F,0xEE,0xE0,0xEE,0xEF,0x1E,0xEE,0x71,0xCE,0xE7,0xBC,0xEE,0x3B,0x8E,0xE3,0xF8,0xEE,0x1F,0x0E,0xE1,0xF0,0xEE,0x0E,0x0E,0xE0,0x00,0xEE,0x00,0x0E, // 'M'
        0xE0,0x1E,0xF0,0x1E,0xF8,0x1E,0xF8,0x1E,0xFC,0x1E,0xFE,0x1E,0xFF,0x1E,0xEF,0x9E,0xEF,0x9E,0xE7,0xDE,0xE3,0xFE,0xE1,0xFE,0xE0,0xFE,0xE0,0xFE,0xE0,0x7E,0xE0,0x3E,0xE0,0x1E,0xE0,0x0E, // 'N'
        0x03,0xF0,0x03,0xFF,0x80,0xFF,0xF8,0x3E,0x0F,0x87,0x80,0xF9,0xF0,0x0F,0x3C,0x01,0xF7,0x80,0x1E,0xF0,0x03,0xDE,0x00,0x7B,0xC0,0x0F,0x78,0x03,0xEF,0x00,0x78,0xF0,0x1F,0x1F,0x07,0xC1,0xFF,0xF0,0x1F,0xFC,0x00,0x7E,0x00, // 'O'
        0xFF,0x83,0xFF,0x8F,0xFF,0x3C,0x3C,0xF0,0x7B,0xC1,0xEF,0x07,0xBC,0x1E,0xF0,0xF3,0xFF,0xCF,0xFE,0x3F,0xC0,0xF0,0x03,0xC0,0x0F,0x00,0x3C,0x00,0xF0,0x03,0xC0,0x00, // 'P'
        0x03,0xF0,0x00,0xFF,0xC0,0x3F,0xFE,0x07,0xC1,0xF0,0x78,0x0F,0x8F,0x80,0x78,0xF0,0x07,0xCF,0x00,0x3C,0xF0,0x03,0xCF,0x00,0x3C,0xF0,0x03,0xCF,0x00,0x7C,0xF0,0x07,0x87,0x80,0xF8,0x7C,0x1F,0x03,0xFF,0xF0,0x1F,0xFC,0x00,0x3F,0xE0,0x00,0x0F,0x00,0x00,0x78,0x00,0x03,0xE0, // 'Q'
        0xFF,0x81,0xFF,0xC3,0xFF,0xC7,0x87,0x8F,0x0F,0x1E,0x1E,0x3C,0x3C,0x78,0x78,0xFF,0xE1,0xFF,0x83,0xFE,0x07,0x9E,0x0F,0x3E,0x1E,0x3C,0x3C,0x7C,0x78,0x7C,0xF0,0xF9,0xE0,0xF8, // 'R'
        0x0F,0xC0,0x7F,0xC3,0xFF,0x1E,0x18,0x78,0x01,0xE0,0x07,0xE0,0x1F,0xE0,0x3F,0xE0,0x7F,0xC0,0x7F,0x00,0x3E,0x00,0x78,0x01,0xC7,0x0F,0x3F,0xFC,0x7F,0xE0,0x7E,0x00, // 'S'
        0xFF,0xFD,0xFF,0xFB,0xFF,0xF0,0x3C,0x00,0x78,0x00,0xF0,0x01,0xE0,0x03,0xC0,0x07,0x80,0x0F,0x00,0x1E,0x00,0x3C,0x00,0x78,0x00,0xF0,0x01,0xE0,0x03,0xC0,0x07,0x80,0x0F,0x00, // 'T'
        0xF8,0x1F,0x7C,0x0F,0xBE,0x07,0xDF,0x03,0xEF,0x81,0xF7,0xC0,0xFB,0xE0,0x7D,0xF0,0x3E,0xF8,0x1F,0x7C,0x0F,0xBE,0x07,0xCF,0x03,0xC7,0x81,0xE3,0xC0,0xF1,0xF0,0xF8,0x7F,0xF8,0x1F,0xF8,0x03,0xF0,0x00, // 'U'
        0xF0,0x07,0xBE,0x01,0xE7,0x80,0xF1,0xE0,0x3C,0x7C,0x1F,0x0F,0x07,0x83,0xC1,0xE0,0x78,0xF0,0x1E,0x3C,0x07,0x8F,0x00,0xF7,0x80,0x3D,0xE0,0x07,0x78,0x01,0xFC,0x00,0x7F,0x00,0x0F,0x80,0x03,0xE0,0x00,0x78,0x00, // 'V'
        0xF8,0x1E,0x07,0xDF,0x03,0xC0,0xF1,0xE0,0xF8,0x1E,0x3C,0x1F,0x87,0x87,0xC3,0xF0,0xF0,0x78,0x7E,0x1E,0x0F,0x1F,0xE3,0xC1,0xE3,0xBC,0xF0,0x1E,0x73,0x9E,0x03,0xDE,0x73,0xC0,0x7B,0x8F,0x70,0x0F,0x70,0xFE,0x00,0xFE,0x1F,0xC0,0x1F,0x83,0xF0,0x03,0xF0,0x7E,0x00,0x3E,0x07,0xC0,0x07,0xC0,0xF8,0x00,0xF0,0x1E,0x00, // 'W'
        0xF8,0x0F,0x9F,0x07,0xC3,0xC1,0xE0,0xF8,0xF0,0x1E,0x3C,0x07,0xDE,0x00,0xFF,0x00,0x1F,0xC0,0x07,0xE0,0x01,0xFC,0x00,0xFF,0x00,0x3F,0xE0,0x1E,0x78,0x07,0x9F,0x03,0xC3,0xE1,0xF0,0x78,0x78,0x1F,0x3E,0x03,0xE0, // 'X'
        0xF0,0x0F,0x3C,0x0F,0x9E,0x07,0x87,0x87,0x83,0xC3,0xC0,0xF3,0xC0,0x79,0xE0,0x1F,0xE0,0x07,0xF0,0x03,0xF0,0x00,0xF0,0x00,0x78,0x00,0x3C,0x00,0x1E,0x00,0x0F,0x00,0x07,0x80,0x03,0xC0,0x01,0xE0,0x00, // 'Y'
        0x7F,0xFC,0xFF,0xF9,0xFF,0xF0,0x07,0xC0,0x0F,0x80,0x3E,0x00,0xF8,0x01,0xF0,0x07,0xC0,0x1F,0x00,0x3E,0x00,0xF8,0x01,0xE0,0x07,0xC0,0x1F,0x00,0x3F,0xFE,0xFF,0xFD,0xFF,0xF8, // 'Z'
        0xFB,0xEF,0xB8,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xFB,0xEF,0x80, // '['
        0xE0,0x1C,0x01,0xC0,0x38,0x07,0x00,0x70,0x0E,0x01,0xC0,0x1C,0x03,0x80,0x38,0x07,0x00,0xE0,0x0E,0x01,0xC0,0x38,0x03,0x80,0x70,0x07,0x00, // '\'
        0xFB,0xEF,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0xFB,0xEF,0x80, // ']'
        0x0E,0x00,0x78,0x07,0xC0,0x3F,0x03,0xB8,0x1C,0xE1,0xE7,0x8E,0x1C,0xF0,0xF0, // '^'
        0xFF,0xBF,0xEF,0xF8, // '_'
        0xF8,0x3C,0x1C,0x0E, // '`'
        0x1F,0x83,0xFC,0x7F,0xE3,0x1E,0x00,0xE0,0x7E,0x3F,0xE7,0xFE,0x78,0xEF,0x1E,0x7F,0xE7,0xFE,0x3C,0x60, // 'a'
        0xF0,0x07,0x80,0x3C,0x01,0xE0,0x0F,0x00,0x7B,0xC3,0xFF,0x9F,0xFC,0xF8,0xF7,0x87,0xBC,0x3D,0xE1,0xEF,0x0F,0x78,0x7B,0xC7,0xDF,0xFC,0xFF,0xC7,0xBC,0x00, // 'b'
        0x1F,0x07,0xF9,0xFF,0x78,0x4F,0x01,0xC0,0x38,0x07,0x00,0xF0,0x1E,0x11,0xFF,0x1F,0xE1,0xF0, // 'c'
        0x00,0xF0,0x07,0x80,0x3C,0x01,0xE0,0x0F,0x0F,0x79,0xFF,0xCF,0xFE,0xF0,0xF7,0x87,0xBC,0x3D,0xC1,0xEF,0x0F,0x78,0x7B,0xC7,0xDF,0xFE,0x7F,0xF1,0xE7,0x80, // 'd'
        0x1F,0x03,0xFC,0x7F,0xEF,0x1E,0xE0,0xEF,0xFE,0xFF,0xEE,0x00,0xF0,0x0F,0x0C,0x7F,0xE3,0xFE,0x1F,0x80, // 'e'
        0x07,0x87,0xE1,0xF8,0xF0,0x3C,0x3F,0xEF,0xF9,0xFE,0x3C,0x0F,0x03,0xC0,0xF0,0x3C,0x0F,0x03,0xC0,0xF0,0x3C,0x0F,0x00, // 'f'
        0x1F,0xF8,0xFF,0xE7,0xFF,0x1C,0x38,0x70,0xE1,0xFF,0x83,0xFC,0x0F,0xE0,0x38,0x01,0xFF,0x03,0xFF,0x0F,0xFC,0x70,0x73,0xC1,0xC7,0xFF,0x1F,0xF8,0x1F,0x80, // 'g'
        0xF0,0x07,0x80,0x3C,0x01,0xE0,0x0F,0x00,0x7B,0xC3,0xFF,0x9F,0xFC,0xF1,0xE7,0x87,0xBC,0x3D,0xE1,0xEF,0x0F,0x78,0x7B,0xC3,0xDE,0x1E,0xF0,0xF7,0x87,0x80, // 'h'
        0x77,0xBC,0xE0,0x7B,0xDE,0xF7,0xBD,0xEF,0x7B,0xDE,0xF7,0x80, // 'i'
        0x1C,0x78,0xF0,0xE0,0x07,0x8F,0x1E,0x3C,0x78,0xF1,0xE3,0xC7,0x8F,0x1E,0x3C,0x78,0xF7,0xEF,0x9E,0x00, // 'j'
        0xF0,0x03,0xC0,0x0F,0x00,0x3C,0x00,0xF0,0x03,0xC3,0xCF,0x1E,0x3C,0x78,0xF3,0xC3,0xDE,0x0F,0xF8,0x3F,0xE0,0xFF,0x83,0xCF,0x0F,0x3C,0x3C,0x78,0xF1,0xF3,0xC3,0xE0, // 'k'
        0xF7,0xBD,0xEF,0x7B,0xDE,0xF7,0xBD,0xEF,0x7B,0xDE,0xF7,0x80, // 'l'
        0xE7,0x8F,0x1F,0xFF,0xF3,0xFF,0xFF,0x78,0xF9,0xEF,0x1E,0x1D,0xE3,0xC3,0xBC,0x78,0x77,0x8F,0x0E,0xF1,0xE1,0xDE,0x3C,0x3B,0xC7,0x87,0x78,0xF0,0xEF,0x1E,0x1C, // 'm'
        0xE7,0x87,0xFF,0x3F,0xF9,0xE3,0xCF,0x0F,0x78,0x7B,0xC3,0xDE,0x1E,0xF0,0xF7,0x87,0xBC,0x3D,0xE1,0xEF,0x0F,0x00, // 'n'
        0x1F,0x81,0xFE,0x1F,0xF9,0xE1,0xEF,0x0F,0x78,0x7B,0x83,0xDC,0x1E,0xF0,0xF7,0x87,0x9F,0xF8,0x7F,0x81,0xF8,0x00, // 'o'
        0xE7,0xC7,0xFF,0x3F,0xF9,0xE1,0xEF,0x0F,0x78,0x7B,0xC3,0xDE,0x1E,0xF0,0xF7,0x8F,0xBF,0xF9,0xFF,0x8F,0x78,0x78,0x03,0xC0,0x1E,0x00,0xF0,0x00, // 'p'
        0x1E,0xF3,0xFF,0x9F,0xFD,0xE1,0xEF,0x0F,0x78,0x7B,0x83,0xDE,0x1E,0xF0,0xF7,0x8F,0xBF,0xFC,0xFF,0xE3,0xCF,0x00,0x78,0x03,0xC0,0x1E,0x00,0xF0, // 'q'
        0xE7,0x7F,0xBF,0xDF,0xEF,0x87,0x83,0xC1,0xE0,0xF0,0x78,0x3C,0x1E,0x0F,0x00, // 'r'
        0x1F,0x07,0xF9,0xFF,0x38,0x47,0x80,0xFC,0x0F,0xE0,0x7E,0x03,0xC4,0x79,0xFF,0x7F,0xC3,0xF0, // 's'
        0x0C,0x07,0x01,0xC0,0xF0,0xFF,0xBF,0xEF,0xF8,0xF0,0x3C,0x0F,0x03,0xC0,0xF0,0x3C,0x0F,0x03,0xF0,0x7E,0x0F,0x00, // 't'
        0xF0,0xEF,0x0E,0xF0,0xEF,0x0E,0xF0,0xEF,0x0E,0xF0,0xEF,0x0E,0xF0,0xEF,0x1E,0x7F,0xE7,0xFE,0x3E,0xE0, // 'u'
        0xF0,0x79,0xC1,0xC7,0x8F,0x1E,0x3C,0x38,0xE0,0xF3,0x83,0xDE,0x07,0x70,0x1D,0xC0,0x7E,0x00,0xF8,0x03,0xE0,0x07,0x00, // 'v'
        0xF0,0xF0,0xF7,0x87,0x87,0x1C,0x3C,0x78,0xF3,0xE3,0xC7,0x9F,0x9C,0x1C,0xFC,0xE0,0xE6,0xEF,0x07,0xF3,0x70,0x1F,0x9F,0x80,0xF8,0xFC,0x07,0xC3,0xC0,0x3E,0x1E,0x00,0xF0,0xF0,0x00, // 'w'
        0xF8,0x79,0xE3,0xC3,0xCE,0x0F,0x78,0x1F,0xC0,0x3E,0x00,0xF8,0x07,0xF0,0x1F,0xC0,0xF7,0x87,0x8F,0x1E,0x3C,0xF0,0x78, // 'x'
        0xF0,0x79,0xE1,0xC7,0x8F,0x1E,0x3C,0x3C,0xE0,0xF3,0x81,0xDC,0x07,0xF0,0x1F,0xC0,0x3E,0x00,0xF8,0x01,0xC0,0x07,0x00,0x3C,0x00,0xE0,0x07,0x80,0x3C,0x00, // 'y'
        0xFF,0xDF,0xFB,0xFE,0x03,0xC0,0xF0,0x3C,0x07,0x01,0xE0,0x78,0x1E,0x03,0xFE,0x7F,0xCF,0xF8, // 'z'
        0x1C,0xF9,0xF7,0x8F,0x1E,0x1C,0x38,0x70,0xE3,0x87,0x0F,0x0E,0x1C,0x38,0x71,0xE3,0xC7,0x87,0xCF,0x87,0x00, // '{'
        0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xE0, // '|'
        0xE1,0xE3,0xE1,0xC3,0x87,0x0E,0x1C,0x78,0x70,0xF0,0xE3,0x8F,0x1E,0x1C,0x38,0x70,0xE1,0xCF,0x9E,0x38,0x00 // '}'
};
const GFXglyph Lato_Black_24Glyphs[] PROGMEM = {
// bitmapOffset, width, height, xAdvance, xOffset, yOffset
        {     0,   1,   1,   7,    0,    0 }, // ' '
        {     1,   6,  18,   8,    1,  -18 }, // '!'
        {    15,   9,   7,  11,    1,  -18 }, // '"'
        {    23,  15,  18,  15,    0,  -18 }, // '#'
        {    57,  14,  23,  15,    0,  -20 }, // '$'
        {    98,  19,  18,  21,    1,  -18 }, // '%'
        {   141,  18,  18,  19,    1,  -18 }, // '&'
        {   182,   4,   7,   6,    1,  -18 }, // '''
        {   186,   6,  24,   8,    1,  -20 }, // '('
        {   204,   6,  24,   8,    1,  -20 }, // ')'
        {   222,   9,   9,  11,    1,  -18 }, // '*'
        {   233,  13,  13,  15,    1,  -15 }, // '+'
        {   255,   5,   8,   7,    1,   -4 }, // ','
        {   260,   8,   3,  10,    1,   -9 }, // '-'
        {   263,   5,   4,   7,    1,   -4 }, // '.'
        {   266,  11,  19,  13,    1,  -18 }, // '/'
        {   293,  14,  18,  15,    0,  -18 }, // '0'
        {   325,  13,  18,  15,    1,  -18 }, // '1'
        {   355,  13,  18,  15,    1,  -18 }, // '2'
        {   385,  13,  18,  15,    1,  -18 }, // '3'
        {   415,  14,  18,  15,    0,  -18 }, // '4'
        {   447,  13,  18,  15,    1,  -18 }, // '5'
        {   477,  13,  18,  15,    1,  -18 }, // '6'
        {   507,  13,  18,  15,    1,  -18 }, // '7'
        {   537,  13,  18,  15,    1,  -18 }, // '8'
        {   567,  13,  18,  15,    1,  -18 }, // '9'
        {   597,   5,  13,   7,    1,  -13 }, // ':'
        {   606,   5,  16,   8,    1,  -13 }, // ';'
        {   616,  10,  12,  15,    2,  -14 }, // '<'
        {   631,  11,   7,  15,    2,  -12 }, // '='
        {   641,  10,  12,  15,    3,  -14 }, // '>'
        {   656,  11,  18,  12,    1,  -18 }, // '?'
        {   681,  20,  21,  21,    1,  -18 }, // '@'
        {   734,  18,  18,  18,    0,  -18 }, // 'A'
        {   775,  14,  18,  17,    2,  -18 }, // 'B'
        {   807,  16,  18,  17,    1,  -18 }, // 'C'
        {   843,  17,  18,  19,    2,  -18 }, // 'D'
        {   882,  12,  18,  15,    2,  -18 }, // 'E'
        {   909,  12,  18,  15,    2,  -18 }, // 'F'
        {   936,  16,  18,  18,    1,  -18 }, // 'G'
        {   972,  16,  18,  20,    2,  -18 }, // 'H'
        {  1008,   5,  18,   8,    2,  -18 }, // 'I'
        {  1020,  10,  18,  11,    0,  -18 }, // 'J'
        {  1043,  16,  18,  18,    2,  -18 }, // 'K'
        {  1079,  11,  18,  14,    2,  -18 }, // 'L'
        {  1104,  20,  18,  24,    2,  -18 }, // 'M'
        {  1149,  16,  18,  20,    2,  -18 }, // 'N'
        {  1185,  19,  18,  21,    1,  -18 }, // 'O'
        {  1228,  14,  18,  16,    2,  -18 }, // 'P'
        {  1260,  20,  21,  21,    1,  -18 }, // 'Q'
        {  1313,  15,  18,  17,    2,  -18 }, // 'R'
        {  1347,  14,  18,  14,    0,  -18 }, // 'S'
        {  1379,  15,  18,  16,    0,  -18 }, // 'T'
        {  1413,  17,  18,  19,    1,  -18 }, // 'U'
        {  1452,  18,  18,  18,    0,  -18 }, // 'V'
        {  1493,  27,  18,  27,    0,  -18 }, // 'W'
        {  1554,  18,  18,  18,    0,  -18 }, // 'X'
        {  1595,  17,  18,  17,    0,  -18 }, // 'Y'
        {  1634,  15,  18,  16,    0,  -18 }, // 'Z'
        {  1668,   6,  23,   9,    2,  -19 }, // '['
        {  1686,  11,  19,  13,    1,  -18 }, // '\'
        {  1713,   6,  23,   9,    1,  -19 }, // ']'
        {  1731,  13,   9,  15,    1,  -18 }, // '^'
        {  1746,  10,   3,  12,    1,    1 }, // '_'
        {  1750,   8,   4,  11,    0,  -18 }, // '`'
        {  1754,  12,  13,  13,    0,  -13 }, // 'a'
        {  1774,  13,  18,  15,    1,  -18 }, // 'b'
        {  1804,  11,  13,  13,    1,  -13 }, // 'c'
        {  1822,  13,  18,  15,    1,  -18 }, // 'd'
        {  1852,  12,  13,  14,    1,  -13 }, // 'e'
        {  1872,  10,  18,  10,    0,  -18 }, // 'f'
        {  1895,  14,  17,  14,    0,  -13 }, // 'g'
        {  1925,  13,  18,  15,    1,  -18 }, // 'h'
        {  1955,   5,  18,   7,    1,  -18 }, // 'i'
        {  1967,   7,  22,   7,   -1,  -18 }, // 'j'
        {  1987,  14,  18,  14,    1,  -18 }, // 'k'
        {  2019,   5,  18,   7,    1,  -18 }, // 'l'
        {  2031,  19,  13,  21,    1,  -13 }, // 'm'
        {  2062,  13,  13,  15,    1,  -13 }, // 'n'
        {  2084,  13,  13,  15,    1,  -13 }, // 'o'
        {  2106,  13,  17,  15,    1,  -13 }, // 'p'
        {  2134,  13,  17,  15,    1,  -13 }, // 'q'
        {  2162,   9,  13,  10,    1,  -13 }, // 'r'
        {  2177,  11,  13,  12,    0,  -13 }, // 's'
        {  2195,  10,  17,  10,    0,  -17 }, // 't'
        {  2217,  12,  13,  15,    1,  -13 }, // 'u'
        {  2237,  14,  13,  14,    0,  -13 }, // 'v'
        {  2260,  21,  13,  21,    0,  -13 }, // 'w'
        {  2295,  14,  13,  14,    0,  -13 }, // 'x'
        {  2318,  14,  17,  14,    0,  -13 }, // 'y'
        {  2348,  11,  13,  12,    1,  -13 }, // 'z'
        {  2366,   7,  23,   8,    1,  -19 }, // '{'
        {  2387,   4,  23,   7,    2,  -19 }, // '|'
        {  2399,   7,  23,   8,    1,  -19 } // '}'
};
const GFXfont Lato_Black_24 PROGMEM = {
        (uint8_t  *)Lato_Black_24Bitmaps,(GFXglyph *)Lato_Black_24Glyphs,0x20, 0x7E, 30};


void printReceivedData(OregonTHN128Data_t *data)
{
    bool negativeTemperature = false;
    static uint32_t rxCount = 0;
    int16_t tempAbs;
    char msg[80];

    // Convert to absolute temperature
    tempAbs = data->temperature;
    if (tempAbs < 0) {
        negativeTemperature = true;
        tempAbs *= -1;
    }
    snprintf_P(msg, sizeof(msg),
               PSTR("RX %lu: Rol: %d, Channel %d, Temp: %s%d.%d, Low batt: %d (0x%08lx)"),
               rxCount++,
               data->rollingAddress, data->channel,
               (negativeTemperature ? "-" : ""), (tempAbs / 10), (tempAbs % 10), data->lowBattery,
               data->rawData);
    Serial.println(msg);
}

void setup()
{
    // Initialize serial port
    Serial.begin(115200);
    Serial.println(F("\nErriez Oregon THN128 433MHz temperature receive"));

    // Turn LED on
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.setFont(&Lato_Black_24);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 20);
    display.println("READING");
    display.setCursor(0, 50);
    display.println("...");
    display.display();

    // Initialize receiver
    OregonTHN128_RxBegin(RF_RX_PIN);
}

void loop()
{
    char line[10];
    char tempStr[5];
    OregonTHN128Data_t data;

    // Check temperature received
    if (OregonTHN128_Available()) {
        // Turn LED on
        digitalWrite(LED_BUILTIN, HIGH);

        // Read temperature
        OregonTHN128_Read(&data);

        // Print received data
        printReceivedData(&data);

        // Display data
        OregonTHN128_TempToString(tempStr, sizeof(tempStr), data.temperature);
        snprintf_P(line, sizeof(line), PSTR("CH %d"), data.channel);
        display.clearDisplay();
        display.setCursor(0, 20);
        display.println(line);
        snprintf_P(line, sizeof(line), PSTR("%s`C"), tempStr);
        display.setCursor(0, 50);
        display.println(line);
        display.display();

        // Turn LED off
        digitalWrite(LED_BUILTIN, LOW);

        // Enable receive
        OregonTHN128_RxEnable();
    }
}