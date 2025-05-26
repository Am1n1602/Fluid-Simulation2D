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
	if (distance > radius*radius) {
		return 0;
	}
	float norm = 4.0f / (PI * pow(radius, 6));
	float diff = std::max(0.0f,radius*radius - distance);
	return diff * diff * norm;
}

float FindDensity(Vector2 Position,float rad,Ball balls[])
{
	float density = 0;
	const float mass = 1;
	for (int i = 0;i < 30;i++) {
		
		float distanceX = balls[i].position.x - Position.x;
		float distanceY = balls[i].position.y - Position.y;
		float dist = sqrt((distanceX * distanceX) + (distanceY * distanceY));
		std::cout << dist << std::endl;
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
	const int NumberParticle = 30;
	Ball ball[NumberParticle];

	int ParticleSize = 15;
	int ParticleSpacing = 12;
	int partPerRow = sqrt(NumberParticle);
	int partPerCol = (NumberParticle - 1) / partPerRow + 1;

	float spacing = 2 * ParticleSize + ParticleSpacing;


	for (int i = 0;i < NumberParticle;i++)
	{
		float x = 500+(i % partPerRow - partPerRow / 2.0f + 0.5f) * spacing;
		float y = 360+(i / partPerRow - partPerCol / 2.0f + 0.5f) * spacing;
		ball[i].position = {x,y};
		ball[i].velocity = { 0,0 };
	}

	float density = FindDensity({ 520,360 }, 23, ball)*1000;
	std::cout << density << std::endl;
	float dampingVelocity = 0.88f;


	while (!WindowShouldClose())
	{
		float dt = GetFrameTime();
		
	/*	for (int i = 0;i < NumberParticle;i++) {
			ball[i].velocity.y += gravity * dt;
			ball[i].position.y += ball[i].velocity.y * dt;
		}

		for (int i = 0;i < NumberParticle;i++) {
			if (ball[i].position.y > SCREENHEIGHT - 20) {
				ball[i].position.y = SCREENHEIGHT - 20;
				ball[i].velocity.y *= -1 * dampingVelocity;
			}
		}*/

		BeginDrawing();
		ClearBackground(BLACK);
		DrawRectangleLinesEx({ 2,2,SCREENWIDTH - 5,SCREENHEIGHT - 5 }, 5, YELLOW);
		for (int i = 0;i < NumberParticle;i++) {
			DrawCircleV(ball[i].position, ParticleSize, SKYBLUE);
		}
		DrawCircleLinesV({ 560,400 }, 100.0f, RED);
		DrawText(TextFormat("%f",density), 800, 100, 20, WHITE);
		
		EndDrawing();


	}
	CloseWindow();

	return 0;

}