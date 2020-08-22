#include "header/Window.h"
#include "header/Pipeline.h"
#include "header/OBJ_Loader.h"

using namespace std;

void addMesh(Scene& scene, vector<objl::Mesh> objMeshes) {
	for (auto& objMesh : objMeshes)
	{
		Mesh mesh;
		for (size_t i = 0; i < objMesh.Vertices.size(); i++)
		{
			auto objVert = objMesh.Vertices[i];
			Vertex v;
			v.point = Vector3(objVert.Position.X, objVert.Position.Y, objVert.Position.Z);
			v.normal = Vector3(objVert.Normal.X, objVert.Normal.Y, objVert.Normal.Z);
			v.texCoord = Vector2(objVert.TextureCoordinate.X, objVert.TextureCoordinate.Y);
			v.color = RGBColor(objVert.TextureCoordinate.X, objVert.TextureCoordinate.Y, 1.0f);
			mesh.vertices.push_back(v);
		}
		mesh.indices = objMesh.Indices;

		scene.addMesh(mesh);
	}
}


int	main(void) {

	IntBuffer colorBuffer(1280, 720);
	Pipeline pipeline(colorBuffer, 512);
	Window window(colorBuffer.get_width(), colorBuffer.get_height(), _T("JM Soft Renderer  "));

	// Load .obj File
	objl::Loader loader;
	const char* obj_path = "../../../../models/spot/_spot_triangulated_good.obj";
	if (!loader.LoadFile(obj_path)) {
		cout << "File loading failed!" << endl;
		return 1;
	}

	Scene scene;
	scene.setLight(
		Matrix().rotate(0, 1, 0, 70.0f).rotate(1, 0, 0, -60.0f).translate(-0.2f, 0.3f, 3.5f),
		4.0f, 4.0f, 100.0f
	);

	scene.setViewMatrix(Matrix().translate(0, 0, 2.5f));
	scene.setPerspective(60.0f, colorBuffer.get_aspect(), 0.1f, 100.0f);
	

	addMesh(scene, loader.LoadedMeshes);


	while (window.is_run())
	{
		pipeline.clearBuffers(Colors::Black);

		pipeline.renderShadowMap(scene);
		pipeline.renderMeshes(scene);

		memcpy(window(), colorBuffer(), colorBuffer.get_size() * sizeof(int));
		window.update();

		// input
		if (window.is_key(VK_ESCAPE)) window.destory();
		if (window.is_key('W')) scene.cameraTranslate(0.0f, -0.02f);
		if (window.is_key('S')) scene.cameraTranslate(0.0f, 0.02f);
		if (window.is_key('E')) scene.cameraTranslate(-0.02f, 0.0f);
		if (window.is_key('Q')) scene.cameraTranslate(0.02f, 0.0f);

		if (window.is_key('A')) scene.modelRotate(2.0f);
		else if (window.is_key('D')) scene.modelRotate(-2.0f);
		else scene.modelRotate(0.25f);
	}


	return 0;
}