/*******************************************************************************
  ArithMax Open Source Calculator Project
  www.zephray.com
*******************************************************************************/
#ifndef __UI_H__
#define __UI_H__

typedef struct pUI_MENU_Item {
  char ShortName[4];
  void (*OnClick)(void);
  char FullName[13];
}UI_MENU_Item;

typedef struct pUI_MENU {
  unsigned char Count;
  UI_MENU_Item Items[];
}UI_MENU;


void UI_DrawBottomMenu(UI_MENU *menu);

#endif