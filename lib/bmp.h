#pragma once
#include <fstream>

float linear_to_gamma(float linear_component) {
	if (linear_component > 0)
		return std::pow(linear_component, 1 / 3);
	return 0;
}

void write_bmp(const std::string& filename, const std::vector<std::vector<color>>& image, int image_width, int image_height) {
	std::ofstream file(filename, std::ios::out | std::ios::binary);

	unsigned char bmp_file_header[14] = {
		'B', 'M',    // Signature
		0, 0, 0, 0,  // File size in bytes
		0, 0,        // Reserved
		0, 0,        // Reserved
		54, 0, 0, 0  // Starting address of pixel array
	};

	unsigned char bmp_info_header[40] = {
		40, 0, 0, 0,  // Header size
		0, 0, 0, 0,   // Image width
		0, 0, 0, 0,   // Image height
		1, 0,         // Number of color planes
		24, 0,        // Bits per pixel
		0, 0, 0, 0,   // Compression method (0 = no compression)
		0, 0, 0, 0,   // Image size (can be 0 for uncompressed)
		0, 0, 0, 0,   // Horizontal resolution (pixels per meter)
		0, 0, 0, 0,   // Vertical resolution (pixels per meter)
		0, 0, 0, 0,   // Number of colors in palette
		0, 0, 0, 0    // Number of important colors
	};

	int file_size = 54 + 3 * image_width * image_height;
	bmp_file_header[2] = file_size;
	bmp_file_header[3] = file_size >> 8;
	bmp_file_header[4] = file_size >> 16;
	bmp_file_header[5] = file_size >> 24;

	bmp_info_header[4] = image_width;
	bmp_info_header[5] = image_width >> 8;
	bmp_info_header[6] = image_width >> 16;
	bmp_info_header[7] = image_width >> 24;
	bmp_info_header[8] = image_height;
	bmp_info_header[9] = image_height >> 8;
	bmp_info_header[10] = image_height >> 16;
	bmp_info_header[11] = image_height >> 24;

	// write headers to file
	file.write(reinterpret_cast<char*>(bmp_file_header), sizeof(bmp_file_header));
	file.write(reinterpret_cast<char*>(bmp_info_header), sizeof(bmp_info_header));

	// write pixel data in BGR format
	for (int j = image_height - 1; j >= 0; j--) {
		for (int i = 0; i < image_width; i++) {
			const color& pixel = image[j][i];
			unsigned char r = static_cast<unsigned char>(255.99 * pixel.x());
			unsigned char g = static_cast<unsigned char>(255.99 * pixel.y());
			unsigned char b = static_cast<unsigned char>(255.99 * pixel.z());

			linear_to_gamma(r);
			linear_to_gamma(g);
			linear_to_gamma(b);

			file.put(b);
			file.put(g);
			file.put(r);
		}
	}
	std::clog << "[" << return_current_time_and_date() << "] " << "Finished writing on .bmp" << ' ';
	file.close();
}
