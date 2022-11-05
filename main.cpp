#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include <fstream>
#include <cmath>

using namespace std;

mt19937_64 mt(time(NULL));
int v[40][40];

void mineGenerator (int n , int m , int& mines , int& freeSpaces) {
    int x = n*m;
    int square_xy , mine_xy;
    uniform_int_distribution<int> matrixRange(0 , x-1);
    uniform_int_distribution<int> mineProbability(1 , freeSpaces);
    square_xy = matrixRange(mt);
    while (v[square_xy/m][square_xy%m]) {
        square_xy = matrixRange(mt);
    }
    mine_xy = mineProbability(mt);
    if (mine_xy>mines) {
        v[square_xy/m][square_xy%m] = 1;
        freeSpaces--;
    }
    else {
        v[square_xy/m][square_xy%m] = 9;
        freeSpaces--;
        mines--;
    }
}

void boardSetup(int n , int m) {
    const int a[8]={1,0,-1,0,1,1,-1,-1};
    const int b[8]={0,1,0,-1,1,-1,-1,1};
    for (int i=0; i<n; i++) {
        for (int j=0; j<m; j++) {
            if (v[i][j]==1) v[i][j]=0;
        }
    }

    //(0,0)
    if (v[0][0]>=9) {
        v[1][1]++;
        v[0][1]++;
        v[1][0]++;
    }
    //(0,m-1)
    if (v[0][m-1]>=9) {
        v[1][m-2]++;
        v[0][m-2]++;
        v[1][m-1]++;
    }
    //(n-1,0)
    if (v[n-1][0]>=9) {
        v[n-2][1]++;
        v[n-1][1]++;
        v[n-2][0]++;
    }
    //(n-1,m-1)
    if (v[n-1][m-1]>=9) {
        v[n-2][m-2]++;
        v[n-1][m-2]++;
        v[n-2][m-1]++;
    }
    //(0,k)
    for (int j=1; j<m-1; j++) {
        if (v[0][j]>=9) {
            v[0][j-1]++;
            v[0][j+1]++;
            v[1][j-1]++;
            v[1][j]++;
            v[1][j+1]++;
        }
    }
    //(n-1,k)
    for (int j=1; j<m-1; j++) {
        if (v[n-1][j]>=9) {
            v[n-1][j-1]++;
            v[n-1][j+1]++;
            v[n-2][j-1]++;
            v[n-2][j]++;
            v[n-2][j+1]++;
        }
    }
    //(k,0)
    for (int i=1; i<n-1; i++) {
        if (v[i][0]>=9) {
            v[i-1][0]++;
            v[i+1][0]++;
            v[i-1][1]++;
            v[i][1]++;
            v[i+1][1]++;
        }
    }
    //(k,m-1)
    for (int i=1; i<n-1; i++) {
        if (v[i][m-1]>=9) {
            v[i-1][m-1]++;
            v[i+1][m-1]++;
            v[i-1][m-2]++;
            v[i][m-2]++;
            v[i+1][m-2]++;
        }
    }
    //everything else
    for (int i=1; i<n-1; i++) {
        for (int j=1; j<m-1;j++) {
            if (v[i][j]>=9) {
                for (int ik=0; ik<8; ik++) {
                    v[i+a[ik]][j+b[ik]]++;
                }
            }
        }
    }

    for (int i=0; i<n; i++) {
        for (int j=0; j<m; j++) {
            if (v[i][j]>9) {
                v[i][j] = 9;
            }
        }
    }
}

void showBoard(int n , int m , char gameBoard[][40] , int mines , int flags) {
    cout<<"     ";
    for (int j=1; j<m*2; j++) {
        cout<<"_";
    }
    for (int i=0; i<n; i++) {
        if (i>n-10) printf("\n %d  |" , n-i);
        else printf("\n %d |" , n-i);
        for (int j=0; j<m; j++) {
            printf("%c|" , gameBoard[i][j]);
        }
    }
    cout<<"\n     ";
    for (int j=1; j<=m; j+=2) {
        if (j<=7) cout<<j<<"   ";
        else cout<<j<<"  ";
    }
    printf("      %d mines" , mines-flags);
    cout<<"\n       ";
    for (int j=2; j<=m; j+=2) {
        if (j<=7) cout<<j<<"   ";
        else cout<<j<<"  ";
    }
    cout<<endl;
}

