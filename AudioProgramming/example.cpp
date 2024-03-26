#include<random>
#include <iostream>
#include <vector>
#include <numeric>
#include <sndfile.hh>
#include <math.h>


#include "gnuplot-iostream.h"

using namespace std;

void draw_gnuplot(vector<float> v0, string name) {
	Gnuplot gp;
	string title = "set title '" + name + "' \n";
	gp << title;
	gp << "plot '-' with lines title 'outputData'";

	gp.send(v0);

	cin.get();
}
/*
void time_shift(vector<float> audioData, int shiftSamples);
void inversion(vector<float> audioData);
void increaseSamplingFrequency(vector<float> audioData, int length);
void reduceSamplingFrequency(vector<float> audioData, float factor);
void writeAudioFile(const std::vector<float>& audioData, const char* filename, int sampleRate);
vector<float> readAudioFile(string fileName);
void addSignal(vector<float> a1, vector<float> a2);
void multiplySignal(vector<float> a1, vector<float> a2);
vector<float> convolution(vector<float> signal, vector<float> h);
void LPF(vector<float> signal, int N, float Wc);
void BPF(vector<float> signal, int N, float Wc1, float Wc2);
void HPF(vector<float> signal, int N, float Wc, float omega);
void BSF(vector<float> signal, int N, float Wc1, float Wc2, float omega);
void applyFade(vector<float> audioSignal, float fadeDurationInSeconds, bool fadeIn);
void applyEcho(vector<float> audioSignal, float delayInSeconds, float decayFactor);
void applyReverb(vector<float> audioSignal, float delayTime, float decayFactor);
void applyFlanger(vector<float> audioSignal, float rate, float depth, float delay, float feedback);
*/
class AudioFile {
private:
	vector<float> audioData;
	int sampleRate;

public:
	AudioFile(vector<float> data, int sampleRate) {
		this->audioData = data;
		this->sampleRate = sampleRate;
	}
	vector<float> getAudioData() {
		return this->audioData;
	}
	int getSampleRate() {
		return this->sampleRate;
	}
	void time_shift(int shiftSamples);
	void increaseSamplingFrequency(int length);
	void inversion();
	void reduceSamplingFrequency(float factor);
	void addSignal(vector<float> a2);
	void multiplySignal(vector<float> a2);
	void LPF(int N, float Wc);
	void BPF(int N, float Wc1, float Wc2);
	void HPF(int N, float Wc, float omega);
	void BSF( int N, float Wc1, float Wc2, float omega);
	void applyFade( float fadeDurationInSeconds, bool fadeIn);
	void applyEcho(float delayInSeconds, float decayFactor);
	void applyReverb(float delayTime, float decayFactor);
	void applyFlanger( float rate, float depth, float delay, float feedback);
};

