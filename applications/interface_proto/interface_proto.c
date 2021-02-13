#include <furi.h>
#include <gui/gui.h>
#include <gui/elements.h>
#include <input/input.h>

//Tamagochi start screen concept ()

#define OFFSET_MID 80
#define OFFSET_FULL 108

extern TIM_HandleTypeDef SPEAKER_TIM;

// dirtyyy 
const uint8_t Plugins_14_0[] = {0xE7,0x00,0xA5,0x00,0x99,0x01,0x01,0x02,0x01,0x02,0x81,0x01,0x81,0x0E,0xE7,0x08,0x24,0x18,0x58,0x20,0x40,0x20,0x30,0x18,0x10,0x08,0xF0,0x0F,};
const uint8_t Settings_14_0[] = {0x03,0x07,0x87,0x04,0x8E,0x02,0x9C,0x32,0xF8,0x2C,0x50,0x20,0x30,0x1E,0x1E,0x03,0x81,0x04,0xCD,0x09,0x53,0x13,0x50,0x26,0x48,0x2C,0x38,0x18,};
const uint8_t FileManager_14_0[] = {0xFC,0x07,0x04,0x04,0xF4,0x05,0x04,0x04,0xF7,0x05,0x05,0x04,0xF5,0x3F,0x15,0x20,0x0D,0x20,0x0D,0x10,0x05,0x10,0x05,0x08,0x03,0x08,0xFE,0x07,};
const uint8_t DolphinExcited_64x63_0[] = {0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x08,0x00,0x00,0x00,0xF8,0xFF,0x00,0x00,0x04,0x00,0x00,0x80,0x07,0x00,0x07,0x00,0x02,0x00,0x00,0x70,0x00,0x00,0x18,0x00,0x01,0x00,0x00,0x08,0x00,0x00,0x20,0x00,0x00,0x0E,0x00,0x04,0x00,0x00,0xC0,0x00,0xC0,0x01,0x00,0x02,0x00,0x00,0x00,0x01,0x38,0x00,0x00,0x01,0x00,0x00,0x00,0x02,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x40,0x00,0x00,0x00,0x00,0x08,0xE0,0x03,0x40,0x00,0x00,0x00,0x00,0x08,0x1C,0x0C,0x20,0x00,0x3C,0x00,0x00,0x10,0x03,0x10,0x20,0x00,0xC3,0x00,0x00,0x90,0x00,0x20,0x10,0x80,0x78,0x01,0x00,0x70,0x00,0x40,0x10,0x40,0xFE,0x03,0x00,0x18,0x00,0x40,0x08,0x40,0xCE,0x03,0x00,0x04,0x00,0x40,0x08,0x20,0x8F,0x07,0x00,0x03,0x00,0x5C,0x08,0x20,0x8F,0x07,0xC0,0x00,0x00,0x3F,0x04,0x20,0xFF,0x07,0x30,0x00,0x80,0x1F,0x04,0x20,0xFF,0x07,0x08,0x00,0xE0,0x0F,0x04,0x68,0xFE,0x03,0x00,0x00,0xF0,0x07,0x04,0x54,0xFE,0x03,0x00,0x00,0xFC,0x03,0x02,0xAA,0x78,0x01,0x00,0x00,0xFE,0x01,0x02,0x54,0xFF,0x01,0x00,0x00,0x7F,0x00,0x02,0xAA,0x06,0x06,0x00,0xC0,0x1F,0x00,0x02,0x54,0x01,0x08,0x00,0xE0,0x07,0x00,0x02,0xAA,0x00,0x00,0x00,0xF8,0x01,0x00,0x02,0x54,0x00,0x00,0x00,0x7E,0x00,0x00,0x01,0x28,0x00,0x00,0x80,0xBF,0x01,0x00,0x01,0x00,0x00,0x00,0xE0,0xE1,0x1F,0x00,0x01,0x00,0x40,0x00,0x38,0x80,0xF5,0x07,0x01,0x00,0x80,0x01,0x0F,0x00,0xAE,0x7A,0x01,0x00,0x00,0xFE,0x0F,0x00,0x58,0xD5,0x01,0x00,0x00,0x80,0xFF,0x00,0xA0,0xFA,0x01,0x00,0x00,0x00,0x00,0xFF,0xFF,0x8F,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x01,0x00,0x00,0x00,0x00,0x00,0x80,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x7C,0x00,0x01,0x00,0x00,0x00,0x00,0xE0,0x03,0x00,0x01,0x00,0x00,0x00,0xFF,0xDF,0x00,0x00,0x01,0x00,0x00,0x00,0xAA,0xAA,0x00,0x00,0x01,0x00,0x00,0x00,0x54,0xD5,0x00,0x00,0x01,0x00,0x00,0x00,0xA8,0xAA,0x00,0x00,0x01,0x00,0x00,0x00,0x54,0x55,0x01,0x00,0x01,0x00,0x00,0x00,0xA8,0xAA,0x01,0x00,0x01,0x00,0x00,0x00,0x50,0x55,0x01,0x00,0x02,0x00,0x00,0x00,0xA0,0xAA,0x02,0x00,0x02,0x00,0x00,0x00,0x40,0x55,0x03,0x00,0x02,0x00,0x00,0x00,0x80,0xAA,0x02,0x00,0x02,0x00,0x00,0x00,0x00,0x54,0x04,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x08,0x00,};


