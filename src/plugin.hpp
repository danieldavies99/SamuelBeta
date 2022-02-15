#pragma once
#include <rack.hpp>


using namespace rack;

// Declare the Plugin, defined in plugin.cpp
extern Plugin* pluginInstance;

// Declare each Model, defined in each module source file
extern Model* modelSamuel;

// Timing Utils
struct IgnoreClockAfterResetTimer {
	dsp::Timer timer;
	bool shouldIgnore = false;

	void resetTriggered();
	void process(float deltaTime);
};

// struct OledPixelDisplay : LedDisplay {};

struct OledPixel {
	int x;
	int y;
	bool isLit;
};

struct OledPixelDisplay : widget::Widget {
	std::vector<std::vector<OledPixel>> pixels;
	int numPixelsX = 83; // should be multiple of six minus one for letters 
	int numPixelsY = 47; // should be multiple of eight minus one for letters
	NVGcolor pixelColor =  nvgRGBA(233, 79, 61, 255);

	double pixelWidth = mm2px(0.98);
	double pixelWidthWithGaps = mm2px(1.0);
	double borderSize = mm2px(2.0);

	bool shouldShowCursor = true;
	int framesSinceLastCursorChange = 0;
	void initialize();
	void lightPixel(int x, int y, int offsetX, int offsetY);
	void lightAll();
	void darkenAll();
	void drawGrid(const DrawArgs& args);
	void drawLayer(const DrawArgs& args, int layer) override;
	virtual void process() {};
};

struct SteppedRedKnob : RoundKnob {
    SteppedRedKnob() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/RedKnob.svg")));
        snap = true;
    }
};

struct LetterDisplay : OledPixelDisplay {
	std::string* message;

	void onHoverKey(const event::HoverKey &e) override;

	void drawLetter(char letter, int x, int y);
	void drawCursor(int x, int y);
	void drawMessage();
	void process() override;
};

struct SequenceGenerator {
	std::vector<bool> sequence;
	void pushDot();
	void pushDash();
	void pushNewPartInLetter();
	void pushNewLetter();
	void pushNewWord();
	void generateSequence(std::string message);
};