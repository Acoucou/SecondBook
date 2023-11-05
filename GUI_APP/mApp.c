#include "mApp.h"
#include "lvgl.h"
#include "key.h"
#include "led.h"
#include "checkBook.h"
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

extern SemaphoreHandle_t BinarySemaphore; // ��ֵ�ź������
extern unsigned int musicIndex;

extern char *bookid;
int fun = null;
extern unsigned int preview_flag;

lv_style_t bg_style;
lv_style_t indic_style;
lv_style_t btn_bg_style;
lv_style_t btn_rel_style;
lv_style_t btn_pr_style;
lv_style_t btn_tgl_rel_style;
lv_style_t btn_tgl_pr_style;
lv_style_t sel_style;

lv_obj_t *tabview1;
lv_obj_t *kb1;
lv_obj_t *btn1, *btn2, *btn3, *btn4, *error_btn;
lv_obj_t *ta1, *ta2;
lv_obj_t *tmp_obj;
lv_obj_t *tab1_page;
lv_obj_t *tab2_page;
lv_obj_t *quit_mbox, *sell_mbox, *buy_mbox, *preview_mbox, *commit_mbox, *error_mbox;
lv_obj_t *sell_btn, *buy_btn, *preview_btn;
lv_obj_t *ddlist1; // �����б�

void kb_create(lv_obj_t *parent);
void quit_mbox_create(lv_obj_t *parent);
void commit_mbox_create(lv_obj_t *parent);

void creat_tab1();
void creat_tab2();
void lv_example_table_1();

void event_handler(lv_obj_t *obj, lv_event_t event);
void keb_handler(lv_obj_t *obj, lv_event_t event);
void msg_handler(lv_obj_t *obj, lv_event_t event);
void help_handler(lv_obj_t *obj, lv_event_t event);
void error_handler(lv_obj_t *obj, lv_event_t event);

const char *const MBOX_MAP[] = {"Yes", "No", ""};

void kb_create(lv_obj_t *parent)
{
	if (kb1 == NULL)
	{
		kb1 = lv_kb_create(parent, NULL);
		lv_kb_set_cursor_manage(kb1, true);	 // ʹ�ܶԹ��Ľӹ�
		lv_kb_set_mode(kb1, LV_KB_MODE_NUM); // ����Ϊ�ı�����ģʽ,��Ҳ��Ĭ��ֵ
		lv_kb_set_ta(kb1, ta1);				 // ��Ĭ�ϰ�ta1�ı������,�������ͨ�����ĳ�ı��������°󶨵���Ӧ���ı������

		lv_obj_set_event_cb(kb1, keb_handler); // �����Զ�����¼��ص�����
	}
}

