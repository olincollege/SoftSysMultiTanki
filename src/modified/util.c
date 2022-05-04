/*
Copyright (C) 2015-2018 Parallel Realities

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include "util.h"

/*
 * Function: getAngle
 * ----------------------------
 *   Returns the angle between two points
 *
 *   x1, y1: x,y value of the first point 
 *   x2, y2: x,y value of the second point
 *
 *   returns: angle between points in range (0, 360)
 */
float getAngle(int x1, int y1, int x2, int y2)
{
	float angle = -90 + atan2(y1 - y2, x1 - x2) * (180 / PI);
	return angle >= 0 ? angle : 360 + angle;
}

/*
 * Function: calcSlope
 * ----------------------------
 *  Calculates slope between two points and assign delta-x and delta-y to pointers
 *
 *  x1, y1: x,y value of the first point 
 *  x2, y2: x,y value of the second point
 *  dx, dy: pointers to assign the calculated values
 */
void calcSlope(int x1, int y1, int x2, int y2, float *dx, float *dy)
{
	int steps = MAX(abs(x1 - x2), abs(y1 - y2));

	if (steps == 0)
	{
		*dx = *dy = 0;
		return;
	}

	*dx = (x1 - x2);
	*dx /= steps;

	*dy = (y1 - y2);
	*dy /= steps;
}

/*
 * Function: getLineIntersection
 * ----------------------------
 *  See if two lines intersect with each other. 
 *  Assign the point of intersection to pointers if necessary.
 *
 *  p0_x, p0_y: x,y value of the first point of the first line
 *  p1_x, p1_y: x,y value of the second point of the first line
 *  p2_x, p2_y: x,y value of the first point of the second line
 *  p3_x, p3_y: x,y value of the second point of the second line
 *  i_x, i_y: pointers to assign x,y values of the point of intersection
 *  
 *  returns: 1 if lines intersect, 0 if not.
 */
int getLineIntersection(float p0_x, float p0_y, float p1_x, float p1_y, float p2_x, float p2_y, float p3_x, float p3_y, float *i_x, float *i_y)
{
    float s1_x, s1_y, s2_x, s2_y;
    s1_x = p1_x - p0_x;
    s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;
    s2_y = p3_y - p2_y;

    float s, t;
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        // Collision detected
        if (i_x != NULL)
        {
            *i_x = p0_x + (t * s1_x);
        }
        if (i_y != NULL)
        {
            *i_y = p0_y + (t * s1_y);
        }
        return 1;
    }

    return 0; // No collision
}

/*
 * Function: getReflectedAngle
 * ----------------------------
 *  Calculates angle of reflection depending on axis.
 *
 *  angle: the original angle
 *  axis: 0 if horizontal axis, 1 if vertical axis
 *  
 *  returns: angle of reflection in range (0,360)
 */
int getReflectedAngle(int angle, int axis)
{
    if (angle % 90 == 0)
    {
        return (angle + 180) % 360;
    }

    int reflected, segment;

    segment = (angle % 90) * 2;

    if (axis == 0)
    {
        reflected = angle + (180 - segment);
    }
    else if (axis == 1)
    {
        reflected = angle - segment;
    }

    if ((angle > 90 && angle < 180) || (angle > 270 && angle < 360) )
    {
        reflected += 180;
    }

    return reflected % 360;
}

/*
 * Function: getRectOverlap
 * ----------------------------
 *  Checks if two rectangle overlaps.
 *
 *  x1, y1: left top corner of the first rectangle
 *  x2, y2: right bottom corner of the first rectangle
 *  x3, y3: left top corner of the second rectangle
 *  x4, y4: right bottom corner of the second rectangle
 *  
 *  returns: 1 if overlap, 0 if not.
 */
int getRectOverlap(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
    return (x1 < x4 && x3 < x2 && y1 < y4 && y3 < y2 && x1 != x2 && x3 != x4 && y1 != y2 && y3 != y4);
}

/*
 * Function: getPointInsideRect
 * ----------------------------
 *  Checks if a point is inside a rectangle.
 *
 *  x1, y1: left top corner of the rectangle
 *  x2, y2: right bottom corner of the rectangle
 *  x, y: x, y values of the point
 *  
 *  returns: 1 if overlap, 0 if not.
 */
int getPointInsideRect(int x1, int y1, int x2, int y2, int x, int y)
{
    if (x > x1 && x < x2 && y > y1 && y < y2)
    {
        return 1;
    }
    return 0;
}