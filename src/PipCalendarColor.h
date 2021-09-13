/* * This file is part of calendar-ui *
* Copyright © 2009 Nokia Corporation and/or its subsidiary(-ies). All rights reserved.
* Contact: Ilias Biris <ilias.biris@nokia.com>
* This software, including documentation, is protected by copyright controlled by
* Nokia Corporation. All rights are reserved. Copying, including reproducing, storing,
* adapting or translating, any or all of this material requires the prior written consent
* of Nokia Corporation. This material also contains confidential information which may
* not be disclosed to others without the prior written consent of Nokia. */

#ifndef PIPCALENDARCOLOR_H
#define PIPCALENDARCOLOR_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

/**
 * PipCalendarColor:
 *
 * A type to handle a finite set of colors used to visualize the calendars 
 * (#PipCalendarSource) and calendar entries (#PipCalendarEntry).
 */
typedef enum {
	PipCalendarColorInvalid = -1,
	// Calanedar colors range 
	PipCalendarColorFirst = 0,
	PipCalendarColorDarkBlue = PipCalendarColorFirst,
	PipCalendarColorDarkGreen,
	PipCalendarColorDarkRed,
	PipCalendarColorOrange,
	PipCalendarColorViolet,
	PipCalendarColorYellow,
	PipCalendarColorWhite,
	PipCalendarColorBlue,
	PipCalendarColorRed,
	PipCalendarColorGreen,
	PipCalendarColorLast = PipCalendarColorGreen,

	// Text colors
	PipTextColorFirst,
	PipTextColorWhite = PipTextColorFirst,
	PipTextColorBlack,
	PipTextColorRed,
	PipTextColorOrange,
	PipTextColorGreen,
	PipTextColorYellow,
	PipTextColorBlue,
	PipTextColorDarkBlue,
	PipTextColorPurple,
	PipTextColorPink,
	PipTextColorLast = PipTextColorPink,
} PipCalendarColor;

#define PipCalendarColorFirstColor PipCalendarColorDarkBlue
#define PipCalendarColorLastColor PipCalendarColorGreen

const gchar      *pip_calendar_color_to_string   (PipCalendarColor color);
PipCalendarColor  pip_calendar_color_from_string (const gchar *colorname);

GdkPixbuf *
pip_calendar_color_create_pixbuf (
		PipCalendarColor    color,
		gint                width,
		gint                height);

GdkColor const *
pip_calendar_color_get_gdkcolor(PipCalendarColor    color);

G_END_DECLS
#endif
