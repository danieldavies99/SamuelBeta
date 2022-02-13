#include "plugin.hpp"

void SequenceGenerator::pushDot() {
    sequence.push_back(true);
};

void SequenceGenerator::pushDash() {
    sequence.push_back(true);
    sequence.push_back(true);
    sequence.push_back(true);
};

void SequenceGenerator::pushNewPartInLetter() {
    sequence.push_back(false);
};

void SequenceGenerator::pushNewLetter() {
    sequence.push_back(false);
    sequence.push_back(false);
    sequence.push_back(false);
};

void SequenceGenerator::pushNewWord() {
    sequence.push_back(false);
    sequence.push_back(false);
    sequence.push_back(false);
    sequence.push_back(false);
    sequence.push_back(false);
    sequence.push_back(false);
    sequence.push_back(false);
};

void SequenceGenerator::generateSequence(std::string message) {
    std::transform(message.begin(), message.end(),message.begin(), ::toupper);
    for(std::__cxx11::basic_string<char>::size_type i = 0; i < message.size(); i++) {
        switch(message[i]) {
            case 'A': // ._
                pushDot();
                pushNewPartInLetter();
                pushDash();
                break;
            case 'B': // _...
                pushDash();
                pushNewPartInLetter();
                pushDot();
                pushNewPartInLetter();
                pushDot();
                pushNewPartInLetter();
                pushDot();
                break;
            case 'C': // _._.
                pushDash();
                pushNewPartInLetter();
                pushDot();
                pushNewPartInLetter();
                pushDash();
                pushNewPartInLetter();
                pushDot();
                break;
            case 'D': // _..
                pushDash();
                pushNewPartInLetter();
                pushDot();
                pushNewPartInLetter();
                pushDot();
                break;
            case 'E': // .
                pushDot();
                break;
            case 'F': // .._.
                pushDot();
                pushNewPartInLetter();
                pushDot();
                pushNewPartInLetter();
                pushDash();
                pushNewPartInLetter();
                pushDot();
                break;
            case 'G': // __.
                pushDash();
                pushNewPartInLetter();
                pushDash();
                pushNewPartInLetter();
                pushDot();
                break;
            case 'H': // ....
                pushDot();
                pushNewPartInLetter();
                pushDot();
                pushNewPartInLetter();
                pushDot();
                pushNewPartInLetter();
                pushDot();
                break;
            case 'I': // ..
                pushDot();
                pushNewPartInLetter();
                pushDot();
                break;
            case 'J': // .___
                pushDot();
                pushNewPartInLetter();
                pushDash();
                pushNewPartInLetter();
                pushDash();
                pushNewPartInLetter();
                pushDash();
                break;
            case 'K': // _._
                pushDash();
                pushNewPartInLetter();
                pushDot();
                pushNewPartInLetter();
                pushDash();
                break;
            case 'L': // ._..
                pushDot();
                pushNewPartInLetter();
                pushDash();
                pushNewPartInLetter();
                pushDot();
                pushNewPartInLetter();
                pushDot();
                break;
            case 'M': // __
                pushDash();
                pushNewPartInLetter();
                pushDash();
                break;
            case 'N': // _.
                pushDash();
                pushNewPartInLetter();
                pushDot();
                break;
            case 'O': // ___
                pushDash();
                pushNewPartInLetter();
                pushDash();
                pushNewPartInLetter();
                pushDash();
                break;
            case 'P': // .__.
                pushDot();
                pushNewPartInLetter();
                pushDash();
                pushNewPartInLetter();
                pushDash();
                pushNewPartInLetter();
                pushDot();
                break;
            case 'Q': // __._
                pushDash();
                pushNewPartInLetter();
                pushDash();
                pushNewPartInLetter();
                pushDot();
                pushNewPartInLetter();
                pushDash();
                break;
            case 'R': // ._.
                pushDot();
                pushNewPartInLetter();
                pushDash();
                pushNewPartInLetter();
                pushDot();
                break;
            case 'S': // ...
                pushDot();
                pushNewPartInLetter();
                pushDot();
                pushNewPartInLetter();
                pushDot();
                break;
            case 'T': // _
                pushDash();
                break;
            case 'U': // .._
                pushDot();
                pushDot();
                pushDash();
                break;
            case 'V': // ..._
                pushDot();
                pushDot();
                pushDot();
                pushDash();
                break;
            case 'W': // .__
                pushDot();
                pushDash();
                pushDash();
                break;
            case 'X': // _.._
                pushDash();
                pushDot();
                pushDot();
                pushDash();
                break;
            case 'Y': // _.__
                pushDash();
                pushDot();
                pushDash();
                pushDash();
                break;
            case 'Z': // __..
                pushDash();
                pushDash();
                pushDot();
                pushDot();
                break;
        }
        if(message[i] != ' ') {
            pushNewLetter();
        } else {
            pushNewWord();
        }
    }
};