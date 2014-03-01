/*
 *  tslib/tests/ts_calibrate.c
 *
 *  Copyright (C) 2001 Russell King.
 *
 * This file is placed under the GPL.  Please see the file
 * COPYING for more details.
 *
 * $Id: ts_calibrate.c,v 1.8 2004/10/19 22:01:27 dlowder Exp $
 *
 * Basic test program for touchscreen library.
 */
//#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "tslib.h"

//#include "fbutils.h"
//#include "testutils.h"

#include "main.h"
#include "lcd.h"
#include "touch.h"

#define XORMODE	0x80000000
#define XRES 320
#define YRES 240

extern void uDelay(int count);

static int palette [] =
{
	0x000000, 0xffe080, 0xffffff, 0xe0c0a0
};
#define NR_COLORS (sizeof (palette) / sizeof (palette [0]))

typedef struct {
	int x[5], xfb[5];
	int y[5], yfb[5];
	int a[7];
} calibration;

//static void sig(int sig)
//{
//	close_framebuffer ();
//	fflush (stderr);
//	printf ("signal %d caught\n\r", sig);
//	fflush (stdout);
//	exit (1);
//}

int perform_calibration(calibration *cal) {
	int j;
	float n, x, y, x2, y2, xy, z, zx, zy;
	float det, a, b, c, e, f, i;
	float scaling = 65536.0;

// Get sums for matrix
	n = x = y = x2 = y2 = xy = 0;
	for(j=0;j<5;j++) {
		n += 1.0;
		x += (float)cal->x[j];
		y += (float)cal->y[j];
		x2 += (float)(cal->x[j]*cal->x[j]);
		y2 += (float)(cal->y[j]*cal->y[j]);
		xy += (float)(cal->x[j]*cal->y[j]);
	}

// Get determinant of matrix -- check if determinant is too small
	det = n*(x2*y2 - xy*xy) + x*(xy*y - x*y2) + y*(x*xy - y*x2);
	if(det < 0.1 && det > -0.1) {
		printf("ts_calibrate: determinant is too small -- %f\n\r",det);
		return 0;
	}

// Get elements of inverse matrix
	a = (x2*y2 - xy*xy)/det;
	b = (xy*y - x*y2)/det;
	c = (x*xy - y*x2)/det;
	e = (n*y2 - y*y)/det;
	f = (x*y - n*xy)/det;
	i = (n*x2 - x*x)/det;

// Get sums for x calibration
	z = zx = zy = 0;
	for(j=0;j<5;j++) {
		z += (float)cal->xfb[j];
		zx += (float)(cal->xfb[j]*cal->x[j]);
		zy += (float)(cal->xfb[j]*cal->y[j]);
	}

// Now multiply out to get the calibration for framebuffer x coord
	cal->a[0] = (int)((a*z + b*zx + c*zy)*(scaling));
	cal->a[1] = (int)((b*z + e*zx + f*zy)*(scaling));
	cal->a[2] = (int)((c*z + f*zx + i*zy)*(scaling));

	printf("%f %f %f\n\r",(a*z + b*zx + c*zy),
				(b*z + e*zx + f*zy),
				(c*z + f*zx + i*zy));

// Get sums for y calibration
	z = zx = zy = 0;
	for(j=0;j<5;j++) {
		z += (float)cal->yfb[j];
		zx += (float)(cal->yfb[j]*cal->x[j]);
		zy += (float)(cal->yfb[j]*cal->y[j]);
	}

// Now multiply out to get the calibration for framebuffer y coord
	cal->a[3] = (int)((a*z + b*zx + c*zy)*(scaling));
	cal->a[4] = (int)((b*z + e*zx + f*zy)*(scaling));
	cal->a[5] = (int)((c*z + f*zx + i*zy)*(scaling));

	printf("%f %f %f\n\r",(a*z + b*zx + c*zy),
				(b*z + e*zx + f*zy),
				(c*z + f*zx + i*zy));

// If we got here, we're OK, so assign scaling to a[6] and return
	cal->a[6] = (int)scaling;
	return 1;
/*	
// This code was here originally to just insert default values
	for(j=0;j<7;j++) {
		c->a[j]=0;
	}
	c->a[1] = c->a[5] = c->a[6] = 1;
	return 1;
*/

}

