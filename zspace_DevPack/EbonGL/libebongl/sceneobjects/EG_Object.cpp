#include "EG_Object.hpp"
#include "../EG_Polygon.hpp"
#include "../EG_Light.hpp"
#include "../EG_Utils.hpp"
#include "../EG_SceneElement.hpp"
#include "../EG_Color.hpp"
#include "../utilities/EG_LogWriter.hpp"
#include "../utilities/EG_String.hpp"

using namespace std;
using namespace EbonGL;
using namespace MHTypes;

unsigned int EG_Object::shadowStamper = 0;
const double EG_Object::POLYGON_OFFSET = 0.00001;

EG_Object::EG_Object(void)
{
    ambientDiffuse = new EG_Color;
    polygonMode = GL_FILL;
    drawingActive = false;
    shadowActive = false;
    clipPlanesUsed = 0;
    shadowCasting = true;
    shadowReceiving = true;
    isVisible = true;
	isChanged.resize(EG_Utils::getNumViewports(), true);
	scenePos = new EG_SceneElement;
	enableLighting = true;
    zOffset = 0;

    ambientDiffuse->setColor(1.0f, 0.66f, 0.33f, 1.0f);

    for (unsigned int i = 0; i < 4; i++)
        shadowPlane.push_back(new Point3D);

    setRotation(rotation);
}//constructor

EG_Object::EG_Object(const EG_Object &source)
{
    copyFrom(source);
}//copy constructor

EG_Object::~EG_Object(void)
{
    for (unsigned int i = 0; i < shadowPlane.size(); i++)
        delete shadowPlane.at(i);
    shadowPlane.clear();

	delete scenePos;
	scenePos = NULL;

    delete ambientDiffuse;
    ambientDiffuse = NULL;
}//destructor

EG_Object& EG_Object::operator= (const EG_Object &source)
{
    if (this != &source)
        copyFrom(source);

    return *this;
}//=

void EG_Object::copyFrom(const EG_Object &source)
{
	//### Copy shadowPlane ###
    for (unsigned int i = 0; i < shadowPlane.size(); i++)
        delete shadowPlane.at(i);
    shadowPlane.clear();

	for (unsigned int i = 0; i < source.shadowPlane.size(); i++)
		shadowPlane.push_back(new Point3D(*(source.shadowPlane.at(i))));

	//### Copy isChanged ###
	isChanged.resize(source.isChanged.size(), false);
	for (unsigned int i = 0; i < source.isChanged.size(); i++)
		isChanged.set(i, source.isChanged.get(i));

	//### Copy simple member variables ###
    ambientDiffuse->setColor(*source.ambientDiffuse);
	position = source.position;
	rotation = source.rotation;
	polygonMode = source.polygonMode;
	isVisible = source.isVisible;
	clipPlanesUsed = source.clipPlanesUsed;
	drawingActive = source.drawingActive;
	shadowActive = source.shadowActive;
	shadowCasting = source.shadowCasting;
	shadowReceiving = source.shadowReceiving;
	enableLighting = source.enableLighting;
    zOffset = source.zOffset;
}//copyFrom

void EG_Object::setLighting(bool enabled)
{
	setChanged();
	enableLighting = enabled;
}//setLighting

void EG_Object::setPosition(const Point3D &newPosition)
{
	setChanged();
    position = newPosition;
}//setPosition

void EG_Object::setRotation(const EulerAngle &newRotation)
{
	setChanged();
    rotation.copyFrom(newRotation);
}//setRotation

void EG_Object::setAmbientDiffuse(const EG_Color &newColor)
{
	setChanged();
    ambientDiffuse->setColor(newColor);
}//setAmbientDiffuse

EG_Color EG_Object::getAmbientDiffuse(void)
{
    EG_Color result(*ambientDiffuse);

    return result;
}//getAmbientDiffuse

void EG_Object::setPolygonMode(GLenum newMode)
{
	setChanged();
    polygonMode = newMode;
}//setPolygonMode