int main() {

	AudioFile audioFile = readAudioFile("C:\\Users\\giang\\Music\\example.wav");
	

	draw_gnuplot(audioFile.getAudioData(), "file goc");
	
	while (true)
	{
		cout << "1. Time shift \n";
		cout << "2. Inversion \n";
		cout << "3. Increase \n";
		cout << "4. Reduction \n";
		cout << "5. AddSignal \n";
		cout << "6. MultiplySignal \n";
		cout << "7. Convolution \n";
		cout << "8. Filter \n";
		cout << "9. Fade \n";
		cout << "10. Echo \n";
		cout << "11. Reverb \n";
		cout << "12. Planger \n";
		cout << "14. Exit \n";

		int choice; cin >> choice;
		switch (choice)
		{
		case 1: {
			cout << "Nhập thời gian dời theo giây : ";
			int shiftAmount; cin >> shiftAmount;

			long long shiftSamples = shiftAmount * audioFile.getSampleRate();
			audioFile.time_shift(shiftAmount);
			break;
		}
		case 2: {
			cout << "Âm thanh sau khi lật ngược : ";
			audioFile.inversion();
			break;
		}
		case 3: {
			cout << "Nhập tỷ số giãn : ";
			int length; cin >> length;

			//increaseSamplingFrequency(audioData, length);
			audioFile.increaseSamplingFrequency(length);
			break;
		}
		case 4: {
			cout << "Nhập tỷ số nén : ";
			int length; cin >> length;

			//reduceSamplingFrequency(audioData, length);
			audioFile.reduceSamplingFrequency(length);
			break;
		}
		case 5: {
			AudioFile audioFile2 = readAudioFile("C:\\Users\\giang\\Music\\addSignal.wav");
			//addSignal(audioData, audioData2);
			audioFile.addSignal(audioFile2.getAudioData());
			break;
		}
		case 6: {
			//vector<float> audioData2 = readAudioFile("C:\\Users\\giang\\Music\\addSignal.wav");
			AudioFile audioFile2 = readAudioFile("C:\\Users\\giang\\Music\\addSignal.wav");
			//multiplySignal(audioData, audioData2);
			audioFile.multiplySignal(audioFile2.getAudioData());
			break;
		}
		case 7: {
			cout << "Nhập số lượng xung : \n";
			int n; cin >> n;
			cout << "Nhập dãy đáp ứng xung \n";
			vector<float> h(n);
			for (int i = 0; i < n; i++) {
				float x; cin >> x;
				h.push_back(x);
			}
			//vector<float> h = {0.5,0.4,0.1,0.4};
			vector<float> result = convolution(audioFile.getAudioData(), h);
			cout << "File sau khi nhân chập : \n";
			writeAudioFile(result, "C:\\Users\\giang\\Music\\convolution.wav", 44100);
			draw_gnuplot(result, "convolution");
			break;
		}
		case 8: {
			cout << "Chọn bộ lọc : \n";
			cout << "1. LPF\n";
			cout << "2. BPF\n";
			cout << "3. HPF\n";
			cout << "4. BSF\n";
			vector<float> audio = {0, 0, 0.15, 0.1, 0.2, 0.21, 0.1, 0.02 };
			draw_gnuplot(audio, "audio origin");
			int option; cin >> option;
			switch (option) {
				case 1 : {
					cout << "Nhập tần số cắt Wc: \n";
					float Wc; cin >> Wc;
					int N = audio.size();
					//LPF(audio, N, Wc);
					audioFile.LPF(N, Wc);
					break;
				}
				case 2 : {
					cout << "Nhập tần số cắt Wc1: \n";
					float Wc1; cin >> Wc1;
					cout << "Nhập tần số cắt Wc2: \n";
					float Wc2; cin >> Wc2;
					int N = audio.size();
					//BPF(audio, N, Wc1, Wc2);
					audioFile.BPF(N, Wc1, Wc2);
					break;
				}
				case 3 : {
					cout << "Nhập tần số cắt Wc: \n";
					float Wc; cin >> Wc;
					cout << "Nhập omega: \n";
					float omega; cin >> omega;
					int N = audio.size();
					audioFile.HPF( N, Wc, omega);
					break;
				}
				case 4 : {
					cout << "Nhập tần số cắt Wc1: \n";
					float Wc1; cin >> Wc1;
					cout << "Nhập tần số cắt Wc2: \n";
					float Wc2; cin >> Wc2;
					cout << "Nhập omega: \n";
					float omega; cin >> omega;
					int N = audio.size();
					audioFile.BSF(N, Wc1, Wc2, omega);
					break;
				}
				default:
					break;
			}
			break;
		}
		case 9: {
			cout << "Chọn bộ lọc Fade : \n";
			cout << "1. Fade In\n";
			cout << "2. Fade Out\n";
			int option; cin >> option;
			bool fade = option == 1 ? true : false;
			cout << "Nhập thời gian fade \n";
			float time; cin >> time;
			audioFile.applyFade( time, fade);
			break;
		}
		case 10: {
			cout << "Echo : \n";
			cout << "Nhập độ trễ echo : \n";
			float delayInSeconds; cin >> delayInSeconds;
			cout << "Hệ số suy giảm : \n";
			float decayFactor; cin >> decayFactor;
			audioFile.applyEcho( delayInSeconds, decayFactor);
			break;
		}
		case 11: {
			cout << "Reverb : \n";
			cout << "Nhập độ trễ : \n";
			float delayTime; cin >> delayTime;
			cout << "Hệ số suy giảm : \n";
			float decayFactor; cin >> decayFactor;
			audioFile.applyReverb( delayTime, decayFactor);
			break;
		}
		case 12: {
			cout << "Planger : \n";
			cout << "Nhập tốc độ giao động : \n";
			float rate; cin >> rate;
			cout << "Nhập độ sâu giao động: \n";
			float depth; cin >> depth;
			cout << "Nhập thời gian trễ động: \n";
			float delay; cin >> delay;
			cout << "Nhập độ phản hồi : \n";
			float feedback; cin >> feedback;
			audioFile.applyFlanger( rate, depth, delay, feedback);
			break;
		}
		case 14: {
			exit(0);
			break;
		}
		default:
			break;
		}
		
	}
	
	//reverse(audioData.begin(), audioData.end());


	return 0;
}

