#include "SimTaDynLoaders.hpp"

void LoaderManager::registerAllLoaders()
{
  registerLoader(new ShapefileLoader(), "shp");
  registerLoader(new SimTaDynFileLoader(), "spak");
  //registerLoader(new TextureLoader(), "bmp:png:jpg:jpeg:tga:dds:psd:hdr");
}
