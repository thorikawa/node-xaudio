var XAudio = require('../');

describe('xaudio', function() {
	it('play and stop', function(done) {
		this.timeout(10000);
		const xAudio = new XAudio.XAudio(44100);
		xAudio.play(__dirname + '/../assets/mouth.wav');
		setTimeout(() => {
			console.log('stop');
			xAudio.stop();
		}, 2000);
		setTimeout(() => {
			console.log('done');
			done();
		}, 3000);
	});
	it('done callback', function(done) {
		this.timeout(10000);
		const xAudio = new XAudio.XAudio(44100);
		xAudio.play(__dirname + '/../assets/spotting.wav', function(err) {
			console.log('done callback');
			done();
		});
	});
});