void AudioFile::applyFlanger( float rate, float depth, float delay, float feedback) {
	vector<float> audioSignal = this->audioData;
	size_t delaySamples = static_cast<size_t>(delay * this->sampleRate); // 44100 mẫu mỗi giây (sample rate) trong âm thanh CD

	std::vector<double> flangerSignal(audioSignal.size(), 0.0);

	// Tạo âm thanh flanger bằng cách phản ứng pha và thay đổi tốc độ của nó
	for (size_t i = 0; i < audioSignal.size(); ++i) {
		size_t index = static_cast<size_t>(i + delaySamples * (1 + sin(2 * 3.14 * rate * i / this->sampleRate * depth)));

		if (index < audioSignal.size()) {
			flangerSignal[i] = audioSignal[i] + audioSignal[index] * feedback;
		}
	}

	// Kết hợp âm thanh flanger và âm thanh ban đầu
	for (size_t i = 0; i < audioSignal.size(); ++i) {
		audioSignal[i] += flangerSignal[i];
	}
	cout << "Flanger \n";
	writeAudioFile(audioSignal, "C:\\Users\\giang\\Music\\Flanger.wav", this->sampleRate);
	draw_gnuplot(audioSignal, "Flanger");
}

void AudioFile::applyReverb( float delayTime, float decayFactor) {
	vector<float> audioSignal = this->audioData;
	size_t delaySamples = static_cast<size_t>(delayTime * this->sampleRate); // 44100 mẫu mỗi giây (sample rate) trong âm thanh CD

	std::vector<double> reverbSignal(audioSignal.size() + delaySamples, 0.0);

	// Tạo âm thanh reverb bằng cách phản chiếu và thêm âm thanh ban đầu
	for (size_t i = 0; i < audioSignal.size(); ++i) {
		reverbSignal[i] += audioSignal[i];
		if (i + delaySamples < reverbSignal.size()) {
			reverbSignal[i + delaySamples] += audioSignal[i] * decayFactor;
		}
	}

	// Kết hợp âm thanh reverb và âm thanh ban đầu
	for (size_t i = 0; i < audioSignal.size(); ++i) {
		audioSignal[i] += reverbSignal[i];
	}
	cout << "Reverb \n";
	writeAudioFile(audioSignal, "C:\\Users\\giang\\Music\\Reverb.wav", this->sampleRate);
	draw_gnuplot(audioSignal, "Reverb");
}

void AudioFile::applyEcho( float delayInSeconds, float decayFactor) {
	vector<float> audioSignal = this->audioData;
	size_t delaySamples = static_cast<size_t>(delayInSeconds * this->sampleRate); // 44100 là mẫu mỗi giây (sample rate) trong âm thanh CD
	size_t totalSamples = audioSignal.size();
	std::vector<double> echoBuffer(delaySamples, 0.0);

	for (size_t i = 0; i < totalSamples; ++i) {
		if (i >= delaySamples) {
			audioSignal[i] += echoBuffer[i % delaySamples] * decayFactor;
		}

		echoBuffer[i % delaySamples] = audioSignal[i];
	}
	cout << "Echo \n";
	writeAudioFile(audioSignal, "C:\\Users\\giang\\Music\\Echo.wav", this->sampleRate);
	draw_gnuplot(audioSignal, "Echo");
}

void AudioFile::applyFade(float fadeDurationInSeconds, bool fadeIn) {
	vector<float>audioSignal = this->audioData;
	size_t fadeSamples = static_cast<size_t>(fadeDurationInSeconds * this->sampleRate);

	if (fadeSamples > audioSignal.size()) {
		cerr << "Không thể áp dụng fade: Độ dài fade lớn hơn độ dài tín hiệu." << std::endl;
		return;
	}

	double fadeFactorIncrement = 1.0 / fadeSamples;
	double fadeFactor = 0.0;

	if (fadeIn) {
		for (size_t i = 0; i < fadeSamples; ++i) {
			audioSignal[i] *= fadeFactor;
			fadeFactor += fadeFactorIncrement;
		}
		cout << "FadeIn \n";
		writeAudioFile(audioSignal, "C:\\Users\\giang\\Music\\FadeIn.wav", this->sampleRate);
		draw_gnuplot(audioSignal, "FadeIn");
	}
	else {
		for (size_t i = audioSignal.size() - fadeSamples; i < audioSignal.size(); ++i) {
			audioSignal[i] *= fadeFactor;
			fadeFactor -= fadeFactorIncrement;
		}
		cout << "FadeOut \n";
		writeAudioFile(audioSignal, "C:\\Users\\giang\\Music\\FadeOut.wav", this->sampleRate);
		draw_gnuplot(audioSignal, "FadeOut");
	}
}