typedef struct {
    int y[4];
    int x[4];
    int total[4];
    int selected_screen;
    int selected_item;
    int screen;
    int menu_offset;
    int list_offset;
    bool browser_sd;
} State;

typedef struct {
    char* name;
    void (*callback)(State* state);
    const uint8_t* icon;
    const int offset;
} Menu_Item;


/* Menu items 
    - Abilities/Skills  (APPS)
    - Inventory (sd file manager + internal flash items - passport, quest book, keys, etc)
    - Settings - Stats, Power usage, etc

    to extend concept further - 
    2 possible customizable user entities: 
        - items (simple one liners, containing ibutton key or bmp array, etc (custom hats for dolphin :D))
        - skills (complete apps)

*/



typedef struct {
    char* name;
} Submenu_Item;

typedef struct {
    char* name;
    int type;
    void (*callback)(State* state);
} Inventory_Item;


void reset_selected_item(State* state){
    state->selected_item = -1;
}

void open_skills(State* state);
void open_inventory(State* state);
void open_settings(State* state);

void test_callback(State* state){
    reset_selected_item(state);
};

const Submenu_Item skills[]={
    {"NFC"}, {"RFID"}, {"CC1101"}, {"Bluetooth"}, {"Infared"}, {"iButton"}
};

const Inventory_Item inventory[]={
    {"Passport", 99, test_callback}, {"Work key", 1, test_callback}, {"Garage door", 2, test_callback}, 
    {"Health potion", 3, test_callback}, {"My_crack", 4, test_callback}, {"Key_test", 5, test_callback}
};
const Inventory_Item inventory_sd[]={
    {"Quest book", 98, test_callback}, {"Hat", 98, test_callback}, {"--", 98, test_callback}, 
    {"--", 98, test_callback}, {"--", 98, test_callback}, {"--", 98, test_callback}
};

const Submenu_Item settings[]={
    {"Power"}, {"Statistics"}, {"-"}, {"--"}, {"---"}, {"----"}
};

const Menu_Item Menu[] = {
    {"Skills", open_skills, Plugins_14_0, OFFSET_MID},
    {"Inventory", open_inventory, FileManager_14_0, OFFSET_FULL},
    {"Settings", open_settings, Settings_14_0, OFFSET_MID},
};

typedef enum {
    EventTypeTick,
    EventTypeKey,
} EventType;

typedef struct {
    union {
        InputEvent input;
    } value;
    EventType type;
} AppEvent;

void elements_scrollbar_fine(Canvas* canvas, uint8_t width, uint8_t height, uint8_t offset, uint8_t pos, uint8_t total) {
    furi_assert(canvas);

    // prevent overflows
    canvas_set_color(canvas, ColorWhite);
    canvas_draw_box(canvas, width - 3, offset, 3, height - offset - 1);
    // dot line
    canvas_set_color(canvas, ColorBlack);
    for(uint8_t i = offset; i < height + offset; i += 2) {
        canvas_draw_dot(canvas, width - 2, i);
    }
    // Position block
    if(total) {
        uint8_t block_h = ((float)height) / total;
        canvas_draw_box(canvas, width - 3, offset + block_h * pos, 3, block_h);
    }
}

