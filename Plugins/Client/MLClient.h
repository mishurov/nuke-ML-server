// Copyright (c) 2019 Foundry.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//*************************************************************************

#ifndef MLCLIENT_H
#define MLCLIENT_H

// Standard plug-in include files.
#include "DDImage/NukeWrapper.h"
#include "DDImage/Row.h"
#include "DDImage/Thread.h"

// Local include files
#include "MLClientMixin.h"

//! The Machine Learning (ML) Client plug-in connects Nuke to a Python server to apply ML models to images.
/*! This plug-in can connect to a server (given a host and port), which responds
    with a list of available Machine Learning (ML) models and options.
    On every /a renderStripe() call, the image and model options are sent from Nuke to the server,
    there the server can process the image by doing Machine Learning inference,
    finally the resulting image is sent back to Nuke.
*/
class MLClient : public MLClientMixin<DD::Image::PlanarIop>
{

public:
  // Static consts
  static const char* const kClassName;
  static const char* const kHelpString;

public:
  //! Constructor. Initialize parent class.
  MLClient(Node* node);
  virtual ~MLClient();

public:
  // DDImage::Iop overrides

  //! The maximum number of input connections the operator can have.
  int maximum_inputs() const;
  //! The minimum number of input connections the operator can have.
  int minimum_inputs() const;
  /*! Return the text Nuke should draw on the arrow head for input \a input
      in the DAG window. This should be a very short string, one letter
      ideally. Return null or an empty string to not label the arrow.
  */
  const char* input_label(int input, char* buffer) const;

  bool useStripes() const;
  bool renderFullPlanes() const;

  void _validate(bool);
  void getRequests(const DD::Image::Box& box, const DD::Image::ChannelSet& channels, int count, DD::Image::RequestOutput &reqData) const;

  /*! This function is called by Nuke for processing the current image.
      The image and model options are sent from Nuke to the server,
      there the server can process the image by doing Machine Learning inference,
      finally the resulting image is sent back to Nuke.
      The function tries to reconnect if no connection is set.
  */
  void renderStripe(DD::Image::ImagePlane& imagePlane);

  //! Information to the plug-in manager of DDNewImage/Nuke.
  static const DD::Image::Iop::Description description;

  //! Return the name of the class.
  const char* Class() const;
  const char* node_help() const;

private:
  // Private functions

  //! Compute specific number of inputs, GeoOp uses one input as a material.
  int computed_inputs();
  //! Return a format of for sending to the server, GeoOp hasn't got output format.
  DD::Image::Box getFormat() const;
  //! Parse the response messge from the server, and if it contains
  //! an image, attempt to copy the image to the imagePlane. Return
  //! true on success, false otherwise and fill in the error string.
  bool renderOutputBuffer(mlserver::RespondWrapper& responseWrapper, DD::Image::ImagePlane& imagePlane, std::string& errorMsg);

};

#endif // MLCLIENT_H
