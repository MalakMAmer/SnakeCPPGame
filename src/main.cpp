#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

using namespace std;

Color pink = (Color){255, 114, 187, 255};
Color semiPink = (Color){255, 114, 187, 250};
Color lightPink = (Color){255, 237, 246, 255};
Color darkPink = (Color){143, 4, 74, 255};
Color gold = (Color){255, 243, 82, 255};

enum  status {OPENING, ACTIVE, CLOSE};
status currentStatus = OPENING;


// float currentSpeed = 0.2;

// typedef enum TextAlignment {
//     TEXT_ALIGN_LEFT   = 0,
//     TEXT_ALIGN_TOP    = 0,
//     TEXT_ALIGN_CENTRE = 1,
//     TEXT_ALIGN_MIDDLE = 1,
//     TEXT_ALIGN_RIGHT  = 2,
//     TEXT_ALIGN_BOTTOM = 2
// } TextAlignment;

int offset = 75;

int cellSizes= 30;
int cellCount = 25;
int WIDTH=2*offset + cellSizes*cellCount;
int HIGHT=2*offset + cellSizes*cellCount;

double lastUpdateTime = 0;



bool ElementInDeque(Vector2 element, deque<Vector2> deque){
    for(unsigned int i=0; i<deque.size(); i++){
        if(Vector2Equals(deque[i], element)){
            return true;
        }
    }
    return false;
}