void EG_Object::startDrawing(bool enableTextures)
{
    //### Make sure we don't call startDrawing() twice in a row ###
    if (drawingActive)
    {
		EG_Utils::writeLog(EG_String("EG_Object::startDrawing(): drawing already started"));
        EG_Utils::fatalError();
    }//if drawingActive
    drawingActive = true;

    //### Move the object to the proper position ###
    glPushMatrix();
    glTranslated(position.x, position.y, position.z);
    glRotated(rotation.pitch, 1, 0, 0);
    glRotated(rotation.yaw, 0, 1, 0);
    glRotated(rotation.roll, 0, 0, 1);
    glTranslated(0, 0, zOffset);

    //### Set object properties ###
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
    glEnable(GL_COLOR_MATERIAL);

    if (enableTextures)
        glEnable(GL_TEXTURE_2D);
    else
        glDisable(GL_TEXTURE_2D);

    glColor4fv(ambientDiffuse->toArray());//seems to be necessary when other classes are using textures

	if (enableLighting)
    {
        glEnable(GL_LIGHTING);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ambientDiffuse->toArray());
    }//if enableLighting
	else
	{
		glDisable(GL_LIGHTING);
        glColor4fv(ambientDiffuse->toArray());
	}//else enableLighting

    //### Disable backface culling ###
    glCullFace(GL_BACK);
    glDisable(GL_CULL_FACE);
}//startDrawing

void EG_Object::finishDrawing(void)
{
    if (!drawingActive)
    {
		EG_Utils::writeLog(EG_String("EG_Object::finishDrawing(): drawing not started"));
        EG_Utils::fatalError();
    }//if !drawingActive

    drawingActive = false;
    glPopMatrix();
}//finishDrawing

bool EG_Object::startShadow(EG_Light *light,
                            EG_Object *surfaceObject,
                            const EG_Polygon *surface)
{
    Plane planeEquation;//computes the equation of the shadow plane
    Plane normalPlane;//the plane we'll use to compute the surface normal
    MHVector normal;//the surface normal
    MHVector revNormal;//the surface normal for counter-clockwise winding, needed by shadow-projection algorithm
    Point3D shadowPos;//the position to draw the shadow
    MHFloat xOffset = 0;//apply polygon offset to prevent flickering
    MHFloat yOffset = 0;
    MHFloat zOffset = 0;
    bool result = false;//true if the shadowPlane faces this object's center

    if ((shadowCasting) && (surfaceObject->shadowReceiving) && (surfaceObject->isVisible))
    {
        //### Make sure we don't call startShadow() twice in a row ###
        if (shadowActive)
        {
			EG_Utils::writeLog(EG_String("EG_Object::startShadow(): drawing already started"));
            EG_Utils::fatalError();
        }//if shadowActive
        shadowActive = true;

        //### Translate and rotate the surface, populates shadowPlane ###
        calcShadowPlane(surface, surfaceObject);

        //### Calculate the shadow plane's surface normal ###
        normalPlane.fromPoints(*shadowPlane.at(0), *shadowPlane.at(1), *shadowPlane.at(2));
        normal = normalPlane.surfaceNormal();

        normalPlane.fromPoints(*shadowPlane.at(2), *shadowPlane.at(1), *shadowPlane.at(0));
        revNormal = normalPlane.surfaceNormal();

        //### Calculate polygon offsets ###
		xOffset = normal.x * (POLYGON_OFFSET * shadowStamper) + (POLYGON_OFFSET * 20);
        yOffset = normal.y * (POLYGON_OFFSET * shadowStamper) + (POLYGON_OFFSET * 20);
        zOffset = normal.z * (POLYGON_OFFSET * shadowStamper) + (POLYGON_OFFSET * 20);
        shadowStamper++;

        //### Apply polygon offsets to our translation ###
        shadowPos.x = position.x + xOffset;
        shadowPos.y = position.y + yOffset;
        shadowPos.z = position.z + zOffset;

        //### Apply polygon offsets to the shadow plane ###
        for (unsigned int i = 0; i < shadowPlane.size(); i++)
        {
            shadowPlane.at(i)->x += xOffset;
            shadowPlane.at(i)->y += yOffset;
            shadowPlane.at(i)->z += zOffset;
        }//for i

        //### Calculate the general plane equation ###
        planeEquation.fromPoints(*shadowPlane.at(0), *shadowPlane.at(1), *shadowPlane.at(2));

        //### The surface is facing this object ###
        if (planeEquation.getSide(position) > 0)
        {
            result = true;

            addShadowClipping(surfaceObject, surface, normal);

            //### Perform the transformation ###
            glPushMatrix();
            shadowProjection(light->getPosition(), *shadowPlane.at(0), revNormal);

            glTranslated(shadowPos.x, shadowPos.y, shadowPos.z);
            glRotated(rotation.pitch, 1, 0, 0);
            glRotated(rotation.yaw, 0, 1, 0);
            glRotated(rotation.roll, 0, 0, 1);

            //### Set object properties ###
            glDisable(GL_LIGHTING);
            glColor4f(0.0, 0.0, 0.0, 0.5);
            glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
        }//if getSide()

        //### The surface is facing away from this object ###
        else
        {
            result = false;
            shadowActive = false;
        }//else getSide()
    }//if shadowCasting && shadowReceiving && isVisible
    else
        result = false;

    return result;
}//startShadow

