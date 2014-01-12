#include <gtk/gtk.h>
#include <time.h>
#include "interface.h"
#include "main.h"
#include "cell.h"

int **cells, **buff, isPause = 1;
clock_t start, stop;

static void calculate_cycle(GtkWidget *widget) {
  int i, j, **tmp;
  for(i = 0; i<MODEL_SIZE_X; i++) {
    for(j = 0; j<MODEL_SIZE_Y; j++) {
      if(cells[i][j] != 0) {
        calculateCell(cells, buff, i, j);
      }
    }
  }
  tmp = cells;
  cells = buff;
  buff = tmp;
  if(!isPause) {
    gtk_widget_queue_draw(widget);
  }
}

static gboolean draw_cb(GtkWidget *widget, cairo_t *cr, gpointer data) {
  int i, j, ziarno;
  stop = clock();
  if(((float)(stop - start))/CLOCKS_PER_SEC < ITER_DELAY && !isPause){
    //printf("%f\n", ITER_DELAY - (((float)(stop - start))/CLOCKS_PER_SEC));
    SLEEP_FUNC((ITER_DELAY - (((float)(stop - start))/CLOCKS_PER_SEC))*1000);
  }
  start = clock();
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
  int count = 0;
  for(i = 0; i<MODEL_SIZE_X; i++) {
    for(j = 0; j<MODEL_SIZE_Y; j++) {
      if(cells[i][j] != 0) {
        count++;
        //printf("%i at [%i, %i]; ", cells[i][j], i, j);
        drawCell(cr, i, j, cells[i][j], CELL_SIZE);
      }
    }
  }
  printf("count: %i\n", count);
  if(!isPause) {
    calculate_cycle(widget);
  }

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
static void startButtonCallback(GtkWidget *widget, gpointer data) {
  isPause = 0;
  GtkWidget *area = GTK_WIDGET(data);
  gtk_widget_queue_draw(area);
}
static void stopButtonCallback(GtkWidget *widget, gpointer data) {
  printf("PAUSE clicked\n");
  isPause = 1;
}

static gboolean buttonPressCallback(GtkWidget *widget, GdkEventButton *event, gpointer data) {
  
  if (event->button == 1)
    {
      //printf("Przycisk wcisniety [%f, %f]\n", event->x, event->y);
      createCell(CHANGE_TYPE((rand()%8)+2,(rand()%5)+1), event->x/(CELL_SIZE+CELL_SEPARATION)-1, event->y/(CELL_SIZE+CELL_SEPARATION)-1, cells);
      gtk_widget_queue_draw(widget);
      //draw_brush (widget, event->x, event->y);
    }
  else if (event->button == 3)
    {
      createCell(0, event->x/(CELL_SIZE+CELL_SEPARATION)-1, event->y/(CELL_SIZE+CELL_SEPARATION)-1, cells);
      gtk_widget_queue_draw(widget);
      //clear_surface ();
      //gtk_widget_queue_draw (widget);
    }

  /* We've handled the event, stop processing */
  return FALSE;
}

static gboolean mouseMotionCallback(GtkWidget *widget, GdkEventMotion *event, gpointer data) {

  if (event->state & GDK_BUTTON1_MASK) {
    createCell(CHANGE_TYPE((rand()%8)+2,(rand()%5)+1), event->x/(CELL_SIZE+CELL_SEPARATION) - 1, event->y/(CELL_SIZE+CELL_SEPARATION) - 1, cells);
    gtk_widget_queue_draw(widget);
    //createCell(4, event->x/(CELL_SIZE+CELL_SEPARATION), event->y/(CELL_SIZE+CELL_SEPARATION), cells);
    //printf("Przycisk ciagniety [%f, %f]\n", event->x, event->y);
  }

  /* We've handled it, stop processing */
  return TRUE;
}

int main(int argc, char *argv[]) {
  GtkBuilder *builder;
  GtkWidget *da;
  GObject *window;
  GObject *startButton, *stopButton, *frame;
  start = clock();
  int seed = time(NULL);
  srand(seed);
  
  initializeCells(&cells, MODEL_SIZE_X, MODEL_SIZE_Y);
  initializeCells(&buff, MODEL_SIZE_X, MODEL_SIZE_Y);
  //cells[52][45] = CHANGE_TYPE(MOVING_R, TYPE1);
  //cells[72][45] = CHANGE_TYPE(MOVING_L, TYPE2);
  //cells[53][47] = CHANGE_TYPE(MOVING_R, TYPE3);
  //cells[72][47] = CHANGE_TYPE(MOVING_L, TYPE4);

  cells[20][20] = CHANGE_TYPE(MOVING_R, TYPE1);
  cells[26][26] = CHANGE_TYPE(MOVING_UP, TYPE5);

  cells[55][8] = CHANGE_TYPE(MOVING_DOWN_L, TYPE1);
  cells[55][9] = CHANGE_TYPE(MOVING_L, TYPE2);
  //cells[26][45] = 3;
  gtk_init(&argc, &argv);

  builder = createButtons();
  window = gtk_builder_get_object(builder, "window");
  gtk_widget_set_size_request(GTK_WIDGET(window), WINDOW_SIZE_X, WINDOW_SIZE_Y);
  gtk_window_set_title (GTK_WINDOW (window), TITLE);
  startButton = gtk_builder_get_object(builder, "startButton");
  stopButton = gtk_builder_get_object(builder, "stopButton");
  g_signal_connect (stopButton, "clicked", G_CALLBACK (stopButtonCallback), NULL);
  frame = gtk_builder_get_object(builder, "frame");
  g_signal_connect(window, "destroy", G_CALLBACK(destroy), NULL);

  da = gtk_drawing_area_new();
  
  gtk_widget_set_size_request(GTK_WIDGET(frame), WINDOW_SIZE_X-40, WINDOW_SIZE_Y-40);
  gtk_widget_set_size_request(da, WINDOW_SIZE_X-50, WINDOW_SIZE_Y-50);

  g_signal_connect(da, "draw", G_CALLBACK(draw_cb), NULL);
  g_signal_connect(da,"configure-event", G_CALLBACK(configure_event_cb), NULL);
  g_signal_connect (da, "motion-notify-event", G_CALLBACK(mouseMotionCallback), da);
  g_signal_connect(da, "button-press-event", G_CALLBACK(buttonPressCallback), da);
  gtk_widget_set_events (da, gtk_widget_get_events(da)
                             | GDK_BUTTON_PRESS_MASK
                             | GDK_POINTER_MOTION_MASK);

  gtk_container_add(GTK_CONTAINER (frame), da);
  g_signal_connect (startButton, "clicked", G_CALLBACK (startButtonCallback), da);

  gtk_widget_show_all(GTK_WIDGET(window));
  //drawCell(da, 15,15,15,CELL_SIZE+11);
  
  gtk_main();
  
  return 0;
}