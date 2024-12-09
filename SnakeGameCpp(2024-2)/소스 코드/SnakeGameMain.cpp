#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>  // system("CLS")를 사용하기 위해 포함
#include "SnakeGame.h"

// 게임 로고 출력 함수
void printLogo() {
    std::cout << "**********************************\n";
    std::cout << "*           SNAKEGAME            *\n";
    std::cout << "*        Developer: J.H.W        *\n";
    std::cout << "**********************************\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));  // 2초 동안 로고 출력
}

// 사용자 이름 입력 함수
std::string getUserName() {
    std::string userName;
    std::cout << "Enter your username: ";
    std::cin >> userName;
    return userName;
}

// 게임 메뉴 출력 함수
void printMenu() {
    std::cout << "\nGame Menu\n";
    std::cout << "1. Start Game\n";
    std::cout << "2. View Stats (Game time & Score)\n";
    std::cout << "3. Exit\n";
    std::cout << "Select an option (1-3): ";
}

// 게임 시작 함수 (기본 틀)
void startGame(const std::string& userName) {
    // 게임 시작 전에 화면을 지웁니다.
    system("CLS");  // 윈도우에서 화면을 지우는 명령

    // 게임 로고 출력
    std::cout << "Game is starting...\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));  // 잠시 대기

    SnakeGame game;
    game.start();

    // 게임 종료 후 점수 출력
    Point2 pt(0, 0);
    pt.printStr("Your Score: " + std::to_string(game.getScore()), mglib::GREEN);  // 녹색 글자
    pt.printStr("Game Time: " + std::to_string(SnakeGame::getGameTime()) + " seconds", mglib::GREEN);  // 흰색 글자
}


int main() {
    // 게임 로고 출력
    printLogo();

    // 사용자 이름 입력 받기
    std::string userName = getUserName();

    // 메뉴 출력 및 사용자 선택 받기
    int choice;
    bool isRunning = true;
    while (isRunning) {
        printMenu();
        std::cin >> choice;

        switch (choice) {
        case 1:
            // 게임 시작
            startGame(userName);
            break;
        case 2:
            std::cout << "Viewing Stats...\n";
            std::cout << "Your Score: " << SnakeGame::getScore() << std::endl;  // static 함수로 점수 출력
            std::cout << "Game Time: " << SnakeGame::getGameTime() << " seconds" << std::endl;  // 게임 시간 출력
            break;
        case 3:
            std::cout << "Exiting game. Goodbye " << userName << "!\n";
            isRunning = false;
            break;
        default:
            std::cout << "Invalid choice. Please choose between 1-3.\n";
            break;
        }
    }

    return 0;
}
