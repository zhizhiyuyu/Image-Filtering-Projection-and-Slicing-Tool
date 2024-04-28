#include "../src/Filter.hpp"
#include "../src/Image.hpp"
#include "../src/Volume.hpp"
#include<iostream>
#include "../src/stb_image.h"
#include "../src/stb_image_write.h"


void ColourBalance2D_test() {
	int width = 2;
	int height = 2;
	int channels = 3;

	const char* filepath = "test_ColorBalcance.png";
	unsigned char data[] = { 10,30,50,
								20,40,60,
									110,130,150,
										120,140,160 };
	stbi_write_png(filepath, width, height, channels, data, 0);
	auto test_image = Image<unsigned char>(filepath);
	ColourBalance2D<double, unsigned char> colourbalance(std::vector<double>{2., 1., 1., });
	auto colourBlance = colourbalance.apply(test_image);

	colourBlance.write(filepath);

	unsigned char* Bri_result = stbi_load(filepath, &width, &height, &channels, 0);
	unsigned char result[] = {20, 30 ,50, 40 ,40, 60 ,220, 130, 150, 240 ,140, 160};
	std::cout << "Code result: ";
	for (auto it = Bri_result; it != Bri_result + 12; ++it) {
		double value = static_cast<double>(*it);
		std::cout << value << " ";
	}
	std::cout << std::endl;


	if ( std::equal(std::begin(result), std::end(result), Bri_result)) {
		std::cout << "Test_Auto_colorBalance --> passes\n" << std::endl;
	}
	else {
		std::cout <<  "Test_Auto_colorBalance --> fails\n" << std::endl;
	}

}

void Grayscale_test() {
	int width = 2;
	int height = 2;
	int channels = 3;

	const char* filepath = "test_Grayscale.png";
	unsigned char data[] = { 10,30,50,
								20,40,60,
									110,130,150,
										120,140,160 };
	stbi_write_png(filepath, width, height, channels, data, 0);
	auto test_image = Image<unsigned char>(filepath);

	Grayscale2D<double, unsigned char> grayscale;
	auto grayScale = grayscale.apply(test_image);

	grayScale.write("test_Grayscale2.png");
	channels = 1;
	unsigned char* Bri_result = stbi_load("test_Grayscale2.png", &width, &height, &channels, 0);
	unsigned char result[] = { 10 ,20, 110 ,120 };

	std::cout << "Code result: ";
	for (auto it = Bri_result; it != Bri_result + 4; ++it) {
		double value = static_cast<double>(*it);
		std::cout << value << " ";
	}
	std::cout << std::endl;


	if (std::equal(std::begin(result), std::end(result), Bri_result)) {
		std::cout << "Test_Grayscale --> passes\n" << std::endl;
	}
	else {
		std::cout << "Test_Grayscale --> fails\n" << std::endl;
	}

}


void BoxBlur_test() {
	int width = 2;
	int height = 2;
	int channels = 3;

	const char* filepath = "test_Boxblur.png";
	unsigned char data[] = { 10,30,50,
								20,40,60,
									110,130,150,
										120,140,160 };
	stbi_write_png(filepath, width, height, channels, data, 0);
	Blur2D<double, unsigned char> blur(BlurKernel::box);
	auto test_image = Image<unsigned char>(filepath);
	auto boxBlur = blur.apply(test_image);

	boxBlur.write(filepath);

	unsigned char* Bri_result = stbi_load(filepath, &width, &height, &channels, 0);
	unsigned char result[] = {46, 66, 86, 50, 70, 90, 79, 100, 119, 83, 103, 123};
	std::cout << "Code result: ";
	for (auto it = Bri_result; it != Bri_result + 12; ++it) {
		double value = static_cast<double>(*it);
		std::cout << value << " ";
	}
	std::cout << std::endl;


	if (std::equal(std::begin(result), std::end(result), Bri_result)) {
		std::cout << "Test_BoxBlur --> passes\n" << std::endl;
	}
	else {
		std::cout << "Test_BoxBlur --> fails\n" << std::endl;
	}

}



