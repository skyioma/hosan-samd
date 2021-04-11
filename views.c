#include "views.h"

#include <EPD/Config/Debug.h>
#include <EPD/Config/DEV_Config.h>
#include <EPD/GUI/GUI_Paint.h>
#include <EPD/e-Paper/EPD_2in13.h>

#define ITEM_ICON_SYMBOLS_COUNT 4

#include "sensor_data.h"

// Depends on title font position and height.
#define X_START_POS 2
#define Y_START_POS 14

#define X_TITLE_START_POS 2
#define Y_TITLE_START_POS 2

#define VIEW_LIST_FONT_HEIGHT 12
#define VIEW_LIST_FONT_WIDTH 7
#define VIEW_LIST_CHARS_COUNT 20
#define VIEW_LIST_FONT_NAME Font12

enum view_id {
  VI_SENSORS_VAR1,
  // NOTE Add above only!
  VI_MENU_STATE,
  VI_COUNT
};

struct position {
  uint8_t x1;
  uint8_t y1;
  uint8_t x2;
  uint8_t y2;
};

enum position_flags {
  PF_ABS_X = 0x01,
  PF_REL_X = 0x02,
  PF_ABS_Y = 0x04,
  PF_REL_Y = 0x08
};

struct position_desc {
  sFONT *font;
  uint8_t pos_flags;
  int16_t x0_add;
  int16_t y0_add;

  int8_t chars_count;
};

enum item_flags {
  IF_DISABLED = 0x01
};

struct item_desc {
  char item_icon[ITEM_ICON_SYMBOLS_COUNT];
  uint8_t item_flags;
  const char* item_text;
  const char* (*getter)(uint8_t width);
  struct position_desc position_desc;
  uint8_t cache_index;
  void (*button_handler)(void);
  const struct item_desc *next_item;
};

struct view_desc {
  const char *view_title;
  void (*background_painter)(void);
  const struct item_desc *first_item;
};

static void sensors_var1_background_painter(void);

static const struct item_desc item_desc_sensors_var1_this_vbat = {
  .item_icon = "mV ",
  .getter = sensor_get_last_vbat,
  .position_desc = { .font = &Font12, .pos_flags = PF_ABS_X | PF_REL_Y, .chars_count = 10 },
  .cache_index = __COUNTER__
};

static const struct item_desc item_desc_sensors_var1_this_humidity = {
  .item_icon = "H %",
  .getter = sensor_get_last_humidity,
  .position_desc = { .font = &Font12, .pos_flags = PF_ABS_X | PF_REL_Y, .chars_count = 10 },
  .cache_index = __COUNTER__,
  .next_item = &item_desc_sensors_var1_this_vbat
};

static const struct item_desc item_desc_sensors_var1_this_pressure = {
  .item_icon = "Pa ",
  .getter = sensor_get_last_pressure,
  .position_desc = { .font = &Font12, .pos_flags = PF_ABS_X | PF_REL_Y, .chars_count = 10 },
  .cache_index = __COUNTER__,
  .next_item = &item_desc_sensors_var1_this_humidity
};

static const struct item_desc item_desc_sensors_var1_this_temperature = {
  .item_icon = "T'C",
  .getter = sensor_get_last_temperature,
  .position_desc = { .font = &Font12, .pos_flags = PF_ABS_X | PF_REL_Y, .chars_count = 10 },
  .cache_index = __COUNTER__,
  .next_item = &item_desc_sensors_var1_this_pressure
};

static const struct item_desc item_desc_sensors_var1_this_static_iaq = {
  .item_icon = "STA",
  .getter = sensor_get_last_static_iaq,
  .position_desc = { .font = &Font12, .pos_flags = PF_ABS_X | PF_REL_Y, .chars_count = 10 },
  .cache_index = __COUNTER__,
  .next_item = &item_desc_sensors_var1_this_temperature
};

