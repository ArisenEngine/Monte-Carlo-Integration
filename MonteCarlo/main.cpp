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

void quads_glossy(hittable_list& world) {
	

	// Materials
	auto left_image = make_shared<image_texture>("left.jpg");
	auto left = make_shared<diffuse_light>(left_image);
	auto right_image = make_shared<image_texture>("right.jpg");
	auto right = make_shared<diffuse_light>(right_image);

	auto front_image = make_shared<image_texture>("front.jpg");
	auto front = make_shared<diffuse_light>(front_image);
	auto back_image = make_shared<image_texture>("back.jpg");
	auto back = make_shared<diffuse_light>(back_image);
	
	auto top_image = make_shared<image_texture>("top.jpg");
	auto top = make_shared<diffuse_light>(top_image);
	auto bottom_image = make_shared<image_texture>("bottom.jpg");
	auto bottom = make_shared<diffuse_light>(bottom_image);

	const int size = 40;

	// Materials
	
	// Quads
	world.add(make_shared<quad>(point3(-size/2., size/2., size/2.), vec3(0, -size, 0), vec3(0, 0, -size), left));
	world.add(make_shared<quad>(point3(size / 2., -size / 2., size / 2.), vec3(0, size, 0), vec3(0, 0, -size), right));

	world.add(make_shared<quad>(point3(-size / 2., size / 2., size / 2.), vec3(size, 0, 0), vec3(0, -size, 0), bottom));
	world.add(make_shared<quad>(point3(-size / 2., -size / 2., -size / 2.), vec3(size, 0, 0), vec3(0, size, 0), top));
	
	world.add(make_shared<quad>(point3(size / 2., size / 2., size / 2.), vec3(-size, 0, 0), vec3(0, 0, -size), back));
	world.add(make_shared<quad>(point3(-size / 2., -size / 2., -size / 2.), vec3(size, 0, 0), vec3(0, 0, size), front));

	// sphere 
	auto metal_sphere0 = make_shared<metal>(color(227 / 255., 181 / 255., 14 / 255.), 0.1);
	auto metal_sphere1 = make_shared<metal>(color(227 / 255., 181 / 255., 14 / 255.), 0.5);
	auto metal_sphere2 = make_shared<metal>(color(227 / 255., 181 / 255., 14 / 255.), 0.9);

	world.add(make_shared<sphere>(point3(-15, 0, 0), 5, metal_sphere0));
	world.add(make_shared<sphere>(point3(0, 0, 0), 5, metal_sphere1));
	world.add(make_shared<sphere>(point3(15, 0, 0), 5, metal_sphere2));
}

void quads_lambertian(hittable_list& world) {

	const int size = 40000;

	// Quads
	auto quad_color = make_shared<lambertian>(color(0.5, 0.2, 0.3));
	world.add(make_shared<quad>(point3(-size / 2., 0, -size / 2.), vec3(size, 0, 0), vec3(0, 0, size), quad_color));


	auto light = make_shared<diffuse_light>(color(2, 2, 2));
	world.add(make_shared<quad>(point3(-2., 15, -2.), vec3(4, 0, 0), vec3(0, 0, 4), light));

	// sphere 
	auto lambertian_sphere = make_shared<lambertian>(color(0.7, 0.7, 0.7));
	world.add(make_shared<sphere>(point3(0, 3, 0), 3, lambertian_sphere));

}

int main(int argc, char* argv[])
{

#if _DEBUG

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#endif

	hittable_list world;
	
	int scene_selection = -1;

	std::cout << " Input a number(0 - 1): ";
	std::cin >> scene_selection;
	switch (scene_selection)
	{
	case 0:
		quads_lambertian(world);
		break;
		
	case 1:
		quads_glossy(world);
		break;
	default:
		return 0;
	}

	const float aspect_ratio = 1;
	const int height = 500;
	const int tile_count = 4;
	const int width = (int)(height * aspect_ratio);
	
	int keyCode = 0;

	//world = hittable_list(make_shared<bvh_node>(world));

	std::vector<camera*> cameras;
	cameras.emplace_back(
		new camera(
			"uniform samples",   // name
			vec3(0, 0, 0),              // pos
			0.1, 10000,                 // near far
			20,                         // fov
			width, height,              // buffer size
			10,                         // bounce
			100,                         // samples,
			false                        // importance sampling
		));

	cameras.emplace_back(
		new camera(
			"importance samples",       // name
			vec3(0, 0, 0),              // pos
			0.1, 10000,                 // near far
			20,                         // fov
			width, height,              // buffer size
			10,                         // bounce
			100,                          // samples
			true                        // importance sampling
		));

	for (auto& _camera : cameras)
	{
		_camera->set_defocus_angle(0.0);
		_camera->set_fov(20.0);
		_camera->set_position(vec3(0, 10, 20));
		_camera->look_at(vec3(0, 2.5, 0));
		_camera->sample_count = 50;
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


























