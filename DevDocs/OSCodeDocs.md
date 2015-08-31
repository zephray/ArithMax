# ArithMax Development Documents

Here is a simple description of ArithMax OS's code.

Current version: 1.1-preview1 

## Hardware Drivers

### LCD Driver

#### Overview:

LCD Driver in the ArithMax OS 1.1 includes driver functions based on bare hardware and a graphic library mainly based on draw pixel function.

#### Using the graphic library:

All the operations are based on buffer (something like surface in SDL). Creating or freeing the buffer need the support of memory management, in the ArithMax OS, it’s done by FVM module. We suppose that you are working with an ArithMax EV3/P with ArithMax OS 1.1.

The basic routine is creating a buffer, draw to that buffer, blit to screen, free your buffer. FVM manage memory in unit of “block”. Basically, you may want to create a new block for your program and free that block when exit or no longer needed. Here are simple example of drawing a pixel.

Example: Drawing a pixel with LCD driver and FVM

	void * *MyMemBlk;
	uint8_t *MyBuffer;
	
	MyMemBlk = FVM_Init();		//Allocate a new block
	FVM_SelectBlk(MyMemBlk);	//Select as current using block
	MyBuffer = LCD_CreateBuffer(30,40);	//Creating a 30*40 buffer
	LCD_SelectBuffer(MyBuffer);	//Select as current using buffer
	LCD_ClearBuffer();			//Set all pixels in buffer to 0
	LCD_SetPixel(10,10,1);		//Set pixel (10,10) to 1(black)
	LCD_BlitBuffer(LCD_Screen,10,10); //Blit to position 10,10 of screen
	LCD_Refresh();				//Remember to trigger an instant refresh
	FVM_FreeBlk(MyMemBlk);		//Free block and everything in it

You should see a point at (20,20) after running these codes.

#### About the font

This graphic library uses dot matrix font and support proportional font. You may use a tool called “FontGen” to create your own font.