static const struct item_desc item_desc_sensors_var1_this_iaq_accuracy = {
  .item_icon = "ACC",
  .getter = sensor_get_last_iaq_accuracy,
  .position_desc = { .font = &Font12, .pos_flags = PF_ABS_X | PF_REL_Y, .chars_count = 10 },
  .cache_index = __COUNTER__,
  .next_item = &item_desc_sensors_var1_this_static_iaq
};

static const struct item_desc item_desc_sensors_var1_this_iaq = {
  .item_icon = "IAQ",
  .getter = sensor_get_last_iaq,
  .position_desc = { .font = &Font12, .pos_flags = PF_ABS_X | PF_REL_Y, .chars_count = 10 },
  .cache_index = __COUNTER__,
  .next_item = &item_desc_sensors_var1_this_iaq_accuracy
};

static const struct item_desc item_desc_sensors_var1_this_bsec_status = {
  .item_icon = "ST ",
  .getter = sensor_get_last_bsec_status,
  .position_desc = { .font = &Font12, .pos_flags = PF_ABS_X | PF_ABS_Y, .chars_count = 10 },
  .cache_index = __COUNTER__,
  .next_item = &item_desc_sensors_var1_this_iaq
};

// VI_SENSORS_VAR1
static const struct view_desc view_desc_sensors_var1 = {
  .view_title = "View: Sensors",
  .background_painter = sensors_var1_background_painter,
  .first_item = &item_desc_sensors_var1_this_bsec_status
};

static const struct item_desc item_desc_menu_save = {
  .item_text = "State Save",
  .position_desc = { .font = &Font12, .pos_flags = PF_ABS_X | PF_REL_Y, .chars_count = 15 },
  .cache_index = __COUNTER__
};

static const struct item_desc item_desc_menu_load = {
  .item_text = "State Load",
  .position_desc = { .font = &Font12, .pos_flags = PF_ABS_X | PF_ABS_Y, .chars_count = 15 },
  .cache_index = __COUNTER__,
  .next_item = &item_desc_menu_save
};

// VI_MENU_STATE
static const struct view_desc menu_desc = {
  .view_title = "Menu",
  .first_item = &item_desc_menu_load
};

static enum view_id prev_view_id = VI_COUNT;
static enum view_id current_view_id = VI_SENSORS_VAR1;
static int8_t current_item_index = -1;

static const struct view_desc *views_array[VI_COUNT] = {
  &view_desc_sensors_var1,
  &menu_desc,
};

static struct position position_cache[5];

static UBYTE *BlackImage;

static void fill_position_cache(void);
static int8_t paint_item_rectangle(int8_t index, uint8_t color);
static const struct position *find_position_cache(int8_t index);
static void run_item_handler(int8_t index);
static void show_menu_view(void);
static void return_back(void);
static void show_view_list(void);
static void view_list_paint(void);

void fill_position_cache()
{
  for (unsigned i = 0; i < VI_COUNT; ++i) {
    const struct view_desc *cur_view = views_array[i];
    const struct item_desc *cur_item = cur_view->first_item;
    int x1 = X_START_POS;
    int y1 = Y_START_POS;
    int x2 = X_START_POS;
    int y2 = Y_START_POS;

    while (cur_item != NULL) {
      const struct position_desc *pos_desc = &cur_item->position_desc;
      const unsigned font_w = pos_desc->font->Width;
      const unsigned font_h = pos_desc->font->Height;
      const uint8_t pos_flags = pos_desc->pos_flags;

      if (pos_flags & PF_ABS_X)
        x1 = X_START_POS + pos_desc->x0_add;
      else if (pos_flags & PF_REL_X)
        x1 = x2 + 1 + pos_desc->x0_add;

      if (pos_flags & PF_ABS_Y)
        y1 = Y_START_POS + pos_desc->y0_add;
      else if (pos_flags & PF_REL_Y)
        y1 = y2 + 1 + pos_desc->y0_add;

      x2 = x1 + pos_desc->chars_count * font_w - 1;
      y2 = y1 + font_h - 1;

      position_cache[cur_item->cache_index].x1 = x1;
      position_cache[cur_item->cache_index].y1 = y1;
      position_cache[cur_item->cache_index].x2 = x2;
      position_cache[cur_item->cache_index].y2 = y2;

      cur_item = cur_item->next_item;
    }
  }
}

