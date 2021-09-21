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
#include <hildon/hildon.h>

#include <libintl.h>

#include "PipCalendarDialog.h"
#include "calendar-widgets-enum-gen.h"
#include "helper-widgets.h"

#include "PipColorPicker.h"

enum
{
  COL_COLOR,
  COL_PIXBUF
};

struct _PipColorPickerPrivate
{
  gboolean disposed;
  GtkListStore *list_store;
  PipCalendarColor color;
  GtkWidget *icon_view;
  GtkWidget *dialog;
};

typedef struct _PipColorPickerPrivate PipColorPickerPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(
  PipColorPicker,
  pip_color_picker,
  G_TYPE_OBJECT
);

#define PRIVATE(picker) \
  ((PipColorPickerPrivate *)pip_color_picker_get_instance_private( \
     (PipColorPicker *)picker))

static void
pip_color_picker_init(PipColorPicker *picker)
{
  PipColorPickerPrivate *priv = PRIVATE(picker);

  priv->dialog = NULL;
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

void
pip_color_picker_add_color(PipColorPicker *picker, PipCalendarColor color)
{
  PipColorPickerPrivate *priv;
  GdkPixbuf *pixbuf;
  GtkTreeIter iter;

  g_return_if_fail(PIP_IS_COLOR_PICKER(picker));

  priv = PRIVATE(picker);
  pixbuf = pip_calendar_color_create_pixbuf(color,
                                            CALENDAR_COLOR_SELECTION_ICON_WIDTH,
                                            HILDON_ICON_PIXEL_SIZE_FINGER);
  g_return_if_fail(pixbuf);

  gtk_list_store_append(priv->list_store, &iter);
  gtk_list_store_set(priv->list_store, &iter,
                     COL_COLOR, color,
                     COL_PIXBUF, pixbuf,
                     -1);
}

void
pip_color_picker_add_all_colors(PipColorPicker *picker,
                                PipColorPickerSet color_set)
{
  PipCalendarColor color;

  g_return_if_fail(PIP_IS_COLOR_PICKER(picker));

  if (color_set == PipColorPickerText)
  {
    for (color = PipTextColorFirst; color <= PipTextColorLast; color++)
      pip_color_picker_add_color(picker, color);
  }
  else if (color_set == PipColorPickerCalendar)
  {
    for (color = PipCalendarColorFirst; color <= PipCalendarColorLast; color++)
      pip_color_picker_add_color(picker, color);
  }
}

void
pip_color_picker_highlight_color(PipColorPicker *picker, PipCalendarColor color)
{
  g_return_if_fail(PIP_IS_COLOR_PICKER(picker));

  PRIVATE(picker)->color = color;
}

static void
on_done_clicked(GtkButton *button, gpointer user_data)
{
  PipColorPicker *picker = PIP_COLOR_PICKER(user_data);
  PipColorPickerPrivate *priv;
  GList *selected_paths;
  GtkTreeIter iter;

  g_return_if_fail(PIP_IS_COLOR_PICKER(picker));

  priv = PRIVATE(picker);

  selected_paths =
    gtk_icon_view_get_selected_items(GTK_ICON_VIEW(priv->icon_view));

  g_return_if_fail(selected_paths != NULL);

  if (gtk_tree_model_get_iter(GTK_TREE_MODEL(priv->list_store), &iter,
                              selected_paths->data))
  {
    gtk_tree_model_get(GTK_TREE_MODEL(priv->list_store), &iter,
                       COL_COLOR, &priv->color,
                       -1);
  }

  g_list_free_full(selected_paths, (GDestroyNotify)gtk_tree_path_free);
}

void
pip_color_picker_run(PipColorPicker *picker)
{
  PipCalendarDialog *dialog;
  PipColorPickerPrivate *priv;
  GtkCellRenderer *cell;
  GtkWidget *button;

  g_return_if_fail(PIP_IS_COLOR_PICKER(picker));

  priv = PRIVATE(picker);

  dialog = PIP_CALENDAR_DIALOG(pip_calendar_dialog_new());

  priv->dialog = GTK_WIDGET(dialog);

  pip_calendar_dialog_set_title(dialog, dgettext("calendar", "cal_fi_color"));
  gtk_widget_set_size_request(GTK_WIDGET(dialog), -1,
                              CALENDAR_COLOR_SELECTION_DIALOG_HEIGHT);

  priv->icon_view = hildon_gtk_icon_view_new(HILDON_UI_MODE_EDIT);
  gtk_icon_view_set_selection_mode(GTK_ICON_VIEW(priv->icon_view),
                                   GTK_SELECTION_SINGLE);
  gtk_icon_view_set_column_spacing(GTK_ICON_VIEW(priv->icon_view), 0);
  gtk_icon_view_set_row_spacing(GTK_ICON_VIEW(priv->icon_view), 4);
  cell = gtk_cell_renderer_pixbuf_new();
  g_object_set(cell, "xpad", 4, "ypad", 4, NULL);
  gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(priv->icon_view), cell, TRUE);
  gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(priv->icon_view), cell,
                                 "pixbuf", 1,
                                 NULL);
  gtk_icon_view_set_model(GTK_ICON_VIEW(priv->icon_view),
                          GTK_TREE_MODEL(priv->list_store));
  pip_calendar_dialog_pack_with_defaults(dialog, priv->icon_view);
  button = pip_calendar_dialog_add_button(
      dialog, dgettext("hildon-libs", "wdgt_bd_done"));
  gtk_widget_show_all(GTK_WIDGET(dialog));

  if (priv->color != PipCalendarColorInvalid)
  {
    GtkTreePath *path = gtk_tree_path_new_from_indices(priv->color, -1);

    gtk_icon_view_select_path(GTK_ICON_VIEW(priv->icon_view), path);
    gtk_tree_path_free(path);
  }

  g_signal_connect(button, "clicked", G_CALLBACK(on_done_clicked), picker);
  gtk_dialog_run(GTK_DIALOG(dialog));

  if (priv->dialog)
    gtk_widget_destroy(priv->dialog);
}

PipCalendarColor
pip_color_picker_get_selected_color(PipColorPicker *picker)
{
  g_return_val_if_fail(PIP_IS_COLOR_PICKER(picker), PipCalendarColorInvalid);

  return PRIVATE(picker)->color;
}

PipCalendarColor
pip_color_picker_select_color(PipCalendarColor highlight_color,
                              PipColorPickerSet color_set)
{
  PipColorPicker *picker = PIP_COLOR_PICKER(pip_color_picker_new());
  PipCalendarColor color;

  g_return_val_if_fail(picker != NULL, PipCalendarColorInvalid);

  pip_color_picker_add_all_colors(picker, color_set);
  pip_color_picker_highlight_color(picker, highlight_color);
  pip_color_picker_run(picker);
  color = pip_color_picker_get_selected_color(picker);
  g_object_unref(picker);

  return color;
}
