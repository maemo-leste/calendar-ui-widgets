#ifndef PIPCOLORPICKER_H
#define PIPCOLORPICKER_H

#include <gtk/gtk.h>
#include "PipCalendarColor.h"

typedef enum {
	PipColorPickerCalendar,
	PipColorPickerText
} PipColorPickerSet;

/***************************/
/* All-in-one function     */
/***************************/
/**
 * Show modal Color picker dialog
 * @param highlight_color color to be highlighted when dialog will be displayed.
                          PipCalendarColorInvalid of no highligh is required 
 * @return selected color, PipCalendarColorInvalid (-1) if dialog was dismissed or some error occured
 */
PipCalendarColor
pip_color_picker_select_color(
                PipCalendarColor highlight_color,
                PipColorPickerSet color_set);


#define PIP_TYPE_COLOR_PICKER            (pip_color_picker_get_type())
#define PIP_COLOR_PICKER(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), PIP_TYPE_COLOR_PICKER, PipColorPicker))
#define PIP_COLOR_PICKER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), PIP_TYPE_COLOR_PICKER, PipColorPickerClass))
#define PIP_IS_COLOR_PICKER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), PIP_TYPE_COLOR_PICKER))
#define PIP_IS_COLOR_PICKER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), PIP_TYPE_COLOR_PICKER))
#define PIP_COLOR_PICKER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), PIP_TYPE_COLOR_PICKER, PipColorPickerClass))

typedef struct _PipColorPicker                 PipColorPicker;
typedef struct _PipColorPickerClass            PipColorPickerClass;

/**
 * PipColorPicker:
 * 
 * 
 */
struct _PipColorPicker
{
    GObject   parent_instance;
};

/**
 * PipColorPickerClass:
 * 
 * 
 */
struct _PipColorPickerClass
{
    GObjectClass parent_instance;
};

/******************************************************************************
 * Public declarations.                                                       *
 *                                                                            *
 *                                                                            *
 ******************************************************************************/
GType pip_color_picker_get_type(void);

/**
 * Create Color Picker dialog
 * @return pointer to newly created color picker
 */
GObject *
pip_color_picker_new ();

/**
 * Add color to pallete
 * @param picker Picker to add color into
 * @param color Color to be added
 */
void
pip_color_picker_add_color (
                PipColorPicker    *picker,
                PipCalendarColor   color);

/**
 * Add all default colors into picker
 * @param picker pointer ot picker object
 */
void
pip_color_picker_add_all_colors (
                PipColorPicker    *picker,
                PipColorPickerSet color_set);

/**
 * Select color.
 * @param picker
 * @param color
 */
void
pip_color_picker_highlight_color (
                PipColorPicker   *picker,
                PipCalendarColor   color);

/**
 * Run modal Color picker dialog
 * @param picker dialog to run
 */
void
pip_color_picker_run(
                PipColorPicker    *picker);

/**
 * Get selected color (once pip_color_picker_run is called)
 * @param picker 
 * @return selected color, -1 if dialog was dismissed
 */
PipCalendarColor
pip_color_picker_get_selected_color (
                PipColorPicker   *picker);

#endif
