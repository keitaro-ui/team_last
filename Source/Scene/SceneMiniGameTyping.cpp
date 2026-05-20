#include "SceneMiniGameTyping.h"
#include <Windows.h>
#include <iostream>

void TypingChecker::Initialize(
    const std::string& text)
{
    targetText = text;

    inputText = "";

    currentIndex = 0;

    for (int i = 0; i < 256; i++)
    {
        oldKeys[i] = false;
        newKeys[i] = false;
    }
}

void TypingChecker::Update()
{
    for (int i = 0; i < 256; i++)
    {
        oldKeys[i] = newKeys[i];

        newKeys[i] =
            GetAsyncKeyState(i) & 0x8000;
    }

    // A-Z
    for (int key = 'A'; key <= 'Z'; key++)
    {
        if (newKeys[key] && !oldKeys[key])
        {
            char input =
                (char)tolower(key);

            char expected =
                tolower(targetText[currentIndex]);

            if (input == expected)
            {
                inputText += input;

                currentIndex++;
            }
        }
    }

    // スペース
    if (newKeys[VK_SPACE] &&
        !oldKeys[VK_SPACE])
    {
        if (targetText[currentIndex] == ' ')
        {
            inputText += ' ';

            currentIndex++;
        }
    }
}

void TypingChecker::Render()
{
    std::cout << "Target:\n";
    std::cout << targetText << "\n\n";

    std::cout << "Input:\n";
    std::cout << inputText << "\n";
}

bool TypingChecker::IsComplete() const
{
    return currentIndex >= targetText.length();
}