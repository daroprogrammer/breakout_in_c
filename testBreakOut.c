#include <stdio.h>
#include <string.h>

#include "gobjects.h"
#include "gevents.h"
#include "ginteractors.h"
#include "gwindow.h"

int main(void) {
    // Instatiate a window
    GWindow window = newGWindow(320, 240);
    // Make a circle without fill
    GOval circle = newGOval(0,0,50,50);
    // Make a black ball
    GOval ball = newGOval(0, 0, 10, 10);
    setColor(ball, "BLACK");
    setFilled(ball, true);
    // Make a button
    GButton button = newGButton("Start");
    // set event to the button
    setActionCommand(button, "click");
    // put the button at the bottom
    addToRegion(window, button, "South");
    // add objects to the window
    add(window, ball);
    add(window, circle);
    // Set the ball's velocity
    double velocityX = 2.0;
    double velocityY = 2.0;
    // Listen for events
    while (true) {
        // events with mouse
        GEvent event = getNextEvent(MOUSE_EVENT);
        // event with button 
         // GActionEvent actionEvent = waitForEvent(ACTION_EVENT);
        
        // listen for events with mouse
        if (event != NULL) {
            if (getEventType(event) == MOUSE_CLICKED) {
                printf("%.0f, %.0f\n", getX(event), getY(event));
            }
            if (getEventType(event) == MOUSE_MOVED) {
                double xCircle = getX(event) - getWidth(circle)/2;
                double yCircle = getY(event) - getHeight(circle)/2;
                setLocation(circle, xCircle, yCircle);
                // printf("%.0f, %.0f\n", getX(event), getY(event));
            }
        }
        // move the ball by velocity
        move(ball, velocityX, velocityY);
        if (getX(ball) + getWidth(ball) > getWidth(window)) {
            velocityX = -velocityX;
        }
        else if (getX(ball) < 0) {
            velocityX = -velocityX;
        }
        else if (getY(ball) + getHeight(ball) > getHeight(window)) {
            velocityY = -velocityY;
        }
        else if (getY(ball) <0) {
            velocityY = -velocityY;
        }
        pause(10);

        // listen for button events
        /*if (actionEvent != NULL) {
            if (strcmp(getActionCommand(actionEvent), "click") == 0) {
                printf("Butotn clicked\n");
            }
        }*/
    }
    // closeGWindow(window);
    return 0;   
}
