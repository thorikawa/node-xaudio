var XAudio = require('../');

describe('xaudio', function() {
	it('play music', function(done) {
		this.timeout(20000);
		const xAudio = new XAudio.XAudio(44100);
		xAudio.play(__dirname + '/../assets/spotting.wav');
		setTimeout(() => {
			console.log('before stop');
			xAudio.stop();
			console.log('after stop');
		}, 10000);
		setTimeout(() => {
			console.log('done');
			done();
		}, 13000);
	})
});