void views_init()
{
  if (sizeof(position_cache) / sizeof(position_cache[0]) < __COUNTER__)
    return;

  fill_position_cache();

  DEV_Module_Init();

  EPD_2IN13_Init(EPD_2IN13_FULL);
  EPD_2IN13_Clear();

  UWORD Imagesize = ((EPD_2IN13_WIDTH % 8 == 0)? (EPD_2IN13_WIDTH / 8 ): (EPD_2IN13_WIDTH / 8 + 1)) * EPD_2IN13_HEIGHT;

  if ((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
    return;
  }

  Paint_NewImage(BlackImage, EPD_2IN13_WIDTH, EPD_2IN13_HEIGHT, 270, WHITE);
  Paint_SelectImage(BlackImage);
  Paint_Clear(WHITE);
}

void views_paint()
{
  EPD_2IN13_Clear();
  EPD_2IN13_Init(EPD_2IN13_PART);

  const struct view_desc *cur_view = views_array[current_view_id];

  Paint_Clear(WHITE);
  Paint_DrawString_EN(X_TITLE_START_POS, Y_TITLE_START_POS, cur_view->view_title, &Font12, WHITE, BLACK);

  if (cur_view->background_painter != NULL)
    cur_view->background_painter();

  const struct item_desc *cur_item = cur_view->first_item;

  while (cur_item != NULL) {
    const uint8_t cache_index = cur_item->cache_index;

    Paint_DrawString_EN(position_cache[cache_index].x1, position_cache[cache_index].y1,
                        cur_item->item_text, cur_item->position_desc.font, WHITE, BLACK);

    cur_item = cur_item->next_item;
  }

  EPD_2IN13_Display(BlackImage);
}

void sensors_var1_background_painter()
{
  Paint_DrawString_EN(2, 122 - 24, "Wed", &Font24, WHITE, BLACK);
  Paint_DrawString_EN(2 + 17 * 3 + 8, 122 - 24, "Mar", &Font24, WHITE, BLACK);
  Paint_DrawNum(2 + 17 * 3 + 8 + 17 * 3 + 8, 122 - 24, 31, &Font24, BLACK, WHITE);
  Paint_DrawNum(2 + 17 * 3 + 8 + 17 * 3 + 8 + 17 * 2 + 12, 122 - 24, 12, &Font24, BLACK, WHITE);
  Paint_DrawString_EN(2 + 17 * 3 + 8 + 17 * 3 + 8 + 17 * 2 + 12 + 17 * 2, 122 - (24 + 20) / 2, ":", &Font20, WHITE, BLACK);
  Paint_DrawNum(2 + 17 * 3 + 8 + 17 * 3 + 8 + 17 * 2 + 12 + 17 * 2 + 14, 122 - 24, 34, &Font24, BLACK, WHITE);
}

const struct position *find_position_cache(int8_t index)
{
  if (index < 0)
    return NULL;

  const struct view_desc *cur_view = views_array[current_view_id];
  const struct item_desc *cur_item = cur_view->first_item;
  int i = 0;

  while (cur_item != NULL && i != index) {
    cur_item = cur_item->next_item;
    ++i;
  }

  if (cur_item != NULL)
    return &position_cache[cur_item->cache_index];

  return NULL;
}

int8_t paint_item_rectangle(int8_t index, uint8_t color)
{
  if (current_view_id != VI_COUNT) {
    const struct position *position = find_position_cache(index);

    if (position == NULL)
      return -1;

    Paint_DrawRectangle(position->x1, position->y1, position->x2, position->y2,
        color, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    EPD_2IN13_Display(BlackImage);
    return 0;
  } else {
    if (index < 0 || index >= VI_MENU_STATE)
      return -1;

    Paint_DrawRectangle(X_START_POS, Y_START_POS + index * VIEW_LIST_FONT_HEIGHT,
                        X_START_POS + VIEW_LIST_CHARS_COUNT * VIEW_LIST_FONT_WIDTH - 1,
                        Y_START_POS + (index + 1) * VIEW_LIST_FONT_HEIGHT - 1,
                        color, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    EPD_2IN13_Display(BlackImage);
    return 0;
  }
}

void run_item_handler(int8_t index)
{
  if (current_view_id != VI_COUNT) {
    if (index < 0)
      return;

    const struct view_desc *cur_view = views_array[current_view_id];
    const struct item_desc *cur_item = cur_view->first_item;
    int i = 0;

    while (cur_item != NULL && i != index) {
      cur_item = cur_item->next_item;
      ++i;
    }

    if (cur_item != NULL && cur_item->button_handler != NULL)
      cur_item->button_handler();
    // TODO Warn?
  } else {
    current_view_id = index;
    prev_view_id = VI_COUNT;
    current_item_index = -1;

    views_paint();
  }
}

void show_view_list()
{
  current_view_id = VI_COUNT;

  view_list_paint();

  current_item_index = 0;
  (void)paint_item_rectangle(current_item_index, BLACK);
}

void view_list_paint()
{
  EPD_2IN13_Clear();
  EPD_2IN13_Init(EPD_2IN13_PART);

  Paint_Clear(WHITE);
  Paint_DrawString_EN(X_TITLE_START_POS, Y_TITLE_START_POS, "Views", &Font12, WHITE, BLACK);

  uint8_t y1 = Y_START_POS;

  for (uint8_t i = 0; i < VI_MENU_STATE; ++i) {
    const struct view_desc *cur_view = views_array[i];

    Paint_DrawString_EN(X_START_POS, y1, cur_view->view_title, &VIEW_LIST_FONT_NAME, WHITE, BLACK);
    y1 += Font12.Height;
  }

  EPD_2IN13_Display(BlackImage);
}

void show_menu_view()
{
  prev_view_id = current_view_id;
  current_view_id = VI_MENU_STATE;

  views_paint();

  current_item_index = 0;
  (void)paint_item_rectangle(current_item_index, BLACK);
}

void return_back()
{
  // In View and no item is selected or in Menu and no way back -> to view list.
  if (current_item_index < 0 || (current_view_id == VI_MENU_STATE && prev_view_id == VI_COUNT)) {
    prev_view_id = VI_COUNT;
    show_view_list();
    return;
  }

  // In Menu and there is a way back -> return back!
  if (prev_view_id != VI_COUNT) {
    current_view_id = prev_view_id;
    prev_view_id = VI_COUNT;
  } // else: repaint current View without selection.

  if (current_view_id == VI_COUNT) {
    show_view_list();
    return;
  }

  current_item_index = -1;
  views_paint();
}

void view_handle_button_1(enum button_press_kind kind)
{
  (void)paint_item_rectangle(current_item_index, WHITE);

  switch (kind) {
    case BPK_CLICK:
      ++current_item_index;

      if (current_item_index < 0)
        current_item_index = 0;

      break;

    case BPK_PUSH:
      if (current_item_index >= 0)
        run_item_handler(current_item_index);
      else
        show_menu_view();

      return;

    case BPK_DOUBLE_CLICK:
      return_back();
      return;

    default:
      return;
  }

  const int8_t result = paint_item_rectangle(current_item_index, BLACK);

  if (result < 0) {
    current_item_index = 0;
    (void)paint_item_rectangle(current_item_index, BLACK);
  }
}