void EG_Object::addShadowClipping(EG_Object *surfaceObject,
                                  const EG_Polygon *surface,
                                  const MHVector &normal)
{
    Plane planeEquation;//compute the general equation of the clipping plane
    Point3D edgePoint1;//two points making up the current surface edge
    Point3D edgePoint2;
    Point3D thirdPoint;//create a third point by extending along the surface normal
    Point3D surfacePosition = surfaceObject->getPosition();
    GLint maxClipPlanes = 0;//the maximum number of clipping planes available
    GLenum clipIndex = 0;//the OpenGL index for the current clipping plane
    unsigned int vertexIndex = 0;//the index of one of surface's vertices

    glGetIntegerv(GL_MAX_CLIP_PLANES, &maxClipPlanes);

    while ((vertexIndex < surface->getNumVertices()) && (vertexIndex < static_cast<unsigned int>(maxClipPlanes)))
    {
        //### Read two edge points ###
        if (vertexIndex == surface->getNumVertices() - 1)
        {
            edgePoint1 = surface->getVertex(vertexIndex);
            edgePoint2 = surface->getVertex(0);
        }//if vertexIndex
        else
        {
            edgePoint1 = surface->getVertex(vertexIndex);
            edgePoint2 = surface->getVertex(vertexIndex + 1);
        }//else vertexIndex

        //### Translate to the object position ###
        edgePoint1.x += surfacePosition.x;
        edgePoint1.y += surfacePosition.y;
        edgePoint1.z += surfacePosition.z;

        edgePoint2.x += surfacePosition.x;
        edgePoint2.y += surfacePosition.y;
        edgePoint2.z += surfacePosition.z;

        //### Create a third point in the clipping plane ###
        thirdPoint.copyFrom(edgePoint2);
        thirdPoint.x -= normal.x;
        thirdPoint.y -= normal.y;
        thirdPoint.z -= normal.z;

        //### Calculate the plane equation ###
        planeEquation.fromPoints(edgePoint1, edgePoint2, thirdPoint);

        //### Create the clipping plane ###
        clipIndex = getGlClipPlane(vertexIndex);
        glClipPlane(clipIndex, &planeEquation.getEquation().at(0));
        glEnable(clipIndex);

        vertexIndex++;
    }//while vertexIndex && vertexIndex

    clipPlanesUsed = vertexIndex;
}//addShadowClipping

void EG_Object::finishShadow(void)
{
    if (!shadowActive)
    {
		EG_Utils::writeLog(EG_String("EG_Object::finishDrawing(): drawing not started"));
        EG_Utils::fatalError();
    }//if !shadowActive

    //### Disable clipping planes ###
    for (unsigned int i = 0; i < clipPlanesUsed; i++)
        glDisable(getGlClipPlane(i));
    clipPlanesUsed = 0;

    //### Restore matrix and properties ###
    glEnable(GL_LIGHTING);
    shadowActive = false;
    glPopMatrix();
}//finishShadow

void EG_Object::shadowProjection(const Point3D &lightPos,
                              const Point3D &surfacePoint,
                              const MHVector &surfaceNormal)
{
    MHFloat c = 0;//constants used to populate the matrix
    MHFloat d = 0;
    vector<GLfloat> matrix;//the transformation matrix to create 2D shadows

    matrix.resize(16, 0);

    //### Compute the constants ###
    d = (surfaceNormal.x * lightPos.x) + (surfaceNormal.y * lightPos.y) + (surfaceNormal.z * lightPos.z);
    c = (surfacePoint.x * surfaceNormal.x) + (surfacePoint.y * surfaceNormal.y) + (surfacePoint.z * surfaceNormal.z) - d;

    //### Create the matrix using column-major ordering ###
    matrix.at(0)  = static_cast<GLfloat>(lightPos.x * surfaceNormal.x + c);
    matrix.at(4)  = static_cast<GLfloat>(surfaceNormal.y * lightPos.x);
    matrix.at(8)  = static_cast<GLfloat>(surfaceNormal.z * lightPos.x);
    matrix.at(12) = static_cast<GLfloat>(-lightPos.x * c - lightPos.x * d);

    matrix.at(1)  = static_cast<GLfloat>(surfaceNormal.x * lightPos.y);
    matrix.at(5)  = static_cast<GLfloat>(lightPos.y * surfaceNormal.y + c);
    matrix.at(9)  = static_cast<GLfloat>(surfaceNormal.z * lightPos.y);
    matrix.at(13) = static_cast<GLfloat>(-lightPos.y * c - lightPos.y * d);

    matrix.at(2)  = static_cast<GLfloat>(surfaceNormal.x * lightPos.z);
    matrix.at(6)  = static_cast<GLfloat>(surfaceNormal.y * lightPos.z);
    matrix.at(10) = static_cast<GLfloat>(lightPos.z * surfaceNormal.z + c);
    matrix.at(14) = static_cast<GLfloat>(-lightPos.z * c - lightPos.z * d);

    matrix.at(3)  = static_cast<GLfloat>(surfaceNormal.x);
    matrix.at(7)  = static_cast<GLfloat>(surfaceNormal.y);
    matrix.at(11) = static_cast<GLfloat>(surfaceNormal.z);
    matrix.at(15) = static_cast<GLfloat>(-d);

    //### Apply the matrix ###
    glMultMatrixf(&matrix.at(0));
}//shadowProjection

