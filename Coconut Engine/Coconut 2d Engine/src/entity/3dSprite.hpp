#ifndef SPRITE3D_HPP
#define SPRITE3D_HPP

#include <SFML/graphics.hpp>
#include <string>

#include "json.hpp"


//assets
constexpr char cube_gltf[] = {
#include "../_assets/cube.RAW_STR_gltf"
};


class Sprite3d {
private:
	json data;

	//scenes	   //parts of a gltf file
	//nodes		   //parts of a gltf file
	//cameras	   //parts of a gltf file
	//meshes	   //parts of a gltf file
	//buffers	   //parts of a gltf file
	//bufferViews  //parts of a gltf file
	//accessors    //parts of a gltf file
	//materials    //parts of a gltf file
	//textures	   //parts of a gltf file
	//images	   //parts of a gltf file
	//samplers	   //parts of a gltf file
	//skins		   //parts of a gltf file
	//animations   //parts of a gltf file

public:
	Sprite3d() { }
	Sprite3d(const char* source);

	json& GetJson() { //temp
		return this->data;
	}
};

#endif