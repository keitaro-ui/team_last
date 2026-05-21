#pragma once
#include <string>

class TypingChecker
{
private:
    std::string targetText;

    std::string inputText;

    int currentIndex;

    bool oldKeys[256];
    bool newKeys[256];

public:
    void Initialize(const std::string& text);

    void Update();

    void Render();

    bool IsComplete() const;
};