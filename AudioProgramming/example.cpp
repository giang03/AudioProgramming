#include<random>
#include <iostream>
#include <vector>
#include <numeric>
#include <sndfile.hh>


#include "gnuplot-iostream.h"

using namespace std;

void draw_gnuplot(vector<double> v0) {
	Gnuplot gp;

	gp << "set title 'outputData' \n";
	gp << "plot '-' with lines title 'outputData'";

	gp.send(v0);

	cin.get();
}


void time_shift(vector<double> audioData, int shiftSamples) {
	vector<double> outputData(audioData.size());
	for (int i = 0; i < audioData.size(); i++) {
		int shifteIndex = i + shiftSamples;
		if (shifteIndex >= 0 && shifteIndex < audioData.size()) {
			outputData[shifteIndex] = audioData[i];
		}
	}
	draw_gnuplot(outputData);
}


int main() {

	SndfileHandle file("C:\\Users\\giang\\Music\\example.wav", SFM_READ);

	if (!file) {
		cerr << "Error: Could not open input file." << std::endl;
		return 1;
	}

	// Hiển thị thông tin về file WAV
	cout << "Sample rate: " << file.samplerate() << " Hz" << endl;
	cout << "Channels: " << file.channels() << endl;
	cout << "Frames: " << file.frames() << endl;

	// Đọc dữ liệu âm thanh từ file
	vector<double> audioData(file.frames() * file.channels());
	file.read(audioData.data(), file.frames() * file.channels());
	
	draw_gnuplot(audioData);

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

			long long shiftSamples = shiftAmount * file.samplerate();
			time_shift(audioData, shiftSamples);
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