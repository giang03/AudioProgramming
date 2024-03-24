#include<random>
#include <iostream>
#include <vector>
#include <numeric>
#include <sndfile.hh>


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
		cout << "5. Exit \n";

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