/* Copyright (c) 2009, Code Aurora Forum. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Code Aurora Forum nor
 *       the names of its contributors may be used to endorse or promote
 *       products derived from this software without specific prior written
 *       permission.
 *
 * Alternatively, provided that this notice is retained in full, this software
 * may be relicensed by the recipient under the terms of the GNU General Public
 * License version 2 ("GPL") and only version 2, in which case the provisions of
 * the GPL apply INSTEAD OF those given above.  If the recipient relicenses the
 * software under the GPL, then the identification text in the MODULE_LICENSE
 * macro must be changed to reflect "GPLv2" instead of "Dual BSD/GPL".  Once a
 * recipient changes the license terms to the GPL, subsequent recipients shall
 * not relicense under alternate licensing terms, including the BSD or dual
 * BSD/GPL terms.  In addition, the following license statement immediately
 * below and between the words START and END shall also then apply when this
 * software is relicensed under the GPL:
 *
 * START
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License version 2 and only version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * END
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <linux/delay.h>
#include <linux/mfd/pmic8058.h>
#include <mach/gpio.h>
#include "msm_fb.h"

#define LCDC_DEBUG

#ifdef LCDC_DEBUG
#define DPRINT(x...)	printk("ld9040 " x)
#else
#define DPRINT(x...)	
#endif

/*
 * Serial Interface
 */
#define LCD_CSX_HIGH	gpio_set_value(spi_cs, 1);
#define LCD_CSX_LOW	gpio_set_value(spi_cs, 0);

#define LCD_SCL_HIGH	gpio_set_value(spi_sclk, 1);
#define LCD_SCL_LOW		gpio_set_value(spi_sclk, 0);

#define LCD_SDI_HIGH	gpio_set_value(spi_sdi, 1);
#define LCD_SDI_LOW		gpio_set_value(spi_sdi, 0);

#define DEFAULT_USLEEP	1	

struct setting_table {
	unsigned char command;	
	unsigned char parameters;
	unsigned char parameter[45];
	long wait;
};

