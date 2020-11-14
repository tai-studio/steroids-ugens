SSinOscFB : PureUGen {
	*ar {
		// arg freq=440.0, phase = 0, feedback=0.0, mul=1.0, add=0.0;
		arg freq=440.0, feedback=0.0, mul=1.0, add=0.0;
		^this.multiNew('audio', freq, feedback).madd(mul, add)
	}
	*kr {
		// arg freq=440.0, phase = 0, feedback=0.0, mul=1.0, add=0.0;
		arg freq=440.0, feedback=0.0, mul=1.0, add=0.0;
		^this.multiNew('control', freq, feedback).madd(mul, add)
	}
}