void EG_Object::calcShadowPlane(const EG_Polygon *surface,
                                EG_Object *shadowObject)
{
    Point3D onePoint;//a single point from the surface
    Matrix3x3 surfaceRotate;//rotates surface points to the surface's orientation

    surfaceRotate.fromEuler(shadowObject->getRotation());

    for (unsigned int i = 0; i < 3; i++)
    {
        //### Rotate the point ###
        onePoint.copyFrom(surface->getVertex(i));
        onePoint = surfaceRotate.rotatePoint(onePoint);

        //### Translate to the surface's position ###
        onePoint.x += shadowObject->getPosition().x;
        onePoint.y += shadowObject->getPosition().y;
        onePoint.z += shadowObject->getPosition().z;

        shadowPlane.at(i)->copyFrom(onePoint);
    }//for i

	EG_Polygon testPolygon;
	vector<GLdouble> testVertices;

	for (unsigned int i = 0; i < shadowPlane.size(); i++)
	{
		testVertices.push_back(shadowPlane.at(i)->x);
		testVertices.push_back(shadowPlane.at(i)->y);
		testVertices.push_back(shadowPlane.at(i)->z);
	}//for i
	testPolygon.setVertices(testVertices);

	testPolygon.addIndex(0);
	testPolygon.addIndex(1);
	testPolygon.addIndex(2);

	testPolygon.draw(GL_POLYGON);
}//calcShadowSurface

GLenum EG_Object::getGlClipPlane(unsigned int index)
{
    GLenum result = 0;//the clipping plane to return

    switch (index)
    {
        case 0:
            result = GL_CLIP_PLANE0;
            break;

        case 1:
            result = GL_CLIP_PLANE1;
            break;

        case 2:
            result = GL_CLIP_PLANE2;
            break;

        case 3:
            result = GL_CLIP_PLANE3;
            break;

        case 4:
            result = GL_CLIP_PLANE4;
            break;

        case 5:
            result = GL_CLIP_PLANE5;
            break;

        default:
			EG_Utils::addLog(EG_String("EG_Object::glClipIndex(): unhandled index "));
            EG_Utils::writeLog(EG_String::fromInt(index));
    }//switch

    return result;
}//getGlClipPlane

void EG_Object::setShadowCasting(bool enabled)
{
	setChanged();
    shadowCasting = enabled;
}//setShadowCasting

void EG_Object::setShadowReceiving(bool enabled)
{
	setChanged();
    shadowReceiving = enabled;
}//setShadowReceiving

void EG_Object::resetShadowStamper(void)
{
    shadowStamper = 0;
}//resetShadowStamper

Point3D EG_Object::getPosition(void)
{
    return position;
}//getPosition

EulerAngle EG_Object::getRotation(void)
{
    return rotation;
}//getRotation

void EG_Object::show(void)
{
	setChanged();
    isVisible = true;
}//show

void EG_Object::hide(void)
{
	setChanged();
    isVisible = false;
}//hide

bool EG_Object::getChanged(unsigned int viewportIndex)
{
	bool result = isChanged.get(viewportIndex);//True if this object was modified since the last call to getChanged

	isChanged.set(viewportIndex, false);

	return result;
}//getChanged

void EG_Object::setChanged(void)
{
	for (unsigned int i = 0; i < isChanged.size(); i++)
		isChanged.set(i, true);
}//setChanged

EG_SceneElement* EG_Object::getElement(void)
{
	return scenePos;
}//getElement
