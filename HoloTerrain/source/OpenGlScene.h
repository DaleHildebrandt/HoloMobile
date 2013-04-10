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
  //static void updateFromViconPosition(ViconDataStreamClient *viconClient);

  static void rotateScene(bool* previousButtonStates);
  static void updateTranslation(float x, float y, float z);
  static void setCurrTexture(int i);

private:

  static int currTexture;

  static bool                    m_isStylusEnabled;
  static zspace::common::Matrix4 m_stylusTransform;
  static float                   m_cubeHalfSize;
  static bool					 white;
  static bool					 red;
  static float					 r;
  static float					 g;
  static float					 b;
  static float					 xTrans;
  static float					 yTrans;
  static float					 zTrans;
  //static zspace::common::Vector3 cubePos;

private:
  static bool createTextureFromBmp(const char* filename);
  static void renderText();
  static void renderNoTexCube();
  static void renderCube();
  static void renderAxes();
  static void renderStylus();
  static void loadModel();
  static void recursive_render (const struct aiScene *sc, const struct aiNode* nd);
};
//OpenGLScene::loader = new ModelLoader();

#endif // __ZSPACE_OPEN_GL_SCENE_H__