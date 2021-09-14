/*
 * pip-color-picker.c
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

#include "calendar-widgets-enum-gen.h"

#include "PipColorPicker.h"

struct _PipColorPickerPrivate
{
  gboolean disposed;
  GtkListStore *list_store;
  PipCalendarColor color;
  GtkWidget *icon_view;
  GtkWidget *calendar_dialog;
};

typedef struct _PipColorPickerPrivate PipColorPickerPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(
  PipColorPicker,
  pip_color_picker,
  G_TYPE_OBJECT
);

#define PRIVATE(picker) \
  pip_color_picker_get_instance_private((PipColorPicker *)picker)

static void
pip_color_picker_init(PipColorPicker *picker)
{
  PipColorPickerPrivate *priv = PRIVATE(picker);

  priv->calendar_dialog = NULL;
  priv->disposed = FALSE;
  priv->icon_view = NULL;
  priv->color = PipCalendarColorInvalid;
  priv->list_store = gtk_list_store_new(2, PIP_TYPE_CALENDAR_COLOR,
                                        GDK_TYPE_PIXBUF);

  /* WTF? - check for a memleak */
  g_object_ref_sink(priv->list_store);
}

static void
pip_color_picker_dispose(GObject *object)
{
  PipColorPickerPrivate *priv = PRIVATE(object);

  if (priv->list_store)
  {
    g_object_unref(priv->list_store);
    priv->list_store = NULL;
  }

  G_OBJECT_CLASS(pip_color_picker_parent_class)->dispose(object);
}

static void
pip_color_picker_class_init(PipColorPickerClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS(klass);

  object_class->dispose = pip_color_picker_dispose;
}

GObject *
pip_color_picker_new()
{
  return g_object_new(PIP_TYPE_COLOR_PICKER, NULL);
}
