#include "plugin.hpp"

void OledPixelDisplay::initialize() {
    for(int i = 0; i < numPixelsX; i++) {
        std::vector<OledPixel> newPixelRow;
        for(int j = 0; j < numPixelsY; j++) {
            OledPixel newPixel;
            newPixel.x = i;
            newPixel.y = j;
            newPixel.isLit = false;
            // pixels[i][j] = newPixel;
            newPixelRow.push_back(newPixel);
        }
        pixels.push_back(newPixelRow);
    }
    // lightAll();
}

void OledPixelDisplay::lightPixel(int x, int y, int offsetX, int offsetY) {
    if(x + offsetX < numPixelsX && y + offsetY < numPixelsY) {
        pixels[x + offsetX][y + offsetY].isLit = true;
    }
}

void OledPixelDisplay::drawLayer(const DrawArgs& args, int layer) {
    if (layer != 1)
        return;

    process();

    nvgBeginPath(args.vg);
    nvgRoundedRect(args.vg, 0, 0, pixelWidthWithGaps * numPixelsX + (borderSize*2), pixelWidthWithGaps * numPixelsY + (borderSize*2), 1);
    nvgFillColor(args.vg, nvgRGBA(0, 0, 0, 255));
    nvgFill(args.vg);

    drawGrid(args);
}

void OledPixelDisplay::lightAll() {
    for(int i = 0; i < numPixelsX; i++) {
        for(int j = 0; j < numPixelsY; j++) {
            pixels[i][j].isLit = true;
        }
    }
}

void OledPixelDisplay::darkenAll() {
    for(int i = 0; i < numPixelsX; i++) {
        for(int j = 0; j < numPixelsY; j++) {
            pixels[i][j].isLit = false;
        }
    }
}

void OledPixelDisplay::drawGrid(const DrawArgs& args) {
    nvgTranslate(args.vg, borderSize, borderSize);
    if(pixels[0].size() < numPixelsY || pixels.size() < numPixelsX) {
        // DEBUG("not enough pixels detected");
        // DEBUG(std::to_string(pixels[0].size()).c_str());
        // DEBUG(std::to_string(pixels.size()).c_str());
        return;
    }
    for(int i = 0; i < numPixelsX; i++) {
        for(int j = 0; j < numPixelsY; j++) {
            if(pixels[i][j].isLit == true) {
                nvgBeginPath(args.vg);
                nvgRect(
                    args.vg,
                    pixelWidthWithGaps*i,
                    pixelWidthWithGaps*j,
                    pixelWidth,
                    pixelWidth
                );
                nvgFillColor(
                    args.vg,
                    pixelColor
                );
                nvgFill(args.vg);
            }
        }
    }
}


void LetterDisplay::process() {
    drawMessage();
}

void LetterDisplay::drawMessage() {
    darkenAll();
    if(message->size() < 1) {
        drawCursor(0,0);
        return;
    }
    std::transform(message->begin(), message->end(),message->begin(), ::toupper);
    for(int i = 0; i < message->size(); i++) {
        int lineLength = (numPixelsX + 1)/6;
        int lineNum = floor(i / lineLength);
        drawLetter((*message)[i], (i - lineNum*lineLength)*6, lineNum * 8);

        int cursorLineNum = floor( (i + 1) / lineLength);
        // DEBUG(std::to_string(cursorLineNum).c_str());
        if(i == message->size() - 1) {
            drawCursor((i + 1 - cursorLineNum*lineLength)*6, cursorLineNum * 8);
        }
    }
}

void LetterDisplay::onHoverKey(const event::HoverKey &e)
{   
    if(e.action == GLFW_PRESS)
	{
		// do stuff
        if(e.key > 48 && e.key < 59 )  { // 0 - 9
            message->append(e.keyName);
        }
        if(e.key > 64 && e.key < 91 )  { // A - Z
            message->append(e.keyName);
        }
        
        if(e.key == GLFW_KEY_SPACE) {
            message->append(" ");
        }
        if(e.key == GLFW_KEY_BACKSPACE) { // backspace
            DEBUG("backspace hit");
            message->pop_back();
        }
	}
}


void LetterDisplay::drawCursor(int x, int y) {
    if(shouldShowCursor) {
        lightPixel(0, 0, x, y);
        lightPixel(0, 1, x, y);
        lightPixel(0, 2, x, y);
        lightPixel(0, 3, x, y);
        lightPixel(0, 4, x, y);
        lightPixel(0, 5, x, y);
        lightPixel(0, 6, x, y);
    }
    if(framesSinceLastCursorChange > 100) {
        shouldShowCursor = !shouldShowCursor;
        framesSinceLastCursorChange = 0;
    }
    framesSinceLastCursorChange++;
}

