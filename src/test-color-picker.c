/*
 * test-color-picker.c
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

#include "PipColorPicker.h"
#include "calendar-widgets-enum-gen.h"

int
main(int argc, char **argv)
{
  GEnumClass *enum_class;
  PipCalendarColor color;
  GEnumValue *color_value;

  hildon_gtk_init(&argc, &argv);
  enum_class = G_ENUM_CLASS(g_type_class_ref(PIP_TYPE_CALENDAR_COLOR));

  color = pip_color_picker_select_color(PipCalendarColorOrange,
                                        PipColorPickerCalendar);
  color_value = g_enum_get_value(enum_class, color);
  printf("Selected Calendar color [%s]\n", color_value->value_name);

  color = pip_color_picker_select_color(PipTextColorGreen,
                                        PipColorPickerText);
  color_value = g_enum_get_value(enum_class, color);
  printf("Selected Text color [%s]\n", color_value->value_name);

  g_type_class_unref(enum_class);

  return 0;
}
