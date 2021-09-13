/* * This file is part of calendar-ui *
* Copyright © 2009 Nokia Corporation and/or its subsidiary(-ies). All rights reserved.
* Contact: Ilias Biris <ilias.biris@nokia.com>
* This software, including documentation, is protected by copyright controlled by
* Nokia Corporation. All rights are reserved. Copying, including reproducing, storing,
* adapting or translating, any or all of this material requires the prior written consent
* of Nokia Corporation. This material also contains confidential information which may
* not be disclosed to others without the prior written consent of Nokia. */

#ifndef PIPCALENDARDIALOG_H
#define PIPCALENDARDIALOG_H


#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include <hildon/hildon-dialog.h>


G_BEGIN_DECLS

#define PIP_TYPE_CALENDAR_DIALOG            (pip_calendar_dialog_get_type())
#define PIP_CALENDAR_DIALOG(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), PIP_TYPE_CALENDAR_DIALOG, PipCalendarDialog))
#define PIP_CALENDAR_DIALOG_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), PIP_TYPE_CALENDAR_DIALOG, PipCalendarDialogClass))
#define PIP_IS_CALENDAR_DIALOG(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), PIP_TYPE_CALENDAR_DIALOG))
#define PIP_IS_CALENDAR_DIALOG_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), PIP_TYPE_CALENDAR_DIALOG))
#define PIP_CALENDAR_DIALOG_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), PIP_TYPE_CALENDAR_DIALOG, PipCalendarDialogClass))

typedef struct _PipCalendarDialog              PipCalendarDialog;
typedef struct _PipCalendarDialogClass         PipCalendarDialogClass;

/**
 * PipCalendarDialog:
 * 
 * A base class for the dialogs used by the calendar UI. 
 */
struct _PipCalendarDialog
{
	GtkDialog                       parent_instance;

	struct _PipCalendarDialogPrivate  *priv;

	GtkWidget        *pannable_area;
};

/**
 * PipCalendarDialogClass:
 * 
 * 
 */
struct _PipCalendarDialogClass
{
	GtkDialogClass    parent_instance;
};


/******************************************************************************
 * Public declarations.                                                       *
 *                                                                            *
 *                                                                            *
 ******************************************************************************/
GType pip_calendar_dialog_get_type(void);
GtkWidget *pip_calendar_dialog_new          (void);

void
pip_calendar_dialog_set_title (
		PipCalendarDialog    *dialog,
		const gchar          *title);

GtkWidget *
pip_calendar_dialog_add_button (
		PipCalendarDialog     *dialog,
		const gchar           *label);

void
pip_calendar_dialog_pack_with_defaults (
		PipCalendarDialog     *dialog,
		GtkWidget             *widget);

void
pip_calendar_dialog_pack_no_scroll (
		PipCalendarDialog     *dialog,
		GtkWidget             *widget,
		gboolean               expand,
		gboolean               fill);

void
pip_calendar_dialog_set_modal_widget_ptr (
		PipCalendarDialog     *dialog,
		GtkWidget             *widget);

void
pip_calendar_dialog_reset_modal_widget_ptr (
		PipCalendarDialog     *dialog);

void
pip_calendar_dialog_reset_modal_ptr(
		PipCalendarDialog     *dialog);

GtkWidget*
pip_calendar_dialog_get_modal_widget_ptr (
		PipCalendarDialog     *dialog);

gboolean
pip_calendar_dialog_is_dialog_open();
G_END_DECLS

#endif
