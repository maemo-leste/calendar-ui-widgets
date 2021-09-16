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

#include <glib/gprintf.h>
#include <hildon/hildon.h>

#include <errno.h>
#include <sys/statfs.h>
#include <syslog.h>

#include "PipCalendarDialog.h"

#define pip_message_print_message(msg) \
  _pip_message_print_message(__FUNCTION__, msg)

#define PIP_INFO(msg) \
  do \
  { \
    if (pip_message_use_syslog()) \
      syslog(LOG_INFO, "CALENDAR-UI-MSG:%s: " msg "\n", __FUNCTION__); \
    else \
      pip_message_print_message("Some note/confirmation is already opened"); \
  } \
  while (0)

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
print_message(const char *fmt_func, const char *func, const char *fmt_msg, ...)
{
  va_list va;

  va_start(va, fmt_msg);
  printf(fmt_func, func);
  g_vprintf(fmt_msg, va);
  putc('\n', stdout);
  fflush(stdout);
  va_end(va);
}

static void
_pip_message_print_message(const char *func, const char *msg)
{
  print_message("\x1B[1;33m%32s()\x1B[0;39m: ", func, msg);
}

static gboolean
checkFileExists(const char *filename)
{
  struct statfs s = {};

  if (statfs(filename, &s))
    return errno != ENOENT;

  return TRUE;
}

static gboolean
pip_message_use_syslog()
{
  static int use_syslog = 0;

  if (!use_syslog)
  {
    if (checkFileExists("/home/user/.calendar/logui"))
      use_syslog = 2;
    else
      use_syslog = 1;
  }

  return use_syslog == 2;
}

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

void
pip_calendar_dialog_pack_no_scroll(PipCalendarDialog *dialog, GtkWidget *widget,
                                   gboolean expand, gboolean fill)
{
  g_return_if_fail(PIP_IS_CALENDAR_DIALOG(dialog));
  g_return_if_fail(GTK_IS_WIDGET(widget));

  gtk_box_pack_start(
    GTK_BOX(GTK_DIALOG(dialog)->vbox), widget, expand, fill, 0);
}

void
pip_calendar_dialog_set_modal_widget_ptr(PipCalendarDialog *dialog,
                                         GtkWidget *widget)
{
  PipCalendarDialogPrivate *priv;

  g_return_if_fail(PIP_IS_CALENDAR_DIALOG(dialog));
  g_return_if_fail(GTK_IS_WIDGET(widget));

  priv = PRIVATE(dialog);

  if (priv->modal_widget)
    PIP_INFO("Some note/confirmation is already opened");

  priv->modal_widget = widget;
}

void
pip_calendar_dialog_reset_modal_widget_ptr(PipCalendarDialog *dialog)
{
  g_return_if_fail(PIP_IS_CALENDAR_DIALOG(dialog));

  PRIVATE(dialog)->modal_widget = NULL;
}

GtkWidget *
pip_calendar_dialog_get_modal_widget_ptr(PipCalendarDialog *dialog)
{
  g_return_val_if_fail(PIP_IS_CALENDAR_DIALOG(dialog), NULL);

  return PRIVATE(dialog)->modal_widget;
}

gboolean
pip_calendar_dialog_is_dialog_open()
{
  return pip_calendar_dialog_open_count > 0;
}
