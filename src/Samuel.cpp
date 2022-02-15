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
		configParam(KNOB_DOT_LENGTH_PARAM, 0.f, 1.f, 0.f, "");
		configParam(KNOB_DASH_LENGTH_PARAM, 0.f, 1.f, 0.f, "");
		configParam(KNOB_NEW_LETTER_LENGTH_PARAM, 0.f, 1.f, 0.f, "");
		configParam(KNOB_NEW_WORD_LENGTH_PARAM, 0.f, 1.f, 0.f, "");

		configInput(INPUT_CLOCK_IN_INPUT, "");
		configOutput(OUTPUT_GATE_OUT_OUTPUT, "");
		configOutput(OUT_END_OUTPUT, "");
		// message.reserve(100);
	}

	IgnoreClockAfterResetTimer ignoreClockAfterResetTimer;
	float lastclockVoltage = 0.0f;

	std::string message = "";
	std::string lastMessage;

	int step = 0;

	SequenceGenerator sequenceGenerator;

	void process(const ProcessArgs& args) override {
		if (message != lastMessage) {
			sequenceGenerator.generateSequence(message);
		}
		if (sequenceGenerator.sequence.size() < 1) {
			return;
		}
		ignoreClockAfterResetTimer.process(1.0 / args.sampleRate);
		const float clockInput = inputs[INPUT_CLOCK_IN_INPUT].getVoltage();
		if(step > sequenceGenerator.sequence.size()) {
			step = 0;
		}
		if (lastclockVoltage == 0 && clockInput != 0 && !ignoreClockAfterResetTimer.shouldIgnore) { // clock detected
			outputs[OUTPUT_GATE_OUT_OUTPUT].setVoltage(sequenceGenerator.sequence[step] ? 10.0f : 0);
			step++;
		}
		lastclockVoltage = clockInput;
		lastMessage = message;
	}
};


struct SamuelWidget : ModuleWidget {
	SamuelWidget(Samuel* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/Samuel.svg")));

		addParam(createParamCentered<SteppedRedKnob>(mm2px(Vec(9.893, 93.806)), module, Samuel::KNOB_DOT_LENGTH_PARAM));
		addParam(createParamCentered<SteppedRedKnob>(mm2px(Vec(25.678, 93.806)), module, Samuel::KNOB_DASH_LENGTH_PARAM));
		addParam(createParamCentered<SteppedRedKnob>(mm2px(Vec(41.464, 93.806)), module, Samuel::KNOB_NEW_LETTER_LENGTH_PARAM));
		addParam(createParamCentered<SteppedRedKnob>(mm2px(Vec(57.249, 93.806)), module, Samuel::KNOB_NEW_WORD_LENGTH_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(57.249, 16.245)), module, Samuel::INPUT_CLOCK_IN_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(85.435, 88.853)), module, Samuel::OUTPUT_GATE_OUT_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(85.435, 99.276)), module, Samuel::OUT_END_OUTPUT));

		if(module) {
			LetterDisplay* display = createWidget<LetterDisplay>(mm2px(Vec(5.759, 25.0)));
			display->initialize();
			display->message = &module->message;
			addChild(display);
		}

	}
};


Model* modelSamuel = createModel<Samuel, SamuelWidget>("Samuel");