struct setting_table lcd_brightness_table_2[] =  {
//30 _01
 {0xF9, 21, 
  { 0x00, 0xD9 ,0xD3, 0xCC   ,0xDA   ,0x00  ,0x46 ,0x00 , 0xB1 ,0xC9, 0xC9 , 0xD9 ,0x00,0x58 ,0x00 , 0xD2 , 0xD0, 0xC9 , 0xD8 ,0x00,0x5E }, 0 },
//40 _02
 {0xF9, 21, 
  { 0x00 ,0xD9   ,0xCE  ,0xCA   ,0xD9   ,0x00  ,0x4F,0x00 , 0xB5 , 0xCA, 0xC7 , 0xD8 ,0x00,0x61 ,0x00 , 0xD2 , 0xCE, 0xC6 , 0xD6 , 0x00,0x69 } , 0},
//50_03
 {0xF9, 21, 
  { 0x00   ,0xD6   ,0xCE  ,0xCA   ,0xD8   ,0x00  ,0x55 ,0x00 , 0xB9 , 0xCA, 0xC6 , 0xD6 ,0x00,0x69 ,0x00, 0xD1 , 0xCC, 0xC6 , 0xD5 ,0x00,0x71}, 0}, 
//60_04
{0xF9, 21, 
  { 0x00 ,0xD5  ,0xCD ,0xC8  ,0xD7  ,0x00 ,0x5B, 0x00 , 0xBA , 0xCA, 0xC5 , 0xD4 ,0x00 ,0x71 , 0x00, 0xD0 , 0xCC, 0xC5 , 0xD3 , 0x00, 0x78}, 0},
//70_05
{0xF9, 21,
  { 0x00  ,0xD4  ,0xCD ,0xC7  ,0xD7  ,0x00 ,0x60 , 0x00 , 0xBC , 0xCB, 0xC4 , 0xD3 ,0x00,0x76 , 0x00 , 0xCF , 0xCC, 0xC3 , 0xD2 ,0x00,0x7F  }, 0},
//80_06
{0xF9, 21,
  { 0x00  ,0xD2  ,0xCB ,0xC6  ,0xD7  ,0x00 ,0x65 , 0x00 , 0xC0 , 0xC9, 0xC3 , 0xD2 ,0x00,0x7C , 0x00 , 0xCE , 0xCA, 0xC2 , 0xD1 ,0x00,0x85  }, 0},
//90_07
{0xF9, 21,
  { 0x00  ,0xD2  ,0xCA ,0xC5  ,0xD4  ,0x00 ,0x6A , 0x00 , 0xC0 , 0xC8, 0xC2 , 0xD0 ,0x00,0x82, 0x00 , 0xCE , 0xC9, 0xC0 , 0xCF ,0x00 ,0x8B  }, 0},
//100_08
{0xF9, 21,
  { 0x00 ,0xD1  ,0xCB ,0xC5  ,0xD4  ,0x00 ,0x6F  , 0x00  , 0xC2 , 0xC8, 0xC2 , 0xCF ,0x00,0x88 , 0x00 , 0xCE , 0xC9, 0xC0 , 0xCE ,0x00,0x92  }, 0},
//110_09
{0xF9, 21,
  { 0x00  ,0xD0  ,0xCB ,0xC3  ,0xD3  ,0x00 ,0x74 , 0x00 , 0xC2 , 0xC8, 0xC1 , 0xCE ,0x00,0x8D , 0x00 , 0xCD , 0xC8, 0xBF , 0xCC ,0x00,0x98  }, 0},
//120_10
{0xF9, 21,
 { 0x00  ,0xD0  ,0xCB ,0xC2  ,0xD3  ,0x00, 0x77 , 0x00 , 0xC2 , 0xC8, 0xC0 , 0xCD ,0x00,0x92 , 0x00 , 0xCD , 0xC8, 0xBE , 0xCC ,0x00,0x9C  }, 0},
//130_11
{0xF9, 21,
  { 0x00  ,0xCF  ,0xCB ,0xC2  ,0xD1  ,0x00 ,0x7B , 0x00 , 0xC2 , 0xC7, 0xBF , 0xCC ,0x00,0x97 , 0x00 , 0xCC , 0xC8, 0xBE , 0xCB ,0x00,0xA1  }, 0},
//140_12
{0xF9, 21,
  { 0x00  ,0xCF  ,0xCA ,0xC1  ,0xD0  ,0x00 ,0x7F , 0x00 , 0xC3 , 0xC6, 0xBF , 0xCB ,0x00,0x9B , 0x00 , 0xCC , 0xC8, 0xBD , 0xC9 ,0x00,0xA6  }, 0},
//150_13
{0xF9, 21,
  { 0x00  ,0xCF  ,0xC9 ,0xC1  ,0xD1  ,0x00 ,0x82 , 0x00 , 0xC3 , 0xC6, 0xBE , 0xCB ,0x00,0x9F , 0x00 , 0xCC , 0xC6, 0xBB , 0xC9 ,0x00,0xAB  }, 0},
//160_14
{0xF9, 21,
  { 0x00  ,0xCE  ,0xC9 ,0xC1  ,0xCF  ,0x00 ,0x85 , 0x00 , 0xC2 , 0xC6, 0xBD , 0xCA ,0x00,0xA3 , 0x00 , 0xCB , 0xC6, 0xBB , 0xC8 ,0x00,0xAF  }, 0},
//170_15
{0xF9, 21,
  { 0x00 ,0xCE  ,0xC7 ,0xC1  ,0xCF  ,0x00 ,0x88  , 0x00 , 0xC3 , 0xC5, 0xBC, 0xCA ,0x00,0xA7 , 0x00 , 0xCB , 0xC5 , 0xBB , 0xC7 ,0x00,0xB3  }, 0},
 //180_16
{0xF9, 21,
  { 0x00  ,0xCE  ,0xC7 ,0xC1  ,0xCD  ,0x00 ,0x8C , 0x00 , 0xC4 , 0xC5, 0xBB , 0xC8 ,0x00,0xAC , 0x00 , 0xCA , 0xC4, 0xBB , 0xC6 ,0x00,0xB8  }, 0},
  //190_17
{0xF9, 21,
  { 0x00 ,0xCE  ,0xC8 ,0xBF  ,0xCD  ,0x00 ,0x8F  , 0x00 , 0xC3 , 0xC5, 0xBA , 0xC8 ,0x00,0xAF , 0x00 , 0xCA , 0xC5, 0xBA , 0xC5 ,0x00,0xBC  }, 0},
//200_18
{0xF9, 21,
  { 0x00  ,0xCF  ,0xC7 ,0xBE  ,0xCD  ,0x00 ,0x92 , 0x00 , 0xC3 , 0xC5, 0xBA , 0xC7 ,0x00,0xB3 , 0x00 , 0xCB , 0xC5, 0xB9 , 0xC4 ,0x00,0xC0  }, 0},
//210_19
{0xF9, 21,
  { 0x00  ,0xCE  ,0xC7 ,0xBD  ,0xCC  ,0x00 ,0x95 , 0x00 , 0xC3 , 0xC4, 0xBA , 0xC6 ,0x00,0xB7 , 0x00 , 0xCA , 0xC5, 0xB8 , 0xC4 ,0x00,0xC3  }, 0},
//220_20
{0xF9, 21,
  { 0x00  ,0xCD  ,0xC6 ,0xBD  ,0xCC  ,0x00 ,0x98 , 0x00 , 0xC3 , 0xC5, 0xB9 , 0xC5 ,0x00,0xBA , 0x00 , 0xC9 , 0xC4, 0xB7 , 0xC4 ,0x00,0xC7  }, 0},
  //230_21
{0xF9, 21,
  { 0x00  ,0xCD  ,0xC6 ,0xBD  ,0xCA  ,0x00 ,0x9B , 0x00 , 0xC3 , 0xC4, 0xB9 , 0xC4 ,0x00,0xBE , 0x00 , 0xCA , 0xC4, 0xB6 , 0xC3 ,0x00,0xCB  }, 0},
//240_22
{0xF9, 21,
  { 0x00  ,0xCC  ,0xC5 ,0xBD  ,0xCA  ,0x00 ,0x9E , 0x00 , 0xC4 , 0xC4, 0xB8 , 0xC4 ,0x00,0xC1 , 0x00 , 0xC9 , 0xC3, 0xB6 , 0xC2 ,0x00,0xCF  }, 0},
//250_23
{0xF9, 21,
  { 0x00  ,0xCC  ,0xC5 ,0xBD  ,0xCA  ,0x00 ,0xA0 , 0x00 , 0xC3 , 0xC3, 0xB8 , 0xC4,0x00,0xC4 , 0x00 , 0xC8 , 0xC2, 0xB5  , 0xC3 ,0x00,0xD2  }, 0},
//260_24
{0xF9, 21,
  { 0x00  ,0xCD  ,0xC5 ,0xB6  ,0xC8  ,0x00 ,0xA3 , 0x00 , 0xC3 , 0xC3, 0xB6 , 0xC3 ,0x00,0xC8 , 0x00 , 0xC8 , 0xC3, 0xB4 , 0xC2 ,0x00,0xD5  }, 0},
//270_25
{0xF9, 21,
  { 0x00  ,0xCC  ,0xC4 ,0xBB  ,0xC7  ,0x00 ,0xA3 , 0x00 , 0xC3 , 0xC2, 0xB6 , 0xC2 ,0x00,0xC8 , 0x00 , 0xC8 , 0xC1, 0xB3 , 0xC1 ,0x00,0xD6  }, 0},
//280_26
{0xF9, 21,
  { 0x00  ,0xCC  ,0xC5 ,0xBA  ,0xC6  ,0x00 ,0xA7 , 0x00 , 0xC3 , 0xC1, 0xB6 , 0xC2 ,0x00,0xCB , 0x00 , 0xC7 , 0xC2, 0xB4 , 0xBF ,0x00,0xDA  }, 0},
//290_27
{0xF9, 21,
  { 0x00  ,0xCB  ,0xC5 ,0xB8  ,0xC6  ,0x00 ,0xAA , 0x00 , 0xC3 , 0xC2, 0xB5 , 0xC1 ,0x00,0xCF , 0x00 , 0xC7 , 0xC2, 0xB2 , 0xBF ,0x00,0xDE  }, 0},
//300_28
{0xF9, 21,
  { 0x00  ,0xCA  ,0xC4 ,0xB8  ,0xC6  ,0x00 ,0xAB , 0x00 , 0xC3 , 0xC1, 0xB4 , 0xC0 ,0x00,0xD1 , 0x00 , 0xC6 , 0xC2, 0xB1 , 0xBF ,0x00,0xDF }, 0},
};

