#include <furi.h>
#include <gui/gui.h>
#include <api-hal-version.h>

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

typedef struct {
} State;

static void input_callback(InputEvent* input_event, void* ctx) {
    osMessageQueueId_t event_queue = ctx;
    AppEvent event;
    event.type = EventTypeKey;
    event.value.input = *input_event;
    osMessageQueuePut(event_queue, &event, 0, osWaitForever);
}

static void render_callback(Canvas* canvas, void* ctx) {
    State* state = (State*)acquire_mutex((ValueMutex*)ctx, 25);
    canvas_clear(canvas);
    canvas_draw_icon_name(canvas, 0, 0, I_PNG_TEST_128x64);

    release_mutex((ValueMutex*)ctx, state);
}

int32_t passport(void* p) {
    State _state;
    ValueMutex state_mutex;

    osMessageQueueId_t event_queue = osMessageQueueNew(1, sizeof(AppEvent), NULL);
    furi_check(event_queue);

    if(!init_mutex(&state_mutex, &_state, sizeof(State))) {
        printf("cannot create mutex\r\n");
        return 0;
    }

    ViewPort* view_port = view_port_alloc();

    view_port_draw_callback_set(view_port, render_callback, &state_mutex);
    view_port_input_callback_set(view_port, input_callback, event_queue);

    Gui* gui = furi_record_open("gui");
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);

    AppEvent event;
    while(1) {
        osStatus_t event_status = osMessageQueueGet(event_queue, &event, NULL, 25);
        if(event_status == osOK) {
            if(event.type == EventTypeKey && event.value.input.type == InputTypeShort) {
                break;
            }
        }

        view_port_update(view_port);
    }

    gui_remove_view_port(gui, view_port);

    view_port_free(view_port);

    furi_record_close("gui");

    delete_mutex(&state_mutex);

    osMessageQueueDelete(event_queue);

    return 0;
}
