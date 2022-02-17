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

struct SteppedRedKnob : RoundKnob {
    SteppedRedKnob() {
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/RedKnob.svg")));
        snap = true;
    }
};

struct OledPixelDisplay : widget::Widget {
	std::vector<std::vector<OledPixel>> pixels;
	int numPixelsX = 0;
	int numPixelsY = 0;
	NVGcolor pixelColor =  nvgRGBA(233, 79, 61, 255);

	double pixelWidth = mm2px(1.03);
	double pixelWidthWithGaps = mm2px(1.04);
	double borderSize = mm2px(2.0);

	void initialize();
	void lightPixel(int x, int y, int offsetX, int offsetY);
	void lightAll();
	void darkenAll();
	void drawGrid(const DrawArgs& args);
	void drawLayer(const DrawArgs& args, int layer) override;
	virtual void process() {};
};

struct CharacterDisplay : OledPixelDisplay {
	void drawLetter(char letter, int x, int y);
	// void process() override;
};

struct TextInputDisplay : CharacterDisplay {
	TextInputDisplay() {
		numPixelsX = 83; // should be multiple of six minus one for letters 
		numPixelsY = 47; // should be multiple of eight minus one for letters
	}
	bool isSelected = false;
	bool shouldShowCursor = true;
	int framesSinceLastCursorChange = 0;
	std::string* message;

	void onButton(const ButtonEvent& e) override;
	void onSelectKey(const SelectKeyEvent& e) override;
	void onSelect(const SelectEvent& e) override;
	void onDeselect(const DeselectEvent& e) override;

	void drawCursor(int x, int y);
	void drawMessage();
	void process() override;
};
 struct LengthValuesDisplay : CharacterDisplay {
	char* val0;
	char* val1;
	char* val2;
	char* val3;

	LengthValuesDisplay() {
		numPixelsX = 71 - 14; // should be multiple of six minus one for letters 
		numPixelsY = 7; // should be multiple of eight minus one for letters
	}
	void process() override;
};

struct SequenceGenerator {
	int dotLength = 1;
	int dashLength = 3;
	int newLetterLength = 3;
	int newWordLength = 7;
	std::vector<bool> sequence;
	void pushDot();
	void pushDash();
	void pushNewPartInLetter();
	void pushNewLetter();
	void pushNewWord();
	void generateSequence(std::string message);
};