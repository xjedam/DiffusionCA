#include <gtk/gtk.h>

static cairo_surface_t *surface = NULL;

GtkBuilder *createButtons() {
  GtkBuilder *builder;

  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "interface.ui", NULL);

  return builder;
}

void clear_surface(){
  cairo_t *cr;

  cr = cairo_create(surface);

  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_paint(cr);

  cairo_destroy(cr);
}

gboolean configure_event_cb(GtkWidget *widget, GdkEventConfigure *event, gpointer data){
  if(surface)
    cairo_surface_destroy(surface);

  surface = gdk_window_create_similar_surface(gtk_widget_get_window (widget),
                                       CAIRO_CONTENT_COLOR,
                                       gtk_widget_get_allocated_width (widget),
                                       gtk_widget_get_allocated_height (widget));

  clear_surface();
  return TRUE;
}

gboolean draw_cb(GtkWidget *widget, cairo_t *cr, gpointer data){
  cairo_set_source_surface(cr, surface, 0, 0);
  cairo_paint(cr);

  return FALSE;
}