void cell_action(int n , int m , char (*gameBoard)[40] , string action , int x , int y , int& winCondition , int& mines , int& flags) {
    const int a[8]={1,0,-1,0,1,1,-1,-1};
    const int b[8]={0,1,0,-1,1,-1,-1,1};
    if (gameBoard[x][y] == '.') {
        if (action == "flag" || action == "f") {
            gameBoard[x][y] = '#';
            flags++;
        }
        else {
            switch (v[x][y]) {
                case 9:
                    winCondition = -1;
                    for (int i=0; i<n; i++) {
                        for (int j=0; j<m; j++) {
                            if (v[i][j] == 9) gameBoard[i][j] = '*';
                        }
                    }
                    break;
                case 0:
                    winCondition--;
                    gameBoard[x][y] = ' ';
                    for (int k=0; k<8; k++) {
                        if (x+a[k]<n && x+a[k]>-1 && y+b[k]<m && y+b[k]>-1 && gameBoard[x+a[k]][y+b[k]] == '.'){
                            cell_action(n , m , gameBoard , "open" , x+a[k] , y+b[k] , winCondition , mines , flags);
                        }
                    }
                    break;
                default:
                    winCondition--;
                    gameBoard[x][y] = v[x][y]+48;
                    break;
            }
        }
    }
    else if (gameBoard[x][y]>48 && gameBoard[x][y]<57) {
        for (int k=0; k<8; k++) {
            if (x+a[k]<n && x+a[k]>-1 && y+b[k]<m && y+b[k]>-1 && gameBoard[x+a[k]][y+b[k]] == '.'){
                cell_action(n , m , gameBoard , "open" , x+a[k] , y+b[k] , winCondition , mines , flags);
            }
        }
    }
    else if (gameBoard[x][y] == '#' && (action == "flag" || action == "f")) {
        gameBoard[x][y] = '.';
        flags--;
    }
}

void updateLeaderboard(string name , int currentScore) {
    struct score{
        string name;
        int score;
    }scores[9999];
    int x = 0;
    ifstream f("scoreboard");
    while (f>>scores[x].name && f>>scores[x].score) {
        x++;
    }
    f.close();
    scores[x].name = name;
    scores[x].score = currentScore;
    for (int i=0; i<=x-1; i++) {
        for (int j=i+1; j<=x; j++) {
            if (scores[j].score>scores[i].score) {
                swap(scores[i].score , scores[j].score);
                swap(scores[i].name , scores[j].name);
            }
        }
    }
    ofstream f2("scoreboard");
    for (int i=0; i<=x; i++) {
        f2<<scores[i].name<<" "<<scores[i].score<<endl;
    }
    f2.close();
}

void game(int n , int m , int mines , int score_multiplier , int time_multiplier) {
    const int a[8]={1,0,-1,0,1,1,-1,-1};
    const int b[8]={0,1,0,-1,1,-1,-1,1};
    char gameBoard[40][40];
    for (int i=0; i<n; i++) {
        for (int j=0; j<m; j++) {
            gameBoard[i][j] = '.';
        }
    }
    int flags = 0 , freeSpaces=n*m , generator_mines = mines , x , y;
    string action;
    showBoard(n , m , gameBoard , mines , flags);
    cout<<"\n>";
    cin>>action>>x>>y;
    while ((action != "open" && action != "o") || x<1 || x>n || y<1 || y>m) {
        cout<<"\nError! Please try again:";
        cin>>action>>x>>y;
    }
    {
        x=n-x; y--;
        v[x][y] = 1; freeSpaces--;
        if (x<n-1) {v[x+1][y] = 1; freeSpaces--;}
        if (y<m-1) {v[x][y+1] = 1; freeSpaces--;}
        if (x>0) {v[x-1][y] = 1; freeSpaces--;}
        if (y>0) {v[x][y-1] = 1; freeSpaces--;}
        if (x<n-1 && y<m-1) {v[x+1][y+1] = 1; freeSpaces--;}
        if (x<n-1 && y>0) {v[x+1][y-1] = 1; freeSpaces--;}
        if (x>0 && y>0) {v[x-1][y-1] = 1; freeSpaces--;}
        if (x>0 && y<m-1) {v[x-1][y+1] = 1; freeSpaces--;}
    }
    while (freeSpaces) {
        mineGenerator(n , m , generator_mines , freeSpaces);
    }
    boardSetup(n , m);
    freeSpaces = n*m-mines;
    cell_action(n , m , gameBoard , action , x , y , freeSpaces , mines , flags);
    auto start = chrono::steady_clock::now();
    while (freeSpaces>0) {
        showBoard(n , m , gameBoard , mines , flags);
        cout<<"\n>";
        cin>>action>>x>>y;
        while ((action != "open" && action != "flag" && action != "f" && action != "o") || x<1 || x>n || y<1 || y>m) {
            cout<<"\nError! Please try again:";
            cin>>action>>x>>y;
        }
        x=n-x; y--;
        cell_action(n , m , gameBoard , action , x , y , freeSpaces , mines , flags);
    }
    if (freeSpaces) {
        showBoard(n , m , gameBoard , mines , flags);
        cout<<"\n\nBOOM! You lost!";
    }
    else {
        auto end = chrono::steady_clock::now();
        int currentScore , garen;
        if (score_multiplier) {
            garen = chrono::duration_cast<chrono::seconds>(end - start).count();
            currentScore = 999999/score_multiplier - garen * time_multiplier;
        }
        for (int i=0; i<n; i++) {
            for (int j=0; j<m; j++) {
                if (gameBoard[i][j] == '.') gameBoard[i][j] = '#';
            }
        }
        showBoard(n , m , gameBoard , 0 , 0);
        cout<<"\n\nYou Won!";
        cout<<"\nScore: ";
        if (score_multiplier) {
            if (currentScore<0) {
                currentScore = 0;
            }
            cout<<currentScore<<"\nEnter your name: ";
            string name;
            cin>>name;
            while (name.length()<3 || name.length()>20) {
                cout<<"\nName must be between [3,20] characters\nTry another name:";
                cin>>name;
            }
            updateLeaderboard(name , currentScore);
        }
        else cout<<"Not supported on custom difficulty.";
    }
}

