/*
#############################################################################
# File: mainwindow.cpp                                                      #
# Modified: 29.12.2020-23.1.2021                                            #
#                                                                           #
# Program author:                                                           #
# Name: Olivia Saukonoja                                                    #
#############################################################################
*/

#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Path");

    ui->gameInfoTextBrowser->setText(GAME_INFO);

    moves_ = 0;
    startingPoint_ = nullptr;
    endingPoint_ = nullptr;

    createPushButtons();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createPushButtons()
{
    for ( int row = 0; row < ROWS; ++row ) {
        // Adds a new vector for every row
        gameboard_.push_back({});
        iconboard_.push_back({});

        for ( int column = 0; column < COLUMNS; ++column ) {

            if ( (row == FIRST_ROW) or (row == LAST_ROW) or
            (row == SECOND_ROW and column == SECOND_COLUMN) or
            (row == THIRD_ROW and column== SECOND_COLUMN) or
            (row == THIRD_ROW and column == THIRD_COLUMN) or
            (row == FOURTH_ROW and column == SECOND_COLUMN) ) {
                // Makes push buttons to the right locations
                QPushButton* pushButton = new QPushButton(this);

                pushButton->setGeometry(LEFT_MARGIN + column * SQUARE_SIDE,
                                        TOP_MARGIN + row * SQUARE_SIDE,
                                        SQUARE_SIDE, SQUARE_SIDE);

                connect(pushButton, &QPushButton::clicked, this,
                        &MainWindow::handleButton);

                gameboard_.at(row).push_back(pushButton);

                // If the first row, makes a green button, and
                // if the last row, makes a red button
                if ( row == FIRST_ROW ) {
                    drawBall(GREEN_FILE, pushButton);
                    iconboard_.at(row).push_back(GREEN_FILE);
                } else if ( row == LAST_ROW ) {
                    drawBall(RED_FILE, pushButton);
                    iconboard_.at(row).push_back(RED_FILE);
                } else {
                    iconboard_.at(row).push_back(EMPTY);
                }
            } else {
                gameboard_.at(row).push_back(nullptr);
                iconboard_.at(row).push_back(EMPTY);
            }
        }
    }
}

void MainWindow::drawBall(const std::string& filename,
                          QPushButton*& pushButton) const
{
    if ( filename == EMPTY ) {
        pushButton->setIcon(QIcon());

    } else {
        // Converts image (png) to a pixmap
        std::string filenameWithAffixes = PREFIX + filename + SUFFIX;
        QPixmap image(QString::fromStdString(filenameWithAffixes));

        // Scales the pixmap
        image = image.scaled(SQUARE_SIDE, SQUARE_SIDE);

        // Sets the pixmap for the new push button
        pushButton->setIcon(image);
    }
}

void MainWindow::handleButton()
{
    // Saves both buttons
    if ( startingPoint_ == nullptr ) {
        QPushButton* button = getButton();
        int row = getLocation(button, true);
        int column = getLocation(button, false);
        // Saves only a button which has an icon
        if ( iconboard_.at(row).at(column) != EMPTY ) {
            startingPoint_ = button;
        }
    } else {
        endingPoint_ = getButton();

        // Checks if the move is possible and moves the ball
        if ( isMovePossible() ) {
            // Checks if the game is at end by winning it
            if ( isGameOver() ) {
                ui->gameInfoTextBrowser->setText(GAME_OVER);
                disableButtons();
            }
        }
    startingPoint_ = nullptr;
    endingPoint_ = nullptr;
    }
}

QPushButton* MainWindow::getButton() const
{
    // Finds the mouse's coordinates
    QPoint mousePos = mapFromGlobal(QCursor::pos());

    for ( int row = 0; row < ROWS; ++row ) {
        for ( int column = 0; column < COLUMNS; ++column ) {

            // If the button exists
            if ( gameboard_.at(row).at(column) != nullptr ) {

                // Gets the push button's coordinates
                QRect widgetRect = gameboard_.at(row).at(column)->geometry();

                if (widgetRect.contains(mousePos)) {
                    // The mouse's position is in the push button
                    return gameboard_.at(row).at(column);
                }
            }
        }
    }
    return nullptr;
}

