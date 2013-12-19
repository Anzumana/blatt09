#ifndef GUI_H
#define GUI_H

void guiConnect(void);
void guiDisconnect(void);
void guiSend(char *input);
char* guiReceive();

void guiSetInput(const char *input, int row, int column);
void guiSetOutput(const char *output, int row, int column);

typedef struct GuiInput {
	int row;
	int column;
	char *value;
} GuiInput;

GuiInput guiGetInput();

#endif
