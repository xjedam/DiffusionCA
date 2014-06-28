#include <gtk/gtk.h>

static cairo_surface_t *surface = NULL;

// creates the buttons defined in interface.ui
GtkBuilder *createButtons() {
  GtkBuilder *builder;

  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "interface.ui", NULL);

  return builder;
}

// clears the drawing surface
void clear_surface(cairo_t *cr){
  cairo_set_source_rgb(cr, 255, 255, 255);
  cairo_paint(cr);
}

// recreate the surface
gboolean configure_event_cb(GtkWidget *widget, GdkEventConfigure *event, gpointer data){
  if(surface)
    cairo_surface_destroy(surface);
  surface = gdk_window_create_similar_surface(gtk_widget_get_window (widget),
                                       CAIRO_CONTENT_COLOR,
                                       gtk_widget_get_allocated_width (widget),
                                       gtk_widget_get_allocated_height (widget));

  return TRUE;
}

// draw square at given coords
void draw_sq(cairo_t *cr, gdouble x, gdouble y, int width){
  cairo_rectangle(cr, x , y, width, width);
}

// draw circle at given coords
void draw_cir(cairo_t *cr, gdouble x, gdouble y, int width){
  cairo_arc(cr, x+1, y+1, width / 2.0, 0, 2 * G_PI);
}




