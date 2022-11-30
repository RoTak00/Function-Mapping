#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <stdio.h>

// Override base class with your custom functionality

float id(float x) {return x;}
float dub(float x) {return 2 * x;}
float inv(float x) {
	if(x == 0) return 0x3f3f3f3f;
	return 1.0f / x;
}
float inv2(float x)
{
	if(x == 0) return 0x3f3f3f3f;
	return 1 + 1.0f / x;
}
float invsq(float x)
{
	if(x <= 0) return 0x3f3f3f3f;
	return 1.0f / sqrt(x);
}

float floorf(float x)
{
	return (int)x;
}
float fracf(float x)
{
	return floorf(x) - x;
}

float trig1(float x)
{
	return sinf(x) * (sinf(x) + 1);
}
float (*currfnc[])(float) = {id, dub, inv, inv2, sqrtf, invsq, floorf, fracf, sinf, cosf, tanf, trig1};
#define FUNC_NUM 12
class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		// Name your application
		sAppName = "Example";
	}

private:

	

	float testFunction(float x)
	{
		//if(x - 0 < 0.00001 && 0 - x < 0.0001) return 0x3f3f3f3f;
		return tanf(x);
	}

	float start = -5, end = 5;
	float precision;
	int width;
	int mult;
	int cfIndex = 0;
	std::vector<olc::vf2d> points; 

	bool done = false;

public:
	bool OnUserCreate() override
	{
		width = (float)ScreenWidth();
		mult = (int)(1.0f * width / (end - start));
		precision = (end - start) / (width / 3);
		for(float t = start; t <= end; t += precision)
		{
			
			float val = currfnc[cfIndex](t);
			if(val == 0x3f3f3f3f) continue;
			points.push_back({t, val});
		}

		

		SetDecalMode(olc::DecalMode::ADDITIVE);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		if(GetMouse(olc::Mouse::LEFT).bPressed)
		{
			cfIndex = (cfIndex + 1) % FUNC_NUM;

			points.clear();
			for(float t = start; t <= end; t += precision)
			{

				float val = currfnc[cfIndex](t);
				if(val == 0x3f3f3f3f) continue;
				points.push_back({t, val});
			}
			done = false;
		}

		if(!done)
		{
		Clear(olc::BLACK);

		DrawLine({0, 100}, {1000, 100}, olc::Pixel(255, 255, 255, 100));
		DrawLine({0, 700}, {1000, 700}, olc::Pixel(255, 255, 255, 100));

		for(auto point : points)
		{
			DrawLine({(int)(point.x * mult + ScreenWidth() / 2), 100}, {(int)(point.y * mult + ScreenWidth() / 2), 700}, olc::Pixel(250, 170, 85, 70));
		}
		
		DrawLine({ScreenWidth() / 2, 0}, {ScreenWidth() / 2, 800}, olc::Pixel(255, 0, 0, 100));

		done = true;
		}
		return true;
	}
};

int main()
{
	Example demo;
	if (demo.Construct(1000, 800, 1, 1))
		demo.Start();
	return 0;
}