struct setting_table lcd_brightness_table[] =  {
//30 _01
 {0xF9, 21, 
  { 0x00, 0xD9 ,0xD3, 0xCC   ,0xDA   ,0x00  ,0x46 ,0x00 , 0xB1 ,0xC9, 0xC9 , 0xD9 ,0x00,0x58 ,0x00 , 0xD2 , 0xD0, 0xC9 , 0xD8 ,0x00,0x5E }, 0 },
//40 _02
 {0xF9, 21, 
  { 0x00 ,0xD9   ,0xCE  ,0xCA   ,0xD9   ,0x00  ,0x4F,0x00 , 0xB5 , 0xCA, 0xC7 , 0xD8 ,0x00,0x61 ,0x00 , 0xD2 , 0xCE, 0xC6 , 0xD6 , 0x00,0x69 } , 0},
//50_03
 {0xF9, 21, 
  { 0x00   ,0xD6   ,0xCE  ,0xCA   ,0xD8   ,0x00  ,0x55 ,0x00 , 0xB9 , 0xCA, 0xC6 , 0xD6 ,0x00,0x69 ,0x00, 0xD1 , 0xCC, 0xC6 , 0xD5 ,0x00,0x71}, 0}, 
//60_04
{0xF9, 21, 
  { 0x00 ,0xD5  ,0xCD ,0xC8  ,0xD7  ,0x00 ,0x5B, 0x00 , 0xBA , 0xCA, 0xC5 , 0xD4 ,0x00 ,0x71 , 0x00, 0xD0 , 0xCC, 0xC5 , 0xD3 , 0x00, 0x78}, 0},
//70_05
{0xF9, 21,
  { 0x00  ,0xD4  ,0xCD ,0xC7  ,0xD7  ,0x00 ,0x60 , 0x00 , 0xBC , 0xCB, 0xC4 , 0xD3 ,0x00,0x76 , 0x00 , 0xCF , 0xCC, 0xC3 , 0xD2 ,0x00,0x7F  }, 0},
//80_06
{0xF9, 21,
  { 0x00  ,0xD2  ,0xCB ,0xC6  ,0xD7  ,0x00 ,0x65 , 0x00 , 0xC0 , 0xC9, 0xC3 , 0xD2 ,0x00,0x7C , 0x00 , 0xCE , 0xCA, 0xC2 , 0xD1 ,0x00,0x85  }, 0},
//90_07
{0xF9, 21,
  { 0x00  ,0xD2  ,0xCA ,0xC5  ,0xD4  ,0x00 ,0x6A , 0x00 , 0xC0 , 0xC8, 0xC2 , 0xD0 ,0x00,0x82, 0x00 , 0xCE , 0xC9, 0xC0 , 0xCF ,0x00 ,0x8B  }, 0},
//100_08
{0xF9, 21,
  { 0x00 ,0xD1  ,0xCB ,0xC5  ,0xD4  ,0x00 ,0x6F  , 0x00  , 0xC2 , 0xC8, 0xC2 , 0xCF ,0x00,0x88 , 0x00 , 0xCE , 0xC9, 0xC0 , 0xCE ,0x00,0x92  }, 0},
//110_09
{0xF9, 21,
  { 0x00  ,0xD0  ,0xCB ,0xC3  ,0xD3  ,0x00 ,0x74 , 0x00 , 0xC2 , 0xC8, 0xC1 , 0xCE ,0x00,0x8D , 0x00 , 0xCD , 0xC8, 0xBF , 0xCC ,0x00,0x98  }, 0},
//120_10
{0xF9, 21,
 { 0x00  ,0xD0  ,0xCB ,0xC2  ,0xD3  ,0x00, 0x77 , 0x00 , 0xC2 , 0xC8, 0xC0 , 0xCD ,0x00,0x92 , 0x00 , 0xCD , 0xC8, 0xBE , 0xCC ,0x00,0x9C  }, 0},
//130_11
{0xF9, 21,
  { 0x00  ,0xCF  ,0xCB ,0xC2  ,0xD1  ,0x00 ,0x7B , 0x00 , 0xC2 , 0xC7, 0xBF , 0xCC ,0x00,0x97 , 0x00 , 0xCC , 0xC8, 0xBE , 0xCB ,0x00,0xA1  }, 0},
//140_12
{0xF9, 21,
  { 0x00  ,0xCF  ,0xCA ,0xC1  ,0xD0  ,0x00 ,0x7F , 0x00 , 0xC3 , 0xC6, 0xBF , 0xCB ,0x00,0x9B , 0x00 , 0xCC , 0xC8, 0xBD , 0xC9 ,0x00,0xA6  }, 0},
//150_13
{0xF9, 21,
  { 0x00  ,0xCF  ,0xC9 ,0xC1  ,0xD1  ,0x00 ,0x82 , 0x00 , 0xC3 , 0xC6, 0xBE , 0xCB ,0x00,0x9F , 0x00 , 0xCC , 0xC6, 0xBB , 0xC9 ,0x00,0xAB  }, 0},
//160_14
{0xF9, 21,
  { 0x00  ,0xCE  ,0xC9 ,0xC1  ,0xCF  ,0x00 ,0x85 , 0x00 , 0xC2 , 0xC6, 0xBD , 0xCA ,0x00,0xA3 , 0x00 , 0xCB , 0xC6, 0xBB , 0xC8 ,0x00,0xAF  }, 0},
//170_15
{0xF9, 21,
  { 0x00 ,0xCE  ,0xC7 ,0xC1  ,0xCF  ,0x00 ,0x88  , 0x00 , 0xC3 , 0xC5, 0xBC, 0xCA ,0x00,0xA7 , 0x00 , 0xCB , 0xC5 , 0xBB , 0xC7 ,0x00,0xB3  }, 0},
 //180_16
{0xF9, 21,
  { 0x00  ,0xCE  ,0xC7 ,0xC1  ,0xCD  ,0x00 ,0x8C , 0x00 , 0xC4 , 0xC5, 0xBB , 0xC8 ,0x00,0xAC , 0x00 , 0xCA , 0xC4, 0xBB , 0xC6 ,0x00,0xB8  }, 0},
  //190_17
{0xF9, 21,
  { 0x00 ,0xCE  ,0xC8 ,0xBF  ,0xCD  ,0x00 ,0x8F  , 0x00 , 0xC3 , 0xC5, 0xBA , 0xC8 ,0x00,0xAF , 0x00 , 0xCA , 0xC5, 0xBA , 0xC5 ,0x00,0xBC  }, 0},
//200_18
{0xF9, 21,
  { 0x00  ,0xCF  ,0xC7 ,0xBE  ,0xCD  ,0x00 ,0x92 , 0x00 , 0xC3 , 0xC5, 0xBA , 0xC7 ,0x00,0xB3 , 0x00 , 0xCB , 0xC5, 0xB9 , 0xC4 ,0x00,0xC0  }, 0},
//210_19
{0xF9, 21,
  { 0x00  ,0xCE  ,0xC7 ,0xBD  ,0xCC  ,0x00 ,0x95 , 0x00 , 0xC3 , 0xC4, 0xBA , 0xC6 ,0x00,0xB7 , 0x00 , 0xCA , 0xC5, 0xB8 , 0xC4 ,0x00,0xC3  }, 0},
//220_20
{0xF9, 21,
  { 0x00  ,0xCD  ,0xC6 ,0xBD  ,0xCC  ,0x00 ,0x98 , 0x00 , 0xC3 , 0xC5, 0xB9 , 0xC5 ,0x00,0xBA , 0x00 , 0xC9 , 0xC4, 0xB7 , 0xC4 ,0x00,0xC7  }, 0},
  //230_21
{0xF9, 21,
  { 0x00  ,0xCD  ,0xC6 ,0xBD  ,0xCA  ,0x00 ,0x9B , 0x00 , 0xC3 , 0xC4, 0xB9 , 0xC4 ,0x00,0xBE , 0x00 , 0xCA , 0xC4, 0xB6 , 0xC3 ,0x00,0xCB  }, 0},
//240_22
{0xF9, 21,
  { 0x00  ,0xCC  ,0xC5 ,0xBD  ,0xCA  ,0x00 ,0x9E , 0x00 , 0xC4 , 0xC4, 0xB8 , 0xC4 ,0x00,0xC1 , 0x00 , 0xC9 , 0xC3, 0xB6 , 0xC2 ,0x00,0xCF  }, 0},
//250_23
{0xF9, 21,
  { 0x00  ,0xCC  ,0xC5 ,0xBD  ,0xCA  ,0x00 ,0xA0 , 0x00 , 0xC3 , 0xC3, 0xB8 , 0xC4,0x00,0xC4 , 0x00 , 0xC8 , 0xC2, 0xB5  , 0xC3 ,0x00,0xD2  }, 0},
//260_24
{0xF9, 21,
  { 0x00  ,0xCD  ,0xC5 ,0xB6  ,0xC8  ,0x00 ,0xA3 , 0x00 , 0xC3 , 0xC3, 0xB6 , 0xC3 ,0x00,0xC8 , 0x00 , 0xC8 , 0xC3, 0xB4 , 0xC2 ,0x00,0xD5  }, 0},
//270_25
{0xF9, 21,
  { 0x00  ,0xCC  ,0xC4 ,0xBB  ,0xC7  ,0x00 ,0xA3 , 0x00 , 0xC3 , 0xC2, 0xB6 , 0xC2 ,0x00,0xC8 , 0x00 , 0xC8 , 0xC1, 0xB3 , 0xC1 ,0x00,0xD6  }, 0},
//280_26
{0xF9, 21,
  { 0x00  ,0xCC  ,0xC5 ,0xBA  ,0xC6  ,0x00 ,0xA7 , 0x00 , 0xC3 , 0xC1, 0xB6 , 0xC2 ,0x00,0xCB , 0x00 , 0xC7 , 0xC2, 0xB4 , 0xBF ,0x00,0xDA  }, 0},
//290_27
{0xF9, 21,
  { 0x00  ,0xCB  ,0xC5 ,0xB8  ,0xC6  ,0x00 ,0xAA , 0x00 , 0xC3 , 0xC2, 0xB5 , 0xC1 ,0x00,0xCF , 0x00 , 0xC7 , 0xC2, 0xB2 , 0xBF ,0x00,0xDE  }, 0},
//300_28
{0xF9, 21,
  { 0x00  ,0xCA  ,0xC4 ,0xB8  ,0xC6  ,0x00 ,0xAB , 0x00 , 0xC3 , 0xC1, 0xB4 , 0xC0 ,0x00,0xD1 , 0x00 , 0xC6 , 0xC2, 0xB1 , 0xBF ,0x00,0xDF }, 0},
};

