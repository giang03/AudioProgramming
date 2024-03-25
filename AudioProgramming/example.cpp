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
void BPF(vector<float> signal, int N, float Wc);
void applyFade(vector<float> audioSignal, float fadeDurationInSeconds, bool fadeIn);
void applyEcho(vector<float> audioSignal, float delayInSeconds, float decayFactor);
void applyReverb(vector<float> audioSignal, float delayTime, float decayFactor);

int main() {

	vector<float> audioData = readAudioFile("C:\\Users\\giang\\Music\\example.wav");
	
	int samplerate = 44100;

	draw_gnuplot(audioData, "file goc");

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
		cout << "14. Exit \n";

		int choice; cin >> choice;
		switch (choice)
		{
		case 1: {
			cout << "Nhập thời gian dời theo giây : ";
			int shiftAmount; cin >> shiftAmount;

			long long shiftSamples = shiftAmount * samplerate;
			time_shift(audioData, shiftSamples);
			break;
		}
		case 2: {
			cout << "Âm thanh sau khi lật ngược : ";
			inversion(audioData);
			break;
		}
		case 3: {
			cout << "Nhập tỷ số giãn : ";
			int length; cin >> length;

			increaseSamplingFrequency(audioData, length);
			break;
		}
		case 4: {
			cout << "Nhập tỷ số nén : ";
			int length; cin >> length;

			reduceSamplingFrequency(audioData, length);
			break;
		}
		case 5: {
			vector<float> audioData2 = readAudioFile("C:\\Users\\giang\\Music\\addSignal.wav");
			addSignal(audioData, audioData2);
			break;
		}
		case 6: {
			vector<float> audioData2 = readAudioFile("C:\\Users\\giang\\Music\\addSignal.wav");
			multiplySignal(audioData, audioData2);
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
			vector<float> result = convolution(audioData, h);
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
			int option; cin >> option;
			switch (option) {
				case 1 : {
					cout << "Nhập tần số cắt Wc: \n";
					float Wc; cin >> Wc;
					int N = audioData.size();
					LPF(audioData, N, Wc);
					break;
				}
				case 2 : {

					break;
				}
				case 3 : {

					break;
				}
				case 4 : {

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
			applyFade(audioData, time, fade);
			break;
		}
		case 10: {
			cout << "Echo : \n";
			cout << "Nhập độ trễ echo : \n";
			float delayInSeconds; cin >> delayInSeconds;
			cout << "Hệ số suy giảm : \n";
			float decayFactor; cin >> decayFactor;
			applyEcho(audioData, delayInSeconds, decayFactor);
			break;
		}
		case 11: {
			cout << "Reverb : \n";
			cout << "Nhập độ trễ : \n";
			float delayTime; cin >> delayTime;
			cout << "Hệ số suy giảm : \n";
			float decayFactor; cin >> decayFactor;
			applyReverb(audioData, delayTime, decayFactor);
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

void applyReverb(vector<float> audioSignal, float delayTime, float decayFactor) {
	size_t delaySamples = static_cast<size_t>(delayTime * 44100); // 44100 mẫu mỗi giây (sample rate) trong âm thanh CD

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
	writeAudioFile(audioSignal, "C:\\Users\\giang\\Music\\Reverb.wav", 44100);
	draw_gnuplot(audioSignal, "Reverb");
}

void applyEcho(vector<float> audioSignal, float delayInSeconds, float decayFactor) {
	size_t delaySamples = static_cast<size_t>(delayInSeconds * 44100); // 44100 là mẫu mỗi giây (sample rate) trong âm thanh CD
	size_t totalSamples = audioSignal.size();
	std::vector<double> echoBuffer(delaySamples, 0.0);

	for (size_t i = 0; i < totalSamples; ++i) {
		if (i >= delaySamples) {
			audioSignal[i] += echoBuffer[i % delaySamples] * decayFactor;
		}

		echoBuffer[i % delaySamples] = audioSignal[i];
	}
	cout << "Echo \n";
	writeAudioFile(audioSignal, "C:\\Users\\giang\\Music\\Echo.wav", 44100);
	draw_gnuplot(audioSignal, "Echo");
}

void applyFade(vector<float>audioSignal, float fadeDurationInSeconds, bool fadeIn) {
	size_t fadeSamples = static_cast<size_t>(fadeDurationInSeconds * 44100);

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
		writeAudioFile(audioSignal, "C:\\Users\\giang\\Music\\FadeIn.wav", 44100);
		draw_gnuplot(audioSignal, "FadeIn");
	}
	else {
		for (size_t i = audioSignal.size() - fadeSamples; i < audioSignal.size(); ++i) {
			audioSignal[i] *= fadeFactor;
			fadeFactor -= fadeFactorIncrement;
		}
		cout << "FadeOut \n";
		writeAudioFile(audioSignal, "C:\\Users\\giang\\Music\\FadeOut.wav", 44100);
		draw_gnuplot(audioSignal, "FadeOut");
	}
}

void BPF(vector<float> signal, int N, float Wc) {
	vector<float> h;
	for (int i = 0; i < N; i++) {
		float x = (Wc * sin(Wc * (i - (N - 1) / 2))) / (3.14 * Wc * (i - (N - 1) / 2));
		h.push_back(x);
	}
	vector<float> window(N, 1);
	vector<float> result = convolution(h, window);
	vector <float> y = convolution(result, signal);
	cout << "LPF \n";
	writeAudioFile(result, "C:\\Users\\giang\\Music\\BPF.wav", 44100);
	draw_gnuplot(y, "BPF");
}

void LPF(vector<float> signal, int N, float Wc) {
	vector<float> h;
	for (int i = 0; i < N; i++) {
		float x = (Wc * sin(Wc * (i - (N - 1) / 2))) / (3.14 * Wc * (i - (N - 1) / 2));
		h.push_back(x);
	}
	vector<float> window(N, 1);
	vector<float> result = convolution(h, window);
	vector <float> y = convolution(result, signal);
	cout << "LPF \n";
	writeAudioFile(result, "C:\\Users\\giang\\Music\\LPF.wav", 44100);
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

void multiplySignal(vector<float> a1, vector<float> a2) {
	int samplingFrequency = 44100;
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

void addSignal(vector<float> a1, vector<float> a2) {
	int samplingFrequency = 44100;
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

void reduceSamplingFrequency(vector<float> audioData, float factor) {
	int samplingFrequency = 44100;
	vector<float> reducedSamples;
	for (int i = 0; i < audioData.size(); i += factor) {
		reducedSamples.push_back(audioData[i]);	
	}
	samplingFrequency /= factor;
	cout << "Tần số lấy mẫu sau khi giảm : " << samplingFrequency << "Hz" << endl;
	writeAudioFile(reducedSamples, "C:\\Users\\giang\\Music\\reduce.wav", samplingFrequency);
	vector<float> outputData = readAudioFile("C:\\Users\\giang\\Music\\reduce.wav");
	draw_gnuplot(outputData, "reduceSampling");
}

// tăng tần số mẫu 
void increaseSamplingFrequency(vector<float> audioData, int length) {
	vector<float> output;
	for (int i = 0; i < audioData.size() ; i++) {
		output.push_back(audioData[i]);
		for (int j = 1; j < length; j++) {
			output.push_back(0.0f);
		}
	}
	int samplingFrequency = 44100 * length;
	cout << "Tần số lấy mẫu sau khi tăng : " << samplingFrequency << "Hz" << endl;
	writeAudioFile(output, "C:\\Users\\giang\\Music\\increase.wav", samplingFrequency);
	vector<float> outputData = readAudioFile("C:\\Users\\giang\\Music\\increase.wav");
	draw_gnuplot(outputData, "increaseSampling");
}

void inversion(vector<float> audioData) {
	reverse(audioData.begin(), audioData.end());
	draw_gnuplot(audioData,"inversion");
}

void time_shift(vector<float> audioData, int shiftSamples) {
	vector<float> outputData(audioData.size());
	for (int i = 0; i < audioData.size(); i++) {
		int shifteIndex = i + shiftSamples;
		if (shifteIndex >= 0 && shifteIndex < audioData.size()) {
			outputData[shifteIndex] = audioData[i];
		}
	}
	draw_gnuplot(outputData,"time_shift");
}

vector<float> readAudioFile(string fileName) {
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
	return audioData;
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