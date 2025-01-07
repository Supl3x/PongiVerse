#include <raylib.h>
#include <stdio.h>
#include <unistd.h> // For sleep()

// Screen and game object constants
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;
const int PADDLE_WIDTH = 15;
const int PADDLE_HEIGHT = 150;
const int BALL_SIZE = 25;

// Paddle and ball speed
const int PADDLE_SPEED = 5;
const int BALL_SPEED = 600;
const float SCORE_DELAY = 2.0f; // 2-second delay after scoring


int main(void) 
    {
        typedef enum {
        MODE_TENNIS,
        MODE_TABLE_TENNIS,
        MODE_AIR_HOCKEY
    } GameMode;

    GameMode currentMode = MODE_TENNIS; // Default mode
    // Initialize the game window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong Game with Background");
    InitAudioDevice(); // Initialize audio system for sound effects
    Texture2D currentBackground;
    Texture2D currentPaddle;
    Texture2D currentBall;
    Sound currentCollisionSound;
    
    // Load PNG textures for HomeScreen and instructions
    Texture2D homeScreenTexture = LoadTexture("homescreen.png");
    // Load all the instruction images
    Texture2D instructionsTexture1 = LoadTexture("instructions1.png");
    Texture2D instructionsTexture2 = LoadTexture("instructions2.png");
    Texture2D instructionsTexture3 = LoadTexture("instructions3.png");
    Texture2D instructionsTexture4 = LoadTexture("instructions4.png");
    Texture2D instructionsTexture5 = LoadTexture("instructions5.png");

    // Store all textures in an array for easy access
    Texture2D instructionsTextures[5] = { instructionsTexture1, instructionsTexture2, instructionsTexture3, instructionsTexture4, instructionsTexture5 };

    // Initialize the current instruction image index
    int currentInstructionIndex = 0;
    
    // Load background music for home screen
    Music homeScreenMusic = LoadMusicStream("homescreen_music.ogg");
    PlayMusicStream(homeScreenMusic); // Play music when the home screen is displayed
    
    // Load PNG textures for Tennis paddles, ball, background, & sounds
    Texture2D tennisPaddle = LoadTexture("Tennispaddle.png");
    Texture2D tennisBall = LoadTexture("ballsmall.png");
    Texture2D tennisBackground = LoadTexture("background.png");
    Sound tennisCollisionSound = LoadSound("collision.ogg"); // Sound for paddle collision with ball    

    // Load textures and sounds for Table Tennis mode
    Texture2D tableTennisBackground = LoadTexture("table_tennis_background.png");
    Texture2D tableTennisPaddle = LoadTexture("table_tennis_paddle.png");
    Texture2D tableTennisBall = LoadTexture("table_tennis_ball.png");
    Sound tableTennisCollisionSound = LoadSound("table_tennis_collision.ogg");

    // Load textures and sounds for Air Hockey mode
    Texture2D airHockeyBackground = LoadTexture("air_hockey_background.png");
    Texture2D airHockeyPaddle = LoadTexture("air_hockey_paddle.png");
    Texture2D airHockeyBall = LoadTexture("air_hockey_ball.png");
    Sound airHockeyCollisionSound = LoadSound("air_hockey_collision.ogg");
    
    // Load sound effects for collisions and game end
    Sound gameEndSound = LoadSound("gameend.ogg"); // Sound when a player wins

    // Load custom font for score display
    Font scoreFont = LoadFont("cartoon.ttf"); // Load custom font file
    
    Sound goalSound = LoadSound("goal_sound.ogg"); //Sound when goal is scored
    
    // Initialize paddle and ball positions
    int paddle1X = 0; // Left paddle position X
    int paddle1Y = (SCREEN_HEIGHT - currentPaddle.height) / 2; // Center paddle vertically
    int paddle2X = SCREEN_WIDTH - currentPaddle.width-90; // Right paddle position X
    int paddle2Y = (SCREEN_HEIGHT - currentPaddle.height) / 2;

    // Ball position (centered) and initial velocity
    Vector2 ballPosition = {(SCREEN_WIDTH / 2) - (BALL_SIZE / 2), (SCREEN_HEIGHT / 2) - (BALL_SIZE / 2)};
    Vector2 ballVelocity = {BALL_SPEED, BALL_SPEED};

    int score1 = 0;  // Left player score
    int score2 = 0;  // Right player score

    SetTargetFPS(120); // Set frame rate

    // Variables for scoring timing
    float scoreTimer = 0.0f; // Delay after scoring
    bool ballActive = true; // Flag to determine if ball is in play
    bool gameEnded = false; // Flag to check if the game has ended
    bool instructionsScreen = false; 
    bool homeScreen = true;

    while (homeScreen && !WindowShouldClose()) 
    {
        UpdateMusicStream(homeScreenMusic); // Update music playback
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        DrawTexture(homeScreenTexture, 0, 0, WHITE);  // Display the home screen image
        
        EndDrawing();

        // Wait for user input to start the game
        if (IsKeyPressed(KEY_ONE)) {
            currentMode = MODE_TENNIS;
            
            homeScreen = false; // Exit the home screen and start the game
            instructionsScreen = true; // Move to the instruction screen
        } else if (IsKeyPressed(KEY_TWO)) {
            
          
            homeScreen = false; // Exit the home screen and start the game
            instructionsScreen = true; // Move to the instruction screen
            currentMode = MODE_TABLE_TENNIS;
        } else if (IsKeyPressed(KEY_THREE)) {
            currentMode = MODE_AIR_HOCKEY;
            
            homeScreen = false; // Exit the home screen and start the game
            instructionsScreen = true; // Move to the instruction screen
        }
    }

    // Display the instruction screen
    while (instructionsScreen && !WindowShouldClose()) 
    {
        UpdateMusicStream(homeScreenMusic); 
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw the current instruction image
        DrawTexture(instructionsTextures[currentInstructionIndex], 0, 0, WHITE); 

        EndDrawing();

        // Handle input to cycle through the images
        if (IsKeyPressed(KEY_RIGHT) && currentInstructionIndex < 4) { // Next image
            currentInstructionIndex++;
        }
        if (IsKeyPressed(KEY_LEFT) && currentInstructionIndex > 0) { // Previous image
            currentInstructionIndex--;
        }

        // Proceed to the game once ENTER is pressed
        if (IsKeyPressed(KEY_ENTER)) {
            StopMusicStream(homeScreenMusic);
            instructionsScreen = false; // Proceed to the game
        }
    }

    // Main game loop
    while (!WindowShouldClose()) 
        
    {
    switch (currentMode) 
    {
        case MODE_TENNIS:
            currentBackground = tennisBackground;
            currentPaddle = tennisPaddle;
            currentBall = tennisBall;
            currentCollisionSound = tennisCollisionSound;
            break;
        case MODE_TABLE_TENNIS:
            currentBackground = tableTennisBackground;
            currentPaddle = tableTennisPaddle;
            currentBall = tableTennisBall;
            currentCollisionSound = tableTennisCollisionSound;
            break;
        case MODE_AIR_HOCKEY:
            currentBackground = airHockeyBackground;
            currentPaddle = airHockeyPaddle;
            currentBall = airHockeyBall;
            currentCollisionSound = airHockeyCollisionSound;
            break;
        }        
        // Left Paddle Movement (W, S for up/down; A, D for left/right)
        if (IsKeyDown(KEY_W) && paddle1Y > 0) paddle1Y -= PADDLE_SPEED; // Move up
        if (IsKeyDown(KEY_S) && paddle1Y < SCREEN_HEIGHT - currentPaddle.height) paddle1Y += PADDLE_SPEED; // Move down
        if (IsKeyDown(KEY_A) && paddle1X > 0) paddle1X -= PADDLE_SPEED; // Move left
        if (IsKeyDown(KEY_D) && paddle1X < (SCREEN_WIDTH / 2) - currentPaddle.width) paddle1X += PADDLE_SPEED; // Move right

        // Right Paddle Movement (Arrow keys for up/down; Left/Right arrows for left/right)
        if (IsKeyDown(KEY_UP) && paddle2Y > 0) paddle2Y -= PADDLE_SPEED; // Move up
        if (IsKeyDown(KEY_DOWN) && paddle2Y < SCREEN_HEIGHT - currentPaddle.height) paddle2Y += PADDLE_SPEED; // Move down
        if (IsKeyDown(KEY_LEFT) && paddle2X > (SCREEN_WIDTH / 2)) paddle2X -= PADDLE_SPEED; // Move left
        if (IsKeyDown(KEY_RIGHT) && paddle2X < SCREEN_WIDTH - currentPaddle.width) paddle2X += PADDLE_SPEED; // Move right

        // Ball Movement Logic
        if (ballActive && !gameEnded) {
            ballPosition.x += ballVelocity.x * GetFrameTime();
            ballPosition.y += ballVelocity.y * GetFrameTime();

            if (ballPosition.y <= 0 || ballPosition.y + BALL_SIZE >= SCREEN_HEIGHT) {
                ballVelocity.y *= -1;  // Reverse Y direction
            }

            // Check for collision with Left Paddle
            if (ballPosition.x <= paddle1X + currentPaddle.width &&
                ballPosition.y + BALL_SIZE >= paddle1Y && ballPosition.y <= paddle1Y + currentPaddle.height) {
                ballVelocity.x *= -1;  // Reverse X direction
                ballPosition.x = paddle1X + currentPaddle.width;  // Adjust to avoid overlap
                PlaySound(currentCollisionSound);  // Play collision sound
            }

            // Check for collision with Right Paddle
            if (ballPosition.x + BALL_SIZE >= paddle2X &&
                ballPosition.y + BALL_SIZE >= paddle2Y && ballPosition.y <= paddle2Y + currentPaddle.height) {
                ballVelocity.x *= -1;  // Reverse X direction
                ballPosition.x = paddle2X - BALL_SIZE; // Adjust position to avoid overlap
                PlaySound(currentCollisionSound);  // Play collision sound
            }

            // Scoring Logic
            if (ballPosition.x < 0) {
                score2++;  // Right player scores
                ballActive = false;
                PlaySound(goalSound);  // Play goal sound
                scoreTimer = SCORE_DELAY;
                ballPosition = (Vector2){(SCREEN_WIDTH / 2) - (BALL_SIZE / 2), (SCREEN_HEIGHT / 2) - (BALL_SIZE / 2)};
            }
            if (ballPosition.x > SCREEN_WIDTH) {
                score1++;  // Left player scores
                ballActive = false;
                PlaySound(goalSound);  // Play goal sound
                scoreTimer = SCORE_DELAY;
                ballPosition = (Vector2){(SCREEN_WIDTH / 2) - (BALL_SIZE / 2), (SCREEN_HEIGHT / 2) - (BALL_SIZE / 2)};
            }
        } 
        else if (!gameEnded) //runs if the game has not ended
        { //delays the ball's reactivation to give players a brief pause after a score.
            scoreTimer -= GetFrameTime(); //timer for pause
            if (scoreTimer <= 0) //resumes the game
            {
                ballActive = true;
                ballVelocity.x = (score1 > score2) ? BALL_SPEED : -BALL_SPEED; //determines where should ball go first after point is scored
            }
        }

        // Check for game end condition
    if (score1 == 7 || score2 == 7) 
    {
        if (!gameEnded) 
        {  // Ensure sound is played only once
            gameEnded = true;
            ballActive = false;
            PlaySound(gameEndSound); // Play game end sound
        }
    }
        // Render game elements
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        DrawTextureEx(currentBackground, (Vector2){0, 0}, 0.0f, 
        (float)SCREEN_WIDTH / currentBackground.width, WHITE);
        
        DrawTexture(currentPaddle, paddle1X, paddle1Y, WHITE);
        DrawTexture(currentPaddle, paddle2X, paddle2Y, WHITE);
        if (ballActive) 
        {
            DrawTexture(currentBall, ballPosition.x, ballPosition.y, WHITE);
        }
        
        // Draw scores
        char scoreText1[10];
        char scoreText2[10];
        sprintf(scoreText1, "%d", score1);
        sprintf(scoreText2, "%d", score2);
        DrawTextEx(scoreFont, scoreText1, (Vector2){SCREEN_WIDTH / 4, 15}, 80, 2, WHITE);
        DrawTextEx(scoreFont, scoreText2, (Vector2){3 * SCREEN_WIDTH / 4, 15}, 80, 2, WHITE); 
        //Syntax (fromwhere_load,whattoprint, vector2 called (x,y),font height, sapce b/w letters, color)
        // Display player labels
        DrawTextEx(scoreFont, "Player 1", (Vector2){50, 50}, 60, 2, WHITE);
        DrawTextEx(scoreFont, "Player 2", (Vector2){SCREEN_WIDTH - 220, 50}, 60, 2, WHITE);

        // Display win message
        if (score1 == 7) 
        {
            DrawTextEx(scoreFont, "Player 1 Wins!", (Vector2){370, 375}, 80, 2, WHITE);
        } 
        else if (score2 == 7) 
        {
            DrawTextEx(scoreFont, "Player 2 Wins!", (Vector2){350, 375}, 80, 2, WHITE);
        }

        EndDrawing();
    }

    // Unload resources
    UnloadTexture(tennisPaddle);
    UnloadTexture(tennisBall);
    UnloadTexture(tennisBackground);
    UnloadTexture(tableTennisBackground);
    UnloadTexture(tableTennisPaddle);
    UnloadTexture(tableTennisBall);
    UnloadTexture(airHockeyBackground);
    UnloadTexture(airHockeyPaddle);
    UnloadTexture(airHockeyBall);
    UnloadTexture(instructionsTexture1);
    UnloadTexture(homeScreenTexture);
    UnloadFont(scoreFont);
    UnloadSound(tennisCollisionSound);
    UnloadSound(tableTennisCollisionSound);
    UnloadSound(airHockeyCollisionSound);
    UnloadSound(gameEndSound);
    UnloadSound(goalSound);
    CloseAudioDevice();

    CloseWindow(); // Close the window and OpenGL context
    return 0;
}
5