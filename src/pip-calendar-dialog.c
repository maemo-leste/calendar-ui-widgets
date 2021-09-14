/*
 * pip-calendar-dialog.c
 *
 * Copyright (C) 2021 Ivaylo Dimitrov <ivo.g.dimitrov.75@gmail.com>
 *
 * This library is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library. If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include <hildon/hildon.h>

#include "PipCalendarDialog.h"

enum
{
  ERROR,
  LAST_SIGNAL
};

struct _PipCalendarDialogPrivate
{
  GtkWidget *modal_widget;
};

typedef struct _PipCalendarDialogPrivate PipCalendarDialogPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(
  PipCalendarDialog,
  pip_calendar_dialog,
  GTK_TYPE_DIALOG
);

#define PRIVATE(dialog) \
  ((PipCalendarDialogPrivate *)pip_calendar_dialog_get_instance_private( \
     (PipCalendarDialog *)dialog))

static guint signals[LAST_SIGNAL] = {};
static gint pip_calendar_dialog_open_count = 0;

static void
pip_calendar_dialog_dispose(GObject *object)
{
  PRIVATE(object)->modal_widget = NULL;
  G_OBJECT_CLASS(pip_calendar_dialog_parent_class)->dispose(object);
}

static void
pip_calendar_dialog_finalize(GObject *object)
{
  pip_calendar_dialog_open_count--;
  G_OBJECT_CLASS(pip_calendar_dialog_parent_class)->finalize(object);
}

static void
pip_calendar_dialog_class_init(PipCalendarDialogClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS(klass);

  object_class->dispose = pip_calendar_dialog_dispose;
  object_class->finalize = pip_calendar_dialog_finalize;

  signals[ERROR] =
    g_signal_new("error",
                 PIP_TYPE_CALENDAR_DIALOG,
                 G_SIGNAL_DETAILED | G_SIGNAL_RUN_LAST, 0, NULL, NULL,
                 g_cclosure_marshal_VOID__STRING,
                 G_TYPE_NONE,
                 1, G_TYPE_STRING);
}

static gboolean
on_key_press_event(GtkWidget *self, GdkEventKey *event, gpointer user_data)
{
  if (event->keyval != GDK_Escape)
    return FALSE;

  gtk_widget_destroy(self);

  return TRUE;
}

static void
pip_calendar_dialog_init(PipCalendarDialog *dialog)
{
  PipCalendarDialogPrivate *priv = PRIVATE(dialog);

  priv->modal_widget = NULL;
  gtk_widget_set_size_request(GTK_WIDGET(dialog), -1, -1);
  gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
  gtk_dialog_set_has_separator(GTK_DIALOG(dialog), FALSE);
  dialog->pannable_area = hildon_pannable_area_new();
  g_object_set(dialog->pannable_area,
               "mov_mode", HILDON_MOVEMENT_MODE_VERT,
               "hovershoot_max", 0,
               "vovershoot_max", 0,
               "hscrollbar_policy", GTK_POLICY_NEVER,
               NULL);
  g_signal_connect_after(dialog, "key-press-event",
                         G_CALLBACK(on_key_press_event), NULL);
  pip_calendar_dialog_open_count++;
}

GtkWidget *
pip_calendar_dialog_new()
{
  return g_object_new(PIP_TYPE_CALENDAR_DIALOG, NULL);
}

void
pip_calendar_dialog_set_title(PipCalendarDialog *dialog, const gchar *title)
{
  g_return_if_fail(PIP_IS_CALENDAR_DIALOG(dialog));

  gtk_window_set_title(GTK_WINDOW(dialog), title);
}

GtkWidget *
pip_calendar_dialog_add_button(PipCalendarDialog *dialog, const gchar *label)
{
  g_return_val_if_fail(PIP_IS_CALENDAR_DIALOG(dialog), NULL);

  return gtk_dialog_add_button(GTK_DIALOG(dialog), label, -1);
}

void
pip_calendar_dialog_pack_with_defaults(PipCalendarDialog *dialog,
                                       GtkWidget *widget)
{
  g_return_if_fail(PIP_IS_CALENDAR_DIALOG(dialog));
  g_return_if_fail(GTK_IS_WIDGET(widget));

  gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox),
                    dialog->pannable_area);

  if (GTK_IS_TREE_VIEW(widget))
  {
    gtk_container_add(GTK_CONTAINER(dialog->pannable_area), widget);
    gtk_widget_show(dialog->pannable_area);
  }
  else
  {
    hildon_pannable_area_add_with_viewport(
      HILDON_PANNABLE_AREA(dialog->pannable_area), widget);
    gtk_widget_show(dialog->pannable_area);
  }
}