bool eventTriggered(double interval){
    double currentTime = GetTime();
    if(currentTime - lastUpdateTime >= interval){
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

class Score{
public:
    
    int CurrentScore = 0;
    int FinalScore = 0;
    int HighestFinal = 0;

    void updateScore(){
        CurrentScore+=5;
        // updateScore();
    }

    void CalcFinal(){
        FinalScore = CurrentScore;
    }

    void calcHighScore(){
        if(HighestFinal<=FinalScore){
            HighestFinal = FinalScore;
        }
    }

    // void updateSpeed(){
    //     if(CurrentScore%20==0){
    //         currentSpeed -= 0.02;
    //     }
    // }

};

class Snaky{
public:

    deque<Vector2> snakeBody= {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
    Vector2 direction = {1,0};
    bool addSeg = false;
    Texture2D texture1;
    Texture2D texture2;
    Texture2D snakeTexture;
    Texture2D snakeFace;

    Snaky(){
        Image head = LoadImage("../../src/graphics/head.png");
        Image tail = LoadImage("../../src/graphics/tail.png");
        texture1 = LoadTextureFromImage(head);
        texture2 = LoadTextureFromImage(tail);
        UnloadImage(head);
        UnloadImage(tail);

        
        Image snakePattern = LoadImage("../../src/graphics/rounded6.png");
        Image snakeface = LoadImage("../../src/graphics/snakeface.png");
        snakeTexture = LoadTextureFromImage(snakePattern);
        snakeFace = LoadTextureFromImage(snakeface);
        UnloadImage(snakePattern);
        UnloadImage(snakeface);
    }

    void Draw(){
        bool white = true;
        for(unsigned int i=0; i< snakeBody.size(); i++){
            float x = snakeBody[i].x;
            float y  = snakeBody[i].y;

            Rectangle segment = Rectangle{x*cellSizes+offset, y*cellSizes+offset, (float)cellSizes, (float)cellSizes};
            DrawRectangleRounded(segment, 0.6, 6, lightPink);
            if(i==0){
                DrawTexture(snakeFace, x*cellSizes+offset , y*cellSizes+offset , WHITE);
            }
            else{
                DrawTexture(snakeTexture, x*cellSizes+offset , y*cellSizes+offset , WHITE);
            }
            Rectangle border  = {(float)x*cellSizes+offset-4, (float)y*cellSizes+offset-4, (float)cellSizes+8, (float)cellSizes+8} ;
            DrawRectangleRoundedLinesEx(border, 0.8, 10, 2, lightPink);
            // white? DrawRectangle(x*cellSizes, y*cellSizes, cellSizes, cellSizes, WHITE): DrawRectangle(x*cellSizes, y*cellSizes, cellSizes, cellSizes, GOLD);
            white = !white;
        }
    }

    void Update(){
        snakeBody.push_front(Vector2Add(snakeBody[0], direction));
        if(addSeg){
            addSeg =false;
        }
        else{
            snakeBody.pop_back();
        }
    }

    void Reset(){
        snakeBody= {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
        direction = {1,0};
    }

    ~Snaky(){
        UnloadTexture(texture1);
        UnloadTexture(texture2);
        UnloadTexture(snakeTexture);
        UnloadTexture(snakeFace);
    }

};

class Food{
public:
    Vector2 position;
    Texture2D texture ;
    int foodRotation;

    Food(deque<Vector2> sBody){
        Image image = LoadImage("../../src/graphics/strawberry.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandomPos(sBody);
    }

    void Draw(){
        // DrawRectangle(position.x*cellSizes , position.y*cellSizes , cellSizes , cellSizes, GOLD);
        DrawTexture(texture, position.x*cellSizes+offset , position.y*cellSizes+offset , WHITE);
        Rectangle border  = {(float)position.x*cellSizes+offset-4, (float)position.y*cellSizes+offset-4, (float)cellSizes+8, (float)cellSizes+8} ;
        DrawRectangleRoundedLinesEx(border, 0.8, 10, 2, lightPink);

        

        // Rectangle foodRec = {
        //     position.x * cellSizes+offset,
        //     position.y * cellSizes+offset,
        //     (float)cellSizes,
        //     (float)cellSizes
        // };

        // Rectangle sourceRec = {
        //     0, 0,
        //     (float)texture.width,
        //     (float)texture.height
        // };
        
        // DrawTexturePro(
        //     texture,
        //     sourceRec,
        //     foodRec,
        //     Vector2 {0, 0},
        //     foodRotation,
        //     pink
        // );

        
    }

    void DrawTransRec(){
        DrawRectangle(position.x*cellSizes+offset , position.y*cellSizes+offset , cellSizes , cellSizes, pink);
    }

    void UpdateRotation(){
        foodRotation = sin(GetTime() * 3) * 5;
    }

    Vector2 GenerateCell(){
        float x = GetRandomValue(0, cellCount-1);
        float y = GetRandomValue(0, cellCount-1);
        return Vector2{x,y};
    }

    Vector2 GenerateRandomPos(deque<Vector2> sBody){
        Vector2 pos = GenerateCell();
        while(ElementInDeque(pos, sBody)){
            pos = GenerateCell();
        }
        return pos;
    }

    ~Food(){
        UnloadTexture(texture);
    }

};

class GamePopUp{
public:

    int screenSize = cellSizes * cellCount;

    Texture texture;
    Texture gameOverTexture;
    Texture screenicon;
    Texture snakeTexture;



    GamePopUp(){
        Image frame = LoadImage("../../src/graphics/Frame.png");
        texture = LoadTextureFromImage(frame);
        UnloadImage(frame);

        Image GameOverframe = LoadImage("../../src/graphics/GameOverFrame.png");
        gameOverTexture = LoadTextureFromImage(GameOverframe);
        UnloadImage(GameOverframe);

        Image icon = LoadImage("../../src/graphics/iconBig.png");
        screenicon = LoadTextureFromImage(icon);
        UnloadImage(icon);
    }


    // Vector2 CalcPositionOfText(Rectangle textContainerRect, string word, int fontSize){
    //     float posTextX, posTextY;
    //     Vector2 textSize = MeasureTextEx(
    //         GetFontDefault(),
    //         word.c_str(),
    //         (float)fontSize,
    //         fontSize * 0.1f
    //     );

    //     posTextX = textContainerRect.x + (textContainerRect.width - textSize.x) / 2;
    //     posTextY = textContainerRect.y + (textContainerRect.height - textSize.y) / 2;
        
    //     return Vector2{posTextX,posTextY};
    // }


    void OpeningForTheGame(){

        // DrawRectangle((float)screenSize/2+offset, (float)screenSize/2+offset, (float)screenSize, (float)screenSize, semiPink);
        // for alignment

        Rectangle rec= Rectangle{
            (float)offset,
            (float)offset,
            (float)screenSize,
            (float)screenSize,
        };

        // Vector2 welcometxt = CalcPositionOfText(rec, "Play", 80);
        // DrawTextEx(font, word[i], textPos, (float)fontSize, fontSize*.1f, RAYWHITE);


        DrawRectangleRounded(rec, 0, 10 ,lightPink);
        DrawTexture(texture,0,0,WHITE);
        DrawText(
            TextFormat( "Welcome To"),
            (float)screenSize / 4 +offset - 40,
            (float)screenSize / 8 + offset+ 40,
            80,
            BLACK
        );
        DrawText(
            "SNAKY!",
            (float)screenSize / 4 + 2*offset- (5*6),
            (float)screenSize / 8 + offset+ 90+ 40,
            80,
            pink
        );
        DrawText(
            "Press the WASD or \nArrow keys to Satrt",
            (float)screenSize / 4 +2*offset - 45,
            (float)screenSize / 8 + offset+ 90 + 90+ 40,
            30,
            semiPink
        );
        DrawTexture(
            screenicon,
            (float)screenSize / 4 + 2*offset -20,
            (float)screenSize / 8 + offset+ 90 + 90+ 75 + 40,
            WHITE
        );
    }

    void GameOverPopupDraw(Score& score){
        Rectangle rec= Rectangle{
            (float)offset,
            (float)offset,
            (float)screenSize,
            (float)screenSize,
        };
        DrawRectangleRounded(rec, 0, 10 ,gold);
        DrawTexture(gameOverTexture,0,0,WHITE);
        DrawText(
            "GAME OVER!",
            (float)screenSize / 8 +offset + 40,
            (float)screenSize / 8 + offset+ 70,
            80,
            pink
        );
        // DrawText( "Snaky", offset-5, 20, 40, lightPink);
        //     DrawText( TextFormat("Score: %i", score.CurrentScore), cellSizes*cellCount-offset-5, 20, 40, lightPink);
        DrawText( 
            TextFormat("Your Score: %d", score.FinalScore), 
            (float)screenSize / 4 + offset+10, 
            (float)screenSize / 8 + offset+ 40 + 90 + 100,
            50, 
            darkPink
        );

        DrawText( 
            TextFormat("Highest Score: %d", score.HighestFinal), 
            (float)screenSize / 4 + offset - 30, 
            (float)screenSize / 8 + offset+ 40 + 90 + 70 + 100,
            50,
            darkPink
        );
    }



    ~GamePopUp(){
        UnloadTexture(texture);
        UnloadTexture(screenicon);
        UnloadTexture(gameOverTexture);
    }

};


class Game{
public:
    Snaky snake = Snaky();
    Food food = Food(snake.snakeBody);
    Score score = Score();
    GamePopUp popup = GamePopUp();
    bool running = true;
    Sound eatSound;
    Sound lossSound;

    Game(){
        InitAudioDevice();
        eatSound = LoadSound("../../src/graphics/eat.mp3");
        lossSound = LoadSound("../../src/graphics/loss.mp3");
    }

    void Draw(){
        if(currentStatus==OPENING){
            running = false;
            popup.OpeningForTheGame();
        }
        else if(currentStatus==ACTIVE){
            food.DrawTransRec();
            food.Draw();
            snake.Draw();
            DrawRectangleLinesEx( Rectangle{(float)offset-5, (float)offset-5, (float)cellCount*cellSizes+10, (float)cellCount*cellSizes+10} ,5,lightPink);
            DrawText( "Snaky", offset-5, 20, 40, lightPink);
            DrawText( TextFormat("Score: %i", score.CurrentScore), cellSizes*cellCount-offset-5, 20, 40, lightPink);
        }
        else if(currentStatus==CLOSE){
            popup.GameOverPopupDraw(score);
        }
        
        // popup.FirstOpeningGame();
    }

    void Update(){
        if(running){
            snake.Update();
            CheckCollisionWithFood();
            CheckCollisionWithEdges();
            CheckCollisionWithTail();
            food.UpdateRotation();
        }
    }

    void CheckCollisionWithFood(){
        if(Vector2Equals(snake.snakeBody[0], food.position)){
            food.position = food.GenerateRandomPos(snake.snakeBody);
            // cout << "Ate the Strawberry!" << endl;
            score.updateScore();
            snake.addSeg = true;
            SetSoundPitch(eatSound, 2.0f);
            PlaySound(eatSound);
        }
    }

    void CheckCollisionWithEdges(){
        if(snake.snakeBody[0].x== cellCount || snake.snakeBody[0].x == -1){
            GameOver();
        }
        if(snake.snakeBody[0].y== cellCount || snake.snakeBody[0].y == -1){
            GameOver();
        }
    }

    void CheckCollisionWithTail(){
        deque<Vector2> headlessBody = snake.snakeBody;
        headlessBody.pop_front();

        if(ElementInDeque(snake.snakeBody[0], headlessBody)){
            GameOver();
        }

    }



    void GameOver(){
        // cout << "Game Over" <<endl;
        snake.Reset();
        food.position = food.GenerateRandomPos(snake.snakeBody);
        currentStatus = CLOSE;
        running = false;
        score.CalcFinal();
        score.calcHighScore();
        score.CurrentScore = 0;
        PlaySound(lossSound);
    }

   

    ~Game(){
        UnloadSound(eatSound);
        UnloadSound(lossSound);
        CloseAudioDevice();
    }
};


int main () {
    cout << "starting the game....." <<endl;
    InitWindow( WIDTH, HIGHT , "Snaky");
    Image icon = LoadImage("../../src/graphics/GameIcon.png");
    SetWindowIcon(icon); 
    UnloadImage(icon);
    SetTargetFPS(60);

    Game game = Game();

    while(WindowShouldClose() == false){
        
        BeginDrawing();
        if(eventTriggered(0.1)){
            game.Update();
        }

        if((IsKeyPressed(KEY_UP)||IsKeyPressed(KEY_W)) && game.snake.direction.y != 1){
            game.snake.direction = {0, -1};
            game.running = true;
        }
        if((IsKeyPressed(KEY_DOWN)||IsKeyPressed(KEY_S)) && game.snake.direction.y != -1){
            game.snake.direction = {0, 1};
            game.running = true;
        }
        if((IsKeyPressed(KEY_LEFT)||IsKeyPressed(KEY_A)) && game.snake.direction.x != 1){
            game.snake.direction = {-1, 0};
            game.running = true;
        }
        if((IsKeyPressed(KEY_RIGHT)||IsKeyPressed(KEY_D)) && game.snake.direction.x != -1){
            game.snake.direction = {1, 0};
            game.running = true;
        }

        if((IsKeyPressed(KEY_LEFT)||IsKeyPressed(KEY_A)) || (IsKeyPressed(KEY_RIGHT)||IsKeyPressed(KEY_D)) 
        ||(IsKeyPressed(KEY_DOWN)||IsKeyPressed(KEY_S)) || (IsKeyPressed(KEY_UP)||IsKeyPressed(KEY_W))){
            currentStatus = ACTIVE;
        }

        ClearBackground(pink);
        game.Draw();
        
        EndDrawing();

    }


    CloseWindow();
    return 0;
}