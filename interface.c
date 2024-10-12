#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <unistd.h>

#include "interface.h"
#include "mutex.h"

#define TB_IMPL
#include "termbox2.h"

static int block_width = 14;
static int block_height = 4;

void draw_block(int x, int y, const char *label, int empty) {

	tb_set_cell(x, y, 0x250C, TB_WHITE, TB_DEFAULT);
    tb_set_cell(x+block_width-1, y, 0x2510, TB_WHITE, TB_DEFAULT);
    tb_set_cell(x, y+block_height-1, 0x2514, TB_WHITE, TB_DEFAULT);
    tb_set_cell(x+block_width-1, y+block_height-1, 0x2518, TB_WHITE, TB_DEFAULT);
	
	tb_printf(x+(block_width/2) - (strlen(label)/2), y+(block_height/2)-1 ,TB_YELLOW, 0, label);
	tb_printf(x+(block_width/2) - (strlen("empty")/2), y+(block_height/2) ,TB_DIM, 0, "empty");
	
	tb_present();
}

void draw_blocks() {
	int offset_x = 0;
	int offset_y = 6;
	char col_names[6] = {'A', 'B', 'C', 'D', 'E', 'F'};

	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 6; c++) {
			char label[3];
            label[0] = col_names[c];
            label[1] = '1' + r;
            label[2] = '\0';
			draw_block(offset_x+(block_width*c), offset_y+(block_height*r), label, 0);
		}
	}
}

void draw_help_tooltip(const char* tooltip_text) {
	tb_printf(tb_width() - strlen(tooltip_text) - 1, tb_height()-1, TB_DIM, 0, tooltip_text);
	tb_present();
}

void *interface(void *arg) {
	InterfaceArgs* args = (InterfaceArgs*)arg;

	int ret;
	setlocale(LC_ALL, "");

	ret = tb_init();
	if (ret) {
		fprintf(stderr, "tb_init() failed with error code %d\n", ret);
		exit(1);
	}

	tb_set_input_mode(TB_INPUT_ESC | TB_INPUT_MOUSE);
	struct tb_event ev;

	tb_clear();
	tb_present();

	// Show currently selected soundfont.
	tb_printf(0, 0, TB_GREEN, 0, "Soundfont: %s", args->soundfont_file);
	tb_printf(0, 1, TB_GREEN, 0, "Preset: %s", args->soundfont_preset);
	tb_present();

	// Draw interface.
	draw_help_tooltip("Ctrl+q - Quit");
	draw_blocks();
	/* draw_block(10, 10, "A1"); */

	while(1) {
		ret = tb_poll_event(&ev);

		if (ret != TB_OK) {
			if (ret == TB_ERR_POLL && tb_last_errno() == EINTR) {
				/* Poll was interrupted, maybe by a SIGWINCH; try again */
				continue;
			}
			/* Some other error occurred; bail */
			break;
		}

		switch (ev.type) {
			case TB_EVENT_KEY: 
				if (ev.key == TB_KEY_CTRL_Q) {
					tb_shutdown();
					exit(0);
				}

				if (ev.key == TB_KEY_ARROW_UP) {

				}

				if (ev.key == TB_KEY_ARROW_DOWN) {

				}

				break;
		}	
	}
}