// �����˳�ȷ�ϵ���Ϣ�Ի���
void quit_mbox_create(lv_obj_t *parent)
{
	if (quit_mbox) // ��Ҫ�ظ�����
		return;
	quit_mbox = lv_mbox_create(parent, NULL);
	lv_obj_set_size(quit_mbox, lv_obj_get_width(parent) * 0.7f, lv_obj_get_height(parent) / 2);
	lv_mbox_set_text(quit_mbox, "HELP\nHELLO WORLD!");
	lv_mbox_add_btns(quit_mbox, (const char **)MBOX_MAP);
	lv_obj_set_drag(quit_mbox, true); // ���öԻ�����Ա���ק
	lv_obj_align(quit_mbox, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_event_cb(quit_mbox, help_handler);
}

// ����ȷ����ʾ�Ի���
void commit_mbox_create(lv_obj_t *parent)
{
	commit_mbox = lv_mbox_create(parent, NULL);
	lv_obj_set_size(commit_mbox, lv_obj_get_width(parent) * 0.7f, lv_obj_get_height(parent) / 2);
	lv_mbox_set_text(commit_mbox, "TIP\n Commit OK!");
	lv_mbox_add_btns(commit_mbox, (const char **)MBOX_MAP);
	lv_obj_set_drag(commit_mbox, true); // ���öԻ�����Ա���ק
	lv_obj_align(commit_mbox, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_event_cb(commit_mbox, event_handler);
}

// error
void error_mbox_create(lv_obj_t *parent)
{
	error_mbox = lv_mbox_create(parent, NULL);
	lv_obj_set_size(error_mbox, lv_obj_get_width(parent) * 0.7f, lv_obj_get_height(parent) / 2);
	lv_mbox_set_text(error_mbox, "ERROR\n ARE YOU PREVIEW BOOK?");
	lv_mbox_add_btns(error_mbox, (const char **)MBOX_MAP);
	lv_obj_set_drag(error_mbox, true); // ���öԻ�����Ա���ק
	lv_obj_align(error_mbox, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_event_cb(error_mbox, error_handler);
}

void app_init()
{
	lv_obj_t *scr = lv_scr_act(); // ��ȡ��ǰ��Ծ����Ļ����

	// 1.������ʽ
	// 1.1 ����������ʽ
	lv_style_copy(&bg_style, &lv_style_plain);
	bg_style.body.main_color = LV_COLOR_MAKE(49, 49, 49); // ��ɫ����
	bg_style.body.grad_color = bg_style.body.main_color;
	bg_style.body.border.color = LV_COLOR_MAKE(150, 150, 150); // �߿���ɫ
	bg_style.body.border.width = 2;							   // �߿���
	bg_style.text.color = LV_COLOR_WHITE;
	// 1.2 ����ҳ��ָʾ������ʽ
	lv_style_copy(&indic_style, &lv_style_plain_color);
	indic_style.body.main_color = LV_COLOR_MAKE(42, 212, 66); // ָʾ������ɫ,��ɫ
	indic_style.body.grad_color = indic_style.body.main_color;
	indic_style.body.padding.inner = 3; // ����ָʾ���ĸ߶�
	// 1.3 ����ҳ��ѡ��ť���ı�����ʽ
	// lv_style_transp_tight��ʽ�е�inner,left,top,right,bottom���ڼ��ֵ��Ϊ0,����Ϊ����ҳ��ѡ��ť�ܹ�������һ��
	lv_style_copy(&btn_bg_style, &lv_style_transp_tight);
	// 1.4 ������ť����̬�µ�������ʽ
	lv_style_copy(&btn_rel_style, &lv_style_plain_color);
	btn_rel_style.body.main_color = LV_COLOR_MAKE(98, 98, 98);
	btn_rel_style.body.grad_color = btn_rel_style.body.main_color;
	btn_rel_style.body.border.color = LV_COLOR_MAKE(150, 150, 150); // �߿���ɫ
	btn_rel_style.body.border.width = 1;
	btn_rel_style.text.color = LV_COLOR_WHITE; // ������ɫ
	// 1.5 ������ť����̬�µİ�����ʽ
	lv_style_copy(&btn_pr_style, &btn_rel_style);
	btn_pr_style.body.main_color = LV_COLOR_GRAY;
	btn_pr_style.body.grad_color = btn_pr_style.body.main_color;
	// 1.6 ������ť�л�̬�µ�������ʽ
	lv_style_copy(&btn_tgl_rel_style, &btn_rel_style);
	btn_tgl_rel_style.body.main_color = bg_style.body.main_color; // ����������ɫһ��
	btn_tgl_rel_style.body.grad_color = btn_tgl_rel_style.body.main_color;
	// 1.7 ������ť�л�̬�µİ�����ʽ
	// ���ֺ�btn_tgl_rel_styleһ��������
	lv_style_copy(&btn_tgl_pr_style, &btn_tgl_rel_style);

	// 2.����ѡ�
	tabview1 = lv_tabview_create(scr, NULL);
	lv_obj_set_size(tabview1, lv_obj_get_width(scr), lv_obj_get_height(scr)); // ����ѡ��Ĵ�С,����ĻС16����
	lv_obj_align(tabview1, NULL, LV_ALIGN_CENTER, 0, 0);					  // ����Ļ���ж���
	lv_obj_set_event_cb(tabview1, event_handler);							  // �����¼��ص�����
	lv_tabview_set_btns_pos(tabview1, LV_TABVIEW_BTNS_POS_TOP);				  // ����ҳ��ѡ��ť��λ�ڶ���

	lv_tabview_set_style(tabview1, LV_TABVIEW_STYLE_BG, &bg_style);					  // ���ñ�����ʽ
	lv_tabview_set_style(tabview1, LV_TABVIEW_STYLE_INDIC, &indic_style);			  // ����ҳ��ָʾ������ʽ
	lv_tabview_set_style(tabview1, LV_TABVIEW_STYLE_BTN_BG, &btn_bg_style);			  // ����ҳ��ѡ��ť���ı�����ʽ
	lv_tabview_set_style(tabview1, LV_TABVIEW_STYLE_BTN_REL, &btn_rel_style);		  // ���ð�ť����̬�µ�������ʽ
	lv_tabview_set_style(tabview1, LV_TABVIEW_STYLE_BTN_PR, &btn_pr_style);			  // ���ð�ť����̬�µİ�����ʽ
	lv_tabview_set_style(tabview1, LV_TABVIEW_STYLE_BTN_TGL_REL, &btn_tgl_rel_style); // ���ð�ť�л�̬�µ�������ʽ
	lv_tabview_set_style(tabview1, LV_TABVIEW_STYLE_BTN_TGL_PR, &btn_tgl_pr_style);	  // ���ð�ť�л�̬�µİ�����ʽ

	lv_obj_set_click(scr, true);
	lv_obj_set_event_cb(tabview1, event_handler);
}

// �������
void mApp_start()
{
	app_init();

	creat_tab2();
	creat_tab1();
}

void creat_tab2()
{
	// 2.2 ���tab2����ҳ��
	tab2_page = lv_tabview_add_tab(tabview1, LV_SYMBOL_HOME " Home");

	// �ı�
	lv_obj_t *title_label = lv_label_create(tab2_page, NULL);
	lv_label_set_recolor(title_label, true);									   // ʹ���ı��ػ�ɫ����
	lv_label_set_text(title_label, "#ff0000 Second# #0000ff hand# #00ff00 book#"); // ����"Login"����
	lv_obj_align(title_label, NULL, LV_ALIGN_IN_TOP_MID, 0, 30);				   // ���ö��뷽ʽ

	LV_IMG_DECLARE(qrcode);
	lv_obj_t *img2 = lv_img_create(tab2_page, NULL);
	lv_img_set_src(img2, &qrcode);
	lv_obj_align(img2, NULL, LV_ALIGN_IN_TOP_MID, 0, 150); // ���ö��뷽ʽ

	// ������ҳ��2����ӱ�ǩ�Ӷ���
	btn2 = lv_btn_create(tab2_page, NULL);
	lv_obj_set_pos(btn2, 50, 300);
	lv_obj_set_size(btn2, 200, 40);						   // ���ô�С
	lv_obj_align(btn2, NULL, LV_ALIGN_IN_TOP_MID, 0, 500); // ���ö��뷽ʽ
	lv_obj_t *btn_labe2 = lv_label_create(btn2, NULL);
	lv_label_set_text(btn_labe2, "help");	 // ���ð�ť�ϵı���
	lv_obj_set_event_cb(btn2, help_handler); // �����¼��ص�����

	error_btn = lv_btn_create(tab2_page, NULL);
	lv_obj_set_hidden(error_btn, true);
	lv_obj_align(error_btn, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0); // ���ö��뷽ʽ
	lv_obj_set_event_cb(error_btn, error_handler);				 // �����¼��ص�����
}

void creat_tab1()
{
	// 2.1 ���tab1����ҳ��
	lv_obj_t *tab1_page = lv_tabview_add_tab(tabview1, LV_SYMBOL_EDIT " Other");
	lv_obj_set_event_cb(tab1_page, event_handler); // �����¼��ص�����
	// �ı�
	lv_obj_t *title_label = lv_label_create(tab1_page, NULL);
	lv_label_set_recolor(title_label, true);									   // ʹ���ı��ػ�ɫ����
	lv_label_set_text(title_label, "#ff0000 Second# #0000ff hand# #00ff00 book#"); // ����"Login"����
	lv_obj_align(title_label, NULL, LV_ALIGN_IN_TOP_MID, 0, 30);				   // ���ö��뷽ʽ

	// �鼮����ѡ��
	// 1.2 ����ѡ���ѡ��ʱ����ʽ
	lv_style_copy(&sel_style, &lv_style_plain);
	sel_style.body.main_color = LV_COLOR_MAKE(0x5F, 0xB8, 0x78); // ǳ��ɫ����
	sel_style.body.grad_color = sel_style.body.main_color;
	sel_style.text.color = LV_COLOR_WHITE; // �ı�Ϊ��ɫ

	// �ı�
	lv_obj_t *seach_label = lv_label_create(tab1_page, NULL);
	lv_label_set_text(seach_label, "Seach"); // ����"Login"����
	lv_obj_align(seach_label, NULL, LV_ALIGN_IN_TOP_LEFT, 36, 170);	

	// ���������
	ta1 = lv_ta_create(tab1_page, NULL);
	lv_obj_set_size(ta1, 300, 30);							// ���ô�С
	lv_obj_align(ta1, NULL, LV_ALIGN_IN_TOP_MID, -45, 200); // ���ö��뷽ʽ
	lv_ta_set_one_line(ta1, true);							// ʹ�ܵ���ģʽ,�ڵ���ģʽ��,�ı���ĸ߶��ǲ��ܱ����õ�
	lv_ta_set_text(ta1, "");								// ����Ϊ���ı�
	lv_ta_set_placeholder_text(ta1, "Input bookid");		// ����placeholder��ʾ�ı�
	lv_obj_set_event_cb(ta1, event_handler);				// �����¼��ص�����

	// ȷ�ϰ�ť
	btn1 = lv_btn_create(tab1_page, NULL);
	// lv_obj_set_pos(btn1, 50, 300);
	lv_obj_set_size(btn1, 70, 40);							 // ���ô�С
	lv_obj_align(btn1, NULL, LV_ALIGN_IN_TOP_MID, 150, 200); // ���ö��뷽ʽ
	lv_obj_t *btn_label = lv_label_create(btn1, NULL);
	lv_label_set_text(btn_label, "ok");		  // ���ð�ť�ϵı���
	lv_obj_set_event_cb(btn1, event_handler); // �����¼��ص�����

	/* --------------- table -----------*/
	// �ı�
	lv_obj_t *book_label = lv_label_create(tab1_page, NULL);
	lv_label_set_text(book_label, "Book List"); // ����"Login"����
	lv_obj_align(book_label, NULL, LV_ALIGN_IN_TOP_LEFT, 33, 420);	

	// �������
	lv_obj_t *table = lv_table_create(tab1_page, NULL);
	lv_table_set_col_cnt(table, 2);							// ��������Ϊ2
	lv_table_set_row_cnt(table, 5);							// ��������Ϊ3
	lv_obj_set_size(table, 400, 350);						// ���ñ���С
	lv_obj_align(table, NULL, LV_ALIGN_IN_TOP_MID, 0, 450); // ���ö��뷽ʽ

	lv_table_set_col_width(table, 0, 200);
	lv_table_set_col_width(table, 1, 200);
	lv_table_set_col_width(table, 2, 200);
	lv_table_set_col_width(table, 3, 200);
	lv_table_set_col_width(table, 4, 200);

	// ���ñ�ͷ
	lv_table_set_cell_value(table, 0, 0, "BookName");
	lv_table_set_cell_value(table, 1, 0, "LaoRenYuHai");
	lv_table_set_cell_value(table, 2, 0, "Life & Live");
	lv_table_set_cell_value(table, 3, 0, "You And me");
	lv_table_set_cell_value(table, 4, 0, "My Love");

	lv_table_set_cell_value(table, 0, 1, "BookId");
	lv_table_set_cell_value(table, 1, 1, "2133561313");
	lv_table_set_cell_value(table, 2, 1, "3428558943");
	lv_table_set_cell_value(table, 3, 1, "1203456832");
	lv_table_set_cell_value(table, 4, 1, "6283459645");

	// Ӧ����ʽ�����е�Ԫ��
	for (int row = 0; row < 5; row++)
	{
		for (int col = 0; col < 2; col++)
		{

			lv_table_set_cell_align(table, row, col, LV_LABEL_ALIGN_CENTER);
		}
	}

	// ���õ�һ�е���ɫΪǳ��ɫ
	// lv_style_t row_style;
	// lv_style_copy(&row_style, &lv_style_plain); // ����Ĭ����ʽ
	// row_style.body.main_color = LV_COLOR_MAKE(0, 0, 255); // ������Ҫ��ɫΪǳ��ɫ
	// row_style.body.grad_color = LV_COLOR_MAKE(0, 0, 255); // ���ý�����ɫΪǳ��ɫ

	// for (int col = 0; col < 3; col++) {
	// 	lv_table_set_style(table, LV_TABLE_STYLE_CELL1 + col, &row_style); // ���õ�һ�е���ʽ
	// }

	/*Set a smaller height to the table. It'll make it scrollable*/
	// lv_obj_set_height(table, 200);
	// lv_example_table_1();
}

// �����ص�����
void event_handler(lv_obj_t *obj, lv_event_t event)
{
	uint16_t btn_id = 10;
	if (obj == ta1) // �����ı���
	{
		if (event == LV_EVENT_RELEASED) // �����¼�
		{
			if (kb1 == NULL)
				kb_create(lv_scr_act()); // ������̲����ڵĻ�,���ȴ�������
			else
				lv_kb_set_ta(kb1, obj); // ���°��ı������
		}
	}
	else if (obj == btn1) // ȷ�ϰ�ť
	{
		if (event == LV_EVENT_RELEASED)
		{
			commit_mbox_create(lv_obj_get_parent(obj));
			bookid = (char *)lv_ta_get_text(ta1); // ��ȡ�����Id
		}
	}
	else if (obj == commit_mbox) // �ύ�Ի���
	{
		if (event == LV_EVENT_VALUE_CHANGED)
		{
			lv_obj_del(commit_mbox); // �رնԻ���
			commit_mbox = NULL;
		}
	}
}

// ���̴���
void keb_handler(lv_obj_t *obj, lv_event_t event)
{
	if (obj == kb1) // ���̴���
	{
		const char *key_txt = lv_btnm_get_active_btn_text(kb1); // ��ȡ���¼����ı�����,�ŵ�lv_kb_def_event_cb��ǰ�����
		lv_kb_def_event_cb(kb1, event);							// ���ü��̵�Ĭ���¼��ص�����,�����Ǵ����һЩͨ�õĲ���,����������Լ�д����������,�Ǿ�̫�鷳��

		if (event == LV_EVENT_APPLY)
		{
			lv_obj_del(kb1); // ����̼�ʱ�Ѽ���ɾ����
			kb1 = NULL;
		}
		else if (event == LV_EVENT_CANCEL)
		{
			lv_obj_del(kb1); // �������ʱ�Ѽ���ɾ����
			kb1 = NULL;
		}
	}
}

// ��Ϣ����
void msg_handler(lv_obj_t *obj, lv_event_t event)
{
	if (obj == sell_mbox)
	{
		if (event == LV_EVENT_VALUE_CHANGED)
		{
			lv_obj_del(sell_mbox); // �رնԻ���
			sell_mbox = NULL;
		}
	}
	else if (obj == buy_mbox)
	{
		if (event == LV_EVENT_VALUE_CHANGED)
		{
			lv_obj_del(buy_mbox); // �رնԻ���
			buy_mbox = NULL;
		}
	}
	else if (obj == preview_mbox)
	{
		if (event == LV_EVENT_VALUE_CHANGED)
		{
			lv_obj_del(preview_mbox); // �رնԻ���
			preview_mbox = NULL;
		}
	}
}
// ����
void help_handler(lv_obj_t *obj, lv_event_t event)
{
	if (obj == btn2)
	{
		BaseType_t xHigherPriorityTaskWoken;
		if (event == LV_EVENT_RELEASED)
		{
			musicIndex = 4;
			xSemaphoreGiveFromISR(BinarySemaphore, &xHigherPriorityTaskWoken); // �ͷŶ�ֵ�ź�����������ʾ
			quit_mbox_create(lv_obj_get_parent(obj));
		}
	}
	else if (obj == quit_mbox)
	{
		if (event == LV_EVENT_VALUE_CHANGED)
		{
			lv_obj_del(quit_mbox); // �رնԻ���
			quit_mbox = NULL;
		}
	}
}

void error_handler(lv_obj_t *obj, lv_event_t event)
{
	u8 btn_id;
	if (obj == error_btn)
	{
		if (event == LV_EVENT_RELEASED)
		{
			error_mbox_create(lv_obj_get_parent(obj));
		}
	}
	else if (obj == error_mbox)
	{
		if (event == LV_EVENT_VALUE_CHANGED)
		{
			// ��ȡ��ť id
			// Ҳ����ʹ�� btn_id = *((uint16_t*)lv_event_get_data())�ķ�ʽ
			btn_id = lv_mbox_get_active_btn(obj);
			if (btn_id == 0) // Apply ��ť
			{
				preview_flag = 1;
				lv_obj_del(error_mbox); // �رնԻ���
				error_mbox = NULL;
			}
			else if (btn_id == 1) // Close ��ť
			{
				preview_flag = 0;
				lv_obj_del(error_mbox); // �رնԻ���
				error_mbox = NULL;
			}
		}
	}
}

void draw_part_event_cb(lv_obj_t *obj, lv_event_t event)
{
	;
}

void lv_example_table_1()
{
	// ����һ��2��3�еı��
	lv_obj_t *table = lv_table_create(tab1_page, NULL);
	lv_table_set_col_cnt(table, 2);							   // ��������Ϊ2
	lv_table_set_row_cnt(table, 2);							   // ��������Ϊ3
															   // lv_obj_set_size(table, 300, 350); // ���ñ���С
	lv_obj_align(table, btn1, LV_ALIGN_OUT_BOTTOM_MID, 0, 20); // ���ö��뷽ʽ

	// // lv_table_set_col_width(table,0,100);

	// ���ñ�ͷ
	lv_table_set_cell_value(table, 0, 0, "Column 1");
	lv_table_set_cell_value(table, 0, 1, "Column 2");

	// ���������
	// lv_table_set_cell_value(table, 1, 0, "Row 1, Col 1");
	// lv_table_set_cell_value(table, 1, 1, "Row 1, Col 2");
	// lv_table_set_cell_value(table, 2, 0, "Row 2, Col 1");
	// lv_table_set_cell_value(table, 2, 1, "Row 2, Col 2");

	// /*Set a smaller height to the table. It'll make it scrollable*/
	// lv_obj_set_height(table, 200);

	// /*Add an event callback to to apply some custom drawing*/
	// lv_obj_set_event_cb(table, draw_part_event_cb); // �����¼��ص�����
}