void AudioFile::BSF( int N, float Wc1, float Wc2, float omega) {
	vector<float> signal = this->audioData;
	vector<float> h;
	for (int i = 0; i < N; i++) {
		float x;
		if (i == (N - 1) / 2) {
			x = (Wc1 - Wc2) / 3.14;
		}
		else {
			x = omega * (i - (N - 1) / 2) - (sin(Wc1 * (i - (N - 1) / 2)) - sin(Wc2 * (i - (N - 1) / 2))) / (3.14 * (i - (N - 1) / 2));
		}
		h.push_back(x);
	}
	vector<float> window(N, 1);
	vector<float> result = convolution(h, window);
	vector <float> y = convolution(result, signal);
	cout << "BSF \n";
	writeAudioFile(result, "C:\\Users\\giang\\Music\\BSF.wav", this->sampleRate);
	draw_gnuplot(y, "BSF");
}

void AudioFile::HPF( int N, float Wc, float omega) {
	vector<float> signal = this->audioData;
	vector<float> h;
	for (int i = 0; i < N; i++) {
		float x;
		if (i == (N - 1) / 2) {
			x = (Wc) / 3.14;
		}
		else {
			x = omega * (i - (N - 1) / 2) - (sin(Wc * (i - (N - 1) / 2)) / (3.14 * (i - (N - 1) / 2)));
		}
		h.push_back(x);
	}
	vector<float> window(N, 1);
	vector<float> result = convolution(h, window);
	vector <float> y = convolution(result, signal);
	cout << "HPF \n";
	writeAudioFile(result, "C:\\Users\\giang\\Music\\HPF.wav", this->sampleRate);
	draw_gnuplot(y, "HPF");
}

void AudioFile::BPF( int N, float Wc1, float Wc2) {
	vector<float> signal = this->audioData;
	vector<float> h;
	for (int i = 0; i < N; i++) {
		float x;
		if (i == (N - 1) / 2) {
			x = (Wc1 - Wc2) / 3.14;
		}
		x =  (sin(Wc1 * (i - (N - 1) / 2)) - sin(Wc2 * (i - (N - 1) / 2))) / (3.14 * (i - (N - 1) / 2));
		h.push_back(x);
	}
	vector<float> window(N, 1);
	vector<float> result = convolution(h, window);
	vector <float> y = convolution(result, signal);
	cout << "BPF \n";
	writeAudioFile(result, "C:\\Users\\giang\\Music\\BPF.wav", this->sampleRate);
	draw_gnuplot(y, "BPF");
}

void AudioFile::LPF(int N, float Wc) {
	vector<float> signal = this->audioData;
	vector<float> h;
	for (int i = 0; i < N; i++) {
		float x;
		if (i == (N - 1) / 2) {
			x = Wc / 3.14;
		}
		else {
			x = sin(Wc * (i - (N - 1) / 2)) / (3.14 * (i - (N - 1) / 2));
		}
		h.push_back(x);
	}
	vector<float> window(N, 1);
	vector<float> result = convolution(h, window);
	vector <float> y = convolution(result, signal);
	cout << "LPF \n";
	writeAudioFile(result, "C:\\Users\\giang\\Music\\LPF.wav", this->sampleRate);
	draw_gnuplot(y, "LPF");
}

vector<float> convolution(vector<float> signal , vector<float> h) {
	vector<float> result(signal.size() + h.size() - 1, 0.0);
	for (int n = 0; n < result.size(); n++) {
		for (int k = 0; k < h.size(); k++) {
			if (n >= k && n - k < signal.size()) {
				result[n] += signal[n - k] * h[k];
			}
		}
	}
	return result;
}

void AudioFile::multiplySignal( vector<float> a2) {
	vector<float> a1 = this->audioData;
	int samplingFrequency = this->sampleRate;
	vector<float> output;
	int length = a1.size() < a2.size() ? a1.size() : a2.size();
	for (int i = 0; i < length; i++) {
		output.push_back(a1[i] * a2[i]);
	}
	for (int i = length; i < a1.size(); i++) {
		output.push_back(a1[i]);
	}
	for (int i = length; i < a2.size(); i++) {
		output.push_back(a2[i]);
	}
	cout << "File sau khi nhân " << endl;
	writeAudioFile(output, "C:\\Users\\giang\\Music\\outputMultiplySignal.wav", samplingFrequency);
	draw_gnuplot(output, "outputMultiplySignal");
}