void gameSetup() {
    int n = 9 , m = 9 , mines = 10 , score_multiplier = 1000 , time_multiplier = 3;
    string difficulty;
    cout<<"\nChoose difficulty:{Beginner} {Intermediate} {Expert} {Custom} {Confirm}\n";
    cout<<"\n[Beginner] Intermediate Expert Custom\n9x9 10 mines\n\n>";
    cin>>difficulty;
    while (difficulty != "Confirm" && difficulty != "confirm" && difficulty != "cf" && difficulty != "5") {
        if (difficulty == "Beginner" || difficulty == "beginner" || difficulty == "1") {
            cout<<"\n[Beginner] Intermediate Expert Custom\n9x9 10 mines\n\n>";
            n = 9; m = 9; mines = 10; score_multiplier = 1000; time_multiplier = 3;
            cin>>difficulty;
        }
        else if (difficulty == "Intermediate" || difficulty == "intermediate" || difficulty == "2") {
            cout<<"\nBeginner [Intermediate] Expert Custom\n16x16 40 mines\n\n>";
            n = 16; m = 16; mines = 40; score_multiplier = 100; time_multiplier = 8;
            cin>>difficulty;
        }
        else if (difficulty == "Expert" || difficulty == "expert" || difficulty == "3") {
            cout<<"\nBeginner Intermediate [Expert] Custom\n30x16 99 mines\n\n>";
            n = 30; m = 16; mines = 99; score_multiplier = 10; time_multiplier = 25;
            cin>>difficulty;
        }
        else if (difficulty == "Custom" || difficulty == "custom" || difficulty == "4") {
            cout<<"\nEnter [Width] [Height] [Mines]:";
            cin>>n>>m>>mines;
            while (n<5 || m<5 || n>40 || m>40 || mines<1 || mines>((m*n)/2)-1) {
                cout<<"\nWidth and Height must be between [5,40] , and the number of mines must be lower than half of the";
                cout<<" available spaces (less than n*m/2)\nTry again:";
                cin>>n>>m>>mines;
            }
            printf("\nBeginner Intermediate Expert [Custom]\n%dx%d %d mines\n\n>" , n , m , mines);
            score_multiplier = 0;
            cin>>difficulty;
        }
        else if (difficulty == "Confirm" || difficulty == "confirm" || difficulty == "cf" || difficulty == "5") {

        }
        else {
            cout<<"\nError! Please try again:";
            cin>>difficulty;
        }
    }
    game(n , m , mines , score_multiplier , time_multiplier);
}

