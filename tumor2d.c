#include <gtk/gtk.h>
#include "interface.h"
#include "main.h"
#include "cell.h"

int **cells, **buff;

static void calculate_cycle(GtkWidget *widget) {
  int i, j, **tmp;
  for(i = 0; i<MODEL_SIZE_X; i++) {
    for(j = 0; j<MODEL_SIZE_Y; j++) {
      if(cells[i][j] != 0) {
        calculateCell(cells, buff, i, j);
      } else {
        //buff[i][j] = 0;
      }
    }
  }
  tmp = cells;
  cells = buff;
  buff = tmp;
  gtk_widget_queue_draw(widget);
}

static gboolean draw_cb(GtkWidget *widget, cairo_t *cr, gpointer data) {
  int i, j;
  clear_surface(cr);
  guint width, height;
  GdkRGBA color = {.5, .5, .5, 1.0};
  gdk_cairo_set_source_rgba (cr, &color);
  width = gtk_widget_get_allocated_width (widget);
  height = gtk_widget_get_allocated_height (widget);
  // drawCell(cr, 1,1,1,CELL_SIZE);
  // moveCell(cr, 1, 1, 1, 4, CELL_SIZE);
  // gdk_cairo_set_source_rgba (cr, &color);
  // drawCell(cr, 1,1,2,CELL_SIZE);
  // drawCell(cr, 1,2,2,CELL_SIZE);
  // eraseCell(cr, 2,2,CELL_SIZE);
  // gdk_cairo_set_source_rgba (cr, &color);
  // drawCell(cr, 1,2,3,CELL_SIZE);
  for(i = 0; i<MODEL_SIZE_X; i++) {
    for(j = 0; j<MODEL_SIZE_Y; j++) {
      if(cells[i][j] != 0) {
        drawCell(cr, i, j, cells[i][j], CELL_SIZE);
      } else {
        //eraseCell(cr, i, j, CELL_SIZE);
        //gdk_cairo_set_source_rgba (cr, &color);
      }
    }
  }
  calculate_cycle(widget);

  // gtk_style_context_get_color (gtk_widget_get_style_context (widget),
  //                              0,
  //                              &color);
 return FALSE;
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
  
  initializeCells(&cells, MODEL_SIZE_X, MODEL_SIZE_Y);
  initializeCells(&buff, MODEL_SIZE_X, MODEL_SIZE_Y);
  cells[3][3] = 5;
  cells[6][5] = 7;
  gtk_init(&argc, &argv);

  builder = createButtons();
  window = gtk_builder_get_object(builder, "window");
  gtk_widget_set_size_request(GTK_WIDGET(window), WINDOW_SIZE_X, WINDOW_SIZE_Y);
  gtk_window_set_title (GTK_WINDOW (window), TITLE);
  startButton = gtk_builder_get_object(builder, "startButton");
  stopButton = gtk_builder_get_object(builder, "stopButton");
  frame = gtk_builder_get_object(builder, "frame");
  g_signal_connect(window, "destroy", G_CALLBACK(destroy), NULL);

  da = gtk_drawing_area_new();
  gtk_widget_set_size_request(GTK_WIDGET(frame), WINDOW_SIZE_X-40, WINDOW_SIZE_Y-40);
  gtk_widget_set_size_request(da, WINDOW_SIZE_X-50, WINDOW_SIZE_Y-50);
  gtk_container_add(GTK_CONTAINER (frame), da);

  g_signal_connect(da, "draw", G_CALLBACK(draw_cb), NULL);
  g_signal_connect(da,"configure-event", G_CALLBACK(configure_event_cb), NULL);

  //clear_surface();

  gtk_widget_show_all(GTK_WIDGET(window));
  //drawCell(da, 15,15,15,CELL_SIZE+11);
  
  gtk_main();
  
  return 0;
}