void elements_frame_light(Canvas* canvas, uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
    furi_assert(canvas);

    canvas_draw_line(canvas, x + 2, y, x + width - 2, y);
    canvas_draw_line(canvas, x + 1, y + height - 1, x + width - 2, y + height - 1);

    canvas_draw_line(canvas, x, y + 2, x, y + height - 2);
    canvas_draw_line(canvas, x + width - 1, y + 1, x + width - 1, y + height - 2);

    canvas_draw_dot(canvas, x + 1, y + 1);
}



void open_skills(State* state){
    state->menu_offset = Menu[state->selected_screen].offset;
    state->screen = 1;
};

void open_inventory(State* state){
    state->menu_offset = Menu[state->selected_screen].offset;
    reset_selected_item(state);
    state->screen = 2;
}

void open_settings(State* state){
    state->menu_offset = Menu[state->selected_screen].offset;
    state->screen = 3;
}

static void draw_menu_tile(Canvas* canvas, int x, int y, int idx, bool hovered){
    if(hovered){
        canvas_draw_box(canvas, x, y, 20, 20);
    }else{
        canvas_draw_frame(canvas, x, y, 20, 20);
    }

    // round corners by 1px 
    canvas_set_color(canvas, ColorWhite);
    canvas_draw_dot(canvas, x, y);
    canvas_draw_dot(canvas, x+19, y);
    canvas_draw_dot(canvas, x, y+19);
    canvas_draw_dot(canvas, x+19, y+19);

    // invert selected
    canvas_set_color(canvas, hovered ? ColorWhite : ColorBlack);
    canvas_draw_xbm(canvas, x + 3, y + 3, 14, 14, Menu[idx].icon);

    canvas_set_color(canvas, ColorBlack);
}


static void draw_list(Canvas* canvas, State* state, const Submenu_Item* items){
    int y_pos = state->y[state->screen];
    int shift1 = y_pos - 1;
    int shift2 = y_pos + 1;

    if (shift1 > -1){
        canvas_draw_str(canvas, 156 -  state->menu_offset, 13, items[shift1].name);
        canvas_draw_xbm(canvas, 136 -  state->menu_offset, 3, 14, 14, Menu[0].icon);
    }

    elements_frame(canvas, 132 -  state->menu_offset, 22, 148 - state->menu_offset, 20 );
    canvas_draw_str(canvas, 156 -  state->menu_offset, 35, items[y_pos].name);
    canvas_draw_xbm(canvas, 136 -  state->menu_offset, 25, 14, 14, Menu[0].icon);

    if(shift2 <= state->total[state->screen]){
        canvas_draw_str(canvas, 156 -  state->menu_offset, 55, items[shift2].name);
        canvas_draw_xbm(canvas, 136 -  state->menu_offset, 45, 14, 14, Menu[0].icon);
    }

    elements_scrollbar_fine(canvas, 126, 60, 3, y_pos, 6);

}


static void draw_options(Canvas* canvas, State* state, const Inventory_Item* items, const Submenu_Item* skills){
    canvas_draw_str(canvas, 45, 56, "Use");
    canvas_draw_str(canvas, 76, 56, "Remove");
    int selection_pos = state->x[state->screen] == 0 ? 38 : 69;
    canvas_draw_str(canvas, selection_pos, 57, "*");
}

static void draw_tab(Canvas* canvas, State* state){
    bool sd = state->browser_sd;
    char* browser = sd ? "External" : " Internal";
    int offset = sd ? 55 : 0;
    canvas_draw_str(canvas, 26 + offset , 10, browser);

    if(sd){
        canvas_set_color(canvas, ColorWhite);
        canvas_draw_line(canvas, 23, 0, 74, 0);
        canvas_draw_line(canvas, 22, 0, 22, 10);
        canvas_set_color(canvas, ColorBlack);
        canvas_draw_line(canvas, 26, 5, 71, 5);
        canvas_draw_line(canvas, 26, 5, 23, 10);
        canvas_draw_line(canvas, 74, 0, 70, 10);
        canvas_draw_line(canvas, 23, 10, 69, 10);
    }else{
        canvas_set_color(canvas, ColorWhite);
        canvas_draw_line(canvas, 70, 0, 128, 0);
        canvas_draw_line(canvas, 127, 0, 127, 10);

        canvas_set_color(canvas, ColorBlack);
        canvas_draw_line(canvas, 72, 5, 124, 5);
        canvas_draw_line(canvas, 124, 5, 127, 10);
        //canvas_draw_dot(canvas, 71, 1);
        canvas_draw_line(canvas, 70, 0, 74, 10);
        canvas_draw_line(canvas, 75, 10, 128, 10);
    }

};

