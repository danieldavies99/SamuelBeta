#include "plugin.hpp"

struct Samuel : Module {
	enum ParamId {
		PARAMS_LEN
	};
	enum InputId {
		INPUT_CLOCK_IN_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		OUTPUT_GATE_OUT_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

	Samuel() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configInput(INPUT_CLOCK_IN_INPUT, "");
		configOutput(OUTPUT_GATE_OUT_OUTPUT, "");
	}

	IgnoreClockAfterResetTimer ignoreClockAfterResetTimer;
	float lastclockVoltage = 0.0f;

	std::string message = "Daniel Davies Samuel";
	std::string lastMessage;

	int step = 0;

	SequenceGenerator sequenceGenerator;

	void process(const ProcessArgs& args) override {
		if (message != lastMessage) {
			sequenceGenerator.generateSequence(message);
		}
		ignoreClockAfterResetTimer.process(1.0 / args.sampleRate);
		const float clockInput = inputs[INPUT_CLOCK_IN_INPUT].getVoltage();
		if(step > sequenceGenerator.sequence.size()) {
			step = 0;
		}
		if (lastclockVoltage == 0 && clockInput != 0 && !ignoreClockAfterResetTimer.shouldIgnore) { // clock detected
			// DEBUG(sequence[step] ? "yes" : "no");
			// DEBUG(std::to_string(step).c_str());
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

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(9.511, 10.061)), module, Samuel::INPUT_CLOCK_IN_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(40.6, 10.061)), module, Samuel::OUTPUT_GATE_OUT_OUTPUT));

		if(module) {
			LetterDisplay* display = createWidget<LetterDisplay>(mm2px(Vec(10.0, 20.0)));
			display->initialize();
			display->message = &module->message;
			addChild(display);
		}

	}
};


Model* modelSamuel = createModel<Samuel, SamuelWidget>("Samuel");