// Keep track of bowling score for three games.

// Zachary Hayes - zjhayes@dmacc.edu - July 12th, 2019

#include <iostream>
#include <stdio.h>
#include <vector>
#include <time.h>
#include <iomanip>
using namespace std;

class Score
{
private:
    int gameId;
    int score;
public:
    // Constructors
    Score()
    {
        gameId = 0;
        score = 0;
    }
    Score(int gameIdIn) // Create score with game id.
    {
        gameId = gameIdIn;
        score = 0;
    }
    
    // Accessors and mutators
    void setScore(int scoreIn)
    {
        score = scoreIn;
    }
    int getScore()
    {
        return score;
    }
    int getGameId()
    {
        return gameId;
    }
};

struct Scorecard // Keeps track of player's record.
{
    Score highScore;
    Score lowScore;
    int averageScore;
};

class Player
{
private:
    string name;
    vector<Score> scores;
    Scorecard *scoreCard = NULL;
    
    // Private methods.
    void generateScorecard()
    {
        scoreCard = new Scorecard();
        setHighScore();
        setLowScore();
        setAvgScore();
    }
    
    void setHighScore()
    {
        Score *high = new Score();
        
        for(int i = 0; i < scores.size(); i++)      // Find the highest score by this player.
        {
            if(scores[i].getScore() > high -> getScore())
            {
                high = &scores[i];
            }
        }
        
        (*scoreCard).highScore = *high;
    }
    
    void setLowScore()
    {
        const int MAX_SCORE = 300;
        Score *low = new Score();
        low -> setScore(MAX_SCORE);
        
        for(int i = 0; i < scores.size(); i++)      // Find the lowest score by this player.
        {
            if(scores[i].getScore() < low -> getScore())
            {
                low = &scores[i];
            }
        }
        
        (*scoreCard).lowScore = *low;
    }
    
    void setAvgScore()
    {
        int runningSum = 0;
        
        for(int i = 0; i < scores.size(); i++)
        {
            runningSum += scores[i].getScore();
        }
        
        int avgScore = (runningSum / scores.size());
        
        (*scoreCard).averageScore = avgScore;
    }
    
public:
    // Constructors
    Player()
    {
        name = "";
    }
    Player(string nameIn) // Create player with name.
    {
        name = nameIn;
    }
    
    // Helper methods
    void newGame(int gameId)    // Create new Game.
    {
        Score newScore(gameId);     // Creates new score associated with game.
        scores.push_back(newScore); // Add new score to player scores.
    }
    
    /* Add score to relavent game.*/
    void addScore(int gameId, int score)
    {
        for(int i = 0; i < scores.size(); i++)  // For each score saved to player object..
        {
            if(scores[i].getGameId() == gameId) // check if game ID matches and..
            {
                scores[i].setScore(score);      // set score where game ID matches.
                return;
            }
        }
        
        cout << endl << "Error, no matching Game ID." << endl;
    }
    
    // Accessors and mutators
    void setName(string nameIn)
    {
        name = nameIn;
    }
    string getName()
    {
        return name;
    }
    vector<Score>& getScore()
    {
        return scores;
    }
    Scorecard getScorecard()
    {
        if(scoreCard == NULL)
        {
            generateScorecard();
        }
        return *scoreCard;
    }
    
};

class Game
{
private:
    int gameId;
    vector<Player*> players; // Vector of Player pointers.
    
    void generateId()   // Generate random Id number.
    {
        const int ID_LOWER = 10000;
        const int ID_UPPER = 89999;
        gameId = rand() % ID_UPPER + ID_LOWER;
    }
    
public:
    // Constructors
    Game(int numOfPlayers)
    {
        players.resize(numOfPlayers);
        generateId();
    }
    
    // Helper methods
    /* Add new player to end of player list. */
    void addPlayer(Player newPlayer)
    {
        newPlayer.newGame(gameId);      // Add new game to player object.
        players.push_back(&newPlayer);   // Add player pointer to game.
    }
    
    /* Add player to a specific roster index. */
    void replacePlayer(int index, Player &newPlayer)
    {
        newPlayer.newGame(gameId);      // Add new game to player object.
        players[index] = &newPlayer;
    }
    
    // Accessors and mutators
    int getId()
    {
        return gameId;
    }
    vector<Player*> getPlayers()
    {
        return players;
    }
    
};

vector<Game> generateGames(int numberOfGames, vector<Player> &players);
Game createGame(vector<Player> &players);
void populateScores(vector<Game> &games);
void inputPlayerScores(Game &game);
void printLeaderboard(vector<Player> &players);
void printIndividualScore(Player &player);
void printBestPlayer(vector<Player> players);