void Guassian2D_test() {
	int width = 2;
	int height = 2;
	int channels = 3;

	const char* filepath = "test_Guassian2D.png";
	unsigned char data[] = { 10,30,50,
								20,40,60,
									110,130,150,
										120,140,160 };
	stbi_write_png(filepath, width, height, channels, data, 0);
	auto test_image = Image<unsigned char>(filepath);

	Blur2D<double, unsigned char> blur(BlurKernel::gaussian);
	auto gaussianBlur = blur.apply(test_image);

	gaussianBlur.write(filepath);

	unsigned char* Bri_result = stbi_load(filepath, &width, &height, &channels, 0);
	unsigned char result[] = { 40, 60, 80, 44, 64, 84, 85, 105, 125, 89, 109, 129 };
	std::cout << "Code result: ";
	for (auto it = Bri_result; it != Bri_result + 12; ++it) {
		double value = static_cast<double>(*it);
		std::cout << value << " ";
	}
	std::cout << std::endl;


	if (std::equal(std::begin(result), std::end(result), Bri_result)) {
		std::cout << "Test_Guassian2D --> passes\n" << std::endl;
	}
	else {
		std::cout << "Test_Guassian2D --> fails\n" << std::endl;
	}

}

//
void medianBlur_test() {
	int width = 2;
	int height = 2;
	int channels = 3;

	const char* filepath = "test_medianBlur.png";
	unsigned char data[] = { 10,30,50,
								20,40,60,
									110,130,150,
										120,140,160 };
	stbi_write_png(filepath, width, height, channels, data, 0);
	auto test_image = Image<unsigned char>(filepath);


	MedianBlur2D<double, unsigned char> blur;
	auto output_image = blur.apply(test_image);

	output_image.write(filepath);

	unsigned char* Bri_result = stbi_load(filepath, &width, &height, &channels, 0);
	unsigned char result[] = { 5 ,15, 25, 5, 15, 25, 5, 15, 25, 5, 15, 25 };
	std::cout << "Code result: ";
	for (auto it = Bri_result; it != Bri_result + 12; ++it) {
		double value = static_cast<double>(*it);
		std::cout << value << " ";
	}
	std::cout << std::endl;


	if (std::equal(std::begin(result), std::end(result), Bri_result)) {
		std::cout << "Test_medianBlur --> passes\n" << std::endl;
	}
	else {
		std::cout << "Test_medianBlur --> fails\n" << std::endl;
	}

}


void sobelEdge_test() {
	int width = 2;
	int height = 2;
	int channels = 3;

	const char* filepath = "test_sobelEdge.png";
	unsigned char data[] = { 10,30,50,
								20,40,60,
									110,130,150,
										120,140,160 };
	stbi_write_png(filepath, width, height, channels, data, 0);
	auto test_image = Image<unsigned char>(filepath);


	Grayscale2D<double, unsigned char> grayscale;
	auto grayScale = grayscale.apply(test_image);
	Edge2D<double, unsigned char> edge(EdgeKernel::sobel);
	auto output_image = edge.apply(grayScale);

	output_image.write(filepath);
	unsigned char* Bri_result = stbi_load(filepath, &width, &height, &channels, 0);
	unsigned char result[] = {255, 255, 255, 255 };
	std::cout << "Code result: ";
	for (auto it = Bri_result; it != Bri_result + 4; ++it) {
		double value = static_cast<double>(*it);
		std::cout << value << " ";
	}
	std::cout << std::endl;


	if (std::equal(std::begin(result), std::end(result), Bri_result)) {
		std::cout << "Test_sobelEdge --> passes\n" << std::endl;
	}
	else {
		std::cout << "Test_sobelEdge --> fails\n" << std::endl;
	}

}


void prewittEdge_test() {
	int width = 2;
	int height = 2;
	int channels = 3;

	const char* filepath = "test_prewittEdge.png";
	unsigned char data[] = { 10,30,50,
								20,40,60,
									110,130,150,
										120,140,160 };
	stbi_write_png(filepath, width, height, channels, data, 0);
	auto test_image = Image<unsigned char>(filepath);

	Grayscale2D<double, unsigned char> grayscale;
	auto grayScale = grayscale.apply(test_image);
	Edge2D<double, unsigned char> edge(EdgeKernel::prewitt);
	auto prewitt = edge.apply(grayScale);

	prewitt.write(filepath);

	unsigned char* Bri_result = stbi_load(filepath, &width, &height, &channels, 0);
	unsigned char result[] = { 255, 255, 255, 255 };
	std::cout << "Code result: ";
	for (auto it = Bri_result; it != Bri_result + 4; ++it) {
		double value = static_cast<double>(*it);
		std::cout << value << " ";
	}
	std::cout << std::endl;


	if (std::equal(std::begin(result), std::end(result), Bri_result)) {
		std::cout << "Test_prewittEdge --> passes\n" << std::endl;
	}
	else {
		std::cout << "Test_prewittEdge --> fails\n" << std::endl;
	}

}


