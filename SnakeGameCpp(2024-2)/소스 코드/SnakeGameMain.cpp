#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>  // system("CLS")�� ����ϱ� ���� ����
#include "SnakeGame.h"

// ���� �ΰ� ��� �Լ�
void printLogo() {
    std::cout << "**********************************\n";
    std::cout << "*           SNAKEGAME            *\n";
    std::cout << "*        Developer: J.H.W        *\n";
    std::cout << "**********************************\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));  // 2�� ���� �ΰ� ���
}

// ����� �̸� �Է� �Լ�
std::string getUserName() {
    std::string userName;
    std::cout << "Enter your username: ";
    std::cin >> userName;
    return userName;
}

// ���� �޴� ��� �Լ�
void printMenu() {
    std::cout << "\nGame Menu\n";
    std::cout << "1. Start Game\n";
    std::cout << "2. View Stats (Game time & Score)\n";
    std::cout << "3. Exit\n";
    std::cout << "Select an option (1-3): ";
}

// ���� ���� �Լ� (�⺻ Ʋ)
void startGame(const std::string& userName) {
    // ���� ���� ���� ȭ���� ����ϴ�.
    system("CLS");  // �����쿡�� ȭ���� ����� ���

    // ���� �ΰ� ���
    std::cout << "Game is starting...\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));  // ��� ���

    SnakeGame game;
    game.start();

    // ���� ���� �� ���� ���
    Point2 pt(0, 0);
    pt.printStr("Your Score: " + std::to_string(game.getScore()), mglib::GREEN);  // ��� ����
    pt.printStr("Game Time: " + std::to_string(SnakeGame::getGameTime()) + " seconds", mglib::GREEN);  // ��� ����
}


int main() {
    // ���� �ΰ� ���
    printLogo();

    // ����� �̸� �Է� �ޱ�
    std::string userName = getUserName();

    // �޴� ��� �� ����� ���� �ޱ�
    int choice;
    bool isRunning = true;
    while (isRunning) {
        printMenu();
        std::cin >> choice;

        switch (choice) {
        case 1:
            // ���� ����
            startGame(userName);
            break;
        case 2:
            std::cout << "Viewing Stats...\n";
            std::cout << "Your Score: " << SnakeGame::getScore() << std::endl;  // static �Լ��� ���� ���
            std::cout << "Game Time: " << SnakeGame::getGameTime() << " seconds" << std::endl;  // ���� �ð� ���
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
