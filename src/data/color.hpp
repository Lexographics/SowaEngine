#ifndef COLOR_HPP
#define COLOR_HPP
#pragma once

#include "sowa.hpp"

#include <algorithm>
#include <cmath>
#include <sstream>
#include <string>

class Color {
  public:
	Color() : r(1.f), g(1.f), b(1.f), a(1.f) {}
	Color(f32 v) : r(v), g(v), b(v), a(1.f) {}
	Color(f32 r, f32 g, f32 b) : r(r), g(g), b(b), a(1.f) {}
	Color(f32 r, f32 g, f32 b, f32 a) : r(r), g(g), b(b), a(a) {}

	static Color RGBAFloat(f32 r, f32 g, f32 b, f32 a = 1.f) {
		return Color(r, g, b, a);
	}

	static Color RGB(u8 r, u8 g, u8 b, u8 a = 255) {
		return Color(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
	}

	static Color HSV(f32 h, f32 s, f32 v) {
		h = std::fmod(h, 360.f);
		s = std::clamp(s, 0.f, 1.f);
		v = std::clamp(v, 0.f, 1.f);

		int hi = static_cast<int>(floor(h / 60.0)) % 6;
		double f = h / 60.0 - hi;

		double p = v * (1.0 - s);
		double q = v * (1.0 - s * f);
		double t = v * (1.0 - s * (1.0 - f));

		switch (hi) {
		case 0:
			return Color(v, t, p);
		case 1:
			return Color(q, v, p);
		case 2:
			return Color(p, v, t);
		case 3:
			return Color(p, q, v);
		case 4:
			return Color(t, p, v);
		case 5:
			return Color(v, p, q);

		default:
			return Color();
		}
	}

	f32 r;
	f32 g;
	f32 b;
	f32 a;
};

#endif // COLOR_HPP