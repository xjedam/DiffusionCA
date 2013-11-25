#include <gtk/gtk.h>

static cairo_surface_t *surface = NULL;

GtkBuilder *createButtons() {
  GtkBuilder *builder;

  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "interface.ui", NULL);

  return builder;
}

void clear_surface(cairo_t *cr){
  printf("clearing\n\n");

  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_paint(cr);

  //cairo_destroy(cr);
}

gboolean configure_event_cb(GtkWidget *widget, GdkEventConfigure *event, gpointer data){

  if(surface)
    cairo_surface_destroy(surface);
  //gtk_widget_override_background_color(widget, GTK_STATE_NORMAL, color);
  surface = gdk_window_create_similar_surface(gtk_widget_get_window (widget),
                                       CAIRO_CONTENT_COLOR,
                                       gtk_widget_get_allocated_width (widget),
                                       gtk_widget_get_allocated_height (widget));

  //clear_surface();
  return TRUE;
}

void draw_sq(cairo_t *cr, gdouble x, gdouble y, int width){
  cairo_rectangle(cr, x , y, width, width);
}

void draw_cir(cairo_t *cr, gdouble x, gdouble y, int width){
  cairo_arc(cr, x, y, width / 2.0, 0, 2 * G_PI);
}