void LetterDisplay::drawLetter(char letter, int x, int y) {
    // DEBUG("letter " + letter);
    switch (letter) {
        case 'A':
            lightPixel(0, 2, x, y);
            lightPixel(0, 3, x, y);
            lightPixel(0, 4, x, y);
            lightPixel(0, 5, x, y);
            lightPixel(0, 6, x, y);

            lightPixel(1, 1, x, y);
            lightPixel(1, 3, x, y);

            lightPixel(2, 0, x, y);
            lightPixel(2, 3, x, y);

            lightPixel(3, 1, x, y);
            lightPixel(3, 3, x, y);

            lightPixel(4, 2, x, y);
            lightPixel(4, 3, x, y);
            lightPixel(4, 4, x, y);
            lightPixel(4, 5, x, y);
            lightPixel(4, 6, x, y);
            break;
        case 'B':
            lightPixel(0, 0, x, y);
            lightPixel(0, 1, x, y);
            lightPixel(0, 2, x, y);
            lightPixel(0, 3, x, y);
            lightPixel(0, 4, x, y);
            lightPixel(0, 5, x, y);
            lightPixel(0, 6, x, y);

            lightPixel(1, 0, x, y);
            lightPixel(1, 3, x, y);
            lightPixel(1, 6, x, y);

            lightPixel(2, 0, x, y);
            lightPixel(2, 3, x, y);
            lightPixel(2, 6, x, y);

            lightPixel(3, 0, x, y);
            lightPixel(3, 3, x, y);
            lightPixel(3, 6, x, y);

            lightPixel(4, 1, x, y);
            lightPixel(4, 2, x, y);
            lightPixel(4, 4, x, y);
            lightPixel(4, 5, x, y);
            break;
        case 'C':
            lightPixel(0, 1, x, y);
            lightPixel(0, 2, x, y);
            lightPixel(0, 3, x, y);
            lightPixel(0, 4, x, y);
            lightPixel(0, 5, x, y);


            lightPixel(1, 0, x, y);
            lightPixel(1, 6, x, y);

            lightPixel(2, 0, x, y);
            lightPixel(2, 6, x, y);

            lightPixel(3, 0, x, y);
            lightPixel(3, 6, x, y);

            lightPixel(4, 1, x, y);
            lightPixel(4, 5, x, y);
            break;
        case 'D':
            lightPixel(0, 0, x, y);
            lightPixel(0, 1, x, y);
            lightPixel(0, 2, x, y);
            lightPixel(0, 3, x, y);
            lightPixel(0, 4, x, y);
            lightPixel(0, 5, x, y);
            lightPixel(0, 6, x, y);

            lightPixel(1, 0, x, y);
            lightPixel(1, 6, x, y);

            lightPixel(2, 0, x, y);
            lightPixel(2, 6, x, y);

            lightPixel(3, 0, x, y);
            lightPixel(3, 6, x, y);

            lightPixel(4, 1, x, y);
            lightPixel(4, 2, x, y);
            lightPixel(4, 3, x, y);
            lightPixel(4, 4, x, y);
            lightPixel(4, 5, x, y);
            break;
        case 'E':
            lightPixel(0, 0, x, y);
            lightPixel(0, 1, x, y);
            lightPixel(0, 2, x, y);
            lightPixel(0, 3, x, y);
            lightPixel(0, 4, x, y);
            lightPixel(0, 5, x, y);
            lightPixel(0, 6, x, y);

            lightPixel(1, 0, x, y);
            lightPixel(1, 3, x, y);
            lightPixel(1, 6, x, y);

            lightPixel(2, 0, x, y);
            lightPixel(2, 3, x, y);
            lightPixel(2, 6, x, y);

            lightPixel(3, 0, x, y);
            lightPixel(3, 3, x, y);
            lightPixel(3, 6, x, y);

            lightPixel(4, 0, x, y);
            lightPixel(4, 6, x, y);
            break;
        case 'F':
            lightPixel(0, 0, x, y);
            lightPixel(0, 1, x, y);
            lightPixel(0, 2, x, y);
            lightPixel(0, 3, x, y);
            lightPixel(0, 4, x, y);
            lightPixel(0, 5, x, y);
            lightPixel(0, 6, x, y);

            lightPixel(1, 0, x, y);
            lightPixel(1, 3, x, y);

            lightPixel(2, 0, x, y);
            lightPixel(2, 3, x, y);

            lightPixel(3, 0, x, y);
            lightPixel(3, 3, x, y);

            lightPixel(4, 0, x, y);
            break;
        case 'G':
            lightPixel(0, 1, x, y);
            lightPixel(0, 2, x, y);
            lightPixel(0, 3, x, y);
            lightPixel(0, 4, x, y);
            lightPixel(0, 5, x, y);

            lightPixel(1, 0, x, y);
            lightPixel(1, 6, x, y);

            lightPixel(2, 0, x, y);
            lightPixel(2, 4, x, y);
            lightPixel(2, 6, x, y);

            lightPixel(3, 0, x, y);
            lightPixel(3, 4, x, y);
            lightPixel(3, 6, x, y);

            lightPixel(4, 1, x, y);
            lightPixel(4, 4, x, y);
            lightPixel(4, 5, x, y);
            break;
        case 'H':
            lightPixel(0, 0, x, y);
            lightPixel(0, 1, x, y);
            lightPixel(0, 2, x, y);
            lightPixel(0, 3, x, y);
            lightPixel(0, 4, x, y);
            lightPixel(0, 5, x, y);
            lightPixel(0, 6, x, y);

            lightPixel(1, 3, x, y);
            lightPixel(2, 3, x, y);
            lightPixel(3, 3, x, y);

            lightPixel(4, 0, x, y);
            lightPixel(4, 1, x, y);
            lightPixel(4, 2, x, y);
            lightPixel(4, 3, x, y);
            lightPixel(4, 4, x, y);
            lightPixel(4, 5, x, y);
            lightPixel(4, 6, x, y);
            break;
        case 'I':
            lightPixel(1, 0, x, y);
            lightPixel(1, 6, x, y);

            lightPixel(2, 0, x, y);
            lightPixel(2, 1, x, y);
            lightPixel(2, 2, x, y);
            lightPixel(2, 3, x, y);
            lightPixel(2, 4, x, y);
            lightPixel(2, 5, x, y);
            lightPixel(2, 6, x, y);

            lightPixel(3, 0, x, y);
            lightPixel(3, 6, x, y);
            break;
        case 'J':
            lightPixel(0, 4, x, y);
            lightPixel(0, 5, x, y);

            lightPixel(1, 6, x, y);

            lightPixel(2, 6, x, y);

            lightPixel(3, 6, x, y);

            lightPixel(4, 0, x, y);
            lightPixel(4, 1, x, y);
            lightPixel(4, 2, x, y);
            lightPixel(4, 3, x, y);
            lightPixel(4, 4, x, y);
            lightPixel(4, 5, x, y);
            break;
        case 'K':
            lightPixel(0, 0, x, y);
            lightPixel(0, 1, x, y);
            lightPixel(0, 2, x, y);
            lightPixel(0, 3, x, y);
            lightPixel(0, 4, x, y);
            lightPixel(0, 5, x, y);
            lightPixel(0, 6, x, y);

            lightPixel(1, 3, x, y);

            lightPixel(2, 2, x, y);
            lightPixel(2, 4, x, y);

            lightPixel(3, 1, x, y);
            lightPixel(3, 5, x, y);

            lightPixel(4, 0, x, y);
            lightPixel(4, 6, x, y);
            break;
        case 'L':
            lightPixel(0, 0, x, y);
            lightPixel(0, 1, x, y);
            lightPixel(0, 2, x, y);
            lightPixel(0, 3, x, y);
            lightPixel(0, 4, x, y);
            lightPixel(0, 5, x, y);
            lightPixel(0, 6, x, y);

            lightPixel(1, 6, x, y);
            lightPixel(2, 6, x, y);
            lightPixel(3, 6, x, y);
            lightPixel(4, 6, x, y);
            break;
        case 'M':
            lightPixel(0, 0, x, y);
            lightPixel(0, 1, x, y);
            lightPixel(0, 2, x, y);
            lightPixel(0, 3, x, y);
            lightPixel(0, 4, x, y);
            lightPixel(0, 5, x, y);
            lightPixel(0, 6, x, y);

            lightPixel(1, 1, x, y);
            lightPixel(2, 2, x, y);
            lightPixel(3, 1, x, y);

            lightPixel(4, 0, x, y);
            lightPixel(4, 1, x, y);
            lightPixel(4, 2, x, y);
            lightPixel(4, 3, x, y);
            lightPixel(4, 4, x, y);
            lightPixel(4, 5, x, y);
            lightPixel(4, 6, x, y);
            break;
        case 'N':
            lightPixel(0, 0, x, y);
            lightPixel(0, 1, x, y);
            lightPixel(0, 2, x, y);
            lightPixel(0, 3, x, y);
            lightPixel(0, 4, x, y);
            lightPixel(0, 5, x, y);
            lightPixel(0, 6, x, y);

            lightPixel(1, 2, x, y);

            lightPixel(2, 3, x, y);

            lightPixel(3, 4, x, y);

            lightPixel(4, 0, x, y);
            lightPixel(4, 1, x, y);
            lightPixel(4, 2, x, y);
            lightPixel(4, 3, x, y);
            lightPixel(4, 4, x, y);
            lightPixel(4, 5, x, y);
            lightPixel(4, 6, x, y);
            break;
        case 'O':
            lightPixel(0, 1, x, y);
            lightPixel(0, 2, x, y);
            lightPixel(0, 3, x, y);
            lightPixel(0, 4, x, y);
            lightPixel(0, 5, x, y);

            lightPixel(1, 0, x, y);
            lightPixel(1, 6, x, y);

            lightPixel(2, 0, x, y);
            lightPixel(2, 6, x, y);

            lightPixel(3, 0, x, y);
            lightPixel(3, 6, x, y);


            lightPixel(4, 1, x, y);
            lightPixel(4, 2, x, y);
            lightPixel(4, 3, x, y);
            lightPixel(4, 4, x, y);
            lightPixel(4, 5, x, y);
            break;
        case 'P':
            lightPixel(0, 0, x, y);
            lightPixel(0, 1, x, y);
            lightPixel(0, 2, x, y);
            lightPixel(0, 3, x, y);
            lightPixel(0, 4, x, y);
            lightPixel(0, 5, x, y);
            lightPixel(0, 6, x, y);

            lightPixel(1, 0, x, y);
            lightPixel(1, 3, x, y);

            lightPixel(2, 0, x, y);
            lightPixel(2, 3, x, y);

            lightPixel(3, 0, x, y);
            lightPixel(3, 3, x, y);

            lightPixel(4, 1, x, y);
            lightPixel(4, 2, x, y);
            break;
        case 'Q':
            lightPixel(0, 1, x, y);
            lightPixel(0, 2, x, y);
            lightPixel(0, 3, x, y);
            lightPixel(0, 4, x, y);
            lightPixel(0, 5, x, y);

            lightPixel(1, 0, x, y);
            lightPixel(1, 6, x, y);

            lightPixel(2, 0, x, y);
            lightPixel(2, 4, x, y);
            lightPixel(2, 6, x, y);

            lightPixel(3, 0, x, y);
            lightPixel(3, 5, x, y);

            lightPixel(4, 1, x, y);
            lightPixel(4, 2, x, y);
            lightPixel(4, 3, x, y);
            lightPixel(4, 4, x, y);
            lightPixel(4, 6, x, y);
            break;
        case 'R':
            lightPixel(0, 0, x, y);
            lightPixel(0, 1, x, y);
            lightPixel(0, 2, x, y);
            lightPixel(0, 3, x, y);
            lightPixel(0, 4, x, y);
            lightPixel(0, 5, x, y);
            lightPixel(0, 6, x, y);

            lightPixel(1, 0, x, y);
            lightPixel(1, 3, x, y);

            lightPixel(2, 0, x, y);
            lightPixel(2, 3, x, y);

            lightPixel(3, 0, x, y);
            lightPixel(3, 3, x, y);

            lightPixel(4, 1, x, y);
            lightPixel(4, 2, x, y);
            lightPixel(4, 4, x, y);
            lightPixel(4, 5, x, y);
            lightPixel(4, 6, x, y);
            break;
        case 'S':
            lightPixel(0, 1, x, y);
            lightPixel(0, 2, x, y);
            lightPixel(0, 5, x, y);

            lightPixel(1, 0, x, y);
            lightPixel(1, 3, x, y);
            lightPixel(1, 6, x, y);

            lightPixel(2, 0, x, y);
            lightPixel(2, 3, x, y);
            lightPixel(2, 6, x, y);

            lightPixel(3, 0, x, y);
            lightPixel(3, 3, x, y);
            lightPixel(3, 6, x, y);

            lightPixel(4, 1, x, y);
            lightPixel(4, 4, x, y);
            lightPixel(4, 5, x, y);
            break;
        case 'T':
            lightPixel(0, 0, x, y);

            lightPixel(1, 0, x, y);
            
            lightPixel(2, 0, x, y);
            lightPixel(2, 1, x, y);
            lightPixel(2, 2, x, y);
            lightPixel(2, 3, x, y);
            lightPixel(2, 4, x, y);
            lightPixel(2, 5, x, y);
            lightPixel(2, 6, x, y);

            lightPixel(3, 0, x, y);

            lightPixel(4, 0, x, y);
            break;
        case 'U':
            lightPixel(0, 0, x, y);
            lightPixel(0, 1, x, y);
            lightPixel(0, 2, x, y);
            lightPixel(0, 3, x, y);
            lightPixel(0, 4, x, y);
            lightPixel(0, 5, x, y);

            lightPixel(1, 6, x, y);

            lightPixel(2, 6, x, y);

            lightPixel(3, 6, x, y);

            lightPixel(4, 0, x, y);
            lightPixel(4, 1, x, y);
            lightPixel(4, 2, x, y);
            lightPixel(4, 3, x, y);
            lightPixel(4, 4, x, y);
            lightPixel(4, 5, x, y);
            break;
        case 'V':
            lightPixel(0, 0, x, y);
            lightPixel(0, 1, x, y);
            lightPixel(0, 2, x, y);
            lightPixel(0, 3, x, y);
            lightPixel(0, 4, x, y);

            lightPixel(1, 5, x, y);

            lightPixel(2, 6, x, y);

            lightPixel(3, 5, x, y);

            lightPixel(4, 0, x, y);
            lightPixel(4, 1, x, y);
            lightPixel(4, 2, x, y);
            lightPixel(4, 3, x, y);
            lightPixel(4, 4, x, y);
            break;
        case 'W':
            lightPixel(0, 0, x, y);
            lightPixel(0, 1, x, y);
            lightPixel(0, 2, x, y);
            lightPixel(0, 3, x, y);
            lightPixel(0, 4, x, y);
            lightPixel(0, 5, x, y);
            lightPixel(0, 6, x, y);

            lightPixel(1, 5, x, y);

            lightPixel(2, 4, x, y);

            lightPixel(3, 5, x, y);

            lightPixel(4, 0, x, y);
            lightPixel(4, 1, x, y);
            lightPixel(4, 2, x, y);
            lightPixel(4, 3, x, y);
            lightPixel(4, 4, x, y);
            lightPixel(4, 5, x, y);
            lightPixel(4, 6, x, y);
            break;
        case 'X':
            lightPixel(0, 0, x, y);
            lightPixel(0, 1, x, y);
            lightPixel(0, 5, x, y);
            lightPixel(0, 6, x, y);

            lightPixel(1, 2, x, y);
            lightPixel(1, 4, x, y);

            lightPixel(2, 3, x, y);

            lightPixel(3, 2, x, y);
            lightPixel(3, 4, x, y);

            lightPixel(4, 0, x, y);
            lightPixel(4, 1, x, y);
            lightPixel(4, 5, x, y);
            lightPixel(4, 6, x, y);
            break;
        case 'Y':
            lightPixel(0, 0, x, y);
            lightPixel(0, 1, x, y);
            lightPixel(0, 2, x, y);

            lightPixel(1, 3, x, y);

            lightPixel(2, 4, x, y);
            lightPixel(2, 5, x, y);
            lightPixel(2, 6, x, y);


            lightPixel(3, 3, x, y);

            lightPixel(4, 0, x, y);
            lightPixel(4, 1, x, y);
            lightPixel(4, 2, x, y);
            break;
        case 'Z':
            lightPixel(0, 0, x, y);
            lightPixel(0, 5, x, y);
            lightPixel(0, 6, x, y);

            lightPixel(1, 0, x, y);
            lightPixel(1, 4, x, y);
            lightPixel(1, 6, x, y);

            lightPixel(2, 0, x, y);
            lightPixel(2, 3, x, y);
            lightPixel(2, 6, x, y);

            lightPixel(3, 0, x, y);
            lightPixel(3, 2, x, y);
            lightPixel(3, 6, x, y);

            lightPixel(4, 0, x, y);
            lightPixel(4, 1, x, y);
            lightPixel(4, 6, x, y);
            break;
    }
}