static struct setting_table sleep_out_display[] = {
        // Gamma Update
        { 0xFB, 2,
                { 0x02, 0x5A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
        0 },
};

static struct setting_table power_on_sequence_2[] = {
	// Level 2 Command Access
	{ 0xF0,	2, 
	  	{ 0x5A, 0x5A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
	0 },

	// GTCON
	{ 0xF7,	4, 
		{ 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
	0 },	

	// Panel Condition Set
	{ 0xF8,	23, 
		{ 0x05, 0x60, 0x99, 0x6d, 0x80, 0x33, 0x4D, 0x00, 0x00, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  	    0x07, 0x07, 0x20, 0x20, 0x20, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
	0 },

	// Display Condition Set
	{ 0xF2,	5, 
		{ 0x02, 0x03, 0x1C, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
	0 },	

	// Gamma Setting ( below setting is 300cd, 8500K )
#if 1 // CHECK_LATER
	{ 0xF9,	21, 
		{ 0x00, 0xB0, 0xB7, 0xB9, 0xC5, 0x00, 0x65, 0x00, 0xA5, 0xB3, 0xB5, 0xC2, 0x00, 0x89, 0x00, 0xAA, 
		    0xB2, 0xB3, 0xC2, 0x00, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	  	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
	0 },
#else
	{ 0xF9,	21, 
		{ 0x00, 0xC7, 0xC4, 0xBB, 0xC9, 0x00, 0xA3, 0x00, 0xB5, 0xC0, 0xB7, 0xC7, 0x00, 0xB4, 0x00, 0xBD,
	  	    0xBF, 0xB5, 0xC6, 0x00, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
	0 },
#endif
	// Gamma Update
	{ 0xFB,	2, 
		{ 0x02, 0x5A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
	0 },	

	// Dynamic ELVSS Set
	{ 0xB1,	3, 
		{ 0x0D, 0x00, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
	0 },	
	{ 0xB2,	4, 
		{ 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
	0 },	

	// Sleep Out Command
	{ 0x11,	0, 
	  	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
	120 },	

	// Display On Command
	{ 0x29,	0, 
	  	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
	10 },
};

#define POWER_ON_SEQ	(int)(sizeof(power_on_sequence)/sizeof(struct setting_table))
#define POWER_ON_SEQ_2	(int)(sizeof(power_on_sequence_2)/sizeof(struct setting_table))

static struct setting_table power_off_sequence[] = {
	// Display Off Command
	{ 0x28,	0, 
	  	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
	0 },	

	// Sleep In Command
	{ 0x10,	0, 
	  	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
	130 },
};
#define POWER_OFF_SEQ	(int)(sizeof(power_off_sequence)/sizeof(struct setting_table))

static struct setting_table sleep_out_sequence[] = {
	{ 0x11,	0, 
	  	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
	130 },
	{ 0x29,	0, 
	  	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
	5 },
};
#define SLEEP_OUT_SEQ	(int)(sizeof(sleep_out_sequence)/sizeof(struct setting_table))

static struct setting_table sleep_in_sequence[] = {
	{ 0x28,	0, 
	  	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
	0 },
	{ 0x10,	0, 
	  	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  	    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
	130 },
};

#define SLEEP_IN_SEQ	(int)(sizeof(sleep_in_sequence)/sizeof(struct setting_table))

static int lcdc_ld9040_panel_off(struct platform_device *pdev);

static int lcd_brightness = -1;

static int spi_cs;
static int spi_sclk;
//static int spi_sdo;
static int spi_sdi;
//static int spi_dac;
static int lcd_reset;

static int delayed_backlight_value = -1;
static void lcdc_ld9040_set_brightness(int level);

struct ld9040_state_type{
	boolean disp_initialized = TRUE;
	boolean display_on=TRUE;
	boolean disp_powered_up=TRUE;
};

static struct ld9040_state_type ld9040_state = { 0 };
static struct msm_panel_common_pdata *lcdc_ld9040_pdata;

static void setting_table_write(struct setting_table *table)
{
	long i, j;

	LCD_CSX_HIGH
	udelay(DEFAULT_USLEEP);
	LCD_SCL_HIGH 
	udelay(DEFAULT_USLEEP);

	/* Write Command */
	LCD_CSX_LOW
	udelay(DEFAULT_USLEEP);
	LCD_SCL_LOW 
	udelay(DEFAULT_USLEEP);		
	LCD_SDI_LOW 
	udelay(DEFAULT_USLEEP);
	
	LCD_SCL_HIGH 
	udelay(DEFAULT_USLEEP); 

   	for (i = 7; i >= 0; i--) { 
		LCD_SCL_LOW
		udelay(DEFAULT_USLEEP);
		if ((table->command >> i) & 0x1)
			LCD_SDI_HIGH
		else
			LCD_SDI_LOW
		udelay(DEFAULT_USLEEP);	
		LCD_SCL_HIGH
		udelay(DEFAULT_USLEEP);	
	}

	LCD_CSX_HIGH
	udelay(DEFAULT_USLEEP);	

	/* Write Parameter */
	if ((table->parameters) > 0) {
		for (j = 0; j < table->parameters; j++) {
			LCD_CSX_LOW 
			udelay(DEFAULT_USLEEP); 	
			
			LCD_SCL_LOW 
			udelay(DEFAULT_USLEEP); 	
			LCD_SDI_HIGH 
			udelay(DEFAULT_USLEEP);
			LCD_SCL_HIGH 
			udelay(DEFAULT_USLEEP); 	

			for (i = 7; i >= 0; i--) { 
				LCD_SCL_LOW
				udelay(DEFAULT_USLEEP);	
				if ((table->parameter[j] >> i) & 0x1)
					LCD_SDI_HIGH
				else
					LCD_SDI_LOW
				udelay(DEFAULT_USLEEP);	
				LCD_SCL_HIGH
				udelay(DEFAULT_USLEEP);					
			}

			LCD_CSX_HIGH
			udelay(DEFAULT_USLEEP);	
		}
	}

	msleep(table->wait);
}

static void spi_init(void)
{
	/* Setting the Default GPIO's */
	spi_sclk = *(lcdc_ld9040_pdata->gpio_num);
	spi_cs   = *(lcdc_ld9040_pdata->gpio_num + 1);
	spi_sdi  = *(lcdc_ld9040_pdata->gpio_num + 2);
	lcd_reset  = *(lcdc_ld9040_pdata->gpio_num + 3);

	/* Set the output so that we dont disturb the slave device */
	gpio_set_value(spi_sclk, 0);
	gpio_set_value(spi_sdi, 0);

	/* Set the Chip Select De-asserted */
	gpio_set_value(spi_cs, 0);

}

static void ld9040_disp_powerup(void)
{
	DPRINT("start %s\n", __func__);	

	if (!ld9040_state.disp_powered_up && !ld9040_state.display_on) {
		/* Reset the hardware first */

		//TODO: turn on ldo
		msleep(50);
		
		//LCD_RESET_N_HI
		gpio_set_value(lcd_reset, 1);
		msleep(20);
		//LCD_RESET_N_LO
		gpio_set_value(lcd_reset, 0);
		msleep(20);	
		//LCD_RESET_N_HI
		gpio_set_value(lcd_reset, 1);
		msleep(20);

		
		/* Include DAC power up implementation here */
		
	    ld9040_state.disp_powered_up = TRUE;
	}
}

static void ld9040_disp_powerdown(void)
{
	DPRINT("start %s\n", __func__);	

	/* Reset Assert */
	gpio_set_value(lcd_reset, 0);		

	/* turn off LDO */
	//TODO: turn off LDO

	ld9040_state.disp_powered_up = FALSE;
}

static void ld9040_init(void)
{
	mdelay(1);
}

void ld9040_disp_on(void)
{
	int i;

	DPRINT("start %s\n", __func__);	

	if (ld9040_state.disp_powered_up && !ld9040_state.display_on) {
		ld9040_init();
		mdelay(20);

		/* ld9040 setting */
//		if (system_rev >=6 ) 
//		{
			for (i = 0; i < POWER_ON_SEQ_2; i++)
				setting_table_write(&power_on_sequence_2[i]);			
//		}
//		else
//		{
//			for (i = 0; i < POWER_ON_SEQ; i++)
//				setting_table_write(&power_on_sequence[i]);			
//		}

		mdelay(1);
		ld9040_state.display_on = TRUE;
	}
}

void ld9040_sleep_off(void)
{
	int i;

	DPRINT("start %s\n", __func__);	


	for (i = 0; i < POWER_ON_SEQ_2; i++)
				setting_table_write(&power_on_sequence_2[i]);	
	
	mdelay(1);
}

void ld9040_sleep_in(void)
{
	int i;

	DPRINT("start %s\n", __func__); 


		for (i = 0; i < POWER_OFF_SEQ; i++)
			setting_table_write(&power_off_sequence[i]);			

	mdelay(1);
	
}

extern void key_led_control(int on);

static int lcdc_ld9040_panel_on(struct platform_device *pdev)
{
	DPRINT("start %s\n", __func__);	

	if (!ld9040_state.disp_initialized) {
		/* Configure reset GPIO that drives DAC */
		lcdc_ld9040_pdata->panel_config_gpio(1);
		spi_init();	/* LCD needs SPI */
		ld9040_disp_powerup();
		ld9040_disp_on();
		ld9040_state.disp_initialized = TRUE;

		if ( system_rev >= 12 ) // TEMP
			key_led_control(1);
	}

	return 0;
}

static int lcdc_ld9040_panel_off(struct platform_device *pdev)
{
	int i;

	DPRINT("start %s\n", __func__);	

	if ( system_rev >= 12 )	// TEMP
		key_led_control(0);

	if (ld9040_state.disp_powered_up && ld9040_state.display_on) {
		
		for (i = 0; i < POWER_OFF_SEQ; i++)
			setting_table_write(&power_off_sequence[i]);	
		
		lcdc_ld9040_pdata->panel_config_gpio(0);
		ld9040_state.display_on = FALSE;
		ld9040_state.disp_initialized = FALSE;
		ld9040_disp_powerdown();
	}
	return 0;
}

// brightness tuning
#define MAX_BRIGHTNESS_LEVEL 255
#define LOW_BRIGHTNESS_LEVEL 20
#define MAX_BACKLIGHT_VALUE 27 
#define LOW_BACKLIGHT_VALUE 1
#define DIM_BACKLIGHT_VALUE 0
#define BRIGHTNESS_LEVEL_DIVIDER 9
static DEFINE_SPINLOCK(bl_ctrl_lock);

static void lcdc_ld9040_set_brightness(int level)
{
	unsigned long irqflags;
	int tune_level = level;
	int i;
 	// LCD should be turned on prior to backlight
	if(ld9040_state.disp_initialized == FALSE && tune_level > 0) {
		delayed_backlight_value = tune_level;
		return;
	}
	else {
		delayed_backlight_value = -1;
	}
 
	//TODO: lock
       //spin_lock_irqsave(&bl_ctrl_lock, irqflags);

       lcd_brightness = tune_level;
	   
        if (tune_level <= 0) 
	{
               	setting_table_write(&lcd_brightness_table_2[0]);
	} 
	else 
	{
		// keep back light ON
		if(unlikely(lcd_brightness < 0)) {
			lcd_brightness = 0;
		}

               	setting_table_write(&lcd_brightness_table_2[tune_level]);
		
               ld9040_state.display_on = TRUE;

	}

	//gamma_update
	setting_table_write(&sleep_out_display[0]);

	//TODO: unlock
	//spin_unlock_irqrestore(&bl_ctrl_lock, irqflags);
}

static void lcdc_ld9040_set_backlight(struct msm_fb_data_type *mfd)
{	
	int bl_level = mfd->bl_level;
	int tune_level;

	// brightness tuning
	if(bl_level > LOW_BRIGHTNESS_LEVEL){
		tune_level = (bl_level - LOW_BRIGHTNESS_LEVEL) / BRIGHTNESS_LEVEL_DIVIDER + 1; 
		if(tune_level==2)
			tune_level=1;
	}
	else if(bl_level > 0)
		tune_level = DIM_BACKLIGHT_VALUE;
	else
		tune_level = bl_level;

       printk("brightness!!! bl_level=%d, tune_level=%d \n",bl_level,tune_level);

#if 1//sspark 
 	if ((ld9040_state.disp_initialized) 
		&& (ld9040_state.disp_powered_up) 
		&& (ld9040_state.display_on))
	{
		lcdc_ld9040_set_brightness(tune_level);
		return;
	}
#endif
	// turn on lcd if needed
#if 0
	if(tune_level > 0)	{
		if(!ld9040_state.disp_powered_up)
			ld9040_disp_powerup();
		if(!ld9040_state.display_on)
			ld9040_disp_on();
	}
	lcdc_ld9040_set_brightness(tune_level);
#endif
}

static int __init ld9040_probe(struct platform_device *pdev)
{
	DPRINT("start %s\n", __func__);	

	if (pdev->id == 0) {
		lcdc_ld9040_pdata = pdev->dev.platform_data;
		return 0;
	}
	msm_fb_add_device(pdev);
	
	return 0;
}

static void ld9040_shutdown(struct platform_device *pdev)
{
	DPRINT("start %s\n", __func__);	

	lcdc_ld9040_panel_off(pdev);
}

static struct platform_driver this_driver = {
	.probe  = ld9040_probe,
	.shutdown	= ld9040_shutdown,
	.driver = {
		.name   = "lcdc_ld9040_wvga",
	},
};

static struct msm_fb_panel_data ld9040_panel_data = {
	.on = lcdc_ld9040_panel_on,
	.off = lcdc_ld9040_panel_off,
	.set_backlight = lcdc_ld9040_set_backlight,
};

static struct platform_device this_device = {
	.name   = "lcdc_panel",
	.id	= 1,
	.dev	= {
		.platform_data = &ld9040_panel_data,
	}
};

#define LCDC_FB_XRES	480
#define LCDC_FB_YRES	800
#define LCDC_HPW		2
#define LCDC_HBP		16
#define LCDC_HFP		16
#if 0
#define LCDC_VPW		2
#define LCDC_VBP			3
#else 
#define LCDC_VPW		2
#define LCDC_VBP		1
#endif
#define LCDC_VFP		28
 
static int __init lcdc_ld9040_panel_init(void)
{
	int ret;
	struct msm_panel_info *pinfo;

#ifdef CONFIG_FB_MSM_MDDI_AUTO_DETECT
	if (msm_fb_detect_client("lcdc_ld9040_wvga"))
	{
		printk(KERN_ERR "%s: msm_fb_detect_client failed!\n", __func__); 
		return 0;
	}
#endif
	DPRINT("start %s\n", __func__);	
	
	ret = platform_driver_register(&this_driver);
	if (ret)
	{
		printk(KERN_ERR "%s: platform_driver_register failed! ret=%d\n", __func__, ret); 
		return ret;
	}

	pinfo = &ld9040_panel_data.panel_info;
	pinfo->xres = LCDC_FB_XRES;
	pinfo->yres = LCDC_FB_YRES;
	pinfo->type = LCDC_PANEL;
	pinfo->pdest = DISPLAY_1;
	pinfo->wait_cycle = 0;
	pinfo->bpp = 24;
	pinfo->fb_num = 2;
	pinfo->clk_rate = 24576000;
	pinfo->bl_max = 255;
	pinfo->bl_min = 1;

	pinfo->lcdc.h_back_porch = LCDC_HBP;
	pinfo->lcdc.h_front_porch = LCDC_HFP;
	pinfo->lcdc.h_pulse_width = LCDC_HPW;
	pinfo->lcdc.v_back_porch = LCDC_VBP;
	pinfo->lcdc.v_front_porch = LCDC_VFP;
	pinfo->lcdc.v_pulse_width = LCDC_VPW;
	pinfo->lcdc.border_clr = 0;     /* blk */
	pinfo->lcdc.underflow_clr = 0xff;       /* blue */
	pinfo->lcdc.hsync_skew = 0;

	ret = platform_device_register(&this_device);
	if (ret)
	{
		printk(KERN_ERR "%s: platform_device_register failed! ret=%d\n", __func__, ret); 
		platform_driver_unregister(&this_driver);
	}

	return ret;
}

module_init(lcdc_ld9040_panel_init);


