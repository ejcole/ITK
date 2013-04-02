/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

//

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkFilterWatcher.h"

#include "itkDoubleThresholdImageFilter.h"


int itkDoubleThresholdImageFilterTest( int argc, char * argv[] )
{
  if( argc < 7 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << "  inputImageFile  ";
    std::cerr << " outputImageFile threshold1 threshold2 threshold3 threshold4 " << std::endl;
    return EXIT_FAILURE;
    }


  //
  //  The following code defines the input and output pixel types and their
  //  associated image types.
  //
  const unsigned int Dimension = 2;

  typedef unsigned char    InputPixelType;
  typedef unsigned char    OutputPixelType;
  typedef unsigned char    WritePixelType;

  typedef itk::Image< InputPixelType,  Dimension >   InputImageType;
  typedef itk::Image< OutputPixelType, Dimension >   OutputImageType;
  typedef itk::Image< WritePixelType, Dimension >    WriteImageType;


  // readers/writers
  typedef itk::ImageFileReader< InputImageType  >  ReaderType;
  typedef itk::ImageFileWriter< WriteImageType >   WriterType;
  typedef itk::RescaleIntensityImageFilter<OutputImageType, WriteImageType>
                                                   RescaleType;

  // define the fillhole filter
  typedef itk::DoubleThresholdImageFilter<
                            InputImageType,
                            OutputImageType >  DoubleThresholdFilterType;


  // Creation of Reader and Writer filters
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer  = WriterType::New();
  RescaleType::Pointer rescaler = RescaleType::New();

  // Create the filter
  DoubleThresholdFilterType::Pointer  threshold = DoubleThresholdFilterType::New();
  FilterWatcher watcher(threshold, "threshold");

  // Setup the input and output files
  reader->SetFileName( argv[1] );
  writer->SetFileName(  argv[2] );

  // Setup the fillhole method
  threshold->SetInput(  reader->GetOutput() );
  threshold->SetInsideValue( 255 );
  threshold->SetOutsideValue( 0 );
  threshold->SetThreshold1( atoi(argv[3]) );
  threshold->SetThreshold2( atoi(argv[4]) );
  threshold->SetThreshold3( atoi(argv[5]) );
  threshold->SetThreshold4( atoi(argv[6]) );

  // Run the filter
  rescaler->SetInput( threshold->GetOutput() );
  rescaler->SetOutputMinimum(   0 );
  rescaler->SetOutputMaximum( 255 );
  writer->SetInput( rescaler->GetOutput() );
  writer->Update();

  return EXIT_SUCCESS;
}
