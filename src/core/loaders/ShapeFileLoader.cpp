//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of SimTaDyn.
//
// SimTaDyn is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SimTaDyn.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#include "ShapeFileLoader.hpp"

// ESRI Shapefile Technical Description:
// https://www.esri.com/library/whitepapers/pdfs/shapefile.pdf

uint32_t ShapefileLoader::readBigEndianInt()
{
  char buf[4];

  m_infile.read(buf, 4);
  return static_cast<uint32_t>(((buf[0] & 0xFF) << 24) | ((buf[1] & 0xFF) << 16) | ((buf[2] & 0xFF) << 8) | (buf[3] & 0xFF));
}

uint32_t ShapefileLoader::readLittleEndianInt()
{
  char buf[4];

  m_infile.read(buf, 4);
  return static_cast<uint32_t>(((buf[3] & 0xFF) << 24) | ((buf[2] & 0xFF) << 16) | ((buf[1] & 0xFF) << 8) | (buf[0] & 0xFF));
}

double ShapefileLoader::readDouble()
{
  union {
    char buf[8];
    double value;
  } u;
  double convert;

  m_infile.read(u.buf, 8);
  convert = u.value;
  return convert;
}

float ShapefileLoader::readDoubleCastedFloat()
{
  double val = readDouble();
  return static_cast<float>(val);
}

void ShapefileLoader::goToByte(const uint32_t offset)
{
  m_infile.seekg(offset, std::ios::beg);
  if (!m_infile)
    {
      LOGF("ShapefileLoaderException");
      LoaderException e("Failed parsing the shapefile '" + m_filename
                        + "' at offset " + std::to_string(offset));
      throw e;
    }
}

void ShapefileLoader::skypeNBytes(const uint32_t offset)
{
  m_infile.seekg(offset, std::ios::cur);
  if (!m_infile)
    {
      LOGF("ShapefileLoaderException");
      LoaderException e("Failed parsing the shapefile '" + m_filename
                        + "' at offset " + std::to_string(offset));
      throw e;
    }
}

const std::string ShapefileLoader::shapeTypes(const uint32_t id)
{
  std::string shape;

  switch (id)
    {
    case 0:
      shape = "Null Shape";
      break;
    case 1:
      shape = "Point";
      break;
    case 3:
      shape = "Poly Line";
      break;
    case 5:
      shape = "Polygon";
      break;
    case 8:
      shape = "MultiPoint";
      break;
    case 11:
      shape = "PointZ";
      break;
    case 13:
      shape = "PolyLineZ";
      break;
    case 15:
      shape = "PolygonZ";
      break;
    case 18:
      shape = "MultiPointZ";
      break;
    case 21:
      shape = "PointM";
      break;
    case 23:
      shape = "PolyLineM";
      break;
    case 25:
      shape = "PolygonM";
      break;
    case 28:
      shape = "MultiPointM";
      break;
    case 31:
      shape = "MultiPatch";
      break;
    default:
      shape = "Wrong match found";
      break;
    }
  return shape;
}

void ShapefileLoader::checkFileSize()
{
  uint32_t value32b = 0;

  // Get the file size from the file system
  m_infile.seekg(0, std::ios::end);
  m_filelength = m_infile.tellg();
  m_infile.seekg(0, std::ios::beg);

  // Malformed file
  if (m_filelength <= 0)
    goto l_err;

  // Get the file size stored inside the file
  goToByte(24);
  value32b = readBigEndianInt();
  value32b = value32b * static_cast<uint32_t>(sizeof (uint16_t));

  // Wellformed size
  if (value32b == m_filelength)
    {
      //success:
      //std::cout << "File Length: " << m_filelength << std::endl;
      return ;
    }

l_err:
  LOGF("ShapefileLoaderBadLength");
  std::string msg("Incorrect Shapefile size. Expected ");
  msg += std::to_string(m_filelength);
  msg += " bytes but detected ";
  msg += std::to_string(value32b);
  msg += " bytes";
  LoaderException e(msg);
  throw e;
}

void ShapefileLoader::openShapeFile(const std::string& filename)
{
  uint32_t value32b;

  m_filelength = 0;
  m_filename = filename;

  m_infile.open(filename, std::ios::binary | std::ios::in);
  if (!m_infile)
    {
      LOGF("ShapefileLoaderOpenFailed");
      LoaderException e("Failed opening the Shapefile '" + filename + "': " + strerror(errno));
      throw e;
    }

  checkFileSize();

  goToByte(0U);
  value32b = readBigEndianInt();
  if (9994u != value32b)
    {
      LOGF("ShapefileLoaderBadId");
      LoaderException e("Bad Shapefile ID: read " + std::to_string(value32b) + " instead of 9994");
      throw e;
    }
}

uint32_t ShapefileLoader::getShapeVersion()
{
  goToByte(28U);
  return readLittleEndianInt();
}

uint32_t ShapefileLoader::getShapeType()
{
  goToByte(32U);
  return readLittleEndianInt();
}

void ShapefileLoader::getBoundingBox(Vector3f& bbox_min, Vector3f& bbox_max)
{
  goToByte(36U);
  bbox_min.x = readDoubleCastedFloat();
  bbox_min.y = readDoubleCastedFloat();
  bbox_max.x = readDoubleCastedFloat();
  bbox_max.y = readDoubleCastedFloat();
  bbox_min.z = readDoubleCastedFloat();
  bbox_max.z = readDoubleCastedFloat();
}

void ShapefileLoader::getBoundingBox(AABB3f& bbox)
{
  goToByte(36U);
  bbox.m_bbmin.x = readDoubleCastedFloat();
  bbox.m_bbmin.y = readDoubleCastedFloat();
  bbox.m_bbmax.x = readDoubleCastedFloat();
  bbox.m_bbmax.y = readDoubleCastedFloat();
  bbox.m_bbmin.z = readDoubleCastedFloat();
  bbox.m_bbmax.z = readDoubleCastedFloat();
}

uint32_t ShapefileLoader::getRecordAt(SimTaDynSheet& /*sheet*/, const uint32_t offset)
{
  uint32_t record_number, content_length, shape_type;

  goToByte(offset);
  record_number = readBigEndianInt();
  content_length = readBigEndianInt() * static_cast<uint32_t>(sizeof (uint16_t));
  shape_type = readLittleEndianInt();

  //std::cout << "Record Number: " << record_number << ", Content Length: " << content_length << ":" << std::endl;
  //std::cout << "  Shape " << record_number - 1U << " (" << shapeTypes(shape_type) << "): ";
  (void) record_number;

  switch (shape_type)
    {
    case 1: // Point
      m_point.x = readDoubleCastedFloat();
      m_point.y = readDoubleCastedFloat();
      m_point.z = 0.0f;
      //sheet.addNode(m_point);
      std::cerr<<"addNode not implemented" << std::endl;
      break;
    case 11: // PointZ
      m_point.x = readDoubleCastedFloat();
      m_point.y = readDoubleCastedFloat();
      m_point.z = readDoubleCastedFloat();
      //sheet.addNode(m_point);
      std::cerr<<"addNode not implemented" << std::endl;
      break;
    default:
      std::cout << "  Shape " << shapeTypes(shape_type) << " not yet managed. Ignored !" << std::endl;
      skypeNBytes(content_length);
      break;
    }
  return content_length + 8U; // 8U: record header
}

void ShapefileLoader::getAllRecords(SimTaDynSheet& sheet)
{
  uint32_t content_length;
  uint32_t offset = 100U;

  while (offset < m_filelength)
    {
      if (m_infile.eof())
        break;

      content_length = getRecordAt(sheet, offset);
      offset += content_length;
    }
}

void ShapefileLoader::loadFromFile(std::string const& filename, SimTaDynSheetPtr &current_sheet)
{
  bool dummy_sheet = (nullptr == current_sheet);

  LOGI("Loading the shapefile '%s' in an %s", filename.c_str(),
       (dummy_sheet ? "dummy map" : "already opened map"));

  // Create a map
  /*FIXME if (dummy_sheet)
    {
      current_sheet = SimTaDynMapManager::instance().create(filename, false);
      if (nullptr == current_sheet)
        {
          LoaderException e("Already existing");
          throw e;
        }
        }*/

  // Create a sheet or update
  try
    {
      openShapeFile(filename);

      uint32_t value32b = getShapeVersion();
      if (1000 != value32b)
        {
          LOGWS("Expected shapefile version 1000 but found %u: the file '%s' may be not fully interpreted",
                value32b, filename.c_str());
        }

      value32b = getShapeType();
      LOGI("Shapefile Type: %u: %s", value32b, shapeTypes(value32b).c_str());

      std::string shortname = File::fileName(filename);
      SimTaDynSheet *sheet = new SimTaDynSheet(shortname);

      getBoundingBox(sheet->m_bbox);
      // FIXME CPP_LOG(logger::Info) << "Map Bounding Box: " << sheet->m_bbox << std::endl;

      getAllRecords(*sheet);
      m_infile.close();

      if (dummy_sheet)
        {
          //FIXME current_sheet = sheet;
        }
      else
        {
          // Concat the old sheet with the new one: elements, name and bounding box
          //FIXME current_sheet += *sheet;
          current_sheet->m_bbox = merge(current_sheet->m_bbox, sheet->m_bbox); // TODO a mettre dans le code de +=

          if (sheet->m_name != "") // TODO a mettre dans le code de += avec option
            sheet->m_name += "_";
          sheet->m_name += shortname;
        }
    }
  catch (std::exception const &e)
    {
      m_infile.close();
      throw e;
    }
}