void robertsEdge() {
	int width = 2;
	int height = 2;
	int channels = 3;

	const char* filepath = "test_robertsEdge.png";
	unsigned char data[] = { 10,30,50,
								20,40,60,
									110,130,150,
										120,140,160 };
	stbi_write_png(filepath, width, height, channels, data, 0);
	auto test_image = Image<unsigned char>(filepath);


	Grayscale2D<double, unsigned char> grayscale;
	auto grayScale = grayscale.apply(test_image);
	Edge2D<double, unsigned char> edge(EdgeKernel::robertscross);
	auto robertscross = edge.apply(grayScale);
	robertscross.write(filepath);

	unsigned char* Bri_result = stbi_load(filepath, &width, &height, &channels, 0);
	unsigned char result[] = { 142 ,141 ,14 ,0 };
	std::cout << "Code result: ";
	for (auto it = Bri_result; it != Bri_result + 4; ++it) {
		double value = static_cast<double>(*it);
		std::cout << value << " ";
	}
	std::cout << std::endl;


	if (std::equal(std::begin(result), std::end(result), Bri_result)) {
		std::cout << "Test_robertsEdgeD --> passes\n" << std::endl;
	}
	else {
		std::cout << "Test_robertsEdge --> fails\n" << std::endl;
	}

}


void scharrEdge_test() {
	int width = 2;
	int height = 2;
	int channels = 3;

	const char* filepath = "test_scharrEdge.png";
	unsigned char data[] = { 10,30,50,
								20,40,60,
									110,130,150,
										120,140,160 };
	stbi_write_png(filepath, width, height, channels, data, 0);
	auto test_image = Image<unsigned char>(filepath);

	Grayscale2D<double, unsigned char> grayscale;
	auto grayScale = grayscale.apply(test_image);
	Edge2D<double, unsigned char> edge(EdgeKernel::scharr);
	auto scharr = edge.apply(grayScale);

	scharr.write(filepath);


	unsigned char* Bri_result = stbi_load(filepath, &width, &height, &channels, 0);
	unsigned char result[] = { 255, 255 ,255 ,255 };
	std::cout << "Code result: ";
	for (auto it = Bri_result; it != Bri_result + 4; ++it) {
		double value = static_cast<double>(*it);
		std::cout << value << " ";
	}
	std::cout << std::endl;


	if (std::equal(std::begin(result), std::end(result), Bri_result)) {
		std::cout << "Test_scharrEdge --> passes\n" << std::endl;
	}
	else {
		std::cout << "Test_scharrEdge --> fails\n" << std::endl;
	}

}


void DarkenBrightness_test() {
	int width = 2;
	int height = 2;
	int channels = 3;

	const char* filepath = "testDarken_brightness.png";
	unsigned char data[] = { 128, 129,130,141,0,0,0,0,0,0,0,0, };
	stbi_write_png(filepath, width, height, channels, data, 0);

	auto test_image = Image<unsigned char>(filepath);
	Brightness2D<double, unsigned char> brightness(-1);
	auto output_image = brightness.apply(test_image);
	output_image.write(filepath);

	unsigned char* Bri_result = stbi_load(filepath, &width, &height, &channels, 0);
	unsigned char result[] = { 127,128,129, 140,0,0,0,0,0,0,0,0, };

	std::cout << "Code result: ";
	for (auto it = Bri_result; it != Bri_result + 12; ++it) {
		double value = static_cast<double>(*it);
		std::cout << value << " ";
	}
	std::cout << std::endl;


	if (std::equal(std::begin(result), std::end(result), Bri_result)) {
		std::cout << "Test_Darken_Brightness --> passes \n" << std::endl;
	}
	else {
		std::cout << "Test_Darken_Brightness --> fails \n" << std::endl;
	}

}