void put_cross(int x, int y, unsigned colidx)
{
	LCD_Line(x - 10, y, x - 2, y,LCD_COLOR_RED);
	LCD_Line(x + 2, y, x + 10, y,LCD_COLOR_RED);
	LCD_Line(x, y - 10, x, y - 2,LCD_COLOR_RED);
	LCD_Line(x, y + 2, x, y + 10,LCD_COLOR_RED);

#if 1
	LCD_Line(x - 6, y - 9, x - 9, y - 9,LCD_COLOR_RED);
	LCD_Line(x - 9, y - 8, x - 9, y - 6,LCD_COLOR_RED);
	LCD_Line(x - 9, y + 6, x - 9, y + 9,LCD_COLOR_RED);
	LCD_Line(x - 8, y + 9, x - 6, y + 9,LCD_COLOR_RED);
	LCD_Line(x + 6, y + 9, x + 9, y + 9,LCD_COLOR_RED);
	LCD_Line(x + 9, y + 8, x + 9, y + 6,LCD_COLOR_RED);
	LCD_Line(x + 9, y - 6, x + 9, y - 9,LCD_COLOR_RED);
	LCD_Line(x + 8, y - 9, x + 6, y - 9,LCD_COLOR_RED);
#else
	LCD_Line(x - 7, y - 7, x - 4, y - 4,LCD_COLOR_RED);
	LCD_Line(x - 7, y + 7, x - 4, y + 4,LCD_COLOR_RED);
	LCD_Line(x + 4, y - 4, x + 7, y - 7,LCD_COLOR_RED);
	LCD_Line(x + 4, y + 4, x + 7, y + 7,LCD_COLOR_RED);
#endif
}

void getxy(int *x, int *y)
{
	int i;
//#define MAX_SAMPLES 128
//	struct ts_sample samp[MAX_SAMPLES];
//	int index, middle;
//
//	do {
//		if (ts_read_raw(ts, &samp[0], 1) < 0) {
//			perror("ts_read");
//			close_framebuffer ();
//			exit(1);
//		}
//		
//	} while (samp[0].pressure == 0);
//
//	/* Now collect up to MAX_SAMPLES touches into the samp array. */
//	index = 0;
//	do {
//		if (index < MAX_SAMPLES-1)
//			index++;
//		if (ts_read_raw(ts, &samp[index], 1) < 0) {
//			perror("ts_read");
//			close_framebuffer ();
//			exit(1);
//		}
//	} while (samp[index].pressure > 0);
//	printf("Took %d samples...\n\r",index);
//
//	/*
//	 * At this point, we have samples in indices zero to (index-1)
//	 * which means that we have (index) number of samples.  We want
//	 * to calculate the median of the samples so that wild outliers
//	 * don't skew the result.  First off, let's assume that arrays
//	 * are one-based instead of zero-based.  If this were the case
//	 * and index was odd, we would need sample number ((index+1)/2)
//	 * of a sorted array; if index was even, we would need the
//	 * average of sample number (index/2) and sample number
//	 * ((index/2)+1).  To turn this into something useful for the
//	 * real world, we just need to subtract one off of the sample
//	 * numbers.  So for when index is odd, we need sample number
//	 * (((index+1)/2)-1).  Due to integer division truncation, we
//	 * can simplify this to just (index/2).  When index is even, we
//	 * need the average of sample number ((index/2)-1) and sample
//	 * number (index/2).  Calculate (index/2) now and we'll handle
//	 * the even odd stuff after we sort.
//	 */
//	middle = index/2;
//	if (x) {
//		qsort(samp, index, sizeof(struct ts_sample), sort_by_x);
//		if (index & 1)
//			*x = samp[middle].x;
//		else
//			*x = (samp[middle-1].x + samp[middle].x) / 2;
//	}
//	if (y) {
//		qsort(samp, index, sizeof(struct ts_sample), sort_by_y);
//		if (index & 1)
//			*y = samp[middle].y;
//		else
//			*y = (samp[middle-1].y + samp[middle].y) / 2;
//	}
	*x=0;
	*y=0; 
	while(Touch_Detect()==0);
	Touch_Measure_Filter();	 
	while(Touch_Detect()==1);
	*x=x_ori;
	*y=y_ori;
}

