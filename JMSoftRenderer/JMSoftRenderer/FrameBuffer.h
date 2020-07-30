#pragma once

#include "Vector.h"
#include "Color.h"

#define STB_IMAGE_IMPLEMENTATION
#include "include\stb_image.h"

template <class T>
class FrameBuffer {
protected:
	size_t width, height;
	size_t size;
	T* buffer;

public:
	FrameBuffer(size_t width, size_t height) : width(width), height(height), size(width* height) {
		buffer = new T[size];
	}
	~FrameBuffer() {
		delete[] buffer;
	}

	inline size_t get_width() const { return width; }
	inline size_t get_height() const { return height; }
	inline size_t get_size() const { return size; }
	inline float aspect() const { return (float)get_width() / get_height(); }

	inline void set(size_t x, size_t y, const T& data) { assert(y * width + x < size); buffer[y * width + x] = data; }
	inline void set(size_t index, const T& data) { assert(index < size); buffer[index] = data; }
	inline void add(size_t x, size_t y, const T& data) { assert(y * width + x < size); buffer[y * width + x] += data; }
	inline void add(size_t index, const T& data) { assert(index < size); buffer[index] += data; }

	inline void clear(size_t x, size_t y) { assert(y * width + x < size); buffer[y * width + x] = T(); }
	inline void fill(const T& data) {
		for (size_t i = 0; i < size; i++) buffer[i] = data;
	}
	inline T get(size_t x, size_t y) const { assert(y * width + x < size); return buffer[y * width + x]; }
	inline T get(size_t index) const { assert(index < size); return buffer[index]; }
	inline void get(T& ref, size_t x, size_t y) const { assert(y * width + x < size); ref = buffer[y * width + x]; }
	inline void get(T& ref, size_t index) const { assert(index < size); ref = buffer[index]; }

	T* operator()(size_t index = 0) { return buffer + index; }
	T* operator()(size_t x, size_t y) { return buffer + (y * width + x); }

	// x, y ��[0, 1)��Χ��
	inline T get(float x, float y) const {
		x = Math::fract(x);
		y = Math::fract(y);
		return get((size_t)(x * width) % width, (size_t)(y * height) % height);
	}

	// ��ά����(ÿ��Ԫ����[0, 1)��Χ��)
	inline T get(const Vector2& pos) const {
		return get(pos.x, pos.y);
	}
};

typedef FrameBuffer<float> FloatBuffer;
typedef FrameBuffer<int> IntBuffer;
typedef FrameBuffer<RGBColor> ColorBuffer;

shared_ptr<IntBuffer> CreateTexture(const char* filename) {
	int width, height, comp;
	stbi_uc* data = stbi_load(filename, &width, &height, &comp, STBI_rgb);
	if (!data) return shared_ptr<IntBuffer>();
	shared_ptr<IntBuffer> buffer = make_shared<IntBuffer>(width, height);
	for (int i = 0; i < width * height; i++) {
		// ʹ��image���texture
		*(*buffer)(i) = (data[3 * i] << 16) | (data[3 * i + 1] << 8) | data[3 * i + 2];
	}
	stbi_image_free(data);
	return buffer;
}