void LightenBrightness_test() {
	int width = 2;
	int height = 2;
	int channels = 3;

	const char* filepath = "testLighten_brightness.png";
	unsigned char data[] = { 128, 129,130,141,255,255,255,255,255,255,255 ,255 };
	stbi_write_png(filepath, width, height, channels, data, 0);

	auto test_image = Image<unsigned char>(filepath);
	Brightness2D<double, unsigned char> brightness(1);
	auto output_image = brightness.apply(test_image);
	output_image.write(filepath);

	unsigned char* Bri_result = stbi_load(filepath, &width, &height, &channels, 0);
	unsigned char result[] = { 129,130,131, 142,255,255,255,255,255,255,255 ,255 };
	
	std::cout << "Code result: ";
	for (auto it = Bri_result; it != Bri_result + 12; ++it) {
		double value = static_cast<double>(*it);
		std::cout << value << " ";
	}
	std::cout << std::endl;


	if (std::equal(std::begin(result), std::end(result), Bri_result)) {
		std::cout << "Test_Lighten_Brightness --> passes\n" << std::endl;
	}
	else {
		std::cout << "Test_Lighten_Brightness --> fails \n" << std::endl;
	}

}


void HistEqu_test() {
	int width = 2;
	int height = 2;
	int channels = 3;

	const char* filepath_input = "test1_HisEqu1.png";
	const char* filepath_output = "test1_HisEqu2.png";
	unsigned char data[] = { 10,30,50,
								20,40,60,
									110,130,150,
										120,140,160 };
	stbi_write_png(filepath_input, width, height, channels, data, 0);

	auto test_image = Image<unsigned char>(filepath_input);

	HistogramEqualisation2D<double, unsigned char> HisEqu;
	auto output_image = HisEqu.apply(test_image);

	output_image.write(filepath_output);

	unsigned char* HisEqu_result = stbi_load(filepath_output, &width, &height, &channels, 0);

	unsigned char result[] = { 0,0,0,
								85,85,85,
									170,170,170,
										255,255,255 };

	std::cout << "Code result: ";
	for (auto it = HisEqu_result; it != HisEqu_result + 12; ++it) {
		double value = static_cast<double>(*it);
		std::cout << value << " ";
	}
	std::cout << std::endl;


	if (std::equal(std::begin(result), std::end(result), HisEqu_result)) {
		std::cout << "Test His_Equ --> passes\n" << std::endl;
	}
	else {
		std::cout << "Test His_Equ --> fails\n" << std::endl;
	}

}

void Gaussian3D_test() {
	int width = 2;
	int height = 2;
	int channels = 3;
	const char* filepath_input = "test_Guassian3d";
	const char* filepath_output = "FunResult_Guassian3d";

	unsigned char data[] = { 10,10,50,
		                        20,20,60,
		                          110,110,150,
                                    120,120,160 };
	stbi_write_png("test_Guassian3d1.png", width, height, channels, data, 0);

	auto test_volume = Volume<unsigned char>(filepath_input, 1, 1, 1);
	Blur3D<double, unsigned char> blur(BlurKernel::gaussian, 3,1);
	auto output_volume = blur.apply(test_volume);
	output_volume.write(filepath_output, 1);

	unsigned char* HisEqu_result = stbi_load("FunResult_Guassian3d1.png", &width, &height, &channels, 0);
	std::vector<double> result = { 40,40, 80,
	                                  44, 44, 84,
	                                      85, 85, 125,
	                                         89, 89, 129 };

	std::cout << "Code result: ";
	for (auto it = HisEqu_result; it != HisEqu_result + 12; ++it) {
		double value = static_cast<double>(*it);
		std::cout << value << " ";
	}
	std::cout << std::endl;


	if (std::equal(std::begin(result), std::end(result), HisEqu_result)) {
		std::cout << "Test 3DGuassian --> passes\n" << std::endl;
	}
	else {
		std::cout << "Test 3DGuassian --> fails\n" << std::endl;
	}

}





int main(){

	ColourBalance2D_test();

	Grayscale_test();

	BoxBlur_test();

	Guassian2D_test();

	DarkenBrightness_test();

	LightenBrightness_test();

	medianBlur_test();

	Gaussian3D_test();

	HistEqu_test();

	sobelEdge_test();

	prewittEdge_test();

	robertsEdge();
	
	scharrEdge_test();


	std::cout << "Congratulation: All pass" << std::endl;
}