static void draw_inventory(Canvas* canvas, State* state, const Inventory_Item* items, const Submenu_Item* skills){
    int y_pos = state->y[state->screen];
    //Categories? <Left Right>
    draw_tab(canvas, state);
    canvas_set_color(canvas, ColorBlack);

    const char* item_type = items[y_pos].type < 7  ? skills[items[y_pos].type].name : "unknown";
    item_type = items[y_pos].type == 99 ? "Flipper ID" : item_type; //dirtyy

    // Scrollable list, update_offset() needed to calculate index / (unify?);
    for(int i = 0; i < 4; ++i){
        int idx = i + state->list_offset;
        const char* item_name = items[idx].name;
        canvas_draw_str(canvas, 28, 22 + i*13, item_name);
        if(y_pos == idx) elements_frame_light(canvas, 24, 13 + i * 13, 97, 13 );
    }

    elements_scrollbar_fine(canvas, 126, 53, 11, y_pos, state->total[2]+1);
    // Description & Usage 

    if(state->selected_item > -1){
        canvas_set_color(canvas, ColorWhite);
        canvas_draw_box(canvas, 30, 20, 90, 40);
        canvas_set_color(canvas, ColorBlack);
        elements_frame(canvas, 30, 20, 90, 40);
        canvas_draw_xbm(canvas, 34, 25, 14, 14, Menu[0].icon);
        
        canvas_draw_str(canvas, 52, 31, items[y_pos].name);
        canvas_draw_str(canvas, 52, 41, item_type);

        draw_options(canvas, state, items, skills);
    }

}

static void draw_screen(Canvas* canvas, State* state){

    if(state->screen > 0){
        canvas_set_color(canvas, ColorWhite);
        canvas_draw_box(canvas, 108 - state->menu_offset, 0, state->menu_offset, 64 );

        canvas_set_color(canvas, ColorBlack);
        canvas_draw_frame(canvas, 130 -  state->menu_offset , 0,  state->menu_offset - 2, 64);

        canvas_set_color(canvas, ColorWhite);
        canvas_draw_dot(canvas, 130 - state->menu_offset, 0);
        canvas_draw_dot(canvas, 130 - state->menu_offset, 63);

        canvas_draw_dot(canvas, 127, 0);
        canvas_draw_dot(canvas, 127, 63);

        canvas_set_color(canvas, ColorBlack);

    } 
    
    if(state->screen == 0) {
        int x_pos = canvas_string_width(canvas, Menu[state->y[state->screen]].name);
        canvas_draw_str(canvas, 105 - x_pos , 10, Menu[state->y[state->screen]].name);
    }

    if(state->screen == 1){
        draw_list(canvas, state, skills);
    }

    if(state->screen == 2){
        draw_inventory(canvas, state, state->browser_sd ? inventory_sd : inventory, skills);
    }

    if(state->screen == 3){
        draw_list(canvas, state, settings);
    }

}

static void render_callback(Canvas* canvas, void* ctx) {
    State* state = (State*)acquire_mutex((ValueMutex*)ctx, 25);
    canvas_clear(canvas);

    canvas_draw_xbm(canvas, 0, 0, 64, 63, DolphinExcited_64x63_0);
    draw_screen(canvas, state);

    for(int i=0; i < 3; ++i){
        draw_menu_tile(canvas, 108 - state->menu_offset,  i*22, i, 
            (state->selected_screen < 0 && state->y[state->screen] == i) || state->selected_screen == i);
    }

    release_mutex((ValueMutex*)ctx, state);
}

static void update_offset(State* state, int length, int bounds, int offset){
    if(state->y[state->screen] > state->list_offset + (bounds - offset)){
        state->list_offset = state->y[state->screen] + 1; 
    }else if(state->y[state->screen]< bounds+(state->y[state->screen] - (1 - offset))){
        state->list_offset = state->y[state->screen] - 1;
    }

    if(state->list_offset > length - bounds - 1) state->list_offset = length - bounds - 1;
    if(state->list_offset < 0) state->list_offset = 0;
}


