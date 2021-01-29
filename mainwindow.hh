/*
#############################################################################
# File: mainwindow.hh                                                       #
# Modified: 29.12.2020-23.1.2021                                            #
#                                                                           #
# Program author:                                                           #
# Name: Olivia Saukonoja                                                    #
#############################################################################
*/

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QPushButton>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Saves two buttons that has been clicked on by calling getButton
    // Calls for isMovePossible, isGameOver and disableButtons
    void handleButton();

    // Closes the previous program and restarts a new one
    void on_restartButton_clicked();

private:
    Ui::MainWindow *ui;

    // CONSTANTS:

    const int ROWS = 5;
    const int COLUMNS = 4;

    const int FIRST_ROW = 0;
    const int SECOND_ROW = 1;
    const int THIRD_ROW = 2;
    const int FOURTH_ROW = 3;
    const int LAST_ROW = 4;

    const int FIRST_COLUMN = 0;
    const int SECOND_COLUMN = 1;
    const int THIRD_COLUMN = 2;
    const int LAST_COLUMN = 3;

    const int CROSSROADS_X = 1;

    // Margins for the drawing area
    const int TOP_MARGIN = 150;
    const int LEFT_MARGIN = 150;

    // Size of a red, green or white square
    const int SQUARE_SIDE = 70;

    // Constants describing the filenames' affixes
    const std::string PREFIX = ":/";
    const std::string SUFFIX = ".png";

    // Filenames without affixes
    const std::string RED_FILE = "redball";
    const std::string GREEN_FILE = "greenball";

    const std::string EMPTY = "";
    const int NOTHING_SAVED = 0;

    const QString GAME_INFO = "Move all red balls to the top row and all "
                              "green balls to the bottom row. You can't move "
                              "a ball over another ball. "
                              "The smallest possible amount of moves is 31.";

    const QString GAME_OVER = "Game over. Congratulations!";

    // ATTRIBUTES:

    // Stores the pointers of the push buttons
    std::vector<std::vector<QPushButton*>> gameboard_;

    // Stores the filenames of the icons
    std::vector<std::vector<std::string>> iconboard_;

    QPushButton* startingPoint_;
    QPushButton* endingPoint_;

    int moves_;

    // METHODS:

    // Creates the game board and adds information to the data structures
    // Calls for drawBall
    // Returns nothing
    void createPushButtons();

    // Parameters:
    // - filename: the name of the file which includes the picture of the ball
    // - pushButton: button where the icon is going to be drawed
    // Draws the ball to the given button
    // Returns nothing
    void drawBall(const std::string& filename, QPushButton*& pushButton) const;

    // Returns the button that the cursor has clicked on
    QPushButton* getButton() const;

    // Parameters:
    // - button: the button which coordinates are going to be returned
    // - getRow: if true, returns the row of the button, and if false, returns
    //           the column of the button
    // returns integer that tells which row/column the button is in
    int getLocation(QPushButton*& button, bool getRow) const;

    // Checks if the move is possible and if it is, calls for moveBall
    // Returns true if the move was made and false if wasn't
    bool isMovePossible();

    // Parameters:
    // - start: the coordinate of the start point
    // - end: the coordinate of the ending point
    // - constantCoordinate: either row or column where the check is happening
    // - horizontalCheck: true if checking the points horizontally and false
    //                    if checking the points vertically
    // Goes through every point between the start and the end and checks that
    // there aren't balls
    // Returns true if the points are empty and false if aren't
    bool areBetweenPointsEmpty(const int& start, const int& end, const int&
                               constantCoordinate, const bool&
                               horizontalCheck);

    // Parameters:
    // - startX: the x coordinate (column) of the ball
    // - startY: the y coordinate (row) of the ball
    // - endX: the x coordinate (column) of the destination
    // - endY: the y coordinate (row) of the destination
    // Changes the icon of the ball to the destination point and updates
    // the data structure
    void moveBall(const int& startX, const int& startY, const int& endX,
                  const int& endY);

    // Adds one more move to the moves and to the display
    void addMove();

    // Checks if the game situation is at end
    // Returns true if it is and false if isn't
    bool isGameOver();

    // When the game is over, disables buttons
    // Returns nothing
    void disableButtons();

};
#endif // MAINWINDOW_HH