int MainWindow::getLocation(QPushButton *&button, bool getRow) const
{
    for ( int row = 0; row < ROWS; ++row ) {

        for ( int column = 0; column < COLUMNS; ++column ) {

            if ( gameboard_.at(row).at(column) == button ) {
                // The ball's row and column has found
                if ( getRow ) {
                    return row;
                } else {
                    return column;
                }
            }
        }
    }
    return NOTHING_SAVED;
}

bool MainWindow::isMovePossible()
{
    int startX = getLocation(startingPoint_, false);
    int startY = getLocation(startingPoint_, true);
    int endX = getLocation(endingPoint_, false);
    int endY = getLocation(endingPoint_, true);

    // Points are in the same row
    if ( startY == endY ) {
        // Points are the same
        if ( startX == endX ) {
            return false;
        }
        if (areBetweenPointsEmpty(startX, endX, startY, true)) {
            // Actually move the piece
            moveBall(startX, startY, endX, endY);
            return true;
        }
    } else {
        // Checks points between the start and the crossroads, the crossroads
        // and the next point, and the next point and the end
        if ( areBetweenPointsEmpty(startX, CROSSROADS_X, startY, true) and
             areBetweenPointsEmpty(startY, endY, CROSSROADS_X, false) and
             areBetweenPointsEmpty(CROSSROADS_X, endX, endY, true) ) {

            // If succeed, actually move the piece and return true
            moveBall(startX, startY, endX, endY);
            return true;
        }
    }
    return false;
}

bool MainWindow::areBetweenPointsEmpty(const int& start, const int& end,
                                       const int& constantCoordinate,
                                       const bool& horizontalCheck)
{
    // If there are points between starting and ending point
    int distance = abs(start - end);
    if ( distance > 0 ) {

        // Finds the first point where to start the checking
        int between = 0;
        if ( start < end ) {
            between = start + 1;
        } else {
            between = end;
        }
        // Goes through every point between the start and the end
        for ( int i = 0; i < distance; ++i) {
            if ( horizontalCheck ) {
                // Checks the x-coordinates
                if ( iconboard_.at(constantCoordinate).at(between) != EMPTY ) {
                    return false;
                }
            } else {
                // Checks the y-coordinates
                if ( iconboard_.at(between).at(constantCoordinate) != EMPTY ) {
                    return false;
                }
            }
            ++between;
        }
    }
    return true;
}

void MainWindow::moveBall(const int& startX, const int& startY, const int&
                          endX, const int& endY)
{
    // Draws the ball to the right position
    drawBall(iconboard_.at(startY).at(startX),
             gameboard_.at(endY).at(endX));
    drawBall(EMPTY, gameboard_.at(startY).at(startX));

    // Changes the data structure to the right position
    iconboard_.at(endY).at(endX) = iconboard_.at(startY).at(startX);
    iconboard_.at(startY).at(startX) = EMPTY;

    addMove();
}

void MainWindow::addMove()
{
    moves_ += 1;
    // Shows moves to the player
    ui->scoreTextBrowser->setText(QString::number(moves_));
}

bool MainWindow::isGameOver()
{
    // If reds are on the first row
    for ( int i = 0; i < COLUMNS; ++i ) {

        if ( iconboard_.at(FIRST_ROW).at(i) != RED_FILE ) {
            return false;
        }
    }
    // If greens are on the last row
    for ( int i = 0; i < COLUMNS; ++i) {

        if ( iconboard_.at(LAST_ROW).at(i) != GREEN_FILE ) {
            return false;
        }
    }
    return true;
}

void MainWindow::disableButtons()
{
    for ( int row = 0; row < ROWS; ++row ) {

        for ( int column = 0; column < COLUMNS; ++column ) {

            // If the button exists
            if ( gameboard_.at(row).at(column) != nullptr ) {
                gameboard_.at(row).at(column)->setEnabled(false);
            }
        }
    }
}

void MainWindow::on_restartButton_clicked()
{
    // Quits the previous program
    qApp->quit();
    // Starts a new one
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}