int main()
{
    const int MAX_NUM_OF_GAMES = 3;
    srand(static_cast<unsigned int>(time(NULL))); // Seed rand().
    
    vector<Player> players      // Who is bowling?
    {
        Player("Brian"),
        Player("Nessa"),
        Player("Amin"),
        Player("Susanna")
    };
    
    vector<Game> games = generateGames(MAX_NUM_OF_GAMES, players);
    populateScores(games);
    printLeaderboard(players);
    
    cout << endl << endl; // Create space for "Program ended" message.
    
    return 0;
}

/* Generate a game plan with a set number of games and players. */
vector<Game> generateGames(int numberOfGames, vector<Player> &players)
{
    vector<Game> newGamePlan;
    
    for(int i = 0; i < numberOfGames; i++)     // Generates given number of games, and adds players to each.
    {
        newGamePlan.push_back(createGame(players));
    }
    
    return newGamePlan;
}

/* Create game and add players from vector. */
Game createGame(vector<Player> &players)
{
    int numberOfPlayers = (int)players.size();
    Game newGame(numberOfPlayers);  // Create game fit for the number of players provided.
    
    for(int i = 0; i < numberOfPlayers; i++)    // For player slot..
    {
        newGame.replacePlayer(i, players[i]);  // add player from player array.
    }
    
    return newGame;
}

/* Prompt user for player scores for each game.*/
void populateScores(vector<Game> &games)
{
    for(int i = 0; i < games.size(); i++) // For each game in game plan..
    {
        Game currentGame = games[i];
        cout << endl << "--------------------------" << endl;        // Print game banner.
        cout << "GAME " << i + 1 << " (ID: " << currentGame.getId() << ")"  << endl;
        cout << "--------------------------" << endl << endl;
        cout << "Enter scores for . . ." << endl;
        
        inputPlayerScores(currentGame);
    }
}

/* Take user input, and validates, for each player's score. */
void inputPlayerScores(Game &game)
{
    int const MIN_SCORE = 0;
    int const MAX_SCORE = 300;
    vector<Player*> players = game.getPlayers();
    int gameId = game.getId();
    
    for(int i = 0; i < players.size(); i++)
    {
        int score;
        cout << players[i]->getName() << ": ";
        cin >> score;
        
        if(cin.fail() || score < MIN_SCORE || score > MAX_SCORE)
        {
            cout << "Invalid score, must be between 0 and 300. Try again." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Flush buffer.
            --i;      // Reiterate current player.
        }
        else
        {
            players[i]->addScore(gameId, score);
        }
    }
}

/* Print top scores. */
void printLeaderboard(vector<Player> &players)
{
    cout << "---------------------------" << endl;
    cout << "  LEADERBOARD" << endl;
    for(int i = 0; i < players.size(); i++) // Iterate players, print their scores.
    {
        printIndividualScore(players[i]);
    }
    
    printBestPlayer(players);
}

/* Print individual Player's scoresheet. */
void printIndividualScore(Player &player)
{
    Scorecard scoreCard = player.getScorecard();
    cout << "---------------------------" << endl;
    cout << player.getName() << endl;
    cout << "High | Avg | Low" << endl;
    cout << setw(4) << scoreCard.highScore.getScore() << setw(6) << scoreCard.averageScore << setw(6) << scoreCard.lowScore.getScore() << endl;
}

/* Print player with the highest score, their score, and the game they scored highest in. */
void printBestPlayer(vector<Player> players)
{
    int bestIndex = 0;
    
    for(int i = 1; i < players.size(); i++) // Iterate through players and compare highest scores.
    {
        if(players[i].getScorecard().highScore.getScore() > players[bestIndex].getScorecard().highScore.getScore())
        {
            bestIndex = i; // Current player is highest score so far.
        }
    }
    
    cout << "---------------------------" << endl;
    cout << "Best Player: " << players[bestIndex].getName() << endl;
    cout << "Highest Score: " << players[bestIndex].getScorecard().highScore.getScore() << endl;
    cout << "Highest Scoring Game (ID): " << players[bestIndex].getScorecard().highScore.getGameId() << endl;
}



/*
 Write a C++ program for the DMACC spring bowling league.   The league consists of four bowlers who each bowl three games on league bowling night.  Each game has a score of 0 to 300 points.  The program should allow the user to input the three bowling scores for each of the bowlers and should output the average, high score and low score for each of the individual bowlers.  The program should also output the bowler with the highest total of the night and the highest game of the night.
 */