int main() {
    cout<<"\n"
          "  __  __ _____ _   _ ______  _______          ________ ______ _____  ______ _____  \n"
          " |  \\/  |_   _| \\ | |  ____|/ ____\\ \\        / /  ____|  ____|  __ \\|  ____|  __ \\ \n"
          " | \\  / | | | |  \\| | |__  | (___  \\ \\  /\\  / /| |__  | |__  | |__) | |__  | |__) |\n"
          " | |\\/| | | | | . ` |  __|  \\___ \\  \\ \\/  \\/ / |  __| |  __| |  ___/|  __| |  _  / \n"
          " | |  | |_| |_| |\\  | |____ ____) |  \\  /\\  /  | |____| |____| |    | |____| | \\ \\ \n"
          " |_|  |_|_____|_| \\_|______|_____/    \\/  \\/   |______|______|_|    |______|_|  \\_\\\n"
          "                                                                                   \n"
          "                                                                                   ";
    this_thread::sleep_for(2s);
    cout<<"\n\nWanna play?\n";
    string a = "start";
    while (a != "exit") {
        cout<<"\nPossible options (without brackets): {start} {exit} {leaderboard} {rules}\nInput command:";
        cin>>a;
        while (a!="start" && a!="exit" && a!="leaderboard" && a!="rules") {
            cout<<"Can't you read? You can only input one of the following commands (without brackets): {start} {exit} {leaderboard} {rules}";
            cout<<"\nTry again:";
            cin>>a;
        }
        if (a == "leaderboard") {
            cout<<"<----------------------------------->\n"
                  "             Leaderboard             \n"
                  "<----------------------------------->\n";
            int x = 1;
            string q , r;
            ifstream fin("scoreboard");
            while (fin>>q) {
                cout<<x++<<". "<<q;
                fin>>r;
                int k = 35 - ((int) log10(x)) - q.length() - r.length();
                for (k ; k>1; k--) {
                    cout<<".";
                }
                cout<<r<<endl;
            }
        }
        else if (a == "rules") {
            cout<<"The board is divided into cells, with mines randomly distributed. To win, you need \n"
                  "to open all the cells. The number on a cell shows the number of mines adjacent to \n"
                  "it. Using this information, you can determine cells that are safe, and cells that \n"
                  "contain mines. Cells can be opened with the {open} or {o} command, or they can\n"
                  "be flagged/unflagged with the {flag} or {f} command (without brackets). Commands \n"
                  "have to be structured: [action] [y-coordonate] [x-coordonate].\n"
                  "\n"
                  "Example: \"flag 4 6\" will flag the cell with the coordinates 4 6;\n"
                  "\n"
                  "Game board example:\n"
                  "    ___________\n"
                  " 6 |1|*|*|1| | | <---Cell with no adjacent mines. Automatically uncovers all\n"
                  " 5 |1|2|2|1|1| |     adjacent cells when opened.\n"
                  " 4 | | | | |1|#| <---Flagged cell. Flagged cells can't be opened.\n"
                  " 3 | | | | |1|1| <---Numbered cells indicate the number of adjacent mines.\n"
                  " 2 |1|2|1|1|1|*| <---Mine. The game is lost when you open the first mine.\n"
                  " 1 |.|.|.|1|1|.| <---Covered cell.\n"
                  "    1   3   5       \n"
                  "      2   4   6     3 mines\n"
                  "\n"
                  "As you can see , the (3,6) cell has been flagged.\n"
                  "\n"
                  "Winning a standard difficulty game will a earn you a score based on difficulty\n"
                  "and time completion. The scoring system does not support custom difficulty at the\n"
                  "moment.\n";
        }
        else if (a == "start") {
            for (int i=0; i<=39; i++) {
                for (int j=0; j<=39; j++) {
                    v[i][j] = 0;
                }
            }
            gameSetup();
            this_thread::sleep_for(2s);
            cout<<"\n\nWant to start another game?\n";
        }
    }
    cout<<"\n  _______ _                 _                           __                   _             _             _ \n"
          " |__   __| |               | |                         / _|                 | |           (_)           | |\n"
          "    | |  | |__   __ _ _ __ | | __  _   _  ___  _   _  | |_ ___  _ __   _ __ | | __ _ _   _ _ _ __   __ _| |\n"
          "    | |  | '_ \\ / _` | '_ \\| |/ / | | | |/ _ \\| | | | |  _/ _ \\| '__| | '_ \\| |/ _` | | | | | '_ \\ / _` | |\n"
          "    | |  | | | | (_| | | | |   <  | |_| | (_) | |_| | | || (_) | |    | |_) | | (_| | |_| | | | | | (_| |_|\n"
          "    |_|  |_| |_|\\__,_|_| |_|_|\\_\\  \\__, |\\___/ \\__,_| |_| \\___/|_|    | .__/|_|\\__,_|\\__, |_|_| |_|\\__, (_)\n"
          "                                    __/ |                             | |             __/ |         __/ |  \n"
          "                                   |___/                              |_|            |___/         |___/   \n\n";
    this_thread::sleep_for(2s);
    return 0;
}