static void input_callback(InputEvent* input_event, void* ctx) {
    osMessageQueueId_t event_queue = ctx;

    AppEvent event;
    event.type = EventTypeKey;
    event.value.input = *input_event;
    osMessageQueuePut(event_queue, &event, 0, 0);
}

int32_t interface_proto(void* p) {
    osMessageQueueId_t event_queue = osMessageQueueNew(8, sizeof(AppEvent), NULL);
    furi_check(event_queue);

    State _state = {{0,0,0,0},{0,0,0,0},{2,5,5,5}, -1, -1, 0, 0, 0, false};
    ValueMutex state_mutex;
    if(!init_mutex(&state_mutex, &_state, sizeof(State))) {
        printf("[flipseq] cannot create mutex\r\n");
        return 0;
    }

    ViewPort* view_port = view_port_alloc();

    view_port_draw_callback_set(view_port, render_callback, &state_mutex);
    view_port_input_callback_set(view_port, input_callback, event_queue);

    // Open GUI and register view_port
    Gui* gui = furi_record_open("gui");
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);

    AppEvent event;
    while(1) {
        osStatus_t event_status = osMessageQueueGet(event_queue, &event, NULL, osWaitForever);
        State* state = (State*)acquire_mutex_block(&state_mutex);


        if(event_status == osOK) {
            if(event.type == EventTypeKey) {
                if(event.value.input.type == InputTypeLong &&
                   event.value.input.key == InputKeyBack) {
                    printf("[flipseq] bye!\r\n");
                    // TODO remove all view_ports create by app
                    view_port_enabled_set(view_port, false);
                    return 0;
                }
                if(event.value.input.type == InputTypeShort &&
                   event.value.input.key == InputKeyBack) {
                        // cancel action if item selected in inventory
                        if(state->screen == 2 && state->selected_item > -1){
                            reset_selected_item(state);
                        // back 
                        }else{
                            state->menu_offset = 0;
                            state->screen = 0;
                            state->selected_screen = -1;
                            reset_selected_item(state);
                       }
                }
                if(event.value.input.type == InputTypeShort &&
                   event.value.input.key == InputKeyRight) {
                       if(state->screen == 2){
                            if(state->selected_item > -1){
                                state->x[state->screen] = 1;
                            }else{
                                state->browser_sd = true;
                                state->y[state->screen] = 0;
                            }
                        }
                }
                if(event.value.input.type == InputTypeShort &&
                   event.value.input.key == InputKeyLeft) {
                        if(state->screen == 2){ 
                            if(state->selected_item > -1){
                                state->x[state->screen] = 0;
                            }else{
                                state->browser_sd = false;
                                state->y[state->screen] = 0;
                            }
                            
                        }

                }
                if(event.value.input.type == InputTypeShort &&
                   event.value.input.key == InputKeyUp) {
                        if(state->selected_item == -1){
                            --state->y[state->screen];
                            if(state->y[state->screen] < 0){
                                state->y[state->screen] = 0;
                            }
                            update_offset(state, 6, 3, 1);
                        }
                }
                if(event.value.input.type == InputTypeShort &&
                   event.value.input.key == InputKeyDown) {
                        if(state->selected_item == -1){
                            ++state->y[state->screen];
                            if(state->y[state->screen] > state->total[state->screen]){
                                state->y[state->screen] = state->total[state->screen];
                            }
                            update_offset(state, 6, 3, 1);
                        }
                }

                if(event.value.input.type == InputTypeShort &&
                    event.value.input.key == InputKeyOk) {
                        
                        if(state->screen == 0){
                            state->selected_screen = state->y[state->screen];
                            Menu[state->y[state->screen]].callback(state);
                        } else if(state->screen==2){
                            if(state->selected_item == -1){
                                state->selected_item = state->y[state->screen];
                            } else if(state->selected_item > -1){
                                inventory[state->selected_item].callback(state);
                            }
                            
                        }
                }
            }
        }
 

        release_mutex(&state_mutex, state);
        view_port_update(view_port);
    }
}
