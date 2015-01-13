//
// breakout.c
//
// Computer Science 50
// Problem Set 4
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include "gevents.h"
#include "gobjects.h"
#include "gwindow.h"
#include "ginteractors.h"
// height and width of game's window in pixels
#define HEIGHT 570
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10


// height and width of paddle
#define PDLH 10
#define PDLW 100

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);
// my own button saying my name
void initButton(GWindow window);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // instantiate my button
    initButton(window);
    
    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;

    // keep playing until game over
    double velocityX = drand48() * 2.0;
    double velocityY = 2.0;
    /*GEvent eventStart = getNextEvent(MOUSE_EVENT);
    if (eventStart != NULL) {
    if (getEventType(eventStart) == MOUSE_CLICKED) {
    */
    waitForClick();
    while (lives > 0 && bricks > 0)
    {
        // Move the paddle along horizontal axis
        
        // make an event
        GEvent event = getNextEvent(MOUSE_EVENT);
        if (event != NULL) {
            if (getEventType(event) == MOUSE_MOVED) {
                double xPaddle;
                xPaddle = getX(event) - getWidth(paddle)/2;
                setLocation(paddle, xPaddle, HEIGHT - PDLW);
            }
        }
        move(ball, velocityX, velocityY);
        if (getX(ball) + getWidth(ball) > getWidth(window)) {
            velocityX = -velocityX;
        }
        else if (getX(ball) < 0) {
            velocityX = -velocityX;
        }
        else if (getY(ball) + getHeight(ball) > getHeight(window)) {
            // velocityY = -velocityY;
            lives--;
            setLocation(ball, WIDTH/2, HEIGHT/2);
            setLocation(paddle, (WIDTH - getWidth(paddle))/2,HEIGHT - PDLW );
            if (lives == 0 || bricks == 0) {
                setLabel(label, "LOSER");
            }
            waitForClick();
        }
        else if (getY(ball) <0) {
            velocityY = -velocityY;
        }
        pause(10);
        
        GObject object = detectCollision(window, ball);        
        if (object != NULL) {
            if (strcmp(getType(object), "GRect") == 0)
            {
                velocityY = -velocityY;
                if (getY(object) < getY(paddle)) {
                    removeGWindow(window, object);
                    bricks--;
                    static int i = 1;                    
                    // to store 50 through 0 (with '\0'), we need <= 3 chars
                    char s[3];
                    // convert i from int to string
                    sprintf(s, "%i", i);
                    // update label
                    setLabel(label, s);
                    i++;
                    if (bricks == 0) {
                        setLabel(label, "Won");
                    }
                }
            }
        }
    }
    /*} // end if MOUSE_CLICKED
    } // end if eventStart*/
    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{

    char* color[] = {"BLUE", "RED", "GREEN", "PINK", "YELLOW"};
    for (int j = 0; j < ROWS; j++) {
        for (int i = 0; i < COLS; i++) {
            GRect brick2 = newGRect(((40 * i) + ((WIDTH/COLS)-35)), (10+PDLH)* j + 10, (WIDTH/COLS)-10, PDLH);
            setColor(brick2, color[j]);
            setFilled(brick2, true);
            add(window,brick2);
        }
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    // Make a black ball
    GOval ball = newGOval(WIDTH/2, HEIGHT/2, 15, 15);
    setColor(ball, "BLACK");
    setFilled(ball, true);
    add(window, ball);
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    // init paddle
    GRect paddle = newGRect((WIDTH-PDLW)/2, HEIGHT - PDLW, PDLW, PDLH);
    setColor(paddle, "0x000000");
    setFilled(paddle, true);
    add(window,paddle);
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    GLabel label = newGLabel("0");
    setFont(label, "SansSerif-36");
    add(window, label);
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
    return label;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
void initButton(GWindow window) {
    // instantiate button
    GButton button = newGButton("Developed: Daro");
    //setActionCommand(button, "click");

    // add button to southern region of window
    addToRegion(window, button, "SOUTH");
}
