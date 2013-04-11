//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2012 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       OpenGlScene.h
//  Content:    The OpenGlScene class for the zSpace OpenGl stereo samples.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_OPEN_GL_SCENE_H__
#define __ZSPACE_OPEN_GL_SCENE_H__
#include "Input/ViconDataStreamClient.h"
#include "ZSpace/Common/Math/Matrix4.h"
#include "Model/Calibrator.h"
#include "Model\Tablet.h"
#include "Model\Finger.h"

class OpenGlScene
{
public:
  static void initialize();
  static void render(bool msg, Calibrator* calibrator, Tablet* samsungTab, Finger* finger, Finger* thumb);

  static void setStylusVisualizationEnabled(bool isStylusEnabled);
  static void setStylusTransform(const zspace::common::Matrix4& stylusTransform);

  static void rotateScene(bool* previousButtonStates);

private:

  static bool                    m_isStylusEnabled;
  static zspace::common::Matrix4 m_stylusTransform;
  static float                   m_cubeHalfSize;
  static float					 r;
  static float					 g;
  static float					 b;

private:
  static bool createTextureFromBmp(const char* filename);
  static void renderText();
  static void renderNoTexCube();
  static void renderCube();
  static void renderAxes();
  static void renderStylus();
  static void loadModel();
};

#endif // __ZSPACE_OPEN_GL_SCENE_H__