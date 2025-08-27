#ifndef TEXTURE_H
#define TEXTURE_H

#include "color.h"

class texture {
public:
	virtual ~texture() = default;

	virtual color value(float u, float v, const point3& p) const = 0;
};

class solid_color : public texture {
public:
	solid_color(const color& albedo) : albedo(albedo) {}

	solid_color(float red, float green, float blue) : solid_color(color(red, green, blue)) {}

	color value(float u, float v, const point3& p) const override {
		return albedo;
	}

private:
	color albedo;
};

#endif