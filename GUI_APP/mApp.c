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

extern SemaphoreHandle_t BinarySemaphore; // 二值信号量句柄
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
lv_obj_t *ddlist1; // 下拉列表

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
		lv_kb_set_cursor_manage(kb1, true);	 // 使能对光标的接管
		lv_kb_set_mode(kb1, LV_KB_MODE_NUM); // 设置为文本键盘模式,这也是默认值
		lv_kb_set_ta(kb1, ta1);				 // 先默认绑定ta1文本域对象,后面可以通过点击某文本域来重新绑定到相应的文本域对象

		lv_obj_set_event_cb(kb1, keb_handler); // 设置自定义的事件回调函数
	}
}

// 创建退出确认的消息对话框
void quit_mbox_create(lv_obj_t *parent)
{
	if (quit_mbox) // 不要重复创建
		return;
	quit_mbox = lv_mbox_create(parent, NULL);
	lv_obj_set_size(quit_mbox, lv_obj_get_width(parent) * 0.7f, lv_obj_get_height(parent) / 2);
	lv_mbox_set_text(quit_mbox, "HELP\nHELLO WORLD!");
	lv_mbox_add_btns(quit_mbox, (const char **)MBOX_MAP);
	lv_obj_set_drag(quit_mbox, true); // 设置对话框可以被拖拽
	lv_obj_align(quit_mbox, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_event_cb(quit_mbox, help_handler);
}

// 创建确认提示对话框
void commit_mbox_create(lv_obj_t *parent)
{
	commit_mbox = lv_mbox_create(parent, NULL);
	lv_obj_set_size(commit_mbox, lv_obj_get_width(parent) * 0.7f, lv_obj_get_height(parent) / 2);
	lv_mbox_set_text(commit_mbox, "TIP\n Commit OK!");
	lv_mbox_add_btns(commit_mbox, (const char **)MBOX_MAP);
	lv_obj_set_drag(commit_mbox, true); // 设置对话框可以被拖拽
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
	lv_obj_set_drag(error_mbox, true); // 设置对话框可以被拖拽
	lv_obj_align(error_mbox, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_event_cb(error_mbox, error_handler);
}

void app_init()
{
	lv_obj_t *scr = lv_scr_act(); // 获取当前活跃的屏幕对象

	// 1.创建样式
	// 1.1 创建背景样式
	lv_style_copy(&bg_style, &lv_style_plain);
	bg_style.body.main_color = LV_COLOR_MAKE(49, 49, 49); // 纯色背景
	bg_style.body.grad_color = bg_style.body.main_color;
	bg_style.body.border.color = LV_COLOR_MAKE(150, 150, 150); // 边框颜色
	bg_style.body.border.width = 2;							   // 边框宽度
	bg_style.text.color = LV_COLOR_WHITE;
	// 1.2 创建页面指示器的样式
	lv_style_copy(&indic_style, &lv_style_plain_color);
	indic_style.body.main_color = LV_COLOR_MAKE(42, 212, 66); // 指示器的颜色,绿色
	indic_style.body.grad_color = indic_style.body.main_color;
	indic_style.body.padding.inner = 3; // 设置指示器的高度
	// 1.3 创建页面选择按钮栏的背景样式
	// lv_style_transp_tight样式中的inner,left,top,right,bottom等内间距值都为0,这是为了让页面选择按钮能够紧挨在一起
	lv_style_copy(&btn_bg_style, &lv_style_transp_tight);
	// 1.4 创建按钮正常态下的松手样式
	lv_style_copy(&btn_rel_style, &lv_style_plain_color);
	btn_rel_style.body.main_color = LV_COLOR_MAKE(98, 98, 98);
	btn_rel_style.body.grad_color = btn_rel_style.body.main_color;
	btn_rel_style.body.border.color = LV_COLOR_MAKE(150, 150, 150); // 边框颜色
	btn_rel_style.body.border.width = 1;
	btn_rel_style.text.color = LV_COLOR_WHITE; // 字体颜色
	// 1.5 创建按钮正常态下的按下样式
	lv_style_copy(&btn_pr_style, &btn_rel_style);
	btn_pr_style.body.main_color = LV_COLOR_GRAY;
	btn_pr_style.body.grad_color = btn_pr_style.body.main_color;
	// 1.6 创建按钮切换态下的松手样式
	lv_style_copy(&btn_tgl_rel_style, &btn_rel_style);
	btn_tgl_rel_style.body.main_color = bg_style.body.main_color; // 和主背景颜色一致
	btn_tgl_rel_style.body.grad_color = btn_tgl_rel_style.body.main_color;
	// 1.7 创建按钮切换态下的按下样式
	// 保持和btn_tgl_rel_style一样就行了
	lv_style_copy(&btn_tgl_pr_style, &btn_tgl_rel_style);

	// 2.创建选项卡
	tabview1 = lv_tabview_create(scr, NULL);
	lv_obj_set_size(tabview1, lv_obj_get_width(scr), lv_obj_get_height(scr)); // 设置选项卡的大小,比屏幕小16像素
	lv_obj_align(tabview1, NULL, LV_ALIGN_CENTER, 0, 0);					  // 与屏幕居中对齐
	lv_obj_set_event_cb(tabview1, event_handler);							  // 设置事件回调函数
	lv_tabview_set_btns_pos(tabview1, LV_TABVIEW_BTNS_POS_TOP);				  // 设置页面选择按钮栏位于顶部

	lv_tabview_set_style(tabview1, LV_TABVIEW_STYLE_BG, &bg_style);					  // 设置背景样式
	lv_tabview_set_style(tabview1, LV_TABVIEW_STYLE_INDIC, &indic_style);			  // 设置页面指示器的样式
	lv_tabview_set_style(tabview1, LV_TABVIEW_STYLE_BTN_BG, &btn_bg_style);			  // 设置页面选择按钮栏的背景样式
	lv_tabview_set_style(tabview1, LV_TABVIEW_STYLE_BTN_REL, &btn_rel_style);		  // 设置按钮正常态下的松手样式
	lv_tabview_set_style(tabview1, LV_TABVIEW_STYLE_BTN_PR, &btn_pr_style);			  // 设置按钮正常态下的按下样式
	lv_tabview_set_style(tabview1, LV_TABVIEW_STYLE_BTN_TGL_REL, &btn_tgl_rel_style); // 设置按钮切换态下的松手样式
	lv_tabview_set_style(tabview1, LV_TABVIEW_STYLE_BTN_TGL_PR, &btn_tgl_pr_style);	  // 设置按钮切换态下的按下样式

	lv_obj_set_click(scr, true);
	lv_obj_set_event_cb(tabview1, event_handler);
}

// 例程入口
void mApp_start()
{
	app_init();

	creat_tab2();
	creat_tab1();
}

void creat_tab2()
{
	// 2.2 添加tab2内容页面
	tab2_page = lv_tabview_add_tab(tabview1, LV_SYMBOL_HOME " Home");

	// 文本
	lv_obj_t *title_label = lv_label_create(tab2_page, NULL);
	lv_label_set_recolor(title_label, true);									   // 使能文本重绘色功能
	lv_label_set_text(title_label, "#ff0000 Second# #0000ff hand# #00ff00 book#"); // 设置"Login"标题
	lv_obj_align(title_label, NULL, LV_ALIGN_IN_TOP_MID, 0, 30);				   // 设置对齐方式

	LV_IMG_DECLARE(qrcode);
	lv_obj_t *img2 = lv_img_create(tab2_page, NULL);
	lv_img_set_src(img2, &qrcode);
	lv_obj_align(img2, NULL, LV_ALIGN_IN_TOP_MID, 0, 150); // 设置对齐方式

	// 往内容页面2中添加标签子对象
	btn2 = lv_btn_create(tab2_page, NULL);
	lv_obj_set_pos(btn2, 50, 300);
	lv_obj_set_size(btn2, 200, 40);						   // 设置大小
	lv_obj_align(btn2, NULL, LV_ALIGN_IN_TOP_MID, 0, 500); // 设置对齐方式
	lv_obj_t *btn_labe2 = lv_label_create(btn2, NULL);
	lv_label_set_text(btn_labe2, "help");	 // 设置按钮上的标题
	lv_obj_set_event_cb(btn2, help_handler); // 设置事件回调函数

	error_btn = lv_btn_create(tab2_page, NULL);
	lv_obj_set_hidden(error_btn, true);
	lv_obj_align(error_btn, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0); // 设置对齐方式
	lv_obj_set_event_cb(error_btn, error_handler);				 // 设置事件回调函数
}

void creat_tab1()
{
	// 2.1 添加tab1内容页面
	lv_obj_t *tab1_page = lv_tabview_add_tab(tabview1, LV_SYMBOL_EDIT " Other");
	lv_obj_set_event_cb(tab1_page, event_handler); // 设置事件回调函数
	// 文本
	lv_obj_t *title_label = lv_label_create(tab1_page, NULL);
	lv_label_set_recolor(title_label, true);									   // 使能文本重绘色功能
	lv_label_set_text(title_label, "#ff0000 Second# #0000ff hand# #00ff00 book#"); // 设置"Login"标题
	lv_obj_align(title_label, NULL, LV_ALIGN_IN_TOP_MID, 0, 30);				   // 设置对齐方式

	// 书籍功能选择
	// 1.2 创建选择项被选中时的样式
	lv_style_copy(&sel_style, &lv_style_plain);
	sel_style.body.main_color = LV_COLOR_MAKE(0x5F, 0xB8, 0x78); // 浅绿色背景
	sel_style.body.grad_color = sel_style.body.main_color;
	sel_style.text.color = LV_COLOR_WHITE; // 文本为白色

	// 文本
	lv_obj_t *seach_label = lv_label_create(tab1_page, NULL);
	lv_label_set_text(seach_label, "Seach"); // 设置"Login"标题
	lv_obj_align(seach_label, NULL, LV_ALIGN_IN_TOP_LEFT, 36, 170);	

	// 创建输入框
	ta1 = lv_ta_create(tab1_page, NULL);
	lv_obj_set_size(ta1, 300, 30);							// 设置大小
	lv_obj_align(ta1, NULL, LV_ALIGN_IN_TOP_MID, -45, 200); // 设置对齐方式
	lv_ta_set_one_line(ta1, true);							// 使能单行模式,在单行模式下,文本域的高度是不能被设置的
	lv_ta_set_text(ta1, "");								// 设置为空文本
	lv_ta_set_placeholder_text(ta1, "Input bookid");		// 设置placeholder提示文本
	lv_obj_set_event_cb(ta1, event_handler);				// 设置事件回调函数

	// 确认按钮
	btn1 = lv_btn_create(tab1_page, NULL);
	// lv_obj_set_pos(btn1, 50, 300);
	lv_obj_set_size(btn1, 70, 40);							 // 设置大小
	lv_obj_align(btn1, NULL, LV_ALIGN_IN_TOP_MID, 150, 200); // 设置对齐方式
	lv_obj_t *btn_label = lv_label_create(btn1, NULL);
	lv_label_set_text(btn_label, "ok");		  // 设置按钮上的标题
	lv_obj_set_event_cb(btn1, event_handler); // 设置事件回调函数

	/* --------------- table -----------*/
	// 文本
	lv_obj_t *book_label = lv_label_create(tab1_page, NULL);
	lv_label_set_text(book_label, "Book List"); // 设置"Login"标题
	lv_obj_align(book_label, NULL, LV_ALIGN_IN_TOP_LEFT, 33, 420);	

	// 创建表格
	lv_obj_t *table = lv_table_create(tab1_page, NULL);
	lv_table_set_col_cnt(table, 2);							// 设置列数为2
	lv_table_set_row_cnt(table, 5);							// 设置行数为3
	lv_obj_set_size(table, 400, 350);						// 设置表格大小
	lv_obj_align(table, NULL, LV_ALIGN_IN_TOP_MID, 0, 450); // 设置对齐方式

	lv_table_set_col_width(table, 0, 200);
	lv_table_set_col_width(table, 1, 200);
	lv_table_set_col_width(table, 2, 200);
	lv_table_set_col_width(table, 3, 200);
	lv_table_set_col_width(table, 4, 200);

	// 设置表头
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

	// 应用样式到所有单元格
	for (int row = 0; row < 5; row++)
	{
		for (int col = 0; col < 2; col++)
		{

			lv_table_set_cell_align(table, row, col, LV_LABEL_ALIGN_CENTER);
		}
	}

	// 设置第一行的颜色为浅蓝色
	// lv_style_t row_style;
	// lv_style_copy(&row_style, &lv_style_plain); // 复制默认样式
	// row_style.body.main_color = LV_COLOR_MAKE(0, 0, 255); // 设置主要颜色为浅蓝色
	// row_style.body.grad_color = LV_COLOR_MAKE(0, 0, 255); // 设置渐变颜色为浅蓝色

	// for (int col = 0; col < 3; col++) {
	// 	lv_table_set_style(table, LV_TABLE_STYLE_CELL1 + col, &row_style); // 设置第一行的样式
	// }

	/*Set a smaller height to the table. It'll make it scrollable*/
	// lv_obj_set_height(table, 200);
	// lv_example_table_1();
}

// 按键回调函数
void event_handler(lv_obj_t *obj, lv_event_t event)
{
	uint16_t btn_id = 10;
	if (obj == ta1) // 输入文本框
	{
		if (event == LV_EVENT_RELEASED) // 松手事件
		{
			if (kb1 == NULL)
				kb_create(lv_scr_act()); // 如果键盘不存在的话,则先创建键盘
			else
				lv_kb_set_ta(kb1, obj); // 重新绑定文本域对象
		}
	}
	else if (obj == btn1) // 确认按钮
	{
		if (event == LV_EVENT_RELEASED)
		{
			commit_mbox_create(lv_obj_get_parent(obj));
			bookid = (char *)lv_ta_get_text(ta1); // 获取输入的Id
		}
	}
	else if (obj == commit_mbox) // 提交对话框
	{
		if (event == LV_EVENT_VALUE_CHANGED)
		{
			lv_obj_del(commit_mbox); // 关闭对话框
			commit_mbox = NULL;
		}
	}
}

// 键盘处理
void keb_handler(lv_obj_t *obj, lv_event_t event)
{
	if (obj == kb1) // 键盘处理
	{
		const char *key_txt = lv_btnm_get_active_btn_text(kb1); // 获取按下键的文本标题,放到lv_kb_def_event_cb的前面调用
		lv_kb_def_event_cb(kb1, event);							// 调用键盘的默认事件回调函数,帮我们处理掉一些通用的操作,如果让我们自己写代码来处理,那就太麻烦了

		if (event == LV_EVENT_APPLY)
		{
			lv_obj_del(kb1); // 点击√键时把键盘删除掉
			kb1 = NULL;
		}
		else if (event == LV_EVENT_CANCEL)
		{
			lv_obj_del(kb1); // 点击×键时把键盘删除掉
			kb1 = NULL;
		}
	}
}

// 消息框处理
void msg_handler(lv_obj_t *obj, lv_event_t event)
{
	if (obj == sell_mbox)
	{
		if (event == LV_EVENT_VALUE_CHANGED)
		{
			lv_obj_del(sell_mbox); // 关闭对话框
			sell_mbox = NULL;
		}
	}
	else if (obj == buy_mbox)
	{
		if (event == LV_EVENT_VALUE_CHANGED)
		{
			lv_obj_del(buy_mbox); // 关闭对话框
			buy_mbox = NULL;
		}
	}
	else if (obj == preview_mbox)
	{
		if (event == LV_EVENT_VALUE_CHANGED)
		{
			lv_obj_del(preview_mbox); // 关闭对话框
			preview_mbox = NULL;
		}
	}
}
// 帮助
void help_handler(lv_obj_t *obj, lv_event_t event)
{
	if (obj == btn2)
	{
		BaseType_t xHigherPriorityTaskWoken;
		if (event == LV_EVENT_RELEASED)
		{
			musicIndex = 4;
			xSemaphoreGiveFromISR(BinarySemaphore, &xHigherPriorityTaskWoken); // 释放二值信号量，发出提示
			quit_mbox_create(lv_obj_get_parent(obj));
		}
	}
	else if (obj == quit_mbox)
	{
		if (event == LV_EVENT_VALUE_CHANGED)
		{
			lv_obj_del(quit_mbox); // 关闭对话框
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
			// 获取按钮 id
			// 也可以使用 btn_id = *((uint16_t*)lv_event_get_data())的方式
			btn_id = lv_mbox_get_active_btn(obj);
			if (btn_id == 0) // Apply 按钮
			{
				preview_flag = 1;
				lv_obj_del(error_mbox); // 关闭对话框
				error_mbox = NULL;
			}
			else if (btn_id == 1) // Close 按钮
			{
				preview_flag = 0;
				lv_obj_del(error_mbox); // 关闭对话框
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
	// 创建一个2列3行的表格
	lv_obj_t *table = lv_table_create(tab1_page, NULL);
	lv_table_set_col_cnt(table, 2);							   // 设置列数为2
	lv_table_set_row_cnt(table, 2);							   // 设置行数为3
															   // lv_obj_set_size(table, 300, 350); // 设置表格大小
	lv_obj_align(table, btn1, LV_ALIGN_OUT_BOTTOM_MID, 0, 20); // 设置对齐方式

	// // lv_table_set_col_width(table,0,100);

	// 设置表头
	lv_table_set_cell_value(table, 0, 0, "Column 1");
	lv_table_set_cell_value(table, 0, 1, "Column 2");

	// 填充表格数据
	// lv_table_set_cell_value(table, 1, 0, "Row 1, Col 1");
	// lv_table_set_cell_value(table, 1, 1, "Row 1, Col 2");
	// lv_table_set_cell_value(table, 2, 0, "Row 2, Col 1");
	// lv_table_set_cell_value(table, 2, 1, "Row 2, Col 2");

	// /*Set a smaller height to the table. It'll make it scrollable*/
	// lv_obj_set_height(table, 200);

	// /*Add an event callback to to apply some custom drawing*/
	// lv_obj_set_event_cb(table, draw_part_event_cb); // 设置事件回调函数
}
