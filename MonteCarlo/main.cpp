#include "./opencv2/opencv.hpp"
#include "./opencv2/core/opengl.hpp"
#include "./opencv2/highgui/highgui.hpp"
#include "./opencv2/highgui/highgui_c.h"
#include "rt_weekend.h"
#include <cmath>
#include "vec3.h"
#include "ray.h"
#include "interval.h"
#include "material.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "hittable_list.h"
#include "sphere.h"
#include "bvh_node.h"
#include "texture.h"
#include "quad.h"
#include "constant_medium.h"

#if _DEBUG
#pragma comment(lib,"opencv_world481d.lib")
#else
#pragma comment(lib,"opencv_world481.lib")
#endif


void quads(hittable_list& world) {
	

	// Materials
	auto left_red = make_shared<lambertian>(color(1.0, 0.2, 0.2));
	auto back_green = make_shared<lambertian>(color(0.2, 1.0, 0.2));
	auto right_blue = make_shared<lambertian>(color(0.2, 0.2, 1.0));
	auto upper_orange = make_shared<lambertian>(color(1.0, 0.5, 0.0));
	auto lower_teal = make_shared<lambertian>(color(0.2, 0.8, 0.8));

	// Quads
	world.add(make_shared<quad>(point3(-3, -2, 5), vec3(0, 0, -4), vec3(0, 4, 0), left_red));
	world.add(make_shared<quad>(point3(-2, -2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
	world.add(make_shared<quad>(point3(3, -2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
	world.add(make_shared<quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
	world.add(make_shared<quad>(point3(-2, -3, 5), vec3(4, 0, 0), vec3(0, 0, -4), lower_teal));

}

int main(int argc, char* argv[])
{

#if _DEBUG

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#endif

	hittable_list world;
	
	quads(world);

	const float aspect_ratio = 1;
	const int height = 400;
	const int tile_count = 4;
	const int width = (int)(height * aspect_ratio);
	
	int keyCode = 0;

	world = hittable_list(make_shared<bvh_node>(world));

	std::vector<camera*> cameras;
	cameras.emplace_back(new camera("uniform random samples", vec3(0, 0, 0), 0.1, 10000, 60, width, height));
	cameras.emplace_back(new camera("cosine weight random samples", vec3(0, 0, 0), 0.1, 10000, 60, width, height));

	for (auto& _camera : cameras)
	{
		_camera->render(world, width / tile_count);
	}

	while (keyCode != 27)
	{
		for (auto& _camera : cameras)
		{
			_camera->present();
		}

		keyCode = cv::pollKey();
	}

	for (auto& _camera : cameras)
	{
		_camera->dispose();
		delete _camera;
	}

	cameras.clear();

	return 0;
}


























