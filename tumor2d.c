#include <gtk/gtk.h>
#include "buttons.h"
#include "cell.h"

#define SIZE_X 200
#define SIZE_Y 200
#define TITLE "Simple tumor simulator 2d"

static void hello(GtkWidget *widget, gpointer data) {
  g_print("Hello World\n");
}

static gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
  g_print("delete event occurred\n");
  return FALSE;
}
static void destroy(GtkWidget *widget, gpointer data){
  gtk_main_quit();
}

int main(int argc, char *argv[]) {
  GtkBuilder *builder;
  GtkWidget *da;
  GObject *window;
  GObject *startButton, *stopButton, *frame;
  int **cells;

  initializeCells(&cells, SIZE_X, SIZE_Y);
  gtk_init(&argc, &argv);

  builder = createButtons();
  window = gtk_builder_get_object(builder, "window");
  gtk_window_set_title (GTK_WINDOW (window), TITLE);
  startButton = gtk_builder_get_object(builder, "startButton");
  stopButton = gtk_builder_get_object(builder, "stopButton");
  frame = gtk_builder_get_object(builder, "frame");
  g_signal_connect(window, "destroy", G_CALLBACK(destroy), NULL);

  da = gtk_drawing_area_new();
  gtk_widget_set_size_request(GTK_WIDGET(frame), 700, 700);
  gtk_widget_set_size_request(da, 700, 700);
  gtk_container_add(GTK_CONTAINER (frame), da);

  g_signal_connect(da, "draw", G_CALLBACK(draw_cb), NULL);
  g_signal_connect(da,"configure-event", G_CALLBACK(configure_event_cb), NULL);

  clear_surface();

  
  
  gtk_main();
  
  return 0;
}