#include "aboutGUI.hpp"
//#include "graphicsProvider.hpp"
#include "versionProvider.hpp"

void showAbout() {
 int key;
 LCD_Clear(0);
 //y increment between each line: 17; between paragraphs: 20
 LCD_String_5X7(0, 0, (unsigned char*)"Designed by Zweb",1);
 LCD_Update();
 Delay(100);
 LCD_String_5X7(0, 8, (unsigned char*)"Press any key",1);
 LCD_Update();
 GetKey();
}