void AudioFile::addSignal(vector<float> a2) {
	vector<float> a1 = this->audioData;
	int samplingFrequency = this->sampleRate;
	vector<float> output;
	int length = a1.size() < a2.size() ? a1.size() : a2.size();
	for (int i = 0; i < length; i++) {
		output.push_back(a1[i] + a2[i]);
	}
	for (int i = length; i < a1.size(); i++) {
		output.push_back(a1[i]);
	}
	for (int i = length; i < a2.size(); i++) {
		output.push_back(a2[i]);
	}
	cout << "File sau khi thêm " << endl;
	writeAudioFile(output, "C:\\Users\\giang\\Music\\outputAddSignal.wav", samplingFrequency);
	draw_gnuplot(output, "addSinal");
}

void AudioFile::reduceSamplingFrequency(float factor) {
	vector<float> audioData = this->audioData;
	int samplingFrequency = this->sampleRate;
	vector<float> reducedSamples;
	for (int i = 0; i < audioData.size(); i += factor) {
		reducedSamples.push_back(audioData[i]);	
	}
	samplingFrequency /= factor;
	cout << "Tần số lấy mẫu sau khi giảm : " << samplingFrequency << "Hz" << endl;
	writeAudioFile(reducedSamples, "C:\\Users\\giang\\Music\\reduce.wav", samplingFrequency);
	AudioFile outputData = readAudioFile("C:\\Users\\giang\\Music\\reduce.wav");
	draw_gnuplot(outputData.getAudioData(), "reduceSampling");
}

// tăng tần số mẫu 
void AudioFile::increaseSamplingFrequency( int length) {
	vector<float> audioData = this->audioData;
	vector<float> output;
	for (int i = 0; i < audioData.size() ; i++) {
		output.push_back(audioData[i]);
		for (int j = 1; j < length; j++) {
			output.push_back(0.0f);
		}
	}
	int samplingFrequency = this->sampleRate * length;
	cout << "Tần số lấy mẫu sau khi tăng : " << samplingFrequency << "Hz" << endl;
	writeAudioFile(output, "C:\\Users\\giang\\Music\\increase.wav", samplingFrequency);
	AudioFile outputData = readAudioFile("C:\\Users\\giang\\Music\\increase.wav");
	draw_gnuplot(outputData.getAudioData(), "increaseSampling");
}

void AudioFile::inversion() {
	vector<float> audioData = this->audioData;
	reverse(audioData.begin(), audioData.end());
	draw_gnuplot(audioData,"inversion");
}

void AudioFile::time_shift(int shiftSamples) {
	vector<float> audioData = this->audioData;
	vector<float> outputData(audioData.size());
	for (int i = 0; i < audioData.size(); i++) {
		int shifteIndex = i + shiftSamples;
		if (shifteIndex >= 0 && shifteIndex < audioData.size()) {
			outputData[shifteIndex] = audioData[i];
		}
	}
	draw_gnuplot(outputData,"time_shift");
}

AudioFile readAudioFile(string fileName) {
	SndfileHandle file(fileName, SFM_READ);

	if (!file) {
		cerr << "Error: Could not open input file." << std::endl;
	}

	// Hiển thị thông tin về file WAV
	cout << "Tên file : " << fileName << endl;
	cout << "Sample rate: " << file.samplerate() << " Hz" << endl;
	cout << "Channels: " << file.channels() << endl;
	cout << "Frames: " << file.frames() << endl;

	// Đọc dữ liệu âm thanh từ file
	vector<float> audioData(file.frames() * file.channels());
	file.read(audioData.data(), file.frames() * file.channels());
	AudioFile audioFile = AudioFile(audioData, file.samplerate());
	return audioFile;
}

void writeAudioFile(const std::vector<float>& audioData, const char* filename, int sampleRate) {
	// Tạo một đối tượng SndfileHandle để ghi tệp âm thanh
	SndfileHandle outputFile(filename, SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_FLOAT, 2, sampleRate);

	// Kiểm tra xem việc tạo tệp âm thanh có thành công không
	if (!outputFile) {
		std::cerr << "Không thể tạo tệp âm thanh đầu ra." << std::endl;
		return;
	}

	// Ghi dữ liệu âm thanh vào tệp âm thanh mới
	outputFile.write(audioData.data(), audioData.size());
}