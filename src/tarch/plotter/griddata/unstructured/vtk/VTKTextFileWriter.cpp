#ifndef PRECICE_NO_MPI
#include "mpi.h"
#endif
#include "tarch/plotter/griddata/unstructured/vtk/VTKTextFileWriter.h"

#include <stdio.h>
#include <fstream>
#include <iomanip>

tarch::logging::Log tarch::plotter::griddata::unstructured::vtk::VTKTextFileWriter::_log( "tarch::plotter::griddata::unstructured::vtk::VTKTextFileWriter" );


const std::string tarch::plotter::griddata::unstructured::vtk::VTKTextFileWriter::HEADER =
"# vtk DataFile Version 2.0\n "\
"Generated by Peano2 output component $Revision: 1.2 $ Author: Tobias Weinzierl\n "\
"ASCII\n ";


tarch::plotter::griddata::unstructured::vtk::VTKTextFileWriter::VTKTextFileWriter(const int precision):
  _writtenToFile(false),
  _precision(precision),
  _doubleOrFloat(setDoubleOrFloatString(precision)),
  _numberOfVertices(0),
  _numberOfCells(0),
  _numberOfCellEntries(0) {}


tarch::plotter::griddata::unstructured::vtk::VTKTextFileWriter::~VTKTextFileWriter() {
  if (!_writtenToFile) {
    assertion( _numberOfVertices==0, "Still vertices in vtk writer pipeline. Maybe you forgot to call writeToFile() on a data vtk writer?" );
    assertion( _numberOfCells==0, "Still cells in vtk writer pipeline. Maybe you forgot to call writeToFile() on a data vtk writer?" );
    assertion( _numberOfCellEntries==0, "Still cell entries in vtk writer pipeline. Maybe you forgot to call writeToFile() on a data vtk writer?" );
  }
}


void tarch::plotter::griddata::unstructured::vtk::VTKTextFileWriter::clear() {
  _writtenToFile       = false;
  _numberOfVertices    = 0;
  _numberOfCells       = 0;
  _numberOfCellEntries = 0;
  _vertexDescription      = "";
  _cellDescription        = "";
  _cellTypeDescription    = "";
  _vertexDataDescription  = "";
  _cellDataDescription    = "";
}


void tarch::plotter::griddata::unstructured::vtk::VTKTextFileWriter::writeToFile( const std::string& filename ) {
  assertion( !_writtenToFile );

  std::ofstream out;
  out.open( filename.c_str() );
  if ( (!out.fail()) && out.is_open() ) {
    _log.debug( "close()", "opened data file " + filename );
    out << std::setprecision(_precision);

    out << HEADER << std::endl << std::endl;

    out << "DATASET UNSTRUCTURED_GRID" << std::endl
        << "POINTS " << _numberOfVertices << " " << _doubleOrFloat << std::endl << std::endl;
    out << _vertexDescription << std::endl << std::endl;

    out << "CELLS " << _numberOfCells
        << " " << _numberOfCellEntries << std::endl << std::endl;
    out << _cellDescription << std::endl << std::endl;

    out << "CELL_TYPES " << _numberOfCells << std::endl << std::endl;
    out << _cellTypeDescription << std::endl << std::endl;

    if (_numberOfVertices>0 && !_vertexDataDescription.empty()) {
      out << "POINT_DATA " << _numberOfVertices << std::endl << std::endl;
      out << _vertexDataDescription << std::endl << std::endl;
    }

    if (_numberOfCells>0 && !_cellDataDescription.empty() ) {
      out << "CELL_DATA " << _numberOfCells << std::endl << std::endl;
      out << _cellDataDescription << std::endl << std::endl;
    }

    _log.debug( "close()", "data written to " + filename );
  }
  else {
  	_log.error( "close()", "unable to write output file " + filename );
  }

  _writtenToFile = true;
}


bool tarch::plotter::griddata::unstructured::vtk::VTKTextFileWriter::isOpen() {
  return !_writtenToFile;
}


tarch::plotter::griddata::unstructured::UnstructuredGridWriter::VertexWriter*
tarch::plotter::griddata::unstructured::vtk::VTKTextFileWriter::createVertexWriter() {
  return new tarch::plotter::griddata::unstructured::vtk::VTKTextFileWriter::VertexWriter(*this);
}


tarch::plotter::griddata::unstructured::UnstructuredGridWriter::CellWriter*
tarch::plotter::griddata::unstructured::vtk::VTKTextFileWriter::createCellWriter() {
  return new tarch::plotter::griddata::unstructured::vtk::VTKTextFileWriter::CellWriter(*this);
}


void tarch::plotter::griddata::unstructured::vtk::VTKTextFileWriter::validateDataWriterIdentifier( const std::string& identifier ) const {
  if (identifier.empty()) {
    logWarning(
      "validateDataWriterIdentifier(string)",
      "identifier for vtk file is empty. Spaces are not allowed for vtk data field identifiers and some vtk visualisers might crash."
    );
  }
  if (identifier.find(' ')!=std::string::npos) {
    logWarning(
      "validateDataWriterIdentifier(string)",
      "identifier \"" << identifier << "\" contains spaces. Spaces are not allowed for vtk data field identifiers and some vtk visualisers might crash."
    );
  }
}


tarch::plotter::griddata::Writer::CellDataWriter*    tarch::plotter::griddata::unstructured::vtk::VTKTextFileWriter::createCellDataWriter( const std::string& identifier, int recordsPerCell ) {
  validateDataWriterIdentifier(identifier);
  return new tarch::plotter::griddata::unstructured::vtk::VTKTextFileWriter::CellDataWriter(identifier,*this, recordsPerCell);
}


tarch::plotter::griddata::Writer::VertexDataWriter*  tarch::plotter::griddata::unstructured::vtk::VTKTextFileWriter::createVertexDataWriter( const std::string& identifier, int recordsPerVertex ) {
  validateDataWriterIdentifier(identifier);
  return new tarch::plotter::griddata::unstructured::vtk::VTKTextFileWriter::VertexDataWriter(identifier,*this, recordsPerVertex);
}
