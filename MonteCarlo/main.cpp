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

}

int main(int argc, char* argv[])
{

#if _DEBUG

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#endif

	hittable_list world;
	
	quads(world);

	const float aspect_ratio = 1;
	const int height = 500;
	const int tile_count = 4;
	const int width = (int)(height * aspect_ratio);
	
	int keyCode = 0;

	//world = hittable_list(make_shared<bvh_node>(world));

	std::vector<camera*> cameras;
	cameras.emplace_back(
		new camera(
			"uniform random samples",   // name
			vec3(0, 0, 0),              // pos
			0.1, 10000,                 // near far
			60,                         // fov
			width, height,              // buffer size
			10,                         // bounce
			10                          // samples
		));


	for (auto& _camera : cameras)
	{
		_camera->set_defocus_angle(0.0);
		_camera->set_fov(120.0);
		_camera->set_position(vec3(0, 0, -5));
		_camera->look_at(vec3(0, 0, 0));
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


























