#include <raylib.h>
#include <cmath>
#include <math.h>
#include <iostream>
#include <algorithm>

class Ball

{

public:
	Vector2 position;
	Vector2 velocity;
};


static float SmoothingFunction(float radius, float distance)
{
	if (distance > radius) {
		return 0;
	}
	float volume = 4.0f * 3.14 * radius * radius * radius * 0.33f;
	float diff = std::max(0.0f, radius*radius - distance);
	return (diff * diff )/volume;
}

float FindDensity(Vector2 Position,float rad,Ball balls[])
{
	float density = 0;
	const float mass = 1;
	for (int i = 0;i < 50;i++) {
		
		float distanceX = balls[i].position.x - Position.x;
		float distanceY = balls[i].position.y - Position.y;
		float dist = (sqrtf((distanceX * distanceX) + (distanceY * distanceY)));
		float influence = SmoothingFunction(rad, dist);
		density += mass * influence;
	}
	
	return density;
}

int main()
{
	const int SCREENWIDTH = 1080;
	const int SCREENHEIGHT = 720;

	InitWindow(SCREENWIDTH, SCREENHEIGHT, "Fluid_Sim2D");
	SetTargetFPS(60);
	float gravity = 98.0f;
	float dt = GetFrameTime();
	const int NumberParticle = 50;
	Ball ball[NumberParticle];

	int ParticleSize = 10;
	int ParticleSpacing = 6;
	int partPerRow = sqrt(NumberParticle);
	int partPerCol = (NumberParticle - 1) / partPerRow + 1;

	float spacing = 2 * ParticleSize + ParticleSpacing;


	for (int i = 0;i < NumberParticle;i++)
	{
		float x = GetRandomValue(100,1000) + (i % partPerRow - partPerRow / 2.0f + 0.5f) * spacing;
		float y = GetRandomValue(100,600) + (i / partPerRow - partPerCol / 2.0f + 0.5f) * spacing;
		ball[i].position = {x,y};
		ball[i].velocity = { 0,0 };
	}
	float radius = 20.0f;
	Vector2 PositionDensity = { 500,600 };

	float dampingVelocity = 0.98f;


	while (!WindowShouldClose())
	{
		float dt = GetFrameTime();
		
		for (int i = 0;i < NumberParticle;i++) {
			ball[i].velocity.y += gravity * dt;
			ball[i].position.y += ball[i].velocity.y * dt;
		}

		for (int i = 0;i < NumberParticle;i++) {
			if (ball[i].position.y > SCREENHEIGHT - 20) {
				ball[i].position.y = SCREENHEIGHT - 20;
				ball[i].velocity.y *= -1 * dampingVelocity;
			}
		}
		float density = FindDensity({ PositionDensity }, radius, ball) ;
			std::cout << density << std::endl;
		BeginDrawing();
		ClearBackground(BLACK);
		DrawRectangleLinesEx({ 2,2,SCREENWIDTH - 5,SCREENHEIGHT - 5 }, 5, YELLOW);
		for (int i = 0;i < NumberParticle;i++) {
			DrawCircleV(ball[i].position, ParticleSize, SKYBLUE);
		}
		DrawCircleLinesV({ PositionDensity }, radius, RED);
		DrawText(TextFormat("%f",density), 800, 100, 20, WHITE);
		
		EndDrawing();


	}
	CloseWindow();

	return 0;

}