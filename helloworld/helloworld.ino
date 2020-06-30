/*
 * Name: max7219Display
 * Creator: Marco Arduino channel
 * Date:    35,175 views•Jan 8, 2017
 *
 * SourceCode: pastebin.com/raw/V1Qja7Dw
 *
 * YouTube: youtube.com/watch?v=dzuMXUQwKx8
 * Title: Nodemcu ( esp8266 ) and max7219 led matrix display
 *f
 */
int msglen = 0;

#include <MAX7219_Dot_Matrix.h>
#include <string.h>

#define MAX(a, b) (( a < b ) ? a : b ) 

/* Constants -- Global to this file */
const unsigned long MOVE_INTERVAL = 25;  /* mS */
const byte chips = 4;

/* Global -- needs to be used in both setup and loop */
MAX7219_Dot_Matrix display(chips, 2);

/*
 * updateDisplay: updates the max7219 display with message at new position
 * 
 * @messx:         Message to display.
 * @messageOffset: Offset to display the message at
 * Returns the new offset for the next message to be displayed.
 */ 
int updateDisplay (char *messx, int messageOffset)
{
    display.sendSmooth(messx, messageOffset);
    // next time show one pixel onwards
    if (messageOffset++ >= (int) (strlen (messx) * 8))
        messageOffset = - chips * 8;
    return messageOffset;
}

void setup()
{
    display.begin();
}

void loop() {
    static char message[90] = "Hello World";
    static int messageOffset = 0;
    static unsigned long lastMoved = 0;

    delay(1);
    /* update display if time is up */
    if (millis() - lastMoved < MOVE_INTERNAL)
        return;
    messageOffset = updateDisplay(message, messageOffset);
    lastMoved = millis();
}