#include "plugin.hpp"

struct Samuel : Module {
	enum ParamId {
		KNOB_DOT_LENGTH_PARAM,
		KNOB_DASH_LENGTH_PARAM,
		KNOB_NEW_LETTER_LENGTH_PARAM,
		KNOB_NEW_WORD_LENGTH_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		INPUT_CLOCK_IN_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		OUTPUT_GATE_OUT_OUTPUT,
		OUT_END_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

	Samuel() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(KNOB_DOT_LENGTH_PARAM, 0.f, 9.f, 1.f, "");
		configParam(KNOB_DASH_LENGTH_PARAM, 0.f, 9.f, 3.f, "");
		configParam(KNOB_NEW_LETTER_LENGTH_PARAM, 0.f, 9.f, 3.f, "");
		configParam(KNOB_NEW_WORD_LENGTH_PARAM, 0.f, 9.f, 7.f, "");

		configInput(INPUT_CLOCK_IN_INPUT, "");
		configOutput(OUTPUT_GATE_OUT_OUTPUT, "");
		configOutput(OUT_END_OUTPUT, "");
	}

	IgnoreClockAfterResetTimer ignoreClockAfterResetTimer;
	float lastclockVoltage = 0.0f;

	std::string message = "";
	std::string lastMessage;

	char lengthDot = '0';
	char lengthDash = '0';
	char lengthNewLetter = '0';
	char lengthNewWord = '0';
	int step = 0;

	dsp::PulseGenerator endGatePulse;

	SequenceGenerator sequenceGenerator;

	void process(const ProcessArgs& args) override {
		sequenceGenerator.dotLength = (int)params[KNOB_DOT_LENGTH_PARAM].getValue();
		sequenceGenerator.dashLength = (int)params[KNOB_DASH_LENGTH_PARAM].getValue();
		sequenceGenerator.newLetterLength = (int)params[KNOB_NEW_LETTER_LENGTH_PARAM].getValue();
		sequenceGenerator.newWordLength = (int)params[KNOB_NEW_WORD_LENGTH_PARAM].getValue();

		lengthDot = '0' + sequenceGenerator.dotLength;
		lengthDash = '0' + sequenceGenerator.dashLength;
		lengthNewLetter = '0' + sequenceGenerator.newLetterLength;
		lengthNewWord = '0' + sequenceGenerator.newWordLength;

		sequenceGenerator.generateSequence(message); // can make this more efficient, calling per letter rather than the whole sequence each time
		if ((int)sequenceGenerator.sequence.size() < 1) {
			outputs[OUTPUT_GATE_OUT_OUTPUT].setVoltage(0);
			return;
		}
		ignoreClockAfterResetTimer.process(1.0 / args.sampleRate);
		const float clockInput = inputs[INPUT_CLOCK_IN_INPUT].getVoltage();
		if(step > (int)sequenceGenerator.sequence.size()) {
			step = 0;
		}
		if (lastclockVoltage == 0 && clockInput != 0 && !ignoreClockAfterResetTimer.shouldIgnore) { // clock detected
			if(step == 0) {
				endGatePulse.trigger(1e-3f);
			}
			outputs[OUTPUT_GATE_OUT_OUTPUT].setVoltage(sequenceGenerator.sequence[step] ? 10.0f : 0);
			step++;
		}

		const bool shouldPulseEnd = endGatePulse.process(1.0 / args.sampleRate);
		outputs[OUT_END_OUTPUT].setVoltage(shouldPulseEnd ? 10.0 : 0.0);		

		lastclockVoltage = clockInput;
		lastMessage = message;
	}
};


struct SamuelWidget : ModuleWidget {
	SamuelWidget(Samuel* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/Samuel.svg")));

		addParam(createParamCentered<SteppedRedKnob>(mm2px(Vec(9.956, 94.672)), module, Samuel::KNOB_DOT_LENGTH_PARAM));
		addParam(createParamCentered<SteppedRedKnob>(mm2px(Vec(27.942, 94.672)), module, Samuel::KNOB_DASH_LENGTH_PARAM));
		addParam(createParamCentered<SteppedRedKnob>(mm2px(Vec(45.929, 94.672)), module, Samuel::KNOB_NEW_LETTER_LENGTH_PARAM));
		addParam(createParamCentered<SteppedRedKnob>(mm2px(Vec(63.915, 94.672)), module, Samuel::KNOB_NEW_WORD_LENGTH_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(57.249, 16.245)), module, Samuel::INPUT_CLOCK_IN_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(88.935, 88.853)), module, Samuel::OUTPUT_GATE_OUT_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(88.935, 99.276)), module, Samuel::OUT_END_OUTPUT));

		if(module) {
			TextInputDisplay* textInputDisplay = createWidget<TextInputDisplay>(mm2px(Vec(5.759, 25.0)));
			textInputDisplay->initialize();
			textInputDisplay->message = &module->message;
			addChild(textInputDisplay);

			LengthValuesDisplay* lengthValuesDisplay = createWidget<LengthValuesDisplay>(mm2px(Vec(5.759, 105)));
			lengthValuesDisplay->initialize();
			lengthValuesDisplay->val0 = &module->lengthDot;
			lengthValuesDisplay->val1 = &module->lengthDash;
			lengthValuesDisplay->val2 = &module->lengthNewLetter;
			lengthValuesDisplay->val3 = &module->lengthNewWord;
			addChild(lengthValuesDisplay);
		}

	}
};

Model* modelSamuel = createModel<Samuel, SamuelWidget>("Samuel");