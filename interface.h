GtkBuilder *createButtons();
void clear_surface();
gboolean configure_event_cb(GtkWidget *widget, GdkEventConfigure *event, gpointer data);
void draw_sq(cairo_t *cr, gdouble x, gdouble y, int width);
void draw_cir(cairo_t *cr, gdouble x, gdouble y, int width);