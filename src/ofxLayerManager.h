#pragma once

#include "ofMain.h"
#include "ofxLayerConstants.h"

OFX_LAYER_BEGIN_NAMESPACE

class Layer;

class Manager
{
	friend class Layer;
	
public:
	
	Manager();
	~Manager() {}

	void setBackgroundAuto(bool v) { backgroundAuto = v; }
	
	ofFbo& getFramebuffer() { return frameBuffer; }
	
	void setup(int width = ::ofGetWidth(), int height = ::ofGetHeight());
	void update();
	void draw();
	
	template <typename T>
	T* createLayer(float defalut_alpha = 0)
	{
		T *layer = new T;
		layer->manager = this;
		
		string name = layer->getClassName();
		unsigned int class_id = Type2Int<T>::value();
		
		if (layer_class_id_map.find(class_id) != layer_class_id_map.end())
			throw runtime_error("layer must be unique");
		
		layers.push_back(layer);
		layer_class_id_map[class_id] = layer;
		layer_class_name_map[name] = layer;
		
		layer->layerSetup(width, height);
		layer->setAlpha(defalut_alpha);
		
		return layer;
	}
	
	void deleteLayer(Layer *layer);
	
	template <typename T>
	T* getLayer()
	{
		unsigned int class_id = Type2Int<T>::value();
		if (layer_class_id_map.find(class_id) == layer_class_id_map.end())
			return NULL;
		return (T*)layer_class_id_map[class_id];
	}
	
	Layer* getLayerByName(const string& name);
	int getLayerIndexByName(const string& name);
	
	void mute(int index);
	void mute(Layer *layer);
	void mute(const string& name);
	
	void solo(int index);
	void solo(Layer *layer);
	void solo(const string& name);
	
	vector<string> getLayerNames();
	const vector<Layer*>& getLayers();

protected:
	
	vector<Layer*> layers;
	map<string, Layer*> layer_class_name_map;
	map<unsigned int, Layer*> layer_class_id_map;
	
private:
	
	Manager(const Manager&);
	Manager& operator=(const Manager&);
	
	int width, height;
	
	bool backgroundAuto;
	ofFbo frameBuffer;
	ofFbo layerFrameBuffer;
};


OFX_LAYER_END_NAMESPACE
