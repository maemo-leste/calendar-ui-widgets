/*
 * pip-calendar-color.c
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

#include "helper-widgets.h"

#include "PipCalendarColor.h"

typedef struct
{
  GdkColor color;
  int draw_border;
}
color_info;

#define COLOR(r, g, b) \
  { 0x ## r ## g ## b ## 00, 0x ## r ## 00, 0x ## g ## 00, 0x ## b ## 00 }

static const color_info colors[] =
{
  { COLOR(2E, A4, FF), FALSE },
  { COLOR(1B, CF, 3F), FALSE },
  { COLOR(FF, 59, 9F), FALSE },
  { COLOR(FF, 7E, 24), FALSE },
  { COLOR(D6, 7A, FF), FALSE },
  { COLOR(FF, F0, 29), FALSE },
  { COLOR(F2, F2, F2), FALSE },
  { COLOR(33, E8, F2), FALSE },
  { COLOR(FF, 96, A5), FALSE },
  { COLOR(C3, FF, 1D), FALSE },
  { COLOR(FF, FF, FF), FALSE },
  { COLOR(00, 00, 00), TRUE },
  { COLOR(DD, 00, 00), FALSE },
  { COLOR(FF, A2, 00), FALSE },
  { COLOR(00, BE, 04), FALSE },
  { COLOR(FF, FC, 00), FALSE },
  { COLOR(01, 06, DA), FALSE },
  { COLOR(07, 01, 7A), FALSE },
  { COLOR(6D, 00, B9), FALSE },
  { COLOR(FF, 6E, FD), FALSE }
};

static const GdkColor border_color = COLOR(3D, 3D, 3D);

static const color_info *
get_color_info(PipCalendarColor color)
{
  g_return_val_if_fail(color >= PipCalendarColorFirstColor, NULL);
  g_return_val_if_fail(color >= 0 &&
                       color <= (sizeof(colors) / sizeof(colors[0])),
                       NULL);

  return &colors[color];
}

const GdkColor *
pip_calendar_color_get_gdkcolor(PipCalendarColor color)
{
  return &get_color_info(color)->color;
}

const gchar *
pip_calendar_color_to_string(PipCalendarColor color)
{
  switch (color)
  {
    case PipCalendarColorDarkBlue:
    {
      return "DarkBlue";
    }
    case PipCalendarColorDarkGreen:
    {
      return "DarkGreen";
    }
    case PipCalendarColorDarkRed:
    {
      return "DarkRed";
    }    case PipCalendarColorOrange:
    {
      return "Orange";
    }
    case PipCalendarColorViolet:
    {
      return "Violet";
    }
    case PipCalendarColorYellow:
    {
      return "Yellow";
    }
    case PipCalendarColorWhite:
    {
      return "White";
    }
    case PipCalendarColorBlue:
    {
      return "Blue";
    }
    case PipCalendarColorRed:
    {
      return "Red";
    }
    case PipCalendarColorGreen:
    {
      return "Green";
    }
    default:
    {
      return NULL;
    }
  }
}

PipCalendarColor
pip_calendar_color_from_string(const gchar *colorname)
{
  if (!colorname || !g_ascii_strcasecmp(colorname, "red"))
    return PipCalendarColorRed;

  if (!g_ascii_strcasecmp(colorname, "green"))
    return PipCalendarColorGreen;

  if (!g_ascii_strcasecmp(colorname, "blue"))
    return PipCalendarColorBlue;

  if (!g_ascii_strcasecmp(colorname, "white"))
    return PipCalendarColorWhite;

  if (!g_ascii_strcasecmp(colorname, "yellow"))
    return PipCalendarColorYellow;

  if (!g_ascii_strcasecmp(colorname, "darkred"))
    return PipCalendarColorDarkRed;

  if (!g_ascii_strcasecmp(colorname, "darkblue"))
    return PipCalendarColorDarkBlue;

  if (!g_ascii_strcasecmp(colorname, "darkgreen"))
    return PipCalendarColorDarkGreen;

  if (!g_ascii_strcasecmp(colorname, "orange"))
    return PipCalendarColorOrange;

  if (!g_ascii_strcasecmp(colorname, "violet"))
    return PipCalendarColorViolet;

  return PipCalendarColorRed;
}

static GdkPixbuf *
pip_create_pixbuf(const gchar *filename, int width, int height)
{
  GdkPixbuf *pixbuf = NULL;
  GError *error = NULL;
  gint pixbuf_height;
  gint pixbuf_width;
  GdkPixbuf *picture;

  if (!gdk_pixbuf_get_file_info(filename, &pixbuf_width, &pixbuf_height))
    return pixbuf;

  if ((width != pixbuf_width) || (height != pixbuf_height))
  {
    int h;
    int w;
    double r = MIN(((double)width / (double)pixbuf_width),
                   ((double)height / (double)pixbuf_height));
    int _w = r * (double)pixbuf_width;
    int _h = r * (double)pixbuf_height;
    double ratio = (double)_w / (double)_h;

    if (width > _w)
    {
      w = width;
      h = (double)width / ratio + 2.0;
    }
    else
    {
      h = height;
      w = (double)height * ratio + 2.0;
    }

    picture = gdk_pixbuf_new_from_file_at_size(filename, w, h, &error);

    if (error)
    {
      g_error_free(error);
      error = NULL;
    }

    if (picture)
    {
      pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, 1, 8, width, height);

      if (width >= gdk_pixbuf_get_width(picture))
        width = gdk_pixbuf_get_width(picture);

      if (height >= gdk_pixbuf_get_height(picture))
        height = gdk_pixbuf_get_height(picture);

      gdk_pixbuf_copy_area(picture, 0, 0, width, height, pixbuf, 0, 0);
      g_object_unref(picture);

      return pixbuf;
    }
  }
  else
  {
    picture = gdk_pixbuf_new_from_file(filename, &error);

    if (error)
    {
      g_error_free(error);
      error = NULL;
    }
  }

  g_return_val_if_fail(picture != NULL, NULL);

  return picture;
}

static const char *
icon_from_calendar_color(PipCalendarColor color)
{
  switch (color)
  {
    case PipCalendarColorFirst:
    {
      return "calendar_colors_darkblue.png";
    }
    case PipCalendarColorDarkGreen:
    {
      return "calendar_colors_darkgreen.png";
    }
    case PipCalendarColorDarkRed:
    {
      return "calendar_colors_darkred.png";
    }
    case PipCalendarColorOrange:
    {
      return "calendar_colors_orange.png";
    }
    case PipCalendarColorViolet:
    {
      return "calendar_colors_violet.png";
    }
    case PipCalendarColorYellow:
    {
      return "calendar_colors_yellow.png";
    }
    case PipCalendarColorWhite:
    {
      return "calendar_colors_white.png";
    }
    case PipCalendarColorBlue:
    {
      return "calendar_colors_blue.png";
    }
    case PipCalendarColorGreen:
    {
      return "calendar_colors_green.png";
    }
    default:
    {
      return "calendar_colors_red.png";
    }
  }
}

static const char *
icon_allday_from_calendar_color(PipCalendarColor color)
{
  switch (color)
  {
    case PipCalendarColorFirst:
    {
      return "calendar_colors_allday_darkblue.png";
    }
    case PipCalendarColorDarkGreen:
    {
      return "calendar_colors_allday_darkgreen.png";
    }
    case PipCalendarColorDarkRed:
    {
      return "calendar_colors_allday_darkred.png";
    }
    case PipCalendarColorOrange:
    {
      return "calendar_colors_allday_orange.png";
    }
    case PipCalendarColorViolet:
    {
      return "calendar_colors_allday_violet.png";
    }
    case PipCalendarColorYellow:
    {
      return "calendar_colors_allday_yellow.png";
    }
    case PipCalendarColorWhite:
    {
      return "calendar_colors_allday_white.png";
    }
    case PipCalendarColorBlue:
    {
      return "calendar_colors_allday_blue.png";
    }
    case PipCalendarColorGreen:
    {
      return "calendar_colors_allday_green.png";
    }
    default:
    {
      return "calendar_colors_allday_red.png";
    }
  }
}

static GdkPixbuf *
create_calendar_color_pixbuf(PipCalendarColor color, gint width, gint height)
{
  GdkPixbuf *pixbuf;
  gchar *path;
  const char *icon;
  gchar *filename;

  if ((width <= HILDON_ICON_PIXEL_SIZE_FINGER) &&
      (height <= HILDON_ICON_PIXEL_SIZE_FINGER))
  {
    path = g_strdup_printf("%s%s", "/usr/share/icons/hicolor/",
                           "48x48/hildon/");
    icon = icon_from_calendar_color(color);
  }
  else if ((width == CALENDAR_COLOR_SELECTION_ICON_WIDTH) &&
           (height == HILDON_ICON_PIXEL_SIZE_FINGER))
  {
    path = g_strdup_printf("%s%s", "/usr/share/icons/hicolor/",
                           "scalable/hildon/");
    icon = icon_allday_from_calendar_color(color);
  }
  else
  {
    path = g_strdup_printf("%s%s", "/usr/share/icons/hicolor/",
                           "64x64/hildon/");
    icon = icon_from_calendar_color(color);
  }

  filename = g_build_filename(path, icon, NULL);
  pixbuf = pip_create_pixbuf(filename, width, height);
  g_free(filename);
  g_free(path);

  return pixbuf;
}

static GdkPixbuf *
create_text_color_pixbuf(PipCalendarColor color, gint width, gint height)
{
  GdkPixbuf *pixbuf =
    gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, width, height);
  GdkPixmap *pixmap = gdk_pixmap_new(0, width, height, 16);
  GdkGC *gc = gdk_gc_new(GDK_DRAWABLE(pixmap));

  if (gc)
  {
    const color_info *info = get_color_info(color);

    if (info)
    {
      GdkColormap *colormap;

      gdk_gc_set_rgb_fg_color(gc, &info->color);
      gdk_gc_set_fill(gc, GDK_SOLID);
      gdk_draw_rectangle(GDK_DRAWABLE(pixmap), gc, TRUE, 0, 0, width, height);

      if (info->draw_border)
      {
        gdk_gc_set_rgb_fg_color(gc, &border_color);
        gdk_draw_rectangle(GDK_DRAWABLE(pixmap), gc, FALSE, 0, 0, width - 1,
                           height - 1);
      }

      colormap = gdk_drawable_get_colormap(GDK_DRAWABLE(pixmap));

      if (!colormap)
        colormap = gdk_colormap_get_system();

      gdk_pixbuf_get_from_drawable(pixbuf, GDK_DRAWABLE(pixmap), colormap, 0, 0,
                                   0, 0, width, height);
      g_object_unref(gc);
    }
  }

  gdk_drawable_unref(pixmap);

  return pixbuf;
}

GdkPixbuf *
pip_calendar_color_create_pixbuf(PipCalendarColor color, gint width,
                                 gint height)
{
  if (color <= PipCalendarColorLast)
    return create_calendar_color_pixbuf(color, width, height);
  else
    return create_text_color_pixbuf(color, width, height);
}