static void get_sample (calibration *cal,
			int index, int x, int y, char *name)
{
	static int last_x = -1, last_y;

//	if (last_x != -1) {
//#define NR_STEPS 10
//		int dx = ((x - last_x) << 16) / NR_STEPS;
//		int dy = ((y - last_y) << 16) / NR_STEPS;
//		int i;
//		last_x <<= 16;
//		last_y <<= 16;
//		for (i = 0; i < NR_STEPS; i++) {
//			put_cross (last_x >> 16, last_y >> 16, 2 | XORMODE);
//			uDelay (1000);
//			put_cross (last_x >> 16, last_y >> 16, 2 | XORMODE);
//			last_x += dx;
//			last_y += dy;
//		}
//	}

	put_cross(x, y, 2 | XORMODE);
	getxy (&cal->x [index], &cal->y [index]);
	put_cross(x, y, 2 | XORMODE);
//	Delay (300);

	last_x = cal->xfb [index] = x;
	last_y = cal->yfb [index] = y;

	printf("%s : LCD X = %4d Y = %4d\n\r", name, cal->xfb [index], cal->yfb [index]);
	printf("%s : X = %4d Y = %4d\n\r", name, cal->x [index], cal->y [index]);
}

//static int clearbuf(struct tsdev *ts)
//{
//	int fd = ts_fd(ts);
//	fd_set fdset;
//	struct timeval tv;
//	int nfds;
//	struct ts_sample sample;
//
//	while (1) {
//		FD_ZERO(&fdset);
//		FD_SET(fd, &fdset);
//
//		tv.tv_sec = 0;
//		tv.tv_usec = 0;
//
//		nfds = select(fd + 1, &fdset, NULL, NULL, &tv);
//		if (nfds == 0) break;
//
//		if (ts_read_raw(ts, &sample, 1) < 0) {
//			perror("ts_read");
//			exit(1);
//		}
//	}
//}

int ts_test()
{
//	struct tsdev *ts;
	calibration cal;
	int cal_fd;
	char cal_buffer[256];
	char *tsdevice = NULL;
	char *calfile = NULL;
	unsigned int i;

	int xtemp,ytemp;
	int x,y;

  /* Clear the LCD */ 
  LCD_Clear(LCD_COLOR_WHITE);
	get_sample (&cal, 0, 50,        50,        "Top left");
//	clearbuf(ts);
  /* Clear the LCD */ 
//  LCD_Clear(LCD_COLOR_WHITE);
	get_sample (&cal, 1, XRES - 50, 50,        "Top right");
//	clearbuf(ts);
  /* Clear the LCD */ 
//  LCD_Clear(LCD_COLOR_WHITE);
	get_sample (&cal, 2, XRES - 50, YRES - 50, "Bot right");
//	clearbuf(ts);
  /* Clear the LCD */ 
//  LCD_Clear(LCD_COLOR_WHITE);
	get_sample (&cal, 3, 50,        YRES - 50, "Bot left");
//	clearbuf(ts);
  /* Clear the LCD */ 
//  LCD_Clear(LCD_COLOR_WHITE);
	get_sample (&cal, 4, XRES / 2,  YRES / 2,  "Center");

	if (perform_calibration (&cal)) {
		printf ("Calibration constants: ");
		for (i = 0; i < 7; i++) printf("%d ", cal.a [i]);
		printf("\n\r");
//		if ((calfile = getenv("TSLIB_CALIBFILE")) != NULL) {
//			cal_fd = open (calfile, O_CREAT | O_RDWR);
//		} else {
//			cal_fd = open ("/etc/pointercal", O_CREAT | O_RDWR);
//		}
		sprintf (cal_buffer,"%d %d %d %d %d %d %d",
			 cal.a[0], cal.a[1], cal.a[2],
			 cal.a[3], cal.a[4], cal.a[5], cal.a[6]);
		printf ("%d %d %d %d %d %d %d",
			 cal.a[1], cal.a[2], cal.a[0],
			 cal.a[4], cal.a[5], cal.a[3], cal.a[6]);
//		write (cal_fd, cal_buffer, strlen (cal_buffer) + 1);
//		close (cal_fd);
                i = 0;
	} else {
		printf("Calibration failed.\n\r");
		i = -1;
	}
	while(1)
	{
	getxy (&cal.x[0], &cal.y[0]);
	printf("before Calibration x = %d y=%d\n\r",cal.x[0],cal.y[0]);
	   		xtemp = cal.x[0];
			ytemp = cal.y[0];
			x = 	(int)(( cal.a[0] +
					cal.a[1]*xtemp + 
					cal.a[2]*ytemp ) / cal.a[6]);
			y =		(int)(( cal.a[3] +
					cal.a[4]*xtemp +
					cal.a[5]*ytemp ) / cal.a[6]);
	printf("after Calibration x = %d y=%d\n\r",x,y);
        LCD_Clear(LCD_COLOR_WHITE);
        LCD_DispNum(0,0,x,5,0);
        LCD_DispNum(0,16,y,5,0);
	}

//	close_framebuffer();
